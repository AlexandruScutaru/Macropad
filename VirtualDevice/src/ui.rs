use crate::hid::RawReport;
use crossterm::event::{self, Event, KeyCode, KeyEventKind, MouseButton, MouseEventKind};
use ratatui::{
    prelude::*,
    widgets::{
        Block, Borders,
        Paragraph,
        canvas::{Canvas, Circle, Points},
    },
};
use std::{io, time::Duration};

struct DragState {
    is_dragging_encoder: bool,
    last_x: u16,
    accumulated_dx: i16,
}

const DRAG_SENSITIVITY_PIXELS: i16 = 2;
const DEGREES_PER_TICK: f64 = 30.0;

const SELECTED_COLOR: Color = Color::Rgb(69, 133, 136);
const DISABLED_COLOR: Color = Color::Rgb(124, 111, 100);
const PRIMARY_FG_COLOR: Color = Color::Rgb(251, 241, 199);
const SLIDER_KEY_STEP: u8 = 5;

#[derive(Clone, Copy, PartialEq, Eq)]
enum Focus {
    Keypad,
    SliderToggle,
    Sliders,
}

pub struct Ui {
    focus: Focus,
    selected_row: usize,
    selected_col: usize,
    encoder_angle_degrees: f64,
    status_msg: String,
    drag_state: DragState,
    sliders_enabled: bool,
    slider_values: [u8; 4],
    selected_slider: Option<usize>,
    toggle_rect: Rect,
    slider_rects: [Rect; 4],
    dragging_slider: Option<usize>,
    pressed_button: Option<(u8, u8)>,
    grid_rects: [[Rect; 3]; 3],
}

#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub enum UiAction {
    KeypadButton { layer: u8, key: u8 },
    EncoderRotated { direction: bool },
    SlidersEnabledChanged { enabled: bool },
    SliderChanged { index: usize, value: u8 },
}

impl Ui {
    pub fn new() -> Self {
        Self {
            focus: Focus::Keypad,
            selected_row: 0,
            selected_col: 0,
            encoder_angle_degrees: 0.0,
            status_msg: String::from("Ready"),
            drag_state: DragState {
                is_dragging_encoder: false,
                last_x: 0,
                accumulated_dx: 0,
            },
            sliders_enabled: true,
            slider_values: [50; 4],
            selected_slider: None,
            toggle_rect: Rect::default(),
            slider_rects: [Rect::default(); 4],
            dragging_slider: None,
            pressed_button: None,
            grid_rects: [[Rect::default(); 3]; 3],
        }
    }

    pub fn draw(&mut self, frame: &mut Frame<'_>) {
        let area = frame.area();

        let main_layout = Layout::default()
            .direction(Direction::Vertical)
            .constraints([Constraint::Min(9), Constraint::Length(3)])
            .split(area);

        let device_area = main_layout[0];
        let device_layout = Layout::default()
            .direction(Direction::Horizontal)
            .horizontal_margin(2)
            .constraints(vec![Constraint::Percentage(50), Constraint::Percentage(50)])
            .split(device_area);

        self.draw_keypad(frame, device_layout[0]);
        self.draw_sliders(frame, device_layout[1]);
        self.draw_status(frame, main_layout[1]);
    }

    pub async fn poll_input(&mut self) -> io::Result<(bool, Vec<UiAction>)> {
        tokio::time::sleep(Duration::from_millis(16)).await;
        let mut actions = Vec::new();
        let mut keep_running = true;

        while event::poll(Duration::from_millis(0))? {
            let event = event::read()?;
            if matches!(event, Event::Key(key) if key.kind == KeyEventKind::Press && key.code == KeyCode::Char('q'))
            {
                keep_running = false;
                break;
            }

            match event {
                Event::Key(key) => self.handle_kb_event(key, &mut actions),
                Event::Mouse(mouse) => self.handle_mouse_event(mouse, &mut actions),
                _ => {}
            }
        }

        Ok((keep_running, actions))
    }

