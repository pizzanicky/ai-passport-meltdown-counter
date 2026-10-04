#include "meltdown_ui.h"

#include "meltdown_dots.h"

#include "esp_log.h"
#include "lvgl.h"

#include <stdio.h>
#include <string.h>

#define COL_BG MELT_COLOR_BG
#define COL_INK MELT_COLOR_INK
#define COL_DIM MELT_COLOR_DIM
#define COL_BLUE MELT_COLOR_BLUE
#define COL_HI MELT_COLOR_HI
#define COL_UP MELT_COLOR_UP
#define COL_DOWN MELT_COLOR_DOWN
/* Paper field. Zero and the month squares share the red scale; future days are an outline. */
#define COL_ZERO MELT_COLOR_ZERO
#define COL_FUTURE MELT_COLOR_FUTURE
/* Numbers are one A8 image, not one widget per dot. */
#define HERO_X 40
#define HERO_Y 89
#define HERO_W 160
#define HERO_H 108
#define DELTA_X 62
#define DELTA_Y 242
#define DELTA_W 100
#define DELTA_H 28
#define BARS_X 16
#define BARS_Y 248
#define BARS_W 84
#define BARS_H 27
#define CELL_LG 26
#define CELL_SM 22
#define DATE_SLOTS 9
#define PENDING_SLOTS 8

static lv_obj_t *s_scr;
static lv_obj_t *s_date_ch[DATE_SLOTS];
static lv_obj_t *s_mute_text;
static lv_obj_t *s_mute_label;
static lv_obj_t *s_stats_hint;
static lv_obj_t *s_stats_key;
static lv_obj_t *s_motto;
static lv_obj_t *s_stats_back;
static lv_obj_t *s_speaker;
static lv_obj_t *s_slash;
static lv_obj_t *s_mute_key;
static lv_obj_t *s_hero;
static lv_obj_t *s_unit;
static lv_obj_t *s_week;
static lv_obj_t *s_compare;
static lv_obj_t *s_bar_img;
static lv_obj_t *s_bar_hi_img;
static lv_obj_t *s_record;
static lv_obj_t *s_ok_key;
static lv_obj_t *s_down;
static lv_obj_t *s_down_key;
static lv_obj_t *s_page;
static lv_obj_t *s_col_ch[6][2];
static lv_obj_t *s_weekdays[7];
static lv_obj_t *s_cells[6][7];
static lv_obj_t *s_today_ring;
static lv_obj_t *s_up;
static lv_obj_t *s_up_key;
static lv_obj_t *s_detail;
static lv_obj_t *s_pending;
static lv_obj_t *s_pending_ch[PENDING_SLOTS];
static lv_obj_t *s_save;
static uint8_t s_hero_px[HERO_W * HERO_H] __attribute__((aligned(4)));
static uint8_t s_delta_px[DELTA_W * DELTA_H] __attribute__((aligned(4)));
static uint8_t s_bar_px[BARS_W * BARS_H] __attribute__((aligned(4)));
static uint8_t s_bar_hi_px[BARS_W * BARS_H] __attribute__((aligned(4)));
static lv_image_dsc_t s_hero_dsc;
static lv_image_dsc_t s_delta_dsc;
static lv_image_dsc_t s_bar_dsc;
static lv_image_dsc_t s_bar_hi_dsc;
static lv_obj_t *s_hero_img;
static lv_obj_t *s_delta_img;
static lv_obj_t *s_tri_img;
static char s_drawn_hero[16];
static char s_drawn_delta[16];
static uint32_t s_drawn_delta_color;
static int s_drawn_bars[7];
static bool s_drawn_hero_valid;
static bool s_drawn_delta_valid;
static bool s_drawn_bars_valid;

typedef struct {
    uint8_t *buf;
    bool *used;
    int origin_x;
    int origin_y;
    int width;
    int height;
    int stride;
} stamp_t;

static stamp_t s_stamp;

static void show(lv_obj_t *obj, bool visible)
{
    if (!obj) return;
    if (visible) lv_obj_remove_flag(obj, LV_OBJ_FLAG_HIDDEN);
    else lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
}

