#include QMK_KEYBOARD_H
#include "analog.h"
#include "raw_hid.h"

#define RGB_RED_PIN     B5
#define RGB_GREEN_PIN   C6
#define RGB_BLUE_PIN    D7

#define POT_1_PIN   F4
#define POT_2_PIN   F5
#define POT_3_PIN   D4
#define POT_4_PIN   B4

#define RAW_BUFFER_LENGTH  32
#define POT_READING_TIMEOUT_MS 60


enum tap_dance_keys_t {
    TD_ENCODER_SW = 0
};

typedef enum {
    TD_NONE = 0,
    TD_UNKNOWN,
    TD_SINGLE_TAP,
    TD_SINGLE_HOLD
} td_states_t;

typedef struct {
    bool is_press_action;
    td_states_t state;
} td_tap_t;

static td_tap_t g_td_state = {
    .is_press_action = true,
    .state = TD_NONE
};

static td_states_t get_current_td_state(tap_dance_state_t* state);
void td_finished(tap_dance_state_t* state, void* user_data);
void td_reset(tap_dance_state_t* state, void* user_data);

tap_dance_action_t tap_dance_actions[] = {
    [TD_ENCODER_SW] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_finished, td_reset)
};

typedef enum {
    MEDIA = 0,
    LETTERS,
    CUSTOM,

    NUM_LAYERS
} layers_t;

static layers_t g_current_layer = MEDIA;
static layers_t cycle_layers(void);

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} color_t;

static color_t get_layer_color(layers_t layer);
static void setRgbColor(const color_t* const color);

uint8_t raw_hid_buffer[RAW_BUFFER_LENGTH] = { 0 };
uint32_t pot_reading_timer = 0;

static void read_pots(void);
static void send_pot_value(uint8_t pot, uint16_t value);

const uint16_t encoder_actions[] = {
    KC_MUTE, KC_A, KC_MUTE
};

enum custom_keycodes {
    SEND_STRING_KEY = SAFE_RANGE,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [MEDIA] = LAYOUT_ortho_3x3(
        TD(TD_ENCODER_SW),    KC_MYCM,    KC_WHOM,
        KC_MAIL,              KC_CALC,    KC_MSTP,
        KC_MPRV,              KC_MPLY,    KC_MNXT
    ),

    [LETTERS] = LAYOUT_ortho_3x3(
        TD(TD_ENCODER_SW),    KC_B,    KC_C,
        KC_D,                 KC_E,    KC_F,
        KC_G,                 KC_H,    KC_I
    ),

    [CUSTOM] = LAYOUT_ortho_3x3(
        TD(TD_ENCODER_SW),    KC_F13,     SEND_STRING_KEY,
        _______,              _______,    _______,
        _______,              _______,    _______
    ),
};

const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [MEDIA] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [LETTERS] = { ENCODER_CCW_CW(KC_MS_WH_UP, KC_MS_WH_DOWN) },
    [CUSTOM] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
};

void keyboard_post_init_user(void) {
    analogReference(ADC_REF_POWER);

    setPinOutput(RGB_RED_PIN);
    setPinOutput(RGB_GREEN_PIN);
    setPinOutput(RGB_BLUE_PIN);

    setPinInputHigh(POT_1_PIN);
    setPinInputHigh(POT_2_PIN);
    setPinInputHigh(POT_3_PIN);
    setPinInputHigh(POT_4_PIN);

    color_t color = get_layer_color(g_current_layer);
    setRgbColor(&color);

    read_pots();
}

// void suspend_power_down_user(void) {
//     color_t color = get_layer_color(NUM_LAYERS);
//     setRgbColor(&color);
// }

bool process_record_user(uint16_t keycode, keyrecord_t* record) {
    switch (keycode) {
        case SEND_STRING_KEY: {
            if (record->event.pressed) {
                SEND_STRING("Macropad!" SS_TAP(X_ENTER));
            }
            return false;
        }
    }

    return true;
}

void housekeeping_task_user(void) {
   if (timer_elapsed(pot_reading_timer) > POT_READING_TIMEOUT_MS) {
       read_pots();
   }
}

td_states_t get_current_td_state(tap_dance_state_t* state) {
    if (state->count == 1) {
        if (!state->pressed) {
            return TD_SINGLE_TAP;
        }

        return TD_SINGLE_HOLD;
    }

    return TD_UNKNOWN;
}


void td_finished(tap_dance_state_t* state, void* user_data) {
    g_td_state.state = get_current_td_state(state);

    switch (g_td_state.state) {
        case TD_SINGLE_TAP:
            tap_code(encoder_actions[g_current_layer]);
            break;
        case TD_SINGLE_HOLD:
            layer_move(cycle_layers());
            color_t color = get_layer_color(g_current_layer);
            setRgbColor(&color);
            break;
        default:
            break;
    }
}

void td_reset(tap_dance_state_t* state, void* user_data) {
    g_td_state.state = TD_NONE;
}


layers_t cycle_layers() {
    g_current_layer = (g_current_layer + 1) % NUM_LAYERS;
    return g_current_layer;
}

color_t get_layer_color(layers_t layer) {
    switch (layer)
    {
        case MEDIA:        return (color_t) { .r =   0, .g = 255, .b =   0 };       // green
        case LETTERS:      return (color_t) { .r = 255, .g =   0, .b =   0 };       // red
        case CUSTOM:       return (color_t) { .r =   0, .g =   0, .b = 255 };       // blue
        default:           return (color_t) { .r =   0, .g =   0, .b =   0 };       // off
    }
}

void setRgbColor(const color_t* const color) {
    // common anode, hence the inversion of the value
    writePin(RGB_RED_PIN, 255 - color->r);
    writePin(RGB_GREEN_PIN, 255 - color->g);
    writePin(RGB_BLUE_PIN, 255 - color->b);
}

// void raw_hid_receive(uint8_t *data, uint8_t length) {
//     // `data` is a pointer to the buffer containing the received HID report
//     // `length` is the length of the report - always `RAW_EPSIZE`
// }

void read_pots(void) {
   send_pot_value(0, analogReadPin(POT_1_PIN));
   send_pot_value(1, analogReadPin(POT_2_PIN));
   send_pot_value(2, analogReadPin(POT_3_PIN));
   send_pot_value(3, analogReadPin(POT_4_PIN));
   pot_reading_timer = timer_read32();
}

void send_pot_value(uint8_t pot, uint16_t value) {
    if (value > 600) {
        // no pot connected as the pin is pulled high
        return;
    }

    memset(raw_hid_buffer, 0, RAW_BUFFER_LENGTH);
    raw_hid_buffer[1] = pot;
    raw_hid_buffer[2] = value & 0xFF;
    raw_hid_buffer[3] = value >> 8;
    raw_hid_send(raw_hid_buffer, RAW_BUFFER_LENGTH);
}
