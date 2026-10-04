#pragma once

/* Generated palette: light. Regenerate with --theme dark|light. */
#define MELT_COLOR_BG 0xDCE0D9
#define MELT_COLOR_INK 0x283437
#define MELT_COLOR_DIM 0x586765
#define MELT_COLOR_BLUE 0x006DAD
#define MELT_COLOR_HI 0x238BB5
#define MELT_COLOR_UP 0xBC4A19
#define MELT_COLOR_DOWN 0x007B8D
#define MELT_COLOR_ZERO 0x566E64
#define MELT_COLOR_FUTURE 0xACB5AE
#define MELT_HEAT_0 0x566E64
#define MELT_HEAT_1 0x007B8D
#define MELT_HEAT_2 0x946100
#define MELT_HEAT_3 0xBC4A19
#define MELT_HEAT_4 0xC32136

#include "lvgl.h"

#include <stdint.h>

#define MELT_FACE_MENLO 0
#define MELT_FACE_HERO 1
#define MELT_FACE_DELTA 2

typedef struct {
    const char *text;
    const lv_image_dsc_t *image;
    int16_t x;
    int16_t y;
} melt_phrase_t;

typedef struct {
    const lv_image_dsc_t *image;
    uint8_t advance;
} melt_glyph_t;

const melt_phrase_t *melt_dots_phrase(const char *text);
const melt_glyph_t *melt_dots_glyph(int face, char ascii);

extern const lv_image_dsc_t melt_mesh;
extern const lv_image_dsc_t melt_fill_lg;
extern const lv_image_dsc_t melt_fill_sm;
extern const lv_image_dsc_t melt_ring_lg;
extern const lv_image_dsc_t melt_ring_sm;