    fn draw_keypad(&mut self, frame: &mut Frame<'_>, area: Rect) {
        let square_height = area.height.min(area.width / 2);
        let square_width = square_height * 2;
        let square_area = Rect::new(
            area.x + (area.width - square_width),
            area.y + (area.height - square_height) / 2,
            square_width,
            square_height,
        );
        let keypad_block = Block::default()
            .borders(Borders::ALL)
            .title(" Keypad ")
            .border_style(Style::default().fg(Color::Green))
            .add_modifier(Modifier::BOLD);
        let chunks_area = keypad_block.inner(square_area);
        frame.render_widget(keypad_block, square_area);

        let row_cells = Layout::default()
            .direction(Direction::Vertical)
            .constraints([Constraint::Ratio(1, 3); 3])
            .split(chunks_area);

        for row in 0..3 {
            let col_cells = Layout::default()
                .direction(Direction::Horizontal)
                .constraints([Constraint::Ratio(1, 3); 3])
                .split(row_cells[row]);

            for col in 0..3 {
                self.grid_rects[row][col] = col_cells[col];
                let is_selected = self.focus == Focus::Keypad
                    && row == self.selected_row
                    && col == self.selected_col;

                if row == 0 && col == 0 {
                    self.draw_encoder(frame, col_cells[col], is_selected);
                } else {
                    self.draw_button(frame, col_cells[col], is_selected);
                }
            }
        }
    }

    fn draw_sliders(&mut self, frame: &mut Frame<'_>, area: Rect) {
        let square_height = area.height.min(area.width / 2);
        let square_width = square_height * 2;
        let square_area = Rect::new(
            area.x,
            area.y + (area.height - square_height) / 2,
            square_width,
            square_height,
        );
        let sliders_block = Block::default()
            .borders(Borders::ALL)
            .title(" Sliders ")
            .border_style(Style::default().fg(Color::LightBlue))
            .add_modifier(Modifier::BOLD);
        let sliders_area = sliders_block.inner(square_area);
        frame.render_widget(sliders_block, square_area);

        let controls = Layout::default()
            .direction(Direction::Vertical)
            .constraints([Constraint::Length(3), Constraint::Min(0)])
            .split(sliders_area);

        self.toggle_rect = controls[0];
        let toggle_text = if self.sliders_enabled {
            "Enabled [x]"
        } else {
            "Disabled [ ]"
        };
        let toggle_color = if self.focus == Focus::SliderToggle {
            SELECTED_COLOR
        } else if self.sliders_enabled {
            PRIMARY_FG_COLOR
        } else {
            DISABLED_COLOR
        };
        let toggle = Paragraph::new(toggle_text)
            .alignment(Alignment::Center)
            .style(Style::default().fg(toggle_color))
            .block(
                Block::default()
                    .borders(Borders::ALL)
                    .border_style(Style::default().fg(toggle_color)),
            );
        frame.render_widget(toggle, controls[0]);

        let channel_areas = Layout::default()
            .direction(Direction::Horizontal)
            .constraints([Constraint::Ratio(1, 4); 4])
            .split(controls[1]);

        for (index, channel_area) in channel_areas.iter().copied().enumerate() {
            let (color, is_selected) =
                match (self.sliders_enabled, self.selected_slider == Some(index)) {
                    (false, _) => (DISABLED_COLOR, false),
                    (true, true) if self.focus == Focus::Sliders => (SELECTED_COLOR, true),
                    (true, false) => (PRIMARY_FG_COLOR, false),
                    (true, true) => (PRIMARY_FG_COLOR, false),
                };
            let mut border_style = Style::default().fg(color);
            if is_selected {
                border_style = border_style.add_modifier(Modifier::BOLD);
            }
            let channel_block = Block::default()
                .borders(Borders::ALL)
                .title(format!(" CH{} ", index + 1))
                .border_style(border_style);
            let track_area = channel_block.inner(channel_area);
            self.slider_rects[index] = track_area;
            frame.render_widget(channel_block, channel_area);

            let filled_height =
                (u32::from(track_area.height) * u32::from(self.slider_values[index]) + 99) / 100;

            let lines = (0..track_area.height)
                .map(|row| {
                    let filled = u32::from(track_area.height - row) <= filled_height;
                    let (symbol, symbol_color) = if filled {
                        ("█", color)
                    } else {
                        ("│", color)
                    };
                    Line::from(Span::styled(symbol, Style::default().fg(symbol_color)))
                })
                .collect::<Vec<_>>();
            let track = Paragraph::new(lines).alignment(Alignment::Center);
            frame.render_widget(track, track_area);
        }
    }

