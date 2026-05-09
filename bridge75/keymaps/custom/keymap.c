#include QMK_KEYBOARD_H
#include "wireless.h"

enum layers {
    B,
    FN,
    FN2
};

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [B] = LAYOUT_ansi(
        KC_ESC,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,                    KC_F13,
        KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC,          KC_DEL,
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,          KC_VOLU,
        KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,           KC_VOLD,
        KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,                   KC_RSFT, KC_UP,   KC_END,
        KC_LCTL, KC_LGUI, KC_LALT,                            KC_SPC,                             KC_RALT, MO(FN),           KC_LEFT, KC_DOWN, KC_RGHT
    ),

    [FN] = LAYOUT_ansi(
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,                   LCA(KC_DEL),
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, LCTL(KC_BSPC),    _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          KC_BRIU,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          LCTL(KC_ENT),     KC_BRID,
        _______, _______, _______, _______, _______, _______, _______, KC_MUTE, KC_MPRV, KC_MNXT, KC_MPLY,                   MO(FN2), KC_PGUP, _______,
        _______, _______, _______,                            _______,                            KC_F14,  _______,          KC_HOME, KC_PGDN, KC_END
    ),

    [FN2] = LAYOUT_ansi(
        EE_CLR,  KC_BRID, KC_BRIU, KC_MCTL, KC_LPAD, KC_F5,   KC_F6,   KC_MPRV, KC_MPLY, KC_MNXT, KC_MUTE, KC_VOLD, KC_VOLU,                   _______,
        KC_USB,  KC_BT1,  KC_BT2,  KC_BT3,  KC_2G4,  _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,          _______,
        _______, RGB_TOG, RGB_MOD, RGB_RMOD,RGB_HUI, RGB_HUD, RGB_SAI, RGB_SAD, RGB_VAI, RGB_VAD, _______,                   _______, _______, _______,
        _______, _______, _______,                            _______,                            _______, _______,          _______, _______, _______
    )
};
// clang-format on

// Set default RGB to blank
void keyboard_post_init_user(void) {
    rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR);
    rgb_matrix_sethsv_noeeprom(0, 0, 0);
}

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    // Caps Lock Indicator
    if (host_keyboard_led_state().caps_lock) {
        for (uint8_t row = 0; row < MATRIX_ROWS; ++row) {
            for (uint8_t col = 0; col < MATRIX_COLS; ++col) {
                uint8_t index = g_led_config.matrix_co[row][col];

                uint16_t keycode = keymap_key_to_keycode(0, (keypos_t){col, row});
                if (keycode == KC_CAPS && index != NO_LED) {
                    rgb_matrix_set_color(index, 0, 255, 255);
                }
            }
        }
    }
    // Layer Indicator
    uint8_t current_layer = get_highest_layer(layer_state);
    if (current_layer > 0) {
        uint16_t target_keycode = KC_F1 + (current_layer - 1);

        for (uint8_t row = 0; row < MATRIX_ROWS; ++row) {
            for (uint8_t col = 0; col < MATRIX_COLS; ++col) {
                uint8_t index = g_led_config.matrix_co[row][col];

                uint16_t keycode = keymap_key_to_keycode(0, (keypos_t){col, row});
                if (keycode == target_keycode && index != NO_LED) {
                    rgb_matrix_set_color(index, 0, 0, 255);
                }
            }
        }
    }
    // Wired/Pairing Mode Indicator
    if (get_highest_layer(layer_state) == 2) {
        uint16_t target_keycode = KC_USB;
        switch (wireless_get_current_devs()){
            case DEVS_BT1: {
                target_keycode = KC_BT1;
            } break;
            case DEVS_BT2: {
                target_keycode = KC_BT2;
            } break;
            case DEVS_BT3: {
                target_keycode = KC_BT3;
            } break;
            case DEVS_2G4: {
                target_keycode = KC_2G4;
            } break;
            default:
                target_keycode = KC_USB;
                break;
        }

        for (uint8_t row = 0; row < MATRIX_ROWS; ++row) {
            for (uint8_t col = 0; col < MATRIX_COLS; ++col) {
                uint8_t index = g_led_config.matrix_co[row][col];

                uint16_t keycode = keymap_key_to_keycode(2, (keypos_t){col, row});
                if (keycode == target_keycode && index != NO_LED) {
                    rgb_matrix_set_color(index, 0, 0, 255);
                }
            }
        }
    }
    // When Layer 2 is active, highlight active keys
    if (get_highest_layer(layer_state) == 2) {
        for (uint8_t row = 0; row < MATRIX_ROWS; ++row) {
            for (uint8_t col = 0; col < MATRIX_COLS; ++col) {
                uint8_t index = g_led_config.matrix_co[row][col];
                if (index >= led_min && index < led_max && index != NO_LED) {
                    uint16_t keycode = keymap_key_to_keycode(2, (keypos_t){col, row});
                    if (keycode > KC_TRNS) {
                        // Active Layer 2 keys: White
                        rgb_matrix_set_color(index, 128, 128, 128);
                    }
                }
            }
        }
    }
    return false;
}
