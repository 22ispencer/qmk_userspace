// Copyright 2018-2024 Nick Brassel (@tzarc)
// Copyright 2024 Drashna (@drashna)
// SPDX-License-Identifier: GPL-2.0-or-later

#include "drashna.h"
#include <printf.h>
#include "display/painter/menu.h"
#include "display/painter/painter.h"
#include "process_keycode/process_unicode_common.h"
#include "unicode.h"

#ifdef AUDIO_ENABLE
#    include "audio.h"

#    ifndef AG_NORM_SONG
#        define AG_NORM_SONG SONG(AG_NORM_SOUND)
#    endif
#    ifndef AG_SWAP_SONG
#        define AG_SWAP_SONG SONG(AG_SWAP_SOUND)
#    endif
#    ifndef CG_NORM_SONG
#        define CG_NORM_SONG SONG(AG_NORM_SOUND)
#    endif
#    ifndef CG_SWAP_SONG
#        define CG_SWAP_SONG SONG(AG_SWAP_SOUND)
#    endif
static float ag_norm_song[][2] = AG_NORM_SONG;
static float ag_swap_song[][2] = AG_SWAP_SONG;
static float cg_norm_song[][2] = CG_NORM_SONG;
static float cg_swap_song[][2] = CG_SWAP_SONG;
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Display options

static bool menu_handler_display(menu_input_t input) {
    switch (input) {
        case menu_input_left:
            userspace_config.display_mode = (userspace_config.display_mode - 1) % 3;
            if (userspace_config.display_mode > 2) {
                userspace_config.display_mode = 2;
            }
            eeconfig_update_user_config(&userspace_config.raw);
            return false;
        case menu_input_right:
            userspace_config.display_mode = (userspace_config.display_mode + 1) % 3;
            eeconfig_update_user_config(&userspace_config.raw);
            return false;
        default:
            return true;
    }
}