    fn draw_status(&mut self, frame: &mut Frame<'_>, area: Rect) {
        let status = Paragraph::new(self.status_msg.as_str())
            .style(Style::default().fg(PRIMARY_FG_COLOR))
            .block(Block::default().borders(Borders::ALL).title(" Status "));
        frame.render_widget(status, area);
    }

    fn draw_encoder(&mut self, frame: &mut Frame<'_>, area: Rect, is_selected: bool) {
        let canvas_aspect = area.width.max(1) as f64 / (area.height.max(1) as f64 * 2.0);
        let (x_extent, y_extent) = if canvas_aspect >= 1.0 {
            (canvas_aspect, 1.0)
        } else {
            (1.0, 1.0 / canvas_aspect)
        };
        let circle_color = if is_selected {
            SELECTED_COLOR
        } else {
            PRIMARY_FG_COLOR
        };
        let circle_radius = x_extent.min(y_extent);
        let indicator_radius = circle_radius * 0.7;
        let angle_radians = self.encoder_angle_degrees.to_radians();
        let indicator_x = indicator_radius * angle_radians.sin();
        let indicator_y = indicator_radius * angle_radians.cos();
        let indicator = [(indicator_x, indicator_y)];
        let encoder_canvas = Canvas::default()
            .x_bounds([-x_extent, x_extent])
            .y_bounds([-y_extent, y_extent])
            .paint(|ctx| {
                ctx.draw(&Circle {
                    x: 0.0,
                    y: 0.0,
                    radius: circle_radius,
                    color: circle_color,
                });
                ctx.draw(&Points {
                    coords: &indicator,
                    color: Color::Cyan,
                });
            });

        frame.render_widget(encoder_canvas, area);
    }

    fn draw_button(&mut self, frame: &mut Frame<'_>, area: Rect, is_selected: bool) {
        let border_style = if is_selected {
            Style::default()
                .fg(SELECTED_COLOR)
                .add_modifier(Modifier::BOLD)
        } else {
            Style::default().fg(PRIMARY_FG_COLOR)
        };

        let button_block = Block::default()
            .borders(Borders::ALL)
            .border_style(border_style);

        frame.render_widget(button_block, area);
    }