static lv_obj_t *make_image(const lv_image_dsc_t *dsc, int x, int y)
{
    lv_obj_t *img = lv_image_create(s_scr);
    if (dsc) lv_image_set_src(img, dsc);
    lv_obj_set_pos(img, x, y);
    lv_obj_set_style_image_recolor_opa(img, LV_OPA_COVER, 0);
    lv_obj_remove_flag(img, LV_OBJ_FLAG_CLICKABLE);
    show(img, false);
    return img;
}

static lv_obj_t *make_phrase(const char *text, uint32_t color)
{
    const melt_phrase_t *phrase = melt_dots_phrase(text);
    lv_obj_t *img = make_image(phrase ? phrase->image : NULL, phrase ? phrase->x : 0, phrase ? phrase->y : 0);
    lv_obj_set_style_image_recolor(img, lv_color_hex(color), 0);
    return img;
}

static void src_if(lv_obj_t *img, const void *src)
{
    if (!img || !src) return;
    /* lv_image_set_src invalidates even when the pointer is unchanged. */
    if (lv_image_get_src(img) == src) return;
    lv_image_set_src(img, src);
}

static void recolor_if(lv_obj_t *img, uint32_t color)
{
    if (!img) return;
    const lv_color_t next = lv_color_hex(color);
    if (lv_color_eq(lv_obj_get_style_image_recolor(img, 0), next)) return;
    lv_obj_set_style_image_recolor(img, next, 0);
}

static void place_phrase(lv_obj_t *img, const char *text, uint32_t color)
{
    const melt_phrase_t *phrase = melt_dots_phrase(text);
    if (!img || !phrase || !phrase->image) {
        show(img, false);
        return;
    }
    src_if(img, phrase->image);
    lv_obj_set_pos(img, phrase->x, phrase->y);
    recolor_if(img, color);
    show(img, true);
}

static lv_obj_t *make_glyph_slot(void)
{
    const melt_glyph_t *glyph = melt_dots_glyph(MELT_FACE_MENLO, '0');
    lv_obj_t *img = make_image(glyph ? glyph->image : NULL, 0, 0);
    lv_obj_set_style_image_recolor(img, lv_color_hex(COL_INK), 0);
    return img;
}

static void layout_ascii(lv_obj_t **slots, int count, const char *text, int face,
                         int anchor_x, int center_y, int anchor, uint32_t color)
{
    int total = 0;
    const int len = text ? (int)strlen(text) : 0;
    for (int i = 0; i < len; i++) {
        const melt_glyph_t *glyph = melt_dots_glyph(face, text[i]);
        if (glyph) total += glyph->advance;
    }
    int x = anchor_x;
    if (anchor == 1) x = anchor_x - total / 2;
    if (anchor == 2) x = anchor_x - total;
    int slot = 0;
    for (int i = 0; i < len && slot < count; i++) {
        const melt_glyph_t *glyph = melt_dots_glyph(face, text[i]);
        if (!glyph) continue;
        if (glyph->image) {
            src_if(slots[slot], glyph->image);
            const int height = (int)glyph->image->header.h;
            lv_obj_set_pos(slots[slot], x, center_y - height / 2);
            recolor_if(slots[slot], color);
            show(slots[slot], true);
            slot++;
        }
        x += glyph->advance;
    }
    for (; slot < count; slot++) show(slots[slot], false);
}

static void init_mask(lv_image_dsc_t *dsc, const uint8_t *px, int width, int height)
{
    memset(dsc, 0, sizeof *dsc);
    dsc->header.magic = LV_IMAGE_HEADER_MAGIC;
    dsc->header.cf = LV_COLOR_FORMAT_A8;
    dsc->header.flags = LV_IMAGE_FLAGS_MODIFIABLE;
    dsc->header.w = (uint32_t)width;
    dsc->header.h = (uint32_t)height;
    dsc->header.stride = (uint32_t)width;
    dsc->data_size = (uint32_t)(width * height);
    dsc->data = px;
}

