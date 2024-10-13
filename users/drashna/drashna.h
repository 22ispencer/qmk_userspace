// Copyright 2020 Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include QMK_KEYBOARD_H

#include "keyrecords/wrappers.h"
#include "keyrecords/process_records.h"
#include "callbacks.h"
#include "drashna_names.h"
#include "drashna_runtime.h"
#include "drashna_layers.h"
#include "drashna_util.h"
#include "quantum/unicode/unicode.h"

#if defined(RGB_MATRIX_ENABLE)
#    include "rgb/rgb_matrix_stuff.h"
#endif // defined(RGB_MATRIX_ENABLE)
#if defined(CUSTOM_OLED_DRIVER)
#    include "display/oled/oled_stuff.h"
#endif // CUSTOM_OLED_DRIVER

void     matrix_scan_rate_task(void);
uint32_t get_matrix_scan_rate(void);

#ifdef AUDIO_ENABLE
void set_doom_song(layer_state_t state);
#endif // AUDIO_ENABLE
