// Copyright 2023 Massdrop, Inc.
// SPDX-License-Identifier: GPL-2.0-or-later
#include QMK_KEYBOARD_H

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT_65_ansi_blocker(
        KC_ESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC, KC_DEL,
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS, KC_END,
        KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,  KC_VOLU,
        KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,          KC_UP,   KC_VOLD,
        KC_LCTL, KC_LGUI, KC_LALT,                            KC_SPC,                             KC_RALT, MO(1),   KC_LEFT, KC_DOWN, KC_RGHT
    ),
    [1] = LAYOUT_65_ansi_blocker(
        KC_GRV,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  C(KC_BSPC), KC_F13,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_F15,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          C(KC_ENT), KC_VOLU,
        _______, _______, _______, _______, _______, _______, _______, KC_MUTE, KC_MPRV, KC_MNXT, KC_MPLY, MO(2),            KC_PGUP, KC_VOLD,
        _______, _______, _______,                            _______,                            KC_F14,  _______, KC_HOME, KC_PGDN, KC_END
    ),
    [2] = LAYOUT_65_ansi_blocker(
        EE_CLR,  _______, TG(3),   _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, _______,
        _______, _______, _______, _______, _______, QK_BOOT, _______, _______, _______, _______, _______, _______,          _______, _______,
        _______, _______, _______,                            _______,                            _______, _______, _______, _______, _______
    ),
    [3] = LAYOUT_65_ansi_blocker(
        TG(0),   _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, RM_VALD, RM_VALU, _______, _______,
        _______, RM_TOGG, _______, _______, _______, _______, _______, _______, _______, _______, _______, RM_SATD, RM_SATU, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, _______,
        _______, RGB_M_P, RGB_M_B, RGB_M_R, RGB_M_SW,_______, _______, _______, RM_SPDD, RM_SPDU, _______, _______,          RM_HUEU, _______,
        _______, _______, _______,                            _______,                            _______, TG(0),   RM_PREV, RM_HUED, RM_NEXT
    )
};

// Set default RGB to blank
/* commented out temporarily
void keyboard_post_init_user(void) {
    rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR);
    rgb_matrix_sethsv_noeeprom(0, 0, 0);
}
*/

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

    return false;
}