void display_handler_display(char *text_buffer, size_t buffer_len) {
    switch (userspace_config.display_mode) {
        case 0:
            strncpy(text_buffer, "Console", buffer_len - 1);
            return;
        case 1:
            strncpy(text_buffer, "Layer Map", buffer_len - 1);
            return;
        case 2:
            strncpy(text_buffer, "Fonts", buffer_len - 1);
            return;
    }

    strncpy(text_buffer, "Unknown", buffer_len);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Unicode

#ifdef UNICODE_COMMON_ENABLE
static bool menu_handler_unicode(menu_input_t input) {
    switch (input) {
        case menu_input_left:
            unicode_input_mode_step_reverse();
            return false;
        case menu_input_right:
            unicode_input_mode_step();
            return false;
        default:
            return true;
    }
}

void display_handler_unicode(char *text_buffer, size_t buffer_len) {
    switch (get_unicode_input_mode()) {
        case UNICODE_MODE_MACOS:
            strncpy(text_buffer, "macOS", buffer_len - 1);
            return;
        case UNICODE_MODE_LINUX:
            strncpy(text_buffer, "Linux", buffer_len - 1);
            return;
        case UNICODE_MODE_BSD:
            strncpy(text_buffer, "BSD", buffer_len - 1);
            return;
        case UNICODE_MODE_WINDOWS:
            strncpy(text_buffer, "Windows", buffer_len - 1);
            return;
        case UNICODE_MODE_WINCOMPOSE:
            strncpy(text_buffer, "WinCompose", buffer_len - 1);
            return;
        case UNICODE_MODE_EMACS:
            strncpy(text_buffer, "Emacs", buffer_len - 1);
            return;
    }

    strncpy(text_buffer, "Unknown", buffer_len);
}

static bool menu_handler_unicode_typing(menu_input_t input) {
    switch (input) {
        case menu_input_left:
            unicode_typing_mode = (unicode_typing_mode - 1) % UNICODE_MODE_COUNT;
            return false;
        case menu_input_right:
            unicode_typing_mode = (unicode_typing_mode + 1) % UNICODE_MODE_COUNT;
            return false;
        default:
            return true;
    }
}
void display_handler_unicode_typing(char *text_buffer, size_t buffer_len) {
    switch (unicode_typing_mode) {
        case UCTM_NO_MODE:
            strncpy(text_buffer, "Normal", buffer_len - 1); // Normal
            return;
        case UCTM_WIDE:
            strncpy(text_buffer, "Wide", buffer_len - 1); // ｗｉｄｅ
            return;
        case UCTM_SCRIPT:
            strncpy(text_buffer, "Script", buffer_len - 1); // 𝓢𝓬𝓻𝓲𝓹𝓽
            return;
        case UCTM_BLOCKS:
            strncpy(text_buffer, "Blocks", buffer_len - 1); // 🅱🅻🅾🅲🅺🆂
            return;
        case UCTM_REGIONAL:
            strncpy(text_buffer, "Regional",
                    buffer_len - 1); // 🇷‌‌🇪‌‌🇬‌‌🇮‌‌🇴‌‌🇳‌‌🇦‌‌🇱‌‌
            return;
        case UCTM_AUSSIE:
            strncpy(text_buffer, "Aussie", buffer_len - 1); // ǝᴉssnɐ
            return;
        case UCTM_ZALGO:
            strncpy(text_buffer, "Zalgo", buffer_len - 1); // z̴̬̙̐̋͢ā̸̧̺͂ͥ͐͟l̵̪̻͎̈ͭ̋͠g̦ͥo͚ͫͣ
            return;
        case UCTM_SUPER:
            strncpy(text_buffer, "SuperScript", buffer_len - 1); // ˢᵘᵖᵉʳˢᶜʳᶦᵖᵗ
            return;
        case UCTM_COMIC:
            strncpy(text_buffer, "Comic", buffer_len - 1); // ƈơɱıƈ
            return;
        case UCTM_FRAKTUR:
            strncpy(text_buffer, "Fraktur", buffer_len - 1); // 𝔉𝔯𝔞𝔨𝔱𝔲𝔯
            return;
        case UCTM_DOUBLE_STRUCK:
            strncpy(text_buffer, "Double Struck", buffer_len - 1); // 𝕯𝖔𝖚𝖇𝖑𝖊 𝕾𝖙𝖗𝖚𝖈𝖐
            return;
    }

    strncpy(text_buffer, "Unknown", buffer_len);
}

menu_entry_t unicode_entries[] = {
    {
        .flags                 = menu_flag_is_value,
        .text                  = "Unicode mode",
        .child.menu_handler    = menu_handler_unicode,
        .child.display_handler = display_handler_unicode,
    },
    {
        .flags                 = menu_flag_is_value,
        .text                  = "Unicode Typing Mode",
        .child.menu_handler    = menu_handler_unicode_typing,
        .child.display_handler = display_handler_unicode_typing,
    },
};
#endif // UNICODE_COMMON_ENABLE

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// RGB Matrix

#ifdef RGB_MATRIX_ENABLE
static bool menu_handler_rm_enabled(menu_input_t input) {
    switch (input) {
        case menu_input_left:
        case menu_input_right:
            rgb_matrix_toggle();
            return false;
        default:
            return true;
    }
}

void display_handler_rm_enabled(char *text_buffer, size_t buffer_len) {
    snprintf(text_buffer, buffer_len - 1, "%s", rgb_matrix_is_enabled() ? "on" : "off");
}

static bool menu_handler_rm_mode(menu_input_t input) {
    switch (input) {
        case menu_input_left:
            rgb_matrix_step_reverse();
            return false;
        case menu_input_right:
            rgb_matrix_step();
            return false;
        default:
            return true;
    }
}

void display_handler_rm_mode(char *text_buffer, size_t buffer_len) {
    snprintf(text_buffer, buffer_len - 1, "%s", rgb_matrix_get_effect_name());
}

static bool menu_handler_rm_hue(menu_input_t input) {
    switch (input) {
        case menu_input_left:
            rgb_matrix_decrease_hue();
            return false;
        case menu_input_right:
            rgb_matrix_increase_hue();
            return false;
        default:
            return true;
    }
}

void display_handler_rm_hue(char *text_buffer, size_t buffer_len) {
    snprintf(text_buffer, buffer_len - 1, "%d", (int)rgb_matrix_get_hue());
}

static bool menu_handler_rm_sat(menu_input_t input) {
    switch (input) {
        case menu_input_left:
            rgb_matrix_decrease_sat();
            return false;
        case menu_input_right:
            rgb_matrix_increase_sat();
            return false;
        default:
            return true;
    }
}

void display_handler_rm_sat(char *text_buffer, size_t buffer_len) {
    snprintf(text_buffer, buffer_len - 1, "%d", (int)rgb_matrix_get_sat());
}

static bool menu_handler_rm_val(menu_input_t input) {
    switch (input) {
        case menu_input_left:
            rgb_matrix_decrease_val();
            return false;
        case menu_input_right:
            rgb_matrix_increase_val();
            return false;
        default:
            return true;
    }
}

void display_handler_rm_val(char *text_buffer, size_t buffer_len) {
    snprintf(text_buffer, buffer_len - 1, "%d", (int)rgb_matrix_get_val());
}

static bool menu_handler_rm_speed(menu_input_t input) {
    switch (input) {
        case menu_input_left:
            rgb_matrix_decrease_speed();
            return false;
        case menu_input_right:
            rgb_matrix_increase_speed();
            return false;
        default:
            return true;
    }
}

void display_handler_rm_speed(char *text_buffer, size_t buffer_len) {
    snprintf(text_buffer, buffer_len - 1, "%d", (int)rgb_matrix_get_speed());
}

menu_entry_t rgb_matrix_entries[] = {
    {
        .flags                 = menu_flag_is_value,
        .text                  = "RGB Enabled",
        .child.menu_handler    = menu_handler_rm_enabled,
        .child.display_handler = display_handler_rm_enabled,
    },
    {
        .flags                 = menu_flag_is_value,
        .text                  = "RGB Mode",
        .child.menu_handler    = menu_handler_rm_mode,
        .child.display_handler = display_handler_rm_mode,
    },
    {
        .flags                 = menu_flag_is_value,
        .text                  = "RGB Hue",
        .child.menu_handler    = menu_handler_rm_hue,
        .child.display_handler = display_handler_rm_hue,
    },
    {
        .flags                 = menu_flag_is_value,
        .text                  = "RGB Saturation",
        .child.menu_handler    = menu_handler_rm_sat,
        .child.display_handler = display_handler_rm_sat,
    },
    {
        .flags                 = menu_flag_is_value,
        .text                  = "RGB Value",
        .child.menu_handler    = menu_handler_rm_val,
        .child.display_handler = display_handler_rm_val,
    },
    {
        .flags                 = menu_flag_is_value,
        .text                  = "RGB Speed",
        .child.menu_handler    = menu_handler_rm_speed,
        .child.display_handler = display_handler_rm_speed,
    },
};
#endif // RGB_MATRIX_ENABLE

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// RGB Light

#ifdef RGBLIGHT_ENABLE
static bool menu_handler_rgbenabled(menu_input_t input) {
    switch (input) {
        case menu_input_left:
        case menu_input_right:
            rgblight_toggle();
            return false;
        default:
            return true;
    }
}

void display_handler_rgbenabled(char *text_buffer, size_t buffer_len) {
    snprintf(text_buffer, buffer_len - 1, "%s", rgblight_is_enabled() ? "on" : "off");
}

static bool menu_handler_rgbmode(menu_input_t input) {
    switch (input) {
        case menu_input_left:
            rgblight_step_reverse();
            return false;
        case menu_input_right:
            rgblight_step();
            return false;
        default:
            return true;
    }
}

void display_handler_rgbmode(char *text_buffer, size_t buffer_len) {
    snprintf(text_buffer, buffer_len - 1, "%s", rgblight_get_effect_name());
}

static bool menu_handler_rgbhue(menu_input_t input) {
    switch (input) {
        case menu_input_left:
            rgblight_decrease_hue();
            return false;
        case menu_input_right:
            rgblight_increase_hue();
            return false;
        default:
            return true;
    }
}

void display_handler_rgbhue(char *text_buffer, size_t buffer_len) {
    snprintf(text_buffer, buffer_len - 1, "%d", (int)rgblight_get_hue());
}

static bool menu_handler_rgbsat(menu_input_t input) {
    switch (input) {
        case menu_input_left:
            rgblight_decrease_sat();
            return false;
        case menu_input_right:
            rgblight_increase_sat();
            return false;
        default:
            return true;
    }
}

void display_handler_rgbsat(char *text_buffer, size_t buffer_len) {
    snprintf(text_buffer, buffer_len - 1, "%d", (int)rgblight_get_sat());
}

static bool menu_handler_rgbval(menu_input_t input) {
    switch (input) {
        case menu_input_left:
            rgblight_decrease_val();
            return false;
        case menu_input_right:
            rgblight_increase_val();
            return false;
        default:
            return true;
    }
}

void display_handler_rgbval(char *text_buffer, size_t buffer_len) {
    snprintf(text_buffer, buffer_len - 1, "%d", (int)rgblight_get_val());
}

static bool menu_handler_rgbspeed(menu_input_t input) {
    switch (input) {
        case menu_input_left:
            rgblight_decrease_speed();
            return false;
        case menu_input_right:
            rgblight_increase_speed();
            return false;
        default:
            return true;
    }
}

void display_handler_rgbspeed(char *text_buffer, size_t buffer_len) {
    snprintf(text_buffer, buffer_len - 1, "%d", (int)rgblight_get_speed());
}

menu_entry_t rgb_light_entries[] = {
    {
        .flags                 = menu_flag_is_value,
        .text                  = "RGB Enabled",
        .child.menu_handler    = menu_handler_rgbenabled,
        .child.display_handler = display_handler_rgbenabled,
    },
    {
        .flags                 = menu_flag_is_value,
        .text                  = "RGB Mode",
        .child.menu_handler    = menu_handler_rgbmode,
        .child.display_handler = display_handler_rgbmode,
    },
    {
        .flags                 = menu_flag_is_value,
        .text                  = "RGB Hue",
        .child.menu_handler    = menu_handler_rgbhue,
        .child.display_handler = display_handler_rgbhue,
    },
    {
        .flags                 = menu_flag_is_value,
        .text                  = "RGB Saturation",
        .child.menu_handler    = menu_handler_rgbsat,
        .child.display_handler = display_handler_rgbsat,
    },
    {
        .flags                 = menu_flag_is_value,
        .text                  = "RGB Value",
        .child.menu_handler    = menu_handler_rgbval,
        .child.display_handler = display_handler_rgbval,
    },
    {
        .flags                 = menu_flag_is_value,
        .text                  = "RGB Speed",
        .child.menu_handler    = menu_handler_rgbspeed,
        .child.display_handler = display_handler_rgbspeed,
    },
};
#endif // RGBLIGHT_ENABLE

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Backlight

#ifdef BACKLIGHT_ENABLE
static bool menu_handler_bl_enabled(menu_input_t input) {
    switch (input) {
        case menu_input_left:
        case menu_input_right:
            backlight_toggle();
            return false;
        default:
            return true;
    }
}

void display_handler_bl_enabled(char *text_buffer, size_t buffer_len) {
    snprintf(text_buffer, buffer_len - 1, "%s", is_backlight_enabled() ? "on" : "off");
}

static bool menu_handler_bl_level(menu_input_t input) {
    switch (input) {
        case menu_input_left:
            backlight_decrease();
            return false;
        case menu_input_right:
            backlight_increase();
            return false;
        default:
            return true;
    }
}

void display_handler_bl_level(char *text_buffer, size_t buffer_len) {
    snprintf(text_buffer, buffer_len - 1, "%d", (int)get_backlight_level());
}

menu_entry_t backlight_entries[] = {
    {
        .flags                 = menu_flag_is_value,
        .text                  = "Backlight Enabled",
        .child.menu_handler    = menu_handler_bl_enabled,
        .child.display_handler = display_handler_bl_enabled,
    },
    {
        .flags                 = menu_flag_is_value,
        .text                  = "Backlight Level",
        .child.menu_handler    = menu_handler_bl_level,
        .child.display_handler = display_handler_bl_level,
    },
};
#endif // BACKLIGHT_ENABLE
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Audio

#ifdef AUDIO_ENABLE
static bool menu_handler_audio_enabled(menu_input_t input) {
    switch (input) {
        case menu_input_left:
        case menu_input_right:
            if (audio_is_on()) {
                audio_off();
            } else {
                audio_on();
            }
            return false;
        default:
            return true;
    }
}

void display_handler_audio_enabled(char *text_buffer, size_t buffer_len) {
    snprintf(text_buffer, buffer_len - 1, "%s", audio_is_on() ? "on" : "off");
}

static bool menu_handler_music_enabled(menu_input_t input) {
    switch (input) {
        case menu_input_left:
        case menu_input_right:
            music_toggle();
            return false;
        default:
            return true;
    }
}

void display_handler_music_enabled(char *text_buffer, size_t buffer_len) {
    snprintf(text_buffer, buffer_len - 1, "%s", is_music_on() ? "on" : "off");
}

static bool menu_handler_audio_clicky_enabled(menu_input_t input) {
    switch (input) {
        case menu_input_left:
        case menu_input_right:
            clicky_toggle();
            return false;
        default:
            return true;
    }
}

void display_handler_audio_clicky_enabled(char *text_buffer, size_t buffer_len) {
    snprintf(text_buffer, buffer_len - 1, "%s", is_clicky_on() ? "on" : "off");
}

static bool menu_handler_audio_clicky_freq(menu_input_t input) {
    switch (input) {
        case menu_input_left:
            clicky_freq_down();
            return false;
        case menu_input_right:
            clicky_freq_up();
            return false;
        default:
            return true;
    }
}

void display_handler_audio_clicky_freq(char *text_buffer, size_t buffer_len) {
    extern float clicky_freq;
    snprintf(text_buffer, buffer_len - 1, "%.2f", (float)clicky_freq);
}

menu_entry_t audio_entries[] = {
    {
        .flags                 = menu_flag_is_value,
        .text                  = "Audio Enabled",
        .child.menu_handler    = menu_handler_audio_enabled,
        .child.display_handler = display_handler_audio_enabled,
    },
    {
        .flags                 = menu_flag_is_value,
        .text                  = "Music Mode Enabled",
        .child.menu_handler    = menu_handler_music_enabled,
        .child.display_handler = display_handler_music_enabled,
    },
    {
        .flags                 = menu_flag_is_value,
        .text                  = "Audio Clicky Enabled",
        .child.menu_handler    = menu_handler_audio_clicky_enabled,
        .child.display_handler = display_handler_audio_clicky_enabled,
    },
    {
        .flags                 = menu_flag_is_value,
        .text                  = "Audio Clicky Frequency",
        .child.menu_handler    = menu_handler_audio_clicky_freq,
        .child.display_handler = display_handler_audio_clicky_freq,
    },
};
#endif // AUDIO_ENABLE

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Pointing Device

#ifdef POINTING_DEVICE_ENABLE

static bool menu_handler_auto_mouse_enable(menu_input_t input) {
    switch (input) {
        case menu_input_left:
        case menu_input_right:
            set_auto_mouse_enable(!get_auto_mouse_enable());
            return false;
        default:
            return true;
    }
}

void display_handler_auto_mouse_enable(char *text_buffer, size_t buffer_len) {
    snprintf(text_buffer, buffer_len - 1, "%s", get_auto_mouse_enable() ? "on" : "off");
}

static bool menu_handler_auto_mouse_layer(menu_input_t input) {
    switch (input) {
        case menu_input_left:
            set_auto_mouse_layer((get_auto_mouse_layer() - 1) % MAX_USER_LAYERS);
            return false;
        case menu_input_right:
            set_auto_mouse_layer((get_auto_mouse_layer() + 1) % MAX_USER_LAYERS);
            return false;
        default:
            return true;
    }
}

void display_handler_auto_mouse_layer(char *text_buffer, size_t buffer_len) {
    snprintf(text_buffer, buffer_len - 1, "%s", layer_name(get_auto_mouse_layer()));
}

static bool menu_handler_auto_mouse_accel(menu_input_t input) {
    switch (input) {
        case menu_input_left:
        case menu_input_right:
            userspace_config.enable_acceleration ^= 1;
            eeconfig_update_user_config(&userspace_config.raw);
            return false;
        default:
            return true;
    }
}

void display_handler_auto_mouse_accel(char *text_buffer, size_t buffer_len) {
    snprintf(text_buffer, buffer_len - 1, "%s", userspace_config.enable_acceleration ? "on" : "off");
}

extern bool     mouse_jiggler;
extern uint16_t mouse_jiggler_timer;

static bool menu_handler_mouse_jiggler(menu_input_t input) {
    switch (input) {
        case menu_input_left:
        case menu_input_right:
            mouse_jiggler       = !mouse_jiggler;
            mouse_jiggler_timer = timer_read();
            return false;
        default:
            return true;
    }
}

void display_handler_mouse_jiggler(char *text_buffer, size_t buffer_len) {
    snprintf(text_buffer, buffer_len - 1, "%s", mouse_jiggler ? "on" : "off");
}

#    if defined(KEYBOARD_handwired_tractyl_manuform) || defined(KEYBOARD_bastardkb_charybdis)
static bool menu_handler_dpi_config(menu_input_t input) {
    switch (input) {
        case menu_input_left:
            charybdis_cycle_pointer_default_dpi(false);
            return false;
        case menu_input_right:
            charybdis_cycle_pointer_default_dpi(true);
            return false;
        default:
            return true;
    }
}

void display_handler_dpi_config(char *text_buffer, size_t buffer_len) {
    snprintf(text_buffer, buffer_len - 1, "%d", (int)charybdis_get_pointer_default_dpi());
}
#    endif

menu_entry_t pointing_entries[] = {
    {
        .flags                 = menu_flag_is_value,
        .text                  = "Auto Mouse Enable",
        .child.menu_handler    = menu_handler_auto_mouse_enable,
        .child.display_handler = display_handler_auto_mouse_enable,
    },
    {
        .flags                 = menu_flag_is_value,
        .text                  = "Auto Mouse Layer",
        .child.menu_handler    = menu_handler_auto_mouse_layer,
        .child.display_handler = display_handler_auto_mouse_layer,
    },
    {
        .flags                 = menu_flag_is_value,
        .text                  = "Auto Mouse Acceleration",
        .child.menu_handler    = menu_handler_auto_mouse_accel,
        .child.display_handler = display_handler_auto_mouse_accel,
    },
    {
        .flags                 = menu_flag_is_value,
        .text                  = "Mouse Jiggler",
        .child.menu_handler    = menu_handler_mouse_jiggler,
        .child.display_handler = display_handler_mouse_jiggler,
    },
#    if defined(KEYBOARD_handwired_tractyl_manuform) || defined(KEYBOARD_bastardkb_charybdis)
    {
        .flags                 = menu_flag_is_value,
        .text                  = "DPI Config",
        .child.menu_handler    = menu_handler_dpi_config,
        .child.display_handler = display_handler_dpi_config,
    },
#    endif
};
#endif // POINTING_DEVICE_ENABLE

static bool menu_handler_keycode_ctrl_caps(menu_input_t input) {
    switch (input) {
        case menu_input_left:
        case menu_input_right:
            keymap_config.swap_control_capslock = !keymap_config.swap_control_capslock;
            eeconfig_update_keymap(keymap_config.raw);
            return false;
        default:
            return true;
    }
}

void display_handler_keycode_ctrl_caps(char *text_buffer, size_t buffer_len) {
    snprintf(text_buffer, buffer_len - 1, "%s", keymap_config.swap_control_capslock ? "swapped" : "normal");
}

static bool menu_handler_keycode_alt_gui(menu_input_t input) {
    switch (input) {
        case menu_input_left:
        case menu_input_right:
            keymap_config.swap_lalt_lgui = keymap_config.swap_ralt_rgui = !keymap_config.swap_lalt_lgui;
#ifdef AUDIO_ENABLE
            if (keymap_config.swap_ralt_rgui) {
                PLAY_SONG(ag_swap_song);
            } else {
                PLAY_SONG(ag_norm_song);
            }
#endif // AUDIO_ENABLE
            eeconfig_update_keymap(keymap_config.raw);
            return false;
        default:
            return true;
    }
}

void display_handler_keycode_alt_gui(char *text_buffer, size_t buffer_len) {
    snprintf(text_buffer, buffer_len - 1, "%s", keymap_config.swap_lalt_lgui ? "swapped" : "normal");
}

static bool menu_handler_keycode_ctrl_gui(menu_input_t input) {
    switch (input) {
        case menu_input_left:
        case menu_input_right:
            keymap_config.swap_lctl_lgui = keymap_config.swap_rctl_rgui = !keymap_config.swap_lctl_lgui;
#ifdef AUDIO_ENABLE
            if (keymap_config.swap_rctl_rgui) {
                PLAY_SONG(cg_swap_song);
            } else {
                PLAY_SONG(cg_norm_song);
            }
#endif // AUDIO_ENABLE
            eeconfig_update_keymap(keymap_config.raw);
            return false;
        default:
            return true;
    }
}

void display_handler_keycode_ctrl_gui(char *text_buffer, size_t buffer_len) {
    snprintf(text_buffer, buffer_len - 1, "%s", keymap_config.swap_lctl_lgui ? "swapped" : "normal");
}

static bool menu_handler_keycode_disable_gui(menu_input_t input) {
    switch (input) {
        case menu_input_left:
        case menu_input_right:
            keymap_config.no_gui = !keymap_config.no_gui;
            eeconfig_update_keymap(keymap_config.raw);
            return false;
        default:
            return true;
    }
}

void display_handler_keycode_disable_gui(char *text_buffer, size_t buffer_len) {
    snprintf(text_buffer, buffer_len - 1, "%s", keymap_config.no_gui ? "disabled" : "enabled");
}

__attribute__((unused)) static bool menu_handler_keycode_grave_esc(menu_input_t input) {
    switch (input) {
        case menu_input_left:
        case menu_input_right:
            keymap_config.swap_grave_esc = !keymap_config.swap_grave_esc;
            eeconfig_update_keymap(keymap_config.raw);
            return false;
        default:
            return true;
    }
}

__attribute__((unused)) void display_handler_keycode_grave_esc(char *text_buffer, size_t buffer_len) {
    snprintf(text_buffer, buffer_len - 1, "%s", keymap_config.swap_grave_esc ? "swapped" : "normal");
}

__attribute__((unused)) static bool menu_handler_keycode_bslash_bspc(menu_input_t input) {
    switch (input) {
        case menu_input_left:
        case menu_input_right:
            keymap_config.swap_backslash_backspace = !keymap_config.swap_backslash_backspace;
            eeconfig_update_keymap(keymap_config.raw);
            return false;
        default:
            return true;
    }
}

__attribute__((unused)) void display_handler_keycode_bslash_bspc(char *text_buffer, size_t buffer_len) {
    snprintf(text_buffer, buffer_len - 1, "%s", keymap_config.swap_backslash_backspace ? "swapped" : "normal");
}

static bool menu_handler_keycode_nkro(menu_input_t input) {
    switch (input) {
        case menu_input_left:
        case menu_input_right:
            keymap_config.nkro = !keymap_config.nkro;
            clear_keyboard(); // clear first buffer to prevent stuck keys
            eeconfig_update_keymap(keymap_config.raw);
            return false;
        default:
            return true;
    }
}

void display_handler_keycode_nkro(char *text_buffer, size_t buffer_len) {
    snprintf(text_buffer, buffer_len - 1, "%s", keymap_config.nkro ? "on" : "off");
}

static bool menu_handler_keycode_oneshot(menu_input_t input) {
    switch (input) {
        case menu_input_left:
        case menu_input_right:
            keymap_config.oneshot_enable = !keymap_config.oneshot_enable;
            eeconfig_update_keymap(keymap_config.raw);
            return false;
        default:
            return true;
    }
}

void display_handler_keycode_oneshot(char *text_buffer, size_t buffer_len) {
    snprintf(text_buffer, buffer_len - 1, "%s", keymap_config.oneshot_enable ? "on" : "off");
}

static bool menu_handler_keycode_autocorrect(menu_input_t input) {
    switch (input) {
        case menu_input_left:
        case menu_input_right:
            keymap_config.autocorrect_enable = !keymap_config.autocorrect_enable;
            eeconfig_update_keymap(keymap_config.raw);
            return false;
        default:
            return true;
    }
}

void display_handler_keycode_autocorrect(char *text_buffer, size_t buffer_len) {
    snprintf(text_buffer, buffer_len - 1, "%s", keymap_config.autocorrect_enable ? "on" : "off");
}

menu_entry_t keymap_config_entries[] = {
    {
        .flags                 = menu_flag_is_value,
        .text                  = "Control <-> Capslock",
        .child.menu_handler    = menu_handler_keycode_ctrl_caps,
        .child.display_handler = display_handler_keycode_ctrl_caps,
    },
    {
        .flags                 = menu_flag_is_value,
        .text                  = "Alt <-> GUI",
        .child.menu_handler    = menu_handler_keycode_alt_gui,
        .child.display_handler = display_handler_keycode_alt_gui,
    },
    {
        .flags                 = menu_flag_is_value,
        .text                  = "Control <-> GUI",
        .child.menu_handler    = menu_handler_keycode_ctrl_gui,
        .child.display_handler = display_handler_keycode_ctrl_gui,
    },
    {
        .flags                 = menu_flag_is_value,
        .text                  = "Disable GUI",
        .child.menu_handler    = menu_handler_keycode_disable_gui,
        .child.display_handler = display_handler_keycode_disable_gui,
    },
    {
        .flags                 = menu_flag_is_value,
        .text                  = "N-Key Roll Over",
        .child.menu_handler    = menu_handler_keycode_nkro,
        .child.display_handler = display_handler_keycode_nkro,
    },
    {
        .flags                 = menu_flag_is_value,
        .text                  = "Oneshot Keys",
        .child.menu_handler    = menu_handler_keycode_oneshot,
        .child.display_handler = display_handler_keycode_oneshot,
    },
    {
        .flags                 = menu_flag_is_value,
        .text                  = "Autocorrect",
        .child.menu_handler    = menu_handler_keycode_autocorrect,
        .child.display_handler = display_handler_keycode_autocorrect,
    },
    // {
    //     .flags                 = menu_flag_is_value,
    //     .text                  = "Grave <-> Escape",
    //     .child.menu_handler    = menu_handler_keycode_grave_esc,
    //     .child.display_handler = display_handler_keycode_grave_esc,
    // },
    // {
    //     .flags                 = menu_flag_is_value,
    //     .text                  = "Backslash <-> Backspace",
    //     .child.menu_handler    = menu_handler_keycode_bslash_bspc,
    //     .child.display_handler = display_handler_keycode_bslash_bspc,
    // },
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Buy More

menu_entry_t buy_more_entries[] = {
    {
        .flags              = menu_flag_is_parent,
        .text               = "Keymap Config",
        .parent.children    = keymap_config_entries,
        .parent.child_count = ARRAY_SIZE(keymap_config_entries),
    },
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Root menu

menu_entry_t root_entries[] = {
    {
        .flags                 = menu_flag_is_value,
        .text                  = "Display Option",
        .child.menu_handler    = menu_handler_display,
        .child.display_handler = display_handler_display,
    },
#ifdef UNICODE_COMMON_ENABLE
    {
        .flags              = menu_flag_is_parent,
        .text               = "Unicode Settings",
        .parent.children    = unicode_entries,
        .parent.child_count = ARRAY_SIZE(unicode_entries),
    },
#endif // UNICODE_COMMON_ENABLE
#ifdef RGB_MATRIX_ENABLE
    {
        .flags              = menu_flag_is_parent,
        .text               = "RGB Matrix Settings",
        .parent.children    = rgb_matrix_entries,
        .parent.child_count = ARRAY_SIZE(rgb_matrix_entries),
    },
#endif // RGB_MATRIX_ENABLE
#ifdef RGBLIGHT_ENABLE
    {
        .flags              = menu_flag_is_parent,
        .text               = "RGB Light Settings",
        .parent.children    = rgb_light_entries,
        .parent.child_count = ARRAY_SIZE(rgb_matrix_entries),
    },
#endif // RGBLIGHT_ENABLE
#ifdef BACKLIGHT_ENABLE
    {
        .flags              = menu_flag_is_parent,
        .text               = "Backlight Settings",
        .parent.children    = backlight_entries,
        .parent.child_count = ARRAY_SIZE(backlight_entries),
    },
#endif // BACKLIGHT_ENABLE
#ifdef AUDIO_ENABLE
    {
        .flags              = menu_flag_is_parent,
        .text               = "Audio Settings",
        .parent.children    = audio_entries,
        .parent.child_count = ARRAY_SIZE(audio_entries),
    },
#endif // AUDIO_ENABLE
#ifdef POINTING_DEVICE_ENABLE
    {
        .flags              = menu_flag_is_parent,
        .text               = "Pointing Device Settings",
        .parent.children    = pointing_entries,
        .parent.child_count = ARRAY_SIZE(pointing_entries),
    },
#endif // POINTING_DEVICE_ENABLE
    {
        .flags              = menu_flag_is_parent,
        .text               = "More...",
        .parent.children    = buy_more_entries,
        .parent.child_count = ARRAY_SIZE(buy_more_entries),
    },
};

_Static_assert(ARRAY_SIZE(root_entries) <= 8, "Too many root menu entries");

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Root Title

menu_entry_t root = {
    .flags              = menu_flag_is_parent,
    .text               = "Configuration",
    .parent.children    = root_entries,
    .parent.child_count = ARRAY_SIZE(root_entries),
};

menu_state_t display_menu_state = {
#ifndef DISPLAY_MENU_ENABLED_DEFAULT
    .dirty          = false,
    .is_in_menu     = false,
    .menu_stack     = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
    .selected_child = 0xFF,
#else
    .dirty          = true,
    .is_in_menu     = true,
    .menu_stack     = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
    .selected_child = 0x00,
#endif
};

menu_entry_t *get_current_menu(void) {
    if (display_menu_state.menu_stack[0] == 0xFF) {
        return &root;
    }

    menu_entry_t *entry = &root;
    for (int i = 0; i < sizeof(display_menu_state.menu_stack); ++i) {
        if (display_menu_state.menu_stack[i] == 0xFF) {
            return entry;
        }
        entry = &entry->parent.children[display_menu_state.menu_stack[i]];
    }

    return entry;
}

menu_entry_t *get_selected_menu_item(void) {
    return &(get_current_menu()->parent.children[display_menu_state.selected_child]);
}

bool menu_handle_input(menu_input_t input) {
    menu_entry_t *menu     = get_current_menu();
    menu_entry_t *selected = get_selected_menu_item();
    switch (input) {
        case menu_input_exit:
            display_menu_state.is_in_menu = false;
            memset(display_menu_state.menu_stack, 0xFF, sizeof(display_menu_state.menu_stack));
            display_menu_state.selected_child = 0xFF;
            return false;
        case menu_input_back:
            // Iterate backwards through the stack and remove the last entry
            for (uint8_t i = 0; i < sizeof(display_menu_state.menu_stack); ++i) {
                if (display_menu_state.menu_stack[sizeof(display_menu_state.menu_stack) - 1 - i] != 0xFF) {
                    display_menu_state.selected_child =
                        display_menu_state.menu_stack[sizeof(display_menu_state.menu_stack) - 1 - i];
                    display_menu_state.menu_stack[sizeof(display_menu_state.menu_stack) - 1 - i] = 0xFF;
                    break;
                }

                // If we've dropped out of the last entry in the stack, exit the menu
                if (i == sizeof(display_menu_state.menu_stack) - 1) {
                    display_menu_state.is_in_menu     = false;
                    display_menu_state.selected_child = 0xFF;
                }
            }
            return false;
        case menu_input_enter:
            // Only attempt to enter the next menu if we're a parent object
            if (selected->flags & menu_flag_is_parent) {
                // Iterate forwards through the stack and add the selected entry
                for (uint8_t i = 0; i < sizeof(display_menu_state.menu_stack); ++i) {
                    if (display_menu_state.menu_stack[i] == 0xFF) {
                        display_menu_state.menu_stack[i]  = display_menu_state.selected_child;
                        display_menu_state.selected_child = 0;
                        break;
                    }
                }
            }
            return false;
        case menu_input_up:
            display_menu_state.selected_child =
                (display_menu_state.selected_child + menu->parent.child_count - 1) % menu->parent.child_count;
            return false;
        case menu_input_down:
            display_menu_state.selected_child =
                (display_menu_state.selected_child + menu->parent.child_count + 1) % menu->parent.child_count;
            return false;
        case menu_input_left:
        case menu_input_right:
            if (selected->flags & menu_flag_is_value) {
                display_menu_state.dirty = true;
                return selected->child.menu_handler(input);
            }
            return false;
        default:
            return false;
    }
}

bool process_record_menu(uint16_t keycode, keyrecord_t *record) {
    if (keycode == DISPLAY_MENU && record->event.pressed && !display_menu_state.is_in_menu) {
        display_menu_state.is_in_menu     = true;
        display_menu_state.selected_child = 0;
        return false;
    }

    if (display_menu_state.is_in_menu) {
        if (record->event.pressed) {
            switch (keycode) {
                case DISPLAY_MENU:
                    return menu_handle_input(menu_input_exit);
                case KC_ESC:
                    return menu_handle_input(menu_input_back);
                case KC_SPACE:
                case KC_ENTER:
                case KC_RETURN:
                    return menu_handle_input(menu_input_enter);
                case KC_UP:
                case KC_W:
                    return menu_handle_input(menu_input_up);
                case KC_DOWN:
                case KC_S:
                    return menu_handle_input(menu_input_down);
                case KC_LEFT:
                case KC_A:
                    return menu_handle_input(menu_input_left);
                case KC_RIGHT:
                case KC_D:
                    return menu_handle_input(menu_input_right);
                default:
                    return false;
            }
        }
        return false;
    }

    return true;
}

extern painter_font_handle_t font_thintel, font_mono, font_oled;

bool render_menu(painter_device_t display, uint16_t start_x, uint16_t start_y, uint16_t width, uint16_t height) {
    static menu_state_t last_state;
    if (memcmp(&last_state, &display_menu_state, sizeof(menu_state_t)) == 0) {
        return display_menu_state.is_in_menu;
    }

    display_menu_state.dirty = false;
    memcpy(&last_state, &display_menu_state, sizeof(menu_state_t));

    uint16_t render_width = width - start_x;

    if (display_menu_state.is_in_menu) {
        qp_rect(display, start_x, start_y, render_width - 1, height - 1, 0, 0, 0, true);

        // uint8_t       hue      = rgb_matrix_get_hue();
        menu_entry_t *menu     = get_current_menu();
        menu_entry_t *selected = get_selected_menu_item();

        uint16_t y = start_y;
        qp_rect(display, start_x, y, render_width, y + 3, 0, 0, 255, true);
        y += 6;
        qp_drawtext(display, start_x + 4, y, font_oled, menu->text);
        y += font_oled->line_height + 2;
        qp_rect(display, start_x, y, render_width, y + 3, 0, 0, 255, true);
        y += 6;
        for (int i = 0; i < menu->parent.child_count; ++i) {
            menu_entry_t *child = &menu->parent.children[i];
            uint16_t      x     = start_x + 8;
            if (child == selected) {
                x += qp_drawtext_recolor(display, x, y, font_oled,
                                         truncate_text(child->text, render_width, font_oled, false, true), HSV_GREEN,
                                         85, 255, 0);
            } else {
                x += qp_drawtext_recolor(display, x, y, font_oled,
                                         truncate_text(child->text, render_width, font_oled, false, true), 0, 0, 255, 0,
                                         255, 0);
            }
            if (child->flags & menu_flag_is_parent) {
                qp_drawtext(display, 8 + x, y, font_oled, "  >");
            }
            if (child->flags & menu_flag_is_value) {
                char buf[32] = {0};
                child->child.display_handler(buf, sizeof(buf));
                qp_drawtext(display, 8 + x, y, font_oled, buf);
            }
            y += font_oled->line_height + 2;
            qp_rect(display, start_x, y, render_width, y, 0, 0, 255, true);
            y += 3;
        }
        return true;
    }
    return false;
}
