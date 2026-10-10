use std::{fs::OpenOptions, io, os::unix::fs::OpenOptionsExt, sync::Arc, time::Duration};
use tokio::{
    fs::File,
    io::{AsyncReadExt, AsyncWriteExt},
    sync::{Mutex, mpsc},
    task::JoinHandle,
};

pub const REPORT_SIZE: usize = 32;
pub type RawReport = [u8; REPORT_SIZE];
pub const BUTTON_REPORT: u8 = 0x01;
pub const ENCODER_REPORT: u8 = 0x02;
pub const SLIDERS_ENABLED_REPORT: u8 = 0x03;
pub const SLIDER_VALUE_REPORT: u8 = 0x04;

const SLIDER_COUNT: usize = 4;

const UHID_CREATE2: u32 = 11;
const UHID_INPUT2: u32 = 12;
const UHID_OUTPUT: u32 = 6;
const UHID_DESTROY: u32 = 1;
const UHID_DATA_MAX: usize = 4096;
const VENDOR_ID: u32 = 0xFEED;
const PRODUCT_ID: u32 = 0xB00B;
const USAGE_PAGE: u16 = 0xFF60;
const USAGE_ID: u16 = 0x61;

#[repr(C, packed)]
struct UhidCreate2Req {
    name: [u8; 128],
    phys: [u8; 64],
    uniq: [u8; 64],
    rd_size: u16,
    bus: u16,
    vendor: u32,
    product: u32,
    version: u32,
    country: u32,
    rd_data: [u8; 4096],
}

#[repr(C, packed)]
struct UhidInput2Req {
    size: u16,
    data: [u8; 4096],
}

// Vendor-Defined HID Report Descriptor (64-byte Input & 64-byte Output)
const VENDOR_RDESC: &[u8] = &[
    0x06,
    (USAGE_PAGE & 0xFF) as u8,
    (USAGE_PAGE >> 8) as u8, // Usage Page (Vendor Defined 0xFF60)
    0x09,
    USAGE_ID as u8, // Usage (Vendor Usage 0x61)
    0xA1,
    0x01, // Collection (Application)
    // Input Report (Device -> Host Application)
    0x09,
    0x02, //   Usage (Vendor Usage 2)
    0x15,
    0x00, //   Logical Minimum (0)
    0x26,
    0xFF,
    0x00, //   Logical Maximum (255)
    0x75,
    0x08, //   Report Size (8 bits)
    0x95,
    REPORT_SIZE as u8, //   Report Count (32 bytes)
    0x81,
    0x02, //   Input (Data, Var, Abs)
    // Output Report (Host Application -> Device)
    0x09,
    0x03, //   Usage (Vendor Usage 3)
    0x15,
    0x00, //   Logical Minimum (0)
    0x26,
    0xFF,
    0x00, //   Logical Maximum (255)
    0x75,
    0x08, //   Report Size (8 bits)
    0x95,
    REPORT_SIZE as u8, //   Report Count (32 bytes)
    0x91,
    0x02, //   Output (Data, Var, Abs)
    0xC0, // End Collection
];

#[derive(Debug)]
pub enum ApplicationMessage {
    OutputReportReceived(RawReport),
}

pub struct AsyncUhidDevice {
    file: Arc<Mutex<File>>,
    listener: Option<JoinHandle<()>>,
}