static void publish_mask(lv_obj_t *img, bool used, uint32_t color, bool pixels_changed)
{
    if (used) {
        const bool was_hidden = lv_obj_has_flag(img, LV_OBJ_FLAG_HIDDEN);
        const lv_color_t next = lv_color_hex(color);
        const bool color_changed = !lv_color_eq(lv_obj_get_style_image_recolor(img, 0), next);
        if (color_changed) lv_obj_set_style_image_recolor(img, next, 0);
        show(img, true);
        if (pixels_changed && !was_hidden && !color_changed) lv_obj_invalidate(img);
        return;
    }
    show(img, false);
}

static void bind_stamp(uint8_t *buf, bool *used, int origin_x, int origin_y, int width, int height)
{
    s_stamp.buf = buf;
    s_stamp.used = used;
    s_stamp.origin_x = origin_x;
    s_stamp.origin_y = origin_y;
    s_stamp.width = width;
    s_stamp.height = height;
    s_stamp.stride = width;
    *used = false;
    memset(buf, 0, (size_t)width * (size_t)height);
}

static bool blit_glyphs(uint8_t *dst, int width, int height, const char *text, int face, bool bottom)
{
    memset(dst, 0, (size_t)width * (size_t)height);
    if (!text || !text[0]) return false;
    int natural = 0;
    int max_h = 0;
    for (const char *cursor = text; *cursor; cursor++) {
        const melt_glyph_t *glyph = melt_dots_glyph(face, *cursor);
        if (!glyph) continue;
        natural += glyph->advance;
        if (glyph->image && (int)glyph->image->header.h > max_h) max_h = (int)glyph->image->header.h;
    }
    if (natural <= 0 || max_h <= 0) return false;
    int scale = 256;
    if (natural > width) scale = width * 256 / natural;
    if (max_h > height) {
        const int fit = height * 256 / max_h;
        if (fit < scale) scale = fit;
    }
    if (scale < 1) scale = 1;
    const int scaled_h = max_h * scale / 256;
    int x = bottom ? 0 : (width - natural * scale / 256) / 2;
    if (x < 0) x = 0;
    int y0 = bottom ? height - scaled_h : (height - scaled_h) / 2;
    if (y0 < 0) y0 = 0;
    bool used = false;
    for (const char *cursor = text; *cursor; cursor++) {
        const melt_glyph_t *glyph = melt_dots_glyph(face, *cursor);
        if (!glyph) continue;
        const lv_image_dsc_t *image = glyph->image;
        if (image && image->data) {
            const int sw = (int)image->header.w;
            const int sh = (int)image->header.h;
            int dw = sw * scale / 256;
            int dh = sh * scale / 256;
            if (dw < 1) dw = 1;
            if (dh < 1) dh = 1;
            const int gy = y0 + (scaled_h - dh);
            const uint8_t *src = image->data;
            for (int sy = 0; sy < dh; sy++) {
                const int iy = gy + sy;
                if (iy < 0 || iy >= height) continue;
                const int src_y = sy * sh / dh;
                for (int sx = 0; sx < dw; sx++) {
                    const int ix = x + sx;
                    if (ix < 0 || ix >= width) continue;
                    const uint8_t alpha = src[src_y * sw + sx * sw / dw];
                    if (alpha == 0) continue;
                    if (dst[iy * width + ix] < alpha) {
                        dst[iy * width + ix] = alpha;
                        used = true;
                    }
                }
            }
        }
        x += glyph->advance * scale / 256;
    }
    if (used && !bottom) {
        /* Center the visible ink, excluding glyph advance and trailing spacing. */
        int left = width, right = -1;
        for (int y = 0; y < height; y++) for (int px = 0; px < width; px++) {
            if (!dst[y * width + px]) continue;
            if (px < left) left = px;
            if (px > right) right = px;
        }
        const int shift = (width - (right - left + 1)) / 2 - left;
        for (int y = 0; y < height; y++) {
            uint8_t *row = dst + y * width;
            if (shift > 0) { memmove(row + shift, row, width - shift); memset(row, 0, shift); }
            else if (shift < 0) { memmove(row, row - shift, width + shift); memset(row + width + shift, 0, -shift); }
        }
    }
    return used;
}

static const lv_image_dsc_t *fill_for(int cell)
{
    return cell >= CELL_LG ? &melt_fill_lg : &melt_fill_sm;
}

static const lv_image_dsc_t *ring_for(int cell)
{
    return cell >= CELL_LG ? &melt_ring_lg : &melt_ring_sm;
}

