/* Copyright 2022 @ Keychron (https://www.keychron.com)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

// Home row mods
#define HOME_ESC LT(MOTIONS, KC_ESC)

#define HOME_A LGUI_T(KC_A)
#define HOME_R LALT_T(KC_R)
#define HOME_S LSFT_T(KC_S)
#define HOME_T LCTL_T(KC_T)

#define HOME_N LCTL_T(KC_N)
#define HOME_E LSFT_T(KC_E)
#define HOME_I LALT_T(KC_I)
#define HOME_O LGUI_T(KC_O)

#include QMK_KEYBOARD_H
// clang-format off

enum tap_actions{
    HOME,
};

enum layers{
  BASE,
  MOTIONS,
  UNTOUCHED
};

tap_dance_action_t tap_dance_actions[] = {
    [HOME] = ACTION_TAP_DANCE_LAYER_TOGGLE(KC_HOME, UNTOUCHED),
};

bool rgb_matrix_indicators_kb(void) {
    rgb_matrix_set_color_all(0x8D, 0xF5, 0x42);
    return false;
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [BASE] = LAYOUT_ansi_100(
        KC_ESC,    KC_F1,   KC_F2,   KC_F3,  KC_F4,     KC_F5,      KC_F6,    KC_F7,     KC_F8,      KC_F9,     KC_F10,     KC_F11,   KC_F12,   KC_DEL,   TD(HOME),  KC_END,  KC_PGUP,  KC_PGDN, RGB_MOD,
        KC_GRV,    KC_1,    KC_2,    KC_3,   KC_4,      KC_5,       KC_6,     KC_7,      KC_8,       KC_9,      KC_0,       KC_MINS,  KC_EQL,   KC_BSPC,  KC_NUM,   KC_PSLS, KC_PAST,  KC_PMNS,
        KC_TAB,    KC_Q,    KC_W,    KC_F,   KC_P,      KC_G,       KC_J,     KC_L,      KC_U,       KC_Y,      KC_SCLN,    KC_LBRC,  KC_RBRC,  KC_BSLS,  KC_P7,    KC_P8,   KC_P9,
        HOME_ESC,  HOME_A,  HOME_R,  HOME_S, HOME_T,    KC_D,       KC_H,     HOME_N,    HOME_E,     HOME_I,    HOME_O,     KC_QUOT,  KC_ENT,   KC_P4,    KC_P5,    KC_P6,   KC_PPLS,
        KC_LSFT,   KC_Z,    KC_X,    KC_C,   KC_V,      KC_B,       KC_K,     KC_M,      KC_COMM,    KC_DOT,    KC_SLSH,    KC_RSFT,  KC_UP,    KC_P1,    KC_P2,    KC_P3,
        KC_LCTL,   KC_LWIN, KC_LALT, KC_SPC, KC_RALT,   MO(MOTIONS), KC_RCTL,  KC_LEFT,   KC_DOWN,    KC_RGHT,   KC_P0,      KC_PDOT,  KC_PENT),
    [MOTIONS] = LAYOUT_ansi_100(
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,  _______,  _______,  _______,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,            _______,  _______,  _______,  _______,
        _______,  _______,  _______,  _______, KC_UP,     _______,  KC_DOWN,  KC_RGHT,  _______,  _______,  _______,  _______,    _______,  _______,            _______,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,  _______,  KC_LEFT,  KC_DOWN,  _______,  _______,  _______,  _______,              _______,            _______,  _______,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,  _______,  KC_UP,    _______,  _______,  _______,    _______,              _______,  _______,  _______,  _______,  _______,
        _______,  _______,  _______,                                _______,                                _______,  _______,    _______,  _______,  _______,  _______,  _______,  _______,  _______),
    [UNTOUCHED] = LAYOUT_ansi_100(
        KC_ESC,    KC_F1,   KC_F2,   KC_F3,  KC_F4,     KC_F5,      KC_F6,    KC_F7,     KC_F8,      KC_F9,     KC_F10,     KC_F11,   KC_F12,   KC_DEL,   TD(HOME),  KC_END,  KC_PGUP,  KC_PGDN, RGB_MOD,
        KC_GRV,    KC_1,    KC_2,    KC_3,   KC_4,      KC_5,       KC_6,     KC_7,      KC_8,       KC_9,      KC_0,       KC_MINS,  KC_EQL,   KC_BSPC,  KC_NUM,   KC_PSLS, KC_PAST,  KC_PMNS,
        KC_TAB,    KC_Q,    KC_W,    KC_E,   KC_R,      KC_T,       KC_Y,     KC_U,      KC_I,       KC_O,      KC_P,       KC_LBRC,  KC_RBRC,  KC_BSLS,  KC_P7,    KC_P8,   KC_P9,
        KC_CAPS,   KC_A,    KC_S,    KC_D,   KC_F,      KC_G,       KC_H,     KC_J,      KC_K,       KC_L,      KC_SCLN,       KC_QUOT,  KC_ENT,   KC_P4,    KC_P5,    KC_P6,   KC_PPLS,
        KC_LSFT,   KC_Z,    KC_X,    KC_C,   KC_V,      KC_B,       KC_N,     KC_M,      KC_COMM,    KC_DOT,    KC_SLSH,    KC_RSFT,  KC_UP,    KC_P1,    KC_P2,    KC_P3,
        KC_LCTL,   KC_LWIN, KC_LALT, KC_SPC, KC_RALT,   KC_NO,      KC_RCTL,  KC_LEFT,   KC_DOWN,    KC_RGHT,   KC_P0,      KC_PDOT,  KC_PENT),
};
