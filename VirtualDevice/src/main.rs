use crossterm::{
    ExecutableCommand,
    event::{DisableMouseCapture, EnableMouseCapture},
    terminal::{EnterAlternateScreen, LeaveAlternateScreen, disable_raw_mode, enable_raw_mode},
};
use ratatui::{Terminal, backend::CrosstermBackend};
use std::io;

mod hid;
mod ui;
use hid::{ApplicationMessage, AsyncUhidDevice};
use ui::{Ui, UiAction};

#[tokio::main]
async fn main() -> Result<(), Box<dyn std::error::Error>> {
    let (mut hid, mut host_messages) = AsyncUhidDevice::create().await?;

    enable_raw_mode()?;
    io::stdout().execute(EnterAlternateScreen)?;
    io::stdout().execute(EnableMouseCapture)?;
    let mut terminal = Terminal::new(CrosstermBackend::new(io::stdout()))?;
    let mut ui = Ui::new();

    loop {
        terminal.draw(|frame| ui.draw(frame))?;

        let (keep_running, actions) = ui.poll_input().await?;
        for action in actions {
            match action {
                UiAction::KeypadButton { row, col } => {
                    hid.send_button(row, col).await?;
                }
                UiAction::EncoderRotated { direction } => {
                    hid.send_encoder_rotation(direction).await?;
                }
                UiAction::SlidersEnabledChanged { enabled } => {
                    hid.send_sliders_enabled(enabled).await?;
                }
                UiAction::SliderChanged { index, value } => {
                    hid.send_slider_value(index, value).await?;
                }
            }
        }

        if !keep_running {
            break;
        }

        while let Ok(ApplicationMessage::OutputReportReceived(report)) = host_messages.try_recv() {
            ui.handle_host_report(report);
        }
    }

    let destroy_result = hid.destroy().await;

    // cleanup terminal
    io::stdout().execute(DisableMouseCapture)?;
    disable_raw_mode()?;
    io::stdout().execute(LeaveAlternateScreen)?;
    destroy_result?;
    Ok(())
}
