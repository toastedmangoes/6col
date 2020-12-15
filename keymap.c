/* Copyright 2020 @wafflekeebs/@waffle#6666
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

#include QMK_KEYBOARD_H
#include "keycodes.h"
#include "macros.h"
#ifdef UNICODEMAP_ENABLE
#   include "unicodes.h"
#endif
#ifdef OLED_DRIVER_ENABLE
#   include "oled.c"
#endif
#ifdef COMBO_ENABLE
#   include "combos.h"
#endif
#ifdef TAP_DANCE_ENABLE
#   include "tapdance.c"
#endif
#ifdef POIINTING_DEVICE_ENABLE
#   include "pimoroni_trackball.c"
#   include "pimoroni_trackball.h"
#endif

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_QWERTY] = LAYOUT(
    //|---------------------------------------------------|                 |------------------------------------------------------|
       GIT,      KC_Q,   KC_W,    KC_E,    KC_R,    KC_T,                     KC_Y,    KC_U,    KC_I,      KC_O,    KC_P,  MPSCR,
    //|--------+--------+--------+--------+--------+------|                 |--------+--------+--------+--------+--------+---------|
       ALT_TAB,  SFA,    KC_S,    KC_D,    KC_F,    KC_G,                     KC_H,    KC_J,    KC_K,     KC_L,    SFCLN,  MAKE,
    //---------+--------+--------+--------+--------+------|                 |--------+--------+--------+--------+--------+---------|
       CP_PSTE,  CTLTZ,  KC_X,    KC_C,    KC_V,    KC_B,                     KC_N,   KC_M,    KC_COMM,  KC_DOT,  CTLSLH,  CTLS,
    //---------+--------+--------+--------+--------+--------+----|  |--------+--------+--------+--------+--------+--------+--------|
                                           KC_MS_L, LOWER, KC_SPC,   KC_BSPC, RAISE,   KC_MS_R
                                      //|------------------------|  |--------------------------|


  ),

  [_LOWER] = LAYOUT(
    //|------------------------------------------------------|                    |----------------------------------------------------------|
       CONF,   KC_1,    KC_2,    KC_3,     KC_4,     KC_5,                         KC_6,     KC_7,     KC_8,     KC_9,    KC_0,    KC_MNXT,
    //|--------+--------+--------+--------+--------+---------|                    |--------+--------+--------+--------+--------+-------------|
       SETUP,  SFEXM,   KC_AT,   KC_HASH,  KC_DLR,   KC_PERC,                      KC_CIRC,  KC_AMPR,  KC_ASTR,  KC_LPRN, KC_RPRN, KC_MPLY,
    //|--------+--------+--------+--------+--------+---------|                    |--------+--------+--------+--------+--------+-------------|
       ZAD,    CTLESC,  KC_TAB,  KC_CAPS,  KC_TILD,  KC_GRV,                       KC_LEFT,  KC_UP,    KC_DOWN,  KC_RGHT, HAPSAD,  KC_MPRV,
    //|--------+--------+--------+--------+--------+--------+-------|  |--------+--------+--------+--------+--------+--------+---------------|
                                           KC_LGUI, KC_TRNS, KC_SPC,    KC_VOLU,   KC_MS_U,  KC_PGUP
                                        //|-------------------------|  |----------------------------|
  ),

  [_RAISE] = LAYOUT(
    //|-----------------------------------------------------|                    |---------------------------------------------------------------|
       UC_M_WC, KC_MINS, KC_EQL,  KC_LBRC, KC_RBRC, KC_BSLS,                      KC_NOMODE, KC_BLOCKS, KC_AUSSIE,   KC_ASON,  UNIT2,  CG_SWAP,
    //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+------------------|
       UC_M_MA, KC_UNDS, KC_PLUS, KC_LCBR, KC_RCBR, KC_PIPE,                      KC_WIDE,   KC_SCRIPT, KC_REGIONAL, KC_ASOFF, WEEB,   CG_NORM,
    //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+------------------|
       RESET,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                        UNIT,      UNIT3,     UNIT4,       MMAP,     NKINV,  VIA,
    //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+------------------|
                                       KC_PGDN, KC_MS_D, KC_VOLD,             KC_BSPC, KC_TRNS, KC_RALT
                                      //|---------------------------|  |---------------------------------------|
  )
};

/*
  [BLANK] = LAYOUT(
  //|-----------------------------------------------------|                    |-----------------------------------------------------|
     _______, _______, _______, _______, _______, _______,                       _______, _______, _______, _______, _______, _______,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
     _______, _______, _______, _______, _______, _______,                      _______, _______, _______, _______, _______, _______,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
     _______, _______, _______, _______, _______, _______,                      _______, _______, _______, _______, _______, _______,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                         _______, _______, _______,    _______, _______, _______
                                      //|--------------------------|  |--------------------------|
  )
*/