impl AsyncUhidDevice {
    pub async fn create() -> tokio::io::Result<(Self, mpsc::Receiver<ApplicationMessage>)> {
        let std_file = OpenOptions::new()
            .read(true)
            .write(true)
            .custom_flags(libc::O_CLOEXEC | libc::O_NONBLOCK)
            .open("/dev/uhid")?;

        let file = Arc::new(Mutex::new(File::from_std(std_file)));
        let (tx, rx) = mpsc::channel(32);

        // Populate creation request
        let mut req = UhidCreate2Req {
            name: [0; 128],
            phys: [0; 64],
            uniq: [0; 64],
            rd_size: VENDOR_RDESC.len() as u16,
            bus: 3, // BUS_USB
            vendor: VENDOR_ID,
            product: PRODUCT_ID,
            version: 0,
            country: 0,
            rd_data: [0; 4096],
        };

        let name = b"Ratatui Virtual Macropad";
        req.name[..name.len()].copy_from_slice(name);
        req.rd_data[..VENDOR_RDESC.len()].copy_from_slice(VENDOR_RDESC);

        let mut packet = Vec::with_capacity(4 + std::mem::size_of::<UhidCreate2Req>());
        packet.extend_from_slice(&UHID_CREATE2.to_ne_bytes());
        let req_bytes: &[u8] = unsafe {
            std::slice::from_raw_parts(
                &req as *const _ as *const u8,
                std::mem::size_of::<UhidCreate2Req>(),
            )
        };
        packet.extend_from_slice(req_bytes);

        // Register device with kernel
        let mut f_ref = file.lock().await;
        f_ref.write_all(&packet).await?;
        drop(f_ref);

        // Background listener task for UHID_OUTPUT events
        let read_file = Arc::clone(&file);
        let listener = tokio::spawn(async move {
            let mut buf = [0u8; 4 + std::mem::size_of::<UhidCreate2Req>()];
            let file = read_file;

            loop {
                let read_result = {
                    let mut file = file.lock().await;
                    file.read(&mut buf).await
                };

                match read_result {
                    Ok(0) => break,
                    Ok(n) if n >= 4 => {
                        let event_type = u32::from_ne_bytes(buf[0..4].try_into().unwrap());
                        if event_type == UHID_OUTPUT && n >= 4 + UHID_DATA_MAX + 2 {
                            let output_req_data = &buf[4..n];
                            let size = u16::from_ne_bytes(
                                output_req_data[UHID_DATA_MAX..UHID_DATA_MAX + 2]
                                    .try_into()
                                    .unwrap(),
                            ) as usize;

                            let mut report = [0u8; REPORT_SIZE];
                            let copy_len = size.min(REPORT_SIZE);
                            report[..copy_len].copy_from_slice(&output_req_data[..copy_len]);

                            let _ = tx
                                .send(ApplicationMessage::OutputReportReceived(report))
                                .await;
                        }
                    }
                    Ok(_) => {}
                    Err(e) if e.kind() == io::ErrorKind::WouldBlock => {
                        tokio::time::sleep(Duration::from_millis(10)).await;
                    }
                    Err(e) => {
                        eprintln!("UHID read error: {}", e);
                        break;
                    }
                }
            }
        });

        Ok((
            Self {
                file,
                listener: Some(listener),
            },
            rx,
        ))
    }

    pub async fn send_button(&self, row: u8, col: u8) -> tokio::io::Result<()> {
        let mut report = [0; REPORT_SIZE];
        report[0] = BUTTON_REPORT;
        report[1] = row;
        report[2] = col;
        self.send_vendor_report(&report).await
    }

    pub async fn send_encoder_rotation(&self, direction: bool) -> tokio::io::Result<()> {
        let mut report = [0; REPORT_SIZE];
        report[0] = ENCODER_REPORT;
        report[1] = u8::from(direction);
        self.send_vendor_report(&report).await
    }

    pub async fn send_sliders_enabled(&self, enabled: bool) -> tokio::io::Result<()> {
        let mut report = [0; REPORT_SIZE];
        report[0] = SLIDERS_ENABLED_REPORT;
        report[1] = u8::from(enabled);
        self.send_vendor_report(&report).await
    }

    pub async fn send_slider_value(&self, index: usize, value: u8) -> tokio::io::Result<()> {
        if index >= SLIDER_COUNT {
            return Err(io::Error::new(
                io::ErrorKind::InvalidInput,
                format!("slider index {index} is out of range"),
            ));
        }
        if value > 100 {
            return Err(io::Error::new(
                io::ErrorKind::InvalidInput,
                format!("slider value {value} exceeds 100"),
            ));
        }

        let mut report = [0; REPORT_SIZE];
        report[0] = SLIDER_VALUE_REPORT;
        report[1] = index as u8;
        report[2] = value;
        self.send_vendor_report(&report).await
    }

    async fn send_vendor_report(&self, payload: &RawReport) -> tokio::io::Result<()> {
        let mut req = UhidInput2Req {
            size: REPORT_SIZE as u16,
            data: [0; 4096],
        };
        req.data[..REPORT_SIZE].copy_from_slice(payload);

        let mut packet = Vec::with_capacity(4 + std::mem::size_of::<UhidInput2Req>());
        packet.extend_from_slice(&UHID_INPUT2.to_ne_bytes());
        let req_bytes: &[u8] = unsafe {
            std::slice::from_raw_parts(
                &req as *const _ as *const u8,
                std::mem::size_of::<UhidInput2Req>(),
            )
        };
        packet.extend_from_slice(req_bytes);

        let mut f = self.file.lock().await;
        f.write_all(&packet).await
    }

    pub async fn destroy(&mut self) -> tokio::io::Result<()> {
        if let Some(listener) = self.listener.take() {
            listener.abort();
        }

        let mut f = self.file.lock().await;
        f.write_all(&UHID_DESTROY.to_ne_bytes()).await
    }
}

impl Drop for AsyncUhidDevice {
    fn drop(&mut self) {
        if let Some(listener) = self.listener.take() {
            listener.abort();
        }
    }
}