    fn handle_kb_event(&mut self, key: crossterm::event::KeyEvent, actions: &mut Vec<UiAction>) {
        if key.kind == KeyEventKind::Press {
            if key.code == KeyCode::Tab {
                self.focus = match self.focus {
                    Focus::Keypad => Focus::SliderToggle,
                    Focus::SliderToggle => Focus::Sliders,
                    Focus::Sliders => Focus::Keypad,
                };
                if self.focus == Focus::Sliders && self.sliders_enabled {
                    self.selected_slider.get_or_insert(0);
                }
                return;
            }

            match key.code {
                KeyCode::Up | KeyCode::Down if self.focus == Focus::Sliders => {
                    if self.sliders_enabled {
                        if let Some(index) = self.selected_slider {
                            let value = &mut self.slider_values[index];
                            let new_value = if key.code == KeyCode::Up {
                                value.saturating_add(SLIDER_KEY_STEP).min(100)
                            } else {
                                value.saturating_sub(SLIDER_KEY_STEP)
                            };
                            if *value != new_value {
                                *value = new_value;
                                self.status_msg = format!("CH{}: {}%", index + 1, new_value);
                                actions.push(UiAction::SliderChanged {
                                    index,
                                    value: new_value,
                                });
                            }
                        }
                    }
                }
                KeyCode::Left | KeyCode::Right if self.focus == Focus::Sliders => {
                    if self.sliders_enabled {
                        let selected = self.selected_slider.get_or_insert(0);
                        *selected = if key.code == KeyCode::Left {
                            selected.saturating_sub(1)
                        } else {
                            (*selected + 1).min(self.slider_values.len() - 1)
                        };
                    }
                }
                KeyCode::Up | KeyCode::Down | KeyCode::Left | KeyCode::Right
                    if self.focus == Focus::Keypad =>
                {
                    match key.code {
                        KeyCode::Up => self.selected_row = self.selected_row.saturating_sub(1),
                        KeyCode::Down => {
                            if self.selected_row < 2 {
                                self.selected_row += 1;
                            }
                        }
                        KeyCode::Left => self.selected_col = self.selected_col.saturating_sub(1),
                        KeyCode::Right => {
                            if self.selected_col < 2 {
                                self.selected_col += 1;
                            }
                        }
                        _ => unreachable!(),
                    }
                }
                KeyCode::Enter | KeyCode::Char(' ') if self.focus == Focus::SliderToggle => {
                    self.sliders_enabled = !self.sliders_enabled;
                    if !self.sliders_enabled {
                        self.selected_slider = None;
                    }
                    actions.push(UiAction::SlidersEnabledChanged {
                        enabled: self.sliders_enabled,
                    });
                    self.status_msg = if self.sliders_enabled {
                        String::from("Sliders enabled")
                    } else {
                        String::from("Sliders disabled")
                    };
                }
                KeyCode::Enter if self.focus == Focus::Keypad => {
                    let row = self.selected_row as u8;
                    let col = self.selected_col as u8;
                    actions.push(UiAction::KeypadButton { layer: 0, key: row * 3 + col });
                    self.status_msg =
                        format!("Key ({}, {}) pressed", self.selected_row, self.selected_col);
                }
                _ => {}
            }
        }
    }

