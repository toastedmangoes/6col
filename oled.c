/* Copyright 2021 @Itswaffle/@waffle#6666
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

#pragma once
#include <string.h>
bool process_record_user_oled(uint16_t keycode, keyrecord_t *record);
extern uint8_t is_master;
//bongo
#define IDLE_FRAMES 5
#define IDLE_SPEED 30
#define TAP_FRAMES 2
#define TAP_SPEED 40
#define ANIM_FRAME_DURATION 200
#define BONGO_SIZE 512
//felix
#define MIN_WALK_SPEED 10
#define MIN_RUN_SPEED 60
#define FELIX_FRAMES 2
#define FELIX_FRAME_DURATION 200
#define FELIX_SIZE 96
#define FILLED
bool gui_on = true;
uint32_t bongo_anim_timer = 0;
uint32_t bongo_anim_sleep = 0;
uint8_t current_idle_frame = 0;
uint8_t current_tap_frame = 0;
uint32_t felix_anim_timer = 0;
uint32_t felix_anim_sleep = 0;
uint8_t felix_current_frame = 0;
uint8_t felix_prev_wpm = 0;
bool felix_typing = false;

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
  if (is_master) {
    return 3;
  } else {
    return 2;
  }
}

void render_wpm(void) {
    char wpm_string[5];
        oled_write_ln("WPM:", false);
        snprintf(wpm_string,
    sizeof(wpm_string), " %3d",
    get_current_wpm());
        oled_write(wpm_string, false);
};

#    define KEYLOG_LEN 5
char     keylog_str[KEYLOG_LEN] = {};
uint8_t  keylogs_str_idx        = 0;
uint16_t log_timer              = 0;
static uint32_t oled_timer      = 0;

const char code_to_name[60] = {
    ' ', ' ', ' ', ' ', 'a', 'b', 'c', 'd', 'e', 'f',
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p',
    'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
    '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
    'R', 'E', '<', 'T', '_', '-', '=', '[', ']', '\\',
    '#', ';', '\'', '`', ',', '.', '/', ' ', ' ', ' '};

void add_keylog(uint16_t keycode) {
    if ((keycode >= QK_MOD_TAP && keycode <= QK_MOD_TAP_MAX) || (keycode >= QK_LAYER_TAP && keycode <= QK_LAYER_TAP_MAX)) {
        keycode = keycode & 0xFF;
    }

    for (uint8_t i = KEYLOG_LEN - 1; i > 0; i--) {
        keylog_str[i] = keylog_str[i - 1];
    }
    if (keycode < 60) {
        keylog_str[0] = code_to_name[keycode];
    }
    keylog_str[KEYLOG_LEN - 1] = 0;

    log_timer = timer_read();
}

void update_log(void) {
    if (timer_elapsed(log_timer) > 750) {
        add_keylog(0);
    }
}

void render_keylogger_status(void) {
    oled_write(keylog_str, false);
}

void render_klgr(void) {
    bool blink = (timer_read() % 1000) < 500;
    oled_write_ln_P(blink ? PSTR("~ _") : PSTR("~  "), false);
}

void render_qmk_logo(void) {
    static const char PROGMEM font_qmk_logo[16] = {0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0};
    oled_write_P(font_qmk_logo, false);
};

void render_keyboard(void) {
    static const char PROGMEM font_keyboard[16] = {0xba, 0xbb, 0xbc, 0xbd, 0xbe, 0xda, 0xdb, 0xdc, 0xdd, 0xde, 0};
    oled_write_P(font_keyboard, false);
};

void render_mod_ctrl(void) { // ^
    static const char PROGMEM font_ctrl[3] = {0x93, 0x94, 0};
    oled_write_P(font_ctrl, false);
};

void render_mod_alt(void) { // ⌥
    static const char PROGMEM font_alt[3] = {0xb3, 0xb4, 0};
    oled_write_P(font_alt, false);
};

void render_mod_shift(void) { // ⇧
    static const char PROGMEM font_shift[3] = {0xd3, 0xd4, 0};
    oled_write_P(font_shift, false);
};

void render_mod_gui(void) { // win symbol
    static const char PROGMEM font_gui[3] = {0x95, 0x96, 0};
    oled_write_P(font_gui, false);
};

void render_prompt(void) {
    bool blink = (timer_read() % 1000) < 500;
        if (layer_state_is(_LOWER)) {
        oled_write_ln_P(blink ? PSTR("> lo_") : PSTR("> lo "), false);
        } else if (layer_state_is(_RAISE)) {
        oled_write_ln_P(blink ? PSTR("> hi_") : PSTR("> hi "), false);
        } else {
        oled_write_ln_P(blink ? PSTR("> _ ") : PSTR(">     "), false);
        }
};

void render_mod_status(void) {
    bool blink = (timer_read() % 1000) < 500;
        uint8_t modifiers = get_mods() | get_oneshot_mods();
        if (modifiers & MOD_MASK_CTRL) {
            oled_write_ln_P(blink ? PSTR("$ ctl") : PSTR("$ _  "), false);
        } else if (modifiers & MOD_MASK_SHIFT) {
            oled_write_ln_P(blink ? PSTR("$ sft") : PSTR("$ _  "), false);
        } else if (modifiers & MOD_MASK_ALT) {
            oled_write_ln_P(blink ? PSTR("$ alt") : PSTR("$ _  "), false);
        } else if (modifiers & MOD_MASK_GUI) {
            oled_write_ln_P(blink ? PSTR("$ cmd") : PSTR("$ _  "), false);
        } else {
            oled_write_ln_P(blink ? PSTR("$ _  ") : PSTR("$    "), false);
        }
}

void render_keylock_status(uint8_t led_usb_state) {
    bool blink = (timer_read() % 1000) < 500;
        if (led_usb_state & (1 << USB_LED_CAPS_LOCK)) {
            oled_write_ln_P(blink ? PSTR("% cap") : PSTR("% _  "), false);
        } else if (led_usb_state & (1 << USB_LED_NUM_LOCK)) {
            oled_write_ln_P(blink ? PSTR("% num") : PSTR("% _  "), false);
        } else if (led_usb_state & (1 << USB_LED_SCROLL_LOCK)) {
            oled_write_ln_P(blink ? PSTR("% scr") : PSTR("% _  "), false);
        } else {
            oled_write_ln_P(blink ? PSTR("% _  ") : PSTR("%     "), false);
        }
}

#ifdef FELIX
static void render_felix_sit(void) {
	static const char PROGMEM sit[FELIX_FRAMES][FELIX_SIZE] = { {
#ifndef FILLED
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xe0,0x1c,
		0x02,0x05,0x02,0x24,0x04,0x04,0x02,0xa9,0x1e,0xe0,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0xe0,0x10,0x08,0x68,0x10,0x08,0x04,0x03,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x02,0x06,0x82,0x7c,0x03,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x02,0x04,0x0c,0x10,0x10,0x20,0x20,0x20,0x28,
		0x3e,0x1c,0x20,0x20,0x3e,0x0f,0x11,0x1f,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
	}, {
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xe0,0x1c,
		0x02,0x05,0x02,0x24,0x04,0x04,0x02,0xa9,0x1e,0xe0,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0xe0,0x90,0x08,0x18,0x60,0x10,0x08,0x04,0x03,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x02,0x0e,0x82,0x7c,0x03,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x02,0x04,0x0c,0x10,0x10,0x20,0x20,0x20,0x28,
		0x3e,0x1c,0x20,0x20,0x3e,0x0f,0x11,0x1f,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
#else
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xe0,0xfc,
		0xfe,0xfd,0xfe,0xdc,0xfc,0xfc,0xfe,0x5d,0xfe,0xe0,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0xe0,0xf0,0xf8,0x1c,0xf0,0xf8,0xfc,0xff,0xff,0xff,
		0xff,0xff,0xff,0xff,0xff,0xfd,0xf9,0xfd,0x7f,0x03,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x03,0x07,0x0f,0x1f,0x1f,0x3f,0x3f,0x3f,0x37,
		0x21,0x13,0x3f,0x3f,0x3f,0x0f,0x11,0x1f,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
	}, {
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xe0,0xfe,
		0xfd,0xfe,0xfc,0xdc,0xfc,0xfe,0xfd,0x5e,0xfc,0xe0,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0xe0,0xf0,0xf8,0xfc,0x80,0xf0,0xf8,0xfc,0xff,0xff,0xff,
		0xff,0xff,0xff,0xff,0xff,0xfd,0xf1,0xfd,0x7f,0x03,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x03,0x07,0x0f,0x1f,0x1f,0x3f,0x3f,0x3f,0x37,
		0x21,0x13,0x3f,0x3f,0x3f,0x0f,0x11,0x1f,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
#endif
	} };

	felix_current_frame = (felix_current_frame + 1) % FELIX_FRAMES;
	oled_write_raw_P(sit[abs(1 - felix_current_frame)], FELIX_SIZE);
}

static void render_felix_walk(void) {
	static const char PROGMEM walk[FELIX_FRAMES][FELIX_SIZE] = { {
#ifndef FILLED
		0x00,0x00,0x00,0x00,0x00,0x80,0x40,0x20,0x10,0x90,0x90,0x90,0xa0,0xc0,0x80,0x80,
		0x80,0x70,0x08,0x14,0x08,0x90,0x10,0x10,0x08,0xa4,0x78,0x80,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x07,0x08,0xfc,0x01,0x00,0x00,0x00,0x00,0x80,0x00,0x00,
		0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x18,0xea,0x10,0x0f,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x1c,0x20,0x20,0x3c,0x0f,0x11,0x1f,0x03,
		0x06,0x18,0x20,0x20,0x3c,0x0c,0x12,0x1e,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00
	}, {
		0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x40,0x20,0x20,0x20,0x40,0x80,0x00,0x00,0x00,
		0x00,0xe0,0x10,0x28,0x10,0x20,0x20,0x20,0x10,0x48,0xf0,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x1f,0x20,0xf8,0x02,0x01,0x01,0x01,0x01,0x01,0x01,0x01,
		0x03,0x00,0x00,0x00,0x00,0x01,0x00,0x10,0x30,0xd5,0x20,0x1f,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3f,0x20,0x30,0x0c,0x02,0x05,0x09,0x12,0x1e,
		0x02,0x1c,0x14,0x08,0x10,0x20,0x2c,0x32,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00
#else
		0x00,0x00,0x00,0x00,0x00,0x80,0xc0,0xe0,0xf0,0xf0,0x70,0x30,0x20,0x00,0x80,0xc0,
		0xc0,0xf0,0xf8,0xf4,0xf8,0x70,0xf0,0xf0,0xf8,0x74,0xf8,0x80,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x07,0x0f,0xff,0xfd,0xfe,0xff,0xff,0xff,0xff,0xff,0xff,
		0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xf7,0xe7,0xf5,0x1f,0x0f,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x1f,0x3f,0x3f,0x3f,0x0f,0x11,0x1f,0x1f,
		0x03,0x07,0x1f,0x3f,0x3f,0x0f,0x13,0x1f,0x1f,0x00,0x00,0x00,0x00,0x00,0x00,0x00
	}, {
		0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xc0,0xe0,0xe0,0xe0,0xc0,0x80,0x00,0x00,0x00,
		0x00,0xe0,0xf0,0xe8,0xf0,0xe0,0xe0,0xe0,0xf0,0xe8,0xf0,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x1f,0x3f,0xfb,0xfd,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,
		0xff,0xff,0xff,0xff,0xff,0xfe,0xff,0xef,0xcf,0xea,0x3f,0x1f,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3f,0x3f,0x3f,0x0f,0x03,0x07,0x0f,0x1f,0x03,
		0x1f,0x1f,0x17,0x0f,0x1f,0x3f,0x3f,0x33,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00
#endif
	} };

	felix_current_frame = (felix_current_frame + 1) % FELIX_FRAMES;
	oled_write_raw_P(walk[abs(1 - felix_current_frame)], FELIX_SIZE);
}

static void render_felix_run(void) {
	static const char PROGMEM run[FELIX_FRAMES][FELIX_SIZE] = { {
#ifndef FILLED
		0x00,0x00,0x00,0x00,0xe0,0x10,0x08,0x08,0xc8,0xb0,0x80,0x80,0x80,0x80,0x80,0x80,
		0x80,0x40,0x40,0x3c,0x14,0x04,0x08,0x90,0x18,0x04,0x08,0xb0,0x40,0x80,0x00,0x00,
		0x00,0x00,0x00,0x00,0x01,0x02,0xc4,0xa4,0xfc,0x00,0x00,0x00,0x00,0x80,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xc8,0x58,0x28,0x2a,0x10,0x0f,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x0e,0x09,0x04,0x04,0x04,0x04,0x02,0x03,0x02,0x01,0x01,
		0x02,0x02,0x04,0x08,0x10,0x26,0x2b,0x32,0x04,0x05,0x06,0x00,0x00,0x00,0x00,0x00
	}, {
		0x00,0x00,0x00,0xe0,0x10,0x10,0xf0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,
		0x80,0x80,0x78,0x28,0x08,0x10,0x20,0x30,0x08,0x10,0x20,0x40,0x80,0x00,0x00,0x00,
		0x00,0x00,0x00,0x03,0x04,0x08,0x10,0x11,0xf9,0x01,0x01,0x01,0x01,0x01,0x01,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x10,0xb0,0x50,0x55,0x20,0x1f,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x02,0x0c,0x10,0x20,0x28,0x37,
		0x02,0x1e,0x20,0x20,0x18,0x0c,0x14,0x1e,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00
#else
		0x00,0x00,0x00,0x00,0xe0,0xf0,0xf8,0xf8,0xf8,0xb0,0xa0,0x80,0x80,0x80,0x80,0x80,
		0x80,0xc0,0xc0,0xfc,0xf4,0xe4,0xf8,0x70,0xf8,0xe4,0xf8,0x70,0xc0,0x80,0x00,0x00,
		0x00,0x00,0x00,0x00,0x01,0x63,0xe7,0xe7,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
		0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xf7,0x67,0x37,0x35,0x1f,0x0f,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x0e,0x0f,0x07,0x07,0x07,0x07,0x03,0x03,0x03,0x01,0x01,
		0x03,0x03,0x07,0x0f,0x1f,0x3f,0x3b,0x33,0x07,0x07,0x06,0x00,0x00,0x00,0x00,0x00
	}, {
		0x00,0x00,0x00,0xe0,0xf0,0xf0,0xf8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,
		0x80,0x80,0xf8,0xe8,0xc8,0xf0,0xe0,0xf0,0xc8,0xf0,0xe0,0xc0,0x80,0x00,0x00,0x00,
		0x00,0x00,0x00,0x03,0x07,0x0f,0x1f,0x1f,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
		0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xfe,0xef,0xcf,0x6f,0x6a,0x3f,0x1f,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x03,0x0f,0x1f,0x3f,0x3f,0x37,
		0x03,0x1f,0x3f,0x3f,0x1f,0x0f,0x1f,0x3f,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00
#endif
	} };

	felix_current_frame = (felix_current_frame + 1) % FELIX_FRAMES;
	oled_write_raw_P(run[abs(1 - felix_current_frame)], FELIX_SIZE);
}

static void render_felix_bark(void) {
	static const char PROGMEM bark[FELIX_FRAMES][FELIX_SIZE] = { {
#ifndef FILLED
		0x00,0xc0,0x20,0x10,0xd0,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0x40,
		0x3c,0x14,0x04,0x08,0x90,0x18,0x04,0x08,0xb0,0x40,0x80,0x00,0x00,0x00,0x00,0x00,
		0x00,0x03,0x04,0x08,0x10,0x11,0xf9,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x80,0xc8,0x48,0x28,0x2a,0x10,0x0f,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x02,0x0c,0x10,0x20,0x28,0x37,0x02,0x02,
		0x04,0x08,0x10,0x26,0x2b,0x32,0x04,0x05,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00
	}, {
		0x00,0xe0,0x10,0x10,0xf0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0x40,
		0x40,0x2c,0x14,0x04,0x08,0x90,0x18,0x04,0x08,0xb0,0x40,0x80,0x00,0x00,0x00,0x00,
		0x00,0x03,0x04,0x08,0x10,0x11,0xf9,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x80,0xc0,0x48,0x28,0x2a,0x10,0x0f,0x20,0x4a,0x09,0x10,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x02,0x0c,0x10,0x20,0x28,0x37,0x02,0x02,
		0x04,0x08,0x10,0x26,0x2b,0x32,0x04,0x05,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00
#else
		0x00,0xc0,0xe0,0xf0,0xf0,0x30,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0xc0,
		0xfc,0xf4,0xe4,0xf8,0x70,0xf8,0xe4,0xf8,0x70,0xc0,0x80,0x00,0x00,0x00,0x00,0x00,
		0x00,0x03,0x07,0x0f,0x1f,0x1f,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
		0xff,0xff,0xff,0xff,0xff,0xf7,0xf7,0x37,0x35,0x1f,0x0f,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x03,0x0f,0x1f,0x3f,0x3f,0x37,0x03,0x03,
		0x07,0x0f,0x1f,0x3f,0x3b,0x33,0x07,0x07,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00
	}, {
		0x00,0xe0,0xf0,0xf0,0xf0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0xc0,
		0xc0,0xec,0xf4,0xe4,0xf8,0x70,0xf8,0xe4,0xf8,0x70,0xc0,0x80,0x00,0x00,0x00,0x00,
		0x00,0x03,0x07,0x0f,0x1f,0x1f,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
		0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x77,0x37,0x35,0x1f,0x0f,0x20,0x4a,0x09,0x10,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x03,0x0f,0x1f,0x3f,0x3f,0x37,0x03,0x03,
		0x07,0x0f,0x1f,0x3f,0x3b,0x33,0x07,0x07,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00
#endif
	} };

	felix_current_frame = (felix_current_frame + 1) % FELIX_FRAMES;
	oled_write_raw_P(bark[abs(1 - felix_current_frame)], FELIX_SIZE);
}

static void render_felix_sneak(void) {
	static const char PROGMEM sneak[FELIX_FRAMES][FELIX_SIZE] = { {
#ifndef FILLED
		0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x40,0x40,0x40,0x40,0x80,0x00,0x00,0x00,0x00,
		0x00,0x00,0xc0,0x40,0x40,0x80,0x00,0x80,0x40,0x80,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x1e,0x21,0xf0,0x04,0x02,0x02,0x02,0x02,0x03,0x02,0x02,0x04,
		0x04,0x04,0x03,0x01,0x00,0x00,0x09,0x01,0x80,0x80,0xab,0x04,0xf8,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x1c,0x20,0x20,0x3c,0x0f,0x11,0x1f,0x02,0x06,
		0x18,0x20,0x20,0x38,0x08,0x10,0x18,0x04,0x04,0x02,0x02,0x01,0x00,0x00,0x00,0x00
	}, {
		0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x40,0x40,0x40,0x80,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0xe0,0xa0,0x20,0x40,0x80,0xc0,0x20,0x40,0x80,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x3e,0x41,0xf0,0x04,0x02,0x02,0x02,0x03,0x02,0x02,0x02,0x04,
		0x04,0x02,0x01,0x00,0x00,0x00,0x04,0x00,0x40,0x40,0x55,0x82,0x7c,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x3f,0x20,0x30,0x0c,0x02,0x05,0x09,0x12,0x1e,0x04,
		0x18,0x10,0x08,0x10,0x20,0x28,0x34,0x06,0x02,0x01,0x01,0x00,0x00,0x00,0x00,0x00
#else
		0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xc0,0xc0,0xc0,0xc0,0x80,0x00,0x00,0x00,0x00,
		0x00,0x00,0xc0,0x40,0x40,0x80,0x00,0x80,0x40,0x80,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x1e,0x3f,0xff,0xfb,0xfd,0xfd,0xfd,0xfd,0xfd,0xfc,0xfc,0xfc,
		0xfc,0xfe,0xff,0xff,0xfe,0xff,0xf7,0xff,0x7e,0x7f,0x57,0xfc,0xf8,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x1f,0x3f,0x3f,0x3f,0x0f,0x1f,0x1f,0x03,0x07,
		0x1f,0x3f,0x3f,0x3f,0x0f,0x1f,0x1f,0x07,0x07,0x03,0x03,0x01,0x00,0x00,0x00,0x00
	}, {
		0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xc0,0xc0,0xc0,0x80,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0xe0,0xa0,0x20,0xc0,0x80,0xc0,0x20,0xc0,0x80,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x3e,0x7f,0xf7,0xfb,0xfd,0xfd,0xfd,0xfd,0xfc,0xfc,0xfc,0xfc,
		0xfc,0xfe,0xff,0xff,0xff,0xff,0xfb,0xff,0xbf,0xbf,0xab,0xfe,0x7c,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x3f,0x3f,0x3f,0x0f,0x03,0x07,0x0f,0x1f,0x1f,0x07,
		0x1f,0x1f,0x0f,0x1f,0x3f,0x3f,0x37,0x07,0x03,0x01,0x01,0x00,0x00,0x00,0x00,0x00
#endif
	} };

	felix_current_frame = (felix_current_frame + 1) % FELIX_FRAMES;
	oled_write_raw_P(sneak[abs(1 - felix_current_frame)], FELIX_SIZE);
}

static void render_felix_status(void) {
    oled_set_cursor(0, 7);
	void render_phase(void) {
		oled_clear();
		if (get_mods() & MOD_MASK_SHIFT || host_keyboard_led_state().caps_lock) { render_felix_bark(); }
		else if (get_mods() & MOD_MASK_CAG) { render_felix_sneak(); }
		else if (get_current_wpm() >MIN_RUN_SPEED && felix_typing) { render_felix_run(); }
		else if (get_current_wpm() >MIN_WALK_SPEED && felix_typing) { render_felix_walk(); }
		else { render_felix_sit(); }

		if (!(felix_anim_timer%2)) {
			if (get_current_wpm() >=felix_prev_wpm) {
				felix_prev_wpm = get_current_wpm();
				felix_typing = true;
			} else {
				felix_prev_wpm = get_current_wpm()+1;
				felix_typing = false;
			}
		}
	}

	if (get_current_wpm() >0 || get_mods() & MOD_MASK_CSAG) {
		oled_on();
		if (timer_elapsed32(felix_anim_timer) >FELIX_FRAME_DURATION) {
			felix_anim_timer = timer_read32();
			render_phase();
		}
		felix_anim_sleep = timer_read32();
	} else {
		if (timer_elapsed32(felix_anim_sleep) >OLED_TIMEOUT) {
			oled_off();
		} else {
			if (timer_elapsed32(felix_anim_timer) >FELIX_FRAME_DURATION) {
				felix_anim_timer = timer_read32();
				render_phase();
			}
		}
	}
}
#endif

#ifdef BONGO
static void render_bongo(void) {
    static const char PROGMEM idle[IDLE_FRAMES][BONGO_SIZE] = {

        {
          0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,192,192,224,224,224,224,240,240,240,240,240,248,248,252,252,252,248,240,192,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,128,
          0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 24,124,254,254,254,254,254,254,255,255,255,255,255,127,127,255,255,255,255,255,255,255,255,255,127,255,207,207,255, 63, 63, 63, 62,252,248,240,224,192,128,  0,  0,  0,128,128,128,128, 64, 64, 64, 64, 32, 32, 32, 32, 16, 16, 16, 16,  8,  8,  8,  8,  8,  4,  4,  4,  4,  4,  2,  2,  2,  2,  1,  1,  1,  1,  0,  0,  0,
          0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,192,248,252,255,255,255,255,255,255,255,255,243,243,243,242,254,255,255,191, 62, 61, 29, 30, 30, 30, 14, 15, 15, 15, 15,  7,  7, 15, 15, 31, 31, 31, 31, 31, 31, 15,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
          0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,128,128,128, 64, 64, 64, 64, 64, 32, 32, 32, 32, 32, 16, 16, 16, 16, 16,  8,  8,  8,  8,  8,  4,  4,  4,  4,  4,  2,  3,  3,  3,  1,  1,  1,  1,  1,  1,  3,  7,  7,  7, 15, 15, 15, 15, 15,  7,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        },

        {
          0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,192,192,224,224,224,224,240,240,240,240,240,248,248,252,252,252,248,240,192,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,128,
          0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 24,124,254,254,254,254,254,254,255,255,255,255,255,127,127,255,255,255,255,255,255,255,255,255,127,255,207,207,255, 63, 63, 63, 62,252,248,240,224,192,128,  0,  0,  0,128,128,128,128, 64, 64, 64, 64, 32, 32, 32, 32, 16, 16, 16, 16,  8,  8,  8,  8,  8,  4,  4,  4,  4,  4,  2,  2,  2,  2,  1,  1,  1,  1,  0,  0,  0,
          0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,192,248,252,255,255,255,255,255,255,255,255,243,243,243,242,254,255,255,191, 62, 61, 29, 30, 30, 30, 14, 15, 15, 15, 15,  7,  7, 15, 15, 31, 31, 31, 31, 31, 31, 15,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
          0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,128,128,128, 64, 64, 64, 64, 64, 32, 32, 32, 32, 32, 16, 16, 16, 16, 16,  8,  8,  8,  8,  8,  4,  4,  4,  4,  4,  2,  3,  3,  3,  1,  1,  1,  1,  1,  1,  3,  3,  7,  7, 15, 15, 15, 15, 15,  7,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        },

        {
          0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,192,192,192,192,224,224,224,224,240,248,252,254,254,252,248,224,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,128,
          0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 60,254,255,255,254,254,252,252,254,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,159,159,255,127,126,126,124,248,240,224,192,128,  0,  0,  0,  0,128,128,128,128, 64, 64, 64, 64, 32, 32, 32, 32, 16, 16, 16, 16,  8,  8,  8,  8,  8,  4,  4,  4,  4,  4,  2,  2,  2,  2,  1,  1,  1,  1,  0,  0,  0,
          0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,240,249,255,255,255,255,255,255,255,255,231,231,231,228,252,255,255,191, 61, 59, 27, 29, 29, 29, 12, 15, 15, 15, 15,  6,  6, 14, 14, 31, 31, 31, 31, 31, 31, 15,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
          0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,128,128,128, 64, 64, 64, 64, 64, 32, 32, 32, 32, 32, 16, 16, 16, 16, 16,  8,  8,  8,  8,  8,  4,  4,  4,  4,  4,  2,  3,  3,  3,  1,  1,  1,  1,  1,  1,  3,  3,  7,  7, 15, 15, 15, 15, 15,  7,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        },

        {
          0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,  0,  0,  0,  0,  0,128,192,192,224,224,224,224,240,240,240,240,248,252,254,255,255,254,252,240,192,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,128,
          0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 30,255,255,255,255,255,254,254,255,255,255,255,255,127,127,255,255,255,255,255,255,255,255,255,127,255,207,207,255, 63, 63, 63, 62,252,248,240,224,192,128,  0,  0,  0,128,128,128,128, 64, 64, 64, 64, 32, 32, 32, 32, 16, 16, 16, 16,  8,  8,  8,  8,  8,  4,  4,  4,  4,  4,  2,  2,  2,  2,  1,  1,  1,  1,  0,  0,  0,
          0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,240,252,255,255,255,255,255,255,255,255,243,243,243,242,254,255,255,191, 62, 61, 29, 30, 30, 30, 14, 15, 15, 15, 15,  7,  7, 15, 15, 31, 31, 31, 31, 31, 31, 15,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
          0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,128,128,128, 64, 64, 64, 64, 64, 32, 32, 32, 32, 32, 16, 16, 16, 16, 16,  8,  8,  8,  8,  8,  4,  4,  4,  4,  4,  2,  3,  3,  3,  1,  1,  1,  1,  1,  1,  3,  3,  7,  7, 15, 15, 15, 15, 15,  7,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        },

        {
          0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,192,192,224,224,224,224,240,240,240,240,248,248,252,254,254,254,252,248,192,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,128,
          0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 28,254,255,255,254,254,254,254,255,255,255,255,255,127,127,255,255,255,255,255,255,255,255,255,127,255,207,207,255, 63, 63, 63, 62,252,248,240,224,192,128,  0,  0,  0,128,128,128,128, 64, 64, 64, 64, 32, 32, 32, 32, 16, 16, 16, 16,  8,  8,  8,  8,  8,  4,  4,  4,  4,  4,  2,  2,  2,  2,  1,  1,  1,  1,  0,  0,  0,
          0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,240,252,255,255,255,255,255,255,255,255,243,243,243,242,254,255,255,191, 62, 61, 29, 30, 30, 30, 14, 15, 15, 15, 15,  7,  7, 15, 15, 31, 31, 31, 31, 31, 31, 15,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
          0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,128,128,128, 64, 64, 64, 64, 64, 32, 32, 32, 32, 32, 16, 16, 16, 16, 16,  8,  8,  8,  8,  8,  4,  4,  4,  4,  4,  2,  3,  3,  3,  1,  1,  1,  1,  1,  1,  3,  3,  7,  7, 15, 15, 15, 15, 15,  7,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        }

    };

    static const char PROGMEM prep[][BONGO_SIZE] = {

        {
          0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,  0,  0,  0,  0,  0,128,192,192,224,224,224,224,240,240,240,240,248,252,254,255,255,254,252,240,192,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,128,
          0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 30,255,255,255,255,255,254,254,127,127,127,255,255,127,127,255,255,255,255,255,255,255,255,255,127,255,207,207,255,255,255, 31,230,250,247,207,203,255,246,124,  0,  0,128,128,128,128, 64, 64, 64, 64, 32, 32, 32, 32, 16, 16, 16, 16,  8,  8,  8,  8,  8,  4,  4,  4,  4,  4,  2,  2,  2,  2,  1,  1,  1,  1,  0,  0,  0,
          0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,240,252,255,255,255,231,249,250,231,230,251,252, 67,126,126,127,127, 63, 62, 61, 29, 30, 30, 30, 14, 15, 15, 15, 15,  7,  7,  7,  7,  7,  7,  3,  3,  3,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
          0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,128,128,128, 64, 64, 64, 64, 64, 32, 32, 32, 32, 32, 16, 16, 16, 16, 16,  8,  8,  8,  8,  8,  4,  4,  4,  4,  4,  2,  3,  3,  3,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        }

    };

    static const char PROGMEM tap[TAP_FRAMES][BONGO_SIZE] = {

        {
          0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,  0,  0,  0,  0,  0,128,192,192,224,224,224,224,240,240,240,240,248,252,254,255,255,254,252,240,192,128,  0,  0,  0,  0,  0,  0,  0,248,248,248,248,  0,  0,  0,  0,  0,128,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,128,
          0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 30,255,255,255,255,255,254,254,127,127,127,255,255,127,127,255,255,255,255,255,255,255,255,255,127,255,207,207,255,255,255,255,254,252,248,240,224,195,135,  7,  1,  0,184,188,190,159, 95, 95, 79, 76, 32, 32, 32, 32, 16, 16, 16, 16,  8,  8,  8,  8,  8,  4,  4,  4,  4,  4,  2,  2,  2,  2,  1,  1,  1,  1,  0,  0,  0,
          0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,240,252,255,255,255,231,249,250,231,230,251,124, 67,126,126,127,127, 63, 62, 61, 29, 30, 30, 30, 14, 15, 15, 15, 15,  7,  7, 15, 15, 31, 31, 31, 31, 31, 31, 15,  1, 61,124,252,252,252,252,252, 60, 12,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
          0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,128,128,128, 64, 64, 64, 64, 64, 32, 32, 32, 32, 32, 16, 16, 16, 16, 16,  8,  8,  8,  8,  8,  4,  4,  4,  4,  4,  2,  3,  3,  3,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  3,  3,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        },

        {
          0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,  0,  0,  0,  0,  0,128,192,192,224,224,224,224,240,240,240,240,248,252,254,255,255,254,252,240,192,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,128,
          0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 30,255,255,255,255,255,254,254,255,255,255,255,255,127,127,255,255,255,255,255,255,255,255,255,127,255,207,207,255,255,255, 31,230,250,247,207,203,255,246,124,  0,  0,128,128,128,128, 64, 64, 64, 64, 32, 32, 32, 32, 16, 16, 16, 16,  8,  8,  8,  8,  8,  4,  4,  4,  4,  4,  2,  2,  2,  2,  1,  1,  1,  1,  0,  0,  0,
          0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,240,252,255,255,255,255,255,255,255,255,255,255,255,254,254,255,255,191, 62, 61, 29, 30, 30, 30, 14, 15, 15, 15, 15,  7,  7,  7,  7,  7,  7,  3,  3,  3,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
          0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,128,128,128, 64, 64, 64, 64, 64, 32, 32, 32, 32, 32, 16, 16, 16, 16, 16,  8,  8,  8,  8,  8,  4,  4,  4,  4,  4,  2,  3,123,123,121,121,121,121, 57, 49,  3,  3,  7,  7, 15, 15, 15,143,143,135,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        }

    };

    void animation_phase(void) {

        if (get_current_wpm() <=IDLE_SPEED) {
            current_idle_frame = (current_idle_frame + 1) % IDLE_FRAMES;
            oled_write_raw_P(idle[abs((IDLE_FRAMES-1)-current_idle_frame)], BONGO_SIZE);
        }

        if (get_current_wpm() >IDLE_SPEED && get_current_wpm() <TAP_SPEED) {
            oled_write_raw_P(prep[0], BONGO_SIZE);
        }

        if (get_current_wpm() >=TAP_SPEED) {
            current_tap_frame = (current_tap_frame + 1) % TAP_FRAMES;
            oled_write_raw_P(tap[abs((TAP_FRAMES-1)-current_tap_frame)], BONGO_SIZE);
        }
    }

    if (get_current_wpm() != 000) {
        oled_on();

        if (timer_elapsed32(bongo_anim_timer) > ANIM_FRAME_DURATION) {
            bongo_anim_timer = timer_read32();
            animation_phase();
        }

        bongo_anim_sleep = timer_read32();
    } else {
        if (timer_elapsed32(bongo_anim_sleep) > OLED_TIMEOUT) {
            oled_off();
        } else {
            if (timer_elapsed32(bongo_anim_timer) > ANIM_FRAME_DURATION) {
                bongo_anim_timer = timer_read32();
                animation_phase();
            }
        }
    }
}
#endif

void render_main(void) {
    if (get_current_wpm() != 000) {
        update_log();
        oled_set_cursor(0, 0);
        render_wpm();
        oled_set_cursor(0, 3);
        render_qmk_logo();
        oled_set_cursor(0, 7);
        render_keyboard();
        oled_set_cursor(0, 9);
        render_prompt();
        oled_set_cursor(0, 11);
        render_mod_status();
        oled_set_cursor(0, 13);
        render_keylock_status(host_keyboard_leds());
        oled_set_cursor(1, 15);
        render_keylogger_status();
        oled_set_cursor(0, 15);
        render_klgr();
    } else {
        oled_off();
    }
}

void oled_task_user(void) {
#ifdef SPLIT_KEYBOARD
  if (is_keyboard_master())
#else
  if (is_master)
#endif
    {
    render_main();
    } else {
#ifdef FELIX
    render_felix_status();
#endif
#ifdef BONGO
    render_bongo();
    }
#endif
}

bool process_record_user_oled(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
#ifdef OLED_DRIVER_ENABLE
        oled_timer = timer_read32();
        add_keylog(keycode);
#endif
    }
    return true;
}