static uint32_t themed_heat(int level)
{
    static const uint32_t colors[] = {MELT_HEAT_0, MELT_HEAT_1, MELT_HEAT_2, MELT_HEAT_3, MELT_HEAT_4};
    if (level < 0) level = 0;
    if (level > 4) level = 4;
    return colors[level];
}

static void style_cell(lv_obj_t *cell, const meltdown_cell_t *info, int cell_px)
{
    if (info->kind == MELTDOWN_CELL_OUTSIDE) {
        show(cell, false);
        return;
    }
    show(cell, true);
    if (info->kind == MELTDOWN_CELL_FUTURE) {
        src_if(cell, ring_for(cell_px));
        recolor_if(cell, COL_FUTURE);
        return;
    }
    const uint32_t color = info->kind == MELTDOWN_CELL_ZERO
        ? COL_ZERO
        : themed_heat(info->heat);
    src_if(cell, fill_for(cell_px));
    recolor_if(cell, color);
}

static void fill_rect(int x, int y, int width, int height)
{
    if (!s_stamp.buf || width < 1 || height < 1) return;
    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            const int px = x + col - s_stamp.origin_x;
            const int py = y + row - s_stamp.origin_y;
            if (px < 0 || py < 0 || px >= s_stamp.width || py >= s_stamp.height) continue;
            s_stamp.buf[py * s_stamp.stride + px] = 255;
            *s_stamp.used = true;
        }
    }
}

static void paint_bars(const int *heights, bool highlight)
{
    const int bottom = BARS_Y + BARS_H;
    for (int i = 0; i < 7; i++) {
        const int height = heights[i];
        if (height <= 0) continue;
        const int x = BARS_X + i * 12;
        const int y = bottom - height;
        if (highlight) fill_rect(x, y, 7, 1);
        else if (height > 1) fill_rect(x, y + 1, 7, height - 1);
    }
}

static void layout_grid(const meltdown_grid_t *grid)
{
    for (int column = 0; column < 6; column++) {
        if (column >= grid->columns) {
            layout_ascii(s_col_ch[column], 2, "", MELT_FACE_MENLO, 0, 0, 1, COL_INK);
            continue;
        }
        char header[4];
        snprintf(header, sizeof header, "%02d", grid->header_day[column]);
        const int center = grid->origin_x + column * (grid->cell + grid->gap) + grid->cell / 2;
        layout_ascii(s_col_ch[column], 2, header, MELT_FACE_MENLO, center, grid->origin_y - 15, 1, COL_INK);
    }
    for (int row = 0; row < 7; row++) {
        const int center_y = grid->origin_y + row * (grid->cell + grid->gap) + grid->cell / 2;
        const melt_phrase_t *phrase = melt_dots_phrase(meltdown_weekday_zh(row));
        if (phrase && phrase->image) {
            const int width = (int)phrase->image->header.w;
            const int height = (int)phrase->image->header.h;
            lv_obj_set_pos(s_weekdays[row], grid->origin_x - 8 - width, center_y - height / 2);
        }
        for (int column = 0; column < 6; column++) {
            const int x = grid->origin_x + column * (grid->cell + grid->gap);
            const int y = grid->origin_y + row * (grid->cell + grid->gap);
            lv_obj_set_pos(s_cells[column][row], x, y);
        }
    }
}