    fn handle_mouse_event(
        &mut self,
        mouse: crossterm::event::MouseEvent,
        actions: &mut Vec<UiAction>,
    ) {
        let x = mouse.column;
        let y = mouse.row;
        let encoder_rect = self.grid_rects[0][0];

        match mouse.kind {
            MouseEventKind::Down(MouseButton::Left) => {
                if contains_point(self.toggle_rect, x, y) {
                    self.focus = Focus::SliderToggle;
                    self.sliders_enabled = !self.sliders_enabled;
                    self.dragging_slider = None;
                    if !self.sliders_enabled {
                        self.selected_slider = None;
                    }
                    actions.push(UiAction::SlidersEnabledChanged {
                        enabled: self.sliders_enabled,
                    });
                    self.status_msg = if self.sliders_enabled {
                        String::from("Sliders enabled")
                    } else {
                        String::from("Sliders disabled")
                    };
                } else if self.sliders_enabled {
                    if let Some(index) = self.slider_index_at(x, y) {
                        self.focus = Focus::Sliders;
                        self.selected_slider = Some(index);
                        self.dragging_slider = Some(index);
                        if let Some(action) = self.set_slider_from_y(index, y) {
                            actions.push(action);
                        }
                    } else if let Some((row, col)) = self.select_cell_at(x, y, "selected") {
                        self.focus = Focus::Keypad;
                        if (row, col) != (0, 0) {
                            self.pressed_button = Some((row, col));
                        }
                    }
                } else if let Some((row, col)) = self.select_cell_at(x, y, "selected") {
                    self.focus = Focus::Keypad;
                    if (row, col) != (0, 0) {
                        self.pressed_button = Some((row, col));
                    }
                }
            }
            MouseEventKind::Up(MouseButton::Left) => {
                let was_dragging_slider = if let Some(index) = self.dragging_slider.take() {
                    if let Some(action) = self.set_slider_from_y(index, y) {
                        actions.push(action);
                    }
                    true
                } else {
                    false
                };

                if !was_dragging_slider {
                    if self.drag_state.is_dragging_encoder {
                        self.drag_state.is_dragging_encoder = false;
                    } else {
                        if let Some((row, col)) = self.select_cell_at(x, y, "clicked") {
                            actions.push(UiAction::KeypadButton { layer: 0, key: row * 3 + col });
                        }
                    }
                }
            }
            MouseEventKind::Drag(MouseButton::Left) => {
                if self.drag_state.is_dragging_encoder {
                    let dx = x as i16 - self.drag_state.last_x as i16;
                    self.drag_state.accumulated_dx += dx;
                    self.drag_state.last_x = x;

                    while self.drag_state.accumulated_dx.abs() >= DRAG_SENSITIVITY_PIXELS {
                        if self.drag_state.accumulated_dx > 0 {
                            self.encoder_angle_degrees =
                                (self.encoder_angle_degrees + DEGREES_PER_TICK) % 360.0;
                            self.drag_state.accumulated_dx -= DRAG_SENSITIVITY_PIXELS;
                            self.status_msg = String::from("Encoder CW turn");
                            actions.push(UiAction::EncoderRotated { direction: true });
                        } else {
                            self.encoder_angle_degrees =
                                (self.encoder_angle_degrees - DEGREES_PER_TICK + 360.0) % 360.0;
                            self.drag_state.accumulated_dx += DRAG_SENSITIVITY_PIXELS;
                            self.status_msg = String::from("Encoder CCW turn");
                            actions.push(UiAction::EncoderRotated { direction: false });
                        }
                    }
                } else if let Some(index) = self.dragging_slider {
                    if let Some(action) = self.set_slider_from_y(index, y) {
                        actions.push(action);
                    }
                } else if contains_point(encoder_rect, x, y) {
                    self.drag_state.is_dragging_encoder = true;
                    self.drag_state.last_x = x;
                    self.drag_state.accumulated_dx = 0;
                    self.status_msg = String::from("Encoder grabbed");
                }
            }
            _ => {}
        }
    }

    fn slider_index_at(&self, x: u16, y: u16) -> Option<usize> {
        self.slider_rects
            .iter()
            .position(|rect| contains_point(*rect, x, y))
    }

    fn set_slider_from_y(&mut self, index: usize, y: u16) -> Option<UiAction> {
        let rect = self.slider_rects[index];
        if rect.height == 0 {
            return None;
        }

        let y = y.clamp(rect.y, rect.y + rect.height - 1);
        let value = if rect.height == 1 {
            50
        } else {
            (u32::from(rect.y + rect.height - 1 - y) * 100 / u32::from(rect.height - 1)) as u8
        };
        if self.slider_values[index] == value {
            return None;
        }
        self.slider_values[index] = value;
        self.status_msg = format!("CH{}: {value}%", index + 1);
        Some(UiAction::SliderChanged { index, value })
    }

    fn select_cell_at(&mut self, x: u16, y: u16, action: &str) -> Option<(u8, u8)> {
        for row in 0..3 {
            for col in 0..3 {
                if contains_point(self.grid_rects[row][col], x, y) {
                    self.selected_row = row;
                    self.selected_col = col;
                    self.status_msg = format!("Key ({row}, {col}) {action}");

                    return Some((row as u8, col as u8));
                }
            }
        }

        None
    }

    pub fn handle_host_report(&mut self, report: RawReport) {
        self.status_msg = format!("Host report: {:02X?}", &report[..8]);
    }
}

fn contains_point(rect: Rect, x: u16, y: u16) -> bool {
    x >= rect.x && x < rect.x + rect.width && y >= rect.y && y < rect.y + rect.height
}
