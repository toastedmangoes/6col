MCU = STM32F303
SERIAL_DRIVER = usart
WS2812_DRIVER = pwm
MOUSEKEY_ENABLE = yes
#RGBLIGHT_ENABLE = yes
VIA_ENABLE = yes
OLED_DRIVER_ENABLE = yes
AUDIO_ENABLE = no
GRAVE_ESC_ENABLE = no
EXTRAKEY_ENABLE = yes
WPM_ENABLE = no
COMMAND_ENABLE = no
UNICODE_ENABLE = no
UNICODEMAP_ENABLE = yes
AUTO_SHIFT_ENABLE = yes
SWAP_HANDS_ENABLE = no
CONSOLE_ENABLE = yes
COMBO_ENABLE = yes
BOOTMAGIC_ENABLE = lite
TAP_DANCE_ENABLE = yes
POINTING_DEVICE_ENABLE = yes

ifdef POINTING_DEVICE_ENABLE
	SRC += i2c_master.c
endif

ifeq ($strip $(WAFFLE)), yes)
	OPT_DEFS += -DWAFFLE
endif