void meltdown_ui_create(void)
{
    s_scr = lv_obj_create(NULL);
    lv_obj_set_size(s_scr, 240, 320);
    lv_obj_set_style_bg_color(s_scr, lv_color_hex(COL_BG), 0);
    lv_obj_set_style_bg_opa(s_scr, LV_OPA_COVER, 0);
    lv_obj_set_style_bg_image_src(s_scr, &melt_mesh, 0);
    lv_obj_set_style_bg_image_tiled(s_scr, true, 0);
    lv_obj_set_style_border_width(s_scr, 0, 0);
    lv_obj_set_style_pad_all(s_scr, 0, 0);
    lv_obj_set_style_radius(s_scr, 0, 0);
    lv_obj_remove_flag(s_scr, LV_OBJ_FLAG_SCROLLABLE);

    for (int i = 0; i < DATE_SLOTS; i++) s_date_ch[i] = make_glyph_slot();
    s_speaker = make_phrase("icon:speaker", COL_INK);
    s_slash = make_phrase("icon:slash", COL_UP);
    s_mute_key = make_phrase("icon:mute-key", COL_INK);
    s_mute_text = make_phrase("长按切换", COL_INK);
    s_mute_label = make_phrase("静音", COL_INK);
    s_stats_hint = make_phrase("统计", COL_INK);
    s_stats_key = make_phrase("icon:stats", COL_INK);
    s_motto = make_phrase("记下这一刻, 继续向前", COL_DIM);
    s_stats_back = make_phrase("icon:stats-back", COL_INK);
    s_hero = make_phrase("今日崩溃", COL_INK);
    s_unit = make_phrase("次", COL_INK);
    s_week = make_phrase("本周", COL_INK);
    s_compare = make_phrase("较昨日", COL_INK);
    init_mask(&s_bar_dsc, s_bar_px, BARS_W, BARS_H);
    init_mask(&s_bar_hi_dsc, s_bar_hi_px, BARS_W, BARS_H);
    s_bar_img = make_image(&s_bar_dsc, BARS_X, BARS_Y);
    s_bar_hi_img = make_image(&s_bar_hi_dsc, BARS_X, BARS_Y);
    lv_obj_set_pos(s_bar_img, 36, 128);
    lv_obj_set_pos(s_bar_hi_img, 36, 128);
    lv_image_set_pivot(s_bar_img, 0, 0);
    lv_image_set_pivot(s_bar_hi_img, 0, 0);
    lv_image_set_scale(s_bar_img, 512);
    lv_image_set_scale(s_bar_hi_img, 512);
    lv_image_set_antialias(s_bar_img, false);
    lv_image_set_antialias(s_bar_hi_img, false);
    s_ok_key = make_phrase("icon:ok", COL_INK);
    s_record = make_phrase("崩溃时按一下", COL_INK);
    s_down_key = make_phrase("icon:down", COL_INK);
    s_down = make_phrase("本月", COL_INK);
    s_page = make_phrase("03 / 03", COL_INK);
    for (int column = 0; column < 6; column++) {
        s_col_ch[column][0] = make_glyph_slot();
        s_col_ch[column][1] = make_glyph_slot();
    }
    for (int row = 0; row < 7; row++) {
        s_weekdays[row] = make_phrase(meltdown_weekday_zh(row), COL_INK);
        for (int column = 0; column < 6; column++) {
            s_cells[column][row] = make_image(&melt_fill_lg, 0, 0);
        }
    }
    s_today_ring = make_image(&melt_ring_lg, 0, 0);
    lv_obj_set_style_image_recolor(s_today_ring, lv_color_hex(COL_INK), 0);
    s_up_key = make_phrase("icon:up", COL_INK);
    s_up = make_phrase("返回今日", COL_INK);
    s_detail = make_phrase("正在校时", COL_DIM);
    s_pending = make_phrase("未归档 ", COL_DIM);
    for (int i = 0; i < PENDING_SLOTS; i++) s_pending_ch[i] = make_glyph_slot();
    s_save = make_phrase("写入失败", COL_UP);

    init_mask(&s_hero_dsc, s_hero_px, HERO_W, HERO_H);
    init_mask(&s_delta_dsc, s_delta_px, DELTA_W, DELTA_H);
    s_hero_img = make_image(&s_hero_dsc, HERO_X, HERO_Y);
    s_delta_img = make_image(&s_delta_dsc, DELTA_X, DELTA_Y);
    s_tri_img = make_phrase("icon:tri-up", COL_UP);

    lv_mem_monitor_t pool;
    lv_mem_monitor(&pool);
    ESP_LOGI("meltdown_ui", "lvgl pool total=%u free=%u biggest=%u used_pct=%u",
             (unsigned)pool.total_size, (unsigned)pool.free_size,
             (unsigned)pool.free_biggest_size, (unsigned)pool.used_pct);

    lv_screen_load(s_scr);
}

void meltdown_ui_refresh(const meltdown_state_t *state, const char *sync_line, bool save_failed)
{
    if (!state) return;
    const bool today = state->page == MELTDOWN_PAGE_TODAY;
    const bool stats = state->page == MELTDOWN_PAGE_STATS;
    const bool month = state->page == MELTDOWN_PAGE_MONTH;
    const bool sync = state->page == MELTDOWN_PAGE_SYNC;
    const bool assign = state->page == MELTDOWN_PAGE_ASSIGN;
    const bool show_mute = true;

    char date_text[16];
    char count_text[16];
    char delta_text[16];
    char month_text[16];
    char pending_number[16];
    meltdown_view_date(state, date_text, sizeof date_text);
    meltdown_view_month(state, month_text, sizeof month_text);
    meltdown_format_count(assign || sync ? state->record.pending : meltdown_today_count(state),
                          count_text, sizeof count_text);
    meltdown_delta_t delta;
    meltdown_compare_yesterday(state, &delta);
    meltdown_format_delta(&delta, delta_text, sizeof delta_text);
    const bool pending = state->record.pending > 0 && today;
    if (pending) meltdown_format_count(state->record.pending, pending_number, sizeof pending_number);

    if (month) layout_ascii(s_date_ch, DATE_SLOTS, month_text, MELT_FACE_MENLO, 14, 22, 0, COL_INK);
    else layout_ascii(s_date_ch, DATE_SLOTS, date_text, MELT_FACE_MENLO, 14, 22, 0, COL_INK);
    /* The mute cluster and the page index share the top-right. Hide the index during the flash. */
    show(s_page, month && !state->mute_flash);
    show(s_speaker, show_mute);
    show(s_slash, show_mute && state->record.muted);
    show(s_mute_key, today);
    show(s_mute_text, today);
    show(s_mute_label, today);
    show(s_stats_hint, today);
    show(s_stats_key, today);
    show(s_motto, today && !pending && !save_failed);
    show(s_stats_back, stats);

    if (!month) {
        const char *title = "今日崩溃";
        if (sync) title = state->rollback ? "日期回拨" : "日期未确认";
        else if (assign) title = "未归档";
        else if (stats) title = "数据统计";
        place_phrase(s_hero, title, COL_INK);
    } else {
        show(s_hero, false);
    }
    show(s_unit, today);
    show(s_week, stats);
    show(s_compare, stats);

    static bool s_bar_used;
    static bool s_bar_hi_used;
    meltdown_week_slot_t slots[7];
    uint32_t scale = 0;
    meltdown_week_bars(state, slots, &scale);
    int heights[7];
    bool bars_changed = !s_drawn_bars_valid;
    for (int i = 0; i < 7; i++) {
        heights[i] = stats ? meltdown_bar_height_px(slots[i].count, scale) : 0;
        if (!s_drawn_bars_valid || heights[i] != s_drawn_bars[i]) bars_changed = true;
        s_drawn_bars[i] = heights[i];
    }
    s_drawn_bars_valid = true;
    if (bars_changed) {
        bind_stamp(s_bar_px, &s_bar_used, BARS_X, BARS_Y, BARS_W, BARS_H);
        if (stats) paint_bars(heights, false);
        bind_stamp(s_bar_hi_px, &s_bar_hi_used, BARS_X, BARS_Y, BARS_W, BARS_H);
        if (stats) paint_bars(heights, true);
    }
    publish_mask(s_bar_img, s_bar_used, COL_BLUE, bars_changed);
    publish_mask(s_bar_hi_img, s_bar_hi_used, COL_HI, bars_changed);

    show(s_ok_key, today || assign || sync);
    lv_obj_set_pos(s_ok_key, assign ? 14 : 27, assign ? 295 : 218);
    if (!month && !stats) place_phrase(s_record, assign ? "记入今日" : "崩溃时按一下", COL_INK);
    else show(s_record, false);
    if (today || assign) {
        place_phrase(s_down, assign ? "保持分开" : "本月", COL_INK);
        lv_obj_set_pos(s_down_key, assign ? 158 : 179, 296);
        show(s_down_key, true);
    } else {
        show(s_down, false);
        show(s_down_key, false);
    }
    show(s_up, month || stats);
    show(s_up_key, month);

    const bool grid_on = month && meltdown_can_file(state);
    meltdown_grid_t grid = {0};
    bool today_marked = false;
    if (grid_on) {
        meltdown_month_grid(state->today.year, state->today.month, &grid);
        layout_grid(&grid);
    }
    for (int row = 0; row < 7; row++) show(s_weekdays[row], grid_on);
    for (int column = 0; column < 6; column++) {
        if (!grid_on) layout_ascii(s_col_ch[column], 2, "", MELT_FACE_MENLO, 0, 0, 1, COL_INK);
        for (int row = 0; row < 7; row++) {
            if (!grid_on) {
                show(s_cells[column][row], false);
                continue;
            }
            meltdown_cell_t info;
            meltdown_cell_at(state, column, row, &info);
            style_cell(s_cells[column][row], &info, grid.cell);
            if (!info.today) continue;
            const int x = grid.origin_x + column * (grid.cell + grid.gap);
            const int y = grid.origin_y + row * (grid.cell + grid.gap);
            src_if(s_today_ring, ring_for(grid.cell));
            recolor_if(s_today_ring, COL_INK);
            lv_obj_set_pos(s_today_ring, x, y);
            today_marked = true;
        }
    }
    show(s_today_ring, today_marked);

    if (sync) place_phrase(s_detail, sync_line && sync_line[0] ? sync_line : "正在校时", COL_DIM);
    else if (assign) place_phrase(s_detail, "记入今日, 保持分开", COL_DIM);
    else show(s_detail, false);
    if (pending) {
        place_phrase(s_pending, "未归档 ", COL_DIM);
        const melt_phrase_t *phrase = melt_dots_phrase("未归档 ");
        const int x = phrase && phrase->image ? phrase->x + (int)phrase->image->header.w + 4 : 16;
        const int cy = phrase && phrase->image ? phrase->y + (int)phrase->image->header.h / 2 : 192;
        layout_ascii(s_pending_ch, PENDING_SLOTS, pending_number, MELT_FACE_MENLO, x, cy, 0, COL_DIM);
    } else {
        show(s_pending, false);
        layout_ascii(s_pending_ch, PENDING_SLOTS, "", MELT_FACE_MENLO, 0, 0, 0, COL_DIM);
    }
    show(s_save, save_failed);

    const bool want_hero = today || assign;
    const bool hero_changed = !s_drawn_hero_valid || strcmp(s_drawn_hero, count_text) != 0;
    bool hero_used = want_hero && s_drawn_hero[0] != '\0';
    if (want_hero && hero_changed) {
        hero_used = blit_glyphs(s_hero_px, HERO_W, HERO_H, count_text, MELT_FACE_HERO, false);
        snprintf(s_drawn_hero, sizeof s_drawn_hero, "%s", count_text);
        s_drawn_hero_valid = true;
    }
    const uint32_t hero_color = today ? themed_heat(meltdown_heat_level(meltdown_today_count(state))) : COL_INK;
    publish_mask(s_hero_img, want_hero && hero_used, hero_color, hero_changed);

    const uint32_t number_color = (delta.kind == MELTDOWN_DELTA_UP ||
                                   delta.kind == MELTDOWN_DELTA_DOWN)
        ? COL_INK : COL_DIM;
    const bool delta_changed = !s_drawn_delta_valid || strcmp(s_drawn_delta, delta_text) != 0 ||
        s_drawn_delta_color != number_color;
    bool delta_used = stats && s_drawn_delta[0] != '\0';
    if (stats && delta_changed) {
        delta_used = blit_glyphs(s_delta_px, DELTA_W, DELTA_H, delta_text, MELT_FACE_DELTA, true);
        snprintf(s_drawn_delta, sizeof s_drawn_delta, "%s", delta_text);
        s_drawn_delta_color = number_color;
        s_drawn_delta_valid = true;
    }
    publish_mask(s_delta_img, stats && delta_used, number_color, delta_changed);

    if (stats && delta.kind == MELTDOWN_DELTA_UP) place_phrase(s_tri_img, "icon:tri-up", COL_UP);
    else if (stats && delta.kind == MELTDOWN_DELTA_DOWN) place_phrase(s_tri_img, "icon:tri-down", COL_DOWN);
    else show(s_tri_img, false);
}
