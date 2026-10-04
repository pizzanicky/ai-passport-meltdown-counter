#include "meltdown_model.h"

#include <stdio.h>
#include <string.h>

/* Shanghai is a fixed offset. China has no daylight-saving transition. */
#define SHANGHAI_OFFSET_SECONDS (8 * 60 * 60)

static const uint32_t HEAT_HEX[5] = {
    0xB1B6AFu,
    0xE6C7BAu,
    0xD99D86u,
    0xBD6750u,
    0x862F2Du,
};

static const char *const WEEKDAY_EN[7] = {
    "MON", "TUE", "WED", "THU", "FRI", "SAT", "SUN",
};

static const char *const WEEKDAY_ZH[7] = {
    "周一", "周二", "周三", "周四", "周五", "周六", "周日",
};

static void effect_clear(meltdown_effect_t *effect)
{
    memset(effect, 0, sizeof(*effect));
}

static void open_after_sync(meltdown_state_t *state)
{
    if (state->rollback) {
        state->page = MELTDOWN_PAGE_SYNC;
        return;
    }
    if (state->record.pending > 0 && !state->record.pending_kept) {
        state->page = MELTDOWN_PAGE_ASSIGN;
        return;
    }
    if (state->page == MELTDOWN_PAGE_SYNC || state->page == MELTDOWN_PAGE_ASSIGN) {
        state->page = MELTDOWN_PAGE_TODAY;
    }
}

static int64_t days_from_civil(int year, int month, int day)
{
    year -= month <= 2;
    const int era = (year >= 0 ? year : year - 399) / 400;
    const unsigned yoe = (unsigned)(year - era * 400);
    const unsigned doy = (153u * (unsigned)(month + (month > 2 ? -3 : 9)) + 2u) / 5u
        + (unsigned)day - 1u;
    const unsigned doe = yoe * 365u + yoe / 4u - yoe / 100u + doy;
    return (int64_t)era * 146097 + (int64_t)doe - 719468;
}

static void civil_from_days(int64_t z, int *year, int *month, int *day)
{
    z += 719468;
    const int64_t era = (z >= 0 ? z : z - 146096) / 146097;
    const unsigned doe = (unsigned)(z - era * 146097);
    const unsigned yoe = (doe - doe / 1460u + doe / 36524u - doe / 146096u) / 365u;
    const int y = (int)yoe + (int)era * 400;
    const unsigned doy = doe - (365u * yoe + yoe / 4u - yoe / 100u);
    const unsigned mp = (5u * doy + 2u) / 153u;
    const unsigned d = doy - (153u * mp + 2u) / 5u + 1u;
    const unsigned m = mp < 10u ? mp + 3u : mp - 9u;
    *year = y + (m <= 2u);
    *month = (int)m;
    *day = (int)d;
}

static bool add_days(const meltdown_date_t *date, int delta, meltdown_date_t *out)
{
    if (!meltdown_date_valid(date)) return false;
    const int64_t days = days_from_civil(date->year, date->month, date->day) + delta;
    civil_from_days(days, &out->year, &out->month, &out->day);
    return meltdown_date_valid(out);
}

static void write_u16(uint8_t *out, uint16_t value)
{
    out[0] = (uint8_t)value;
    out[1] = (uint8_t)(value >> 8);
}

static void write_u32(uint8_t *out, uint32_t value)
{
    out[0] = (uint8_t)value;
    out[1] = (uint8_t)(value >> 8);
    out[2] = (uint8_t)(value >> 16);
    out[3] = (uint8_t)(value >> 24);
}

static uint16_t read_u16(const uint8_t *in)
{
    return (uint16_t)in[0] | ((uint16_t)in[1] << 8);
}

static uint32_t read_u32(const uint8_t *in)
{
    return (uint32_t)in[0] | ((uint32_t)in[1] << 8) |
           ((uint32_t)in[2] << 16) | ((uint32_t)in[3] << 24);
}

static void toggle_mute(meltdown_state_t *state, meltdown_effect_t *effect)
{
    state->record.muted = !state->record.muted;
    effect->changed = true;
    effect->stop_audio = state->record.muted;
    if (state->page == MELTDOWN_PAGE_MONTH || state->page == MELTDOWN_PAGE_STATS) state->mute_flash = true;
}

static void record_pending(meltdown_state_t *state, meltdown_effect_t *effect)
{
    if (state->record.pending < UINT32_MAX) state->record.pending++;
    effect->changed = true;
    effect->sound = meltdown_sound_for_count(state->record.pending, state->record.muted, false);
    effect->play = effect->sound != MELTDOWN_SOUND_NONE;
}

static void record_today(meltdown_state_t *state, meltdown_effect_t *effect)
{
    uint32_t *slot = &state->record.days[state->today.day - 1];
    if (*slot < UINT32_MAX) *slot += 1u;
    if (state->record.last_day < state->today.day) state->record.last_day = (uint8_t)state->today.day;
    effect->changed = true;
    effect->sound = meltdown_sound_for_count(*slot, state->record.muted, true);
    effect->play = effect->sound != MELTDOWN_SOUND_NONE;
    if (state->page == MELTDOWN_PAGE_MONTH || state->page == MELTDOWN_PAGE_STATS) state->page = MELTDOWN_PAGE_TODAY;
}

void meltdown_record_clear(meltdown_record_t *record)
{
    memset(record, 0, sizeof(*record));
}

void meltdown_state_init(meltdown_state_t *state)
{
    memset(state, 0, sizeof(*state));
    state->page = MELTDOWN_PAGE_SYNC;
}

bool meltdown_date_valid(const meltdown_date_t *date)
{
    if (!date || date->year < 2024 || date->year > 2099) return false;
    if (date->month < 1 || date->month > 12) return false;
    if (date->day < 1 || date->day > meltdown_days_in_month(date->year, date->month)) return false;
    return true;
}

bool meltdown_date_from_unix(int64_t unix_seconds, meltdown_date_t *out)
{
    if (!out) return false;
    const int64_t local = unix_seconds + SHANGHAI_OFFSET_SECONDS;
    if (local < 0) return false;
    civil_from_days(local / 86400, &out->year, &out->month, &out->day);
    return meltdown_date_valid(out);
}

int meltdown_days_in_month(int year, int month)
{
    static const int days[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (month < 1 || month > 12) return 0;
    if (month == 2 && ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)) return 29;
    return days[month];
}

int meltdown_weekday_mon0(const meltdown_date_t *date)
{
    if (!meltdown_date_valid(date)) return 0;
    const int64_t days = days_from_civil(date->year, date->month, date->day);
    int weekday = (int)((days % 7 + 7) % 7);
    weekday = (weekday + 3) % 7; /* 1970-01-01 was Thursday. */
    return weekday;
}

int meltdown_year_month_key(int year, int month)
{
    return year * 12 + month;
}

int meltdown_heat_level(uint32_t count)
{
    if (count == 0) return 0;
    if (count <= 2) return 1;
    if (count <= 5) return 2;
    if (count <= 9) return 3;
    return 4;
}

uint32_t meltdown_heat_hex(int level)
{
    if (level < 0 || level > 4) return HEAT_HEX[0];
    return HEAT_HEX[level];
}

uint16_t meltdown_rgb565(uint32_t hex)
{
    const unsigned r = (hex >> 16) & 0xFFu;
    const unsigned g = (hex >> 8) & 0xFFu;
    const unsigned b = hex & 0xFFu;
    return (uint16_t)(((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3));
}

meltdown_sound_t meltdown_sound_for_count(uint32_t count_after, bool muted, bool filing_trusted)
{
    if (muted || count_after == 0) return MELTDOWN_SOUND_NONE;
    if (!filing_trusted || count_after <= MELTDOWN_AUDIO_C3_MAX) return MELTDOWN_SOUND_C3;
    return MELTDOWN_SOUND_EGG;
}

void meltdown_format_count(uint32_t count, char *out, size_t out_len)
{
    if (!out || out_len == 0) return;
    if (count > MELTDOWN_DISPLAY_MAX) {
        snprintf(out, out_len, "99999+");
        return;
    }
    if (count < 10) snprintf(out, out_len, "%02u", (unsigned)count);
    else snprintf(out, out_len, "%u", (unsigned)count);
}

void meltdown_format_delta(const meltdown_delta_t *delta, char *out, size_t out_len)
{
    if (!out || out_len == 0) return;
    if (!delta || delta->kind == MELTDOWN_DELTA_NONE) {
        snprintf(out, out_len, "--");
        return;
    }
    if (delta->kind == MELTDOWN_DELTA_FLAT) {
        snprintf(out, out_len, "0");
        return;
    }
    if (delta->overflow) snprintf(out, out_len, "99999+");
    else snprintf(out, out_len, "%u", (unsigned)delta->magnitude);
}

void meltdown_compare_yesterday(const meltdown_state_t *state, meltdown_delta_t *out)
{
    memset(out, 0, sizeof(*out));
    if (!meltdown_can_file(state) || state->today.day <= 1) {
        out->kind = MELTDOWN_DELTA_NONE;
        return;
    }
    const uint32_t today = state->record.days[state->today.day - 1];
    const uint32_t yesterday = state->record.days[state->today.day - 2];
    if (today == yesterday) {
        out->kind = MELTDOWN_DELTA_FLAT;
        return;
    }
    if (today > yesterday) {
        const uint32_t diff = today - yesterday;
        out->kind = MELTDOWN_DELTA_UP;
        out->overflow = diff > MELTDOWN_DISPLAY_MAX;
        out->magnitude = out->overflow ? MELTDOWN_DISPLAY_MAX : diff;
        return;
    }
    const uint32_t diff = yesterday - today;
    out->kind = MELTDOWN_DELTA_DOWN;
    out->overflow = diff > MELTDOWN_DISPLAY_MAX;
    out->magnitude = out->overflow ? MELTDOWN_DISPLAY_MAX : diff;
}

int meltdown_bar_height_px(uint32_t count, uint32_t scale_max)
{
    if (count == 0 || scale_max == 0) return 0;
    uint32_t height = count * 27u / scale_max;
    if (height == 0) height = 1;
    if (height > 27u) height = 27u;
    return (int)height;
}

void meltdown_week_bars(const meltdown_state_t *state, meltdown_week_slot_t slots[7],
                        uint32_t *scale_max)
{
    memset(slots, 0, sizeof(meltdown_week_slot_t) * 7);
    uint32_t max_count = 0;
    if (!meltdown_can_file(state)) {
        for (int i = 0; i < 7; i++) slots[i].outside = true;
        if (scale_max) *scale_max = 0;
        return;
    }
    const int weekday = meltdown_weekday_mon0(&state->today);
    for (int i = 0; i < 7; i++) {
        meltdown_date_t date;
        if (!add_days(&state->today, i - weekday, &date) ||
            date.year != state->today.year || date.month != state->today.month) {
            slots[i].outside = true;
            continue;
        }
        if (date.day > state->today.day) {
            slots[i].future = true;
            continue;
        }
        slots[i].present = true;
        slots[i].count = state->record.days[date.day - 1];
        if (slots[i].count > max_count) max_count = slots[i].count;
    }
    if (scale_max) *scale_max = max_count;
}

void meltdown_month_grid(int year, int month, meltdown_grid_t *grid)
{
    memset(grid, 0, sizeof(*grid));
    meltdown_date_t first = {.year = year, .month = month, .day = 1};
    if (!meltdown_date_valid(&first)) return;
    const int leading = meltdown_weekday_mon0(&first);
    const int dim = meltdown_days_in_month(year, month);
    int columns = (leading + dim + 6) / 7;
    if (columns < 4) columns = 4;
    if (columns > 6) columns = 6;
    grid->columns = columns;
    grid->cell = columns >= 6 ? 22 : 26;
    grid->gap = 4;
    const int box_w = 5 * 26 + 4 * 4;
    const int box_h = 7 * 26 + 6 * 4;
    const int width = columns * grid->cell + (columns - 1) * grid->gap;
    const int height = 7 * grid->cell + 6 * grid->gap;
    grid->origin_x = 64 + (box_w - width) / 2;
    grid->origin_y = 75 + (box_h - height) / 2;
    for (int column = 0; column < columns; column++) {
        grid->header_day[column] = column == 0 ? 1 : column * 7 - leading + 1;
    }
}

void meltdown_cell_at(const meltdown_state_t *state, int column, int row, meltdown_cell_t *cell)
{
    memset(cell, 0, sizeof(*cell));
    if (!state || !meltdown_can_file(state) || column < 0 || row < 0 || row > 6) {
        cell->kind = MELTDOWN_CELL_OUTSIDE;
        return;
    }
    meltdown_grid_t grid;
    meltdown_month_grid(state->today.year, state->today.month, &grid);
    if (column >= grid.columns) {
        cell->kind = MELTDOWN_CELL_OUTSIDE;
        return;
    }
    meltdown_date_t first = {.year = state->today.year, .month = state->today.month, .day = 1};
    const int leading = meltdown_weekday_mon0(&first);
    const int day = column * 7 + row - leading + 1;
    const int dim = meltdown_days_in_month(state->today.year, state->today.month);
    if (day < 1 || day > dim) {
        cell->kind = MELTDOWN_CELL_OUTSIDE;
        return;
    }
    cell->day = day;
    cell->today = day == state->today.day;
    if (day > state->today.day) {
        cell->kind = MELTDOWN_CELL_FUTURE;
        return;
    }
    cell->count = state->record.days[day - 1];
    cell->heat = meltdown_heat_level(cell->count);
    cell->kind = cell->count == 0 ? MELTDOWN_CELL_ZERO : MELTDOWN_CELL_HEAT;
}

uint32_t meltdown_today_count(const meltdown_state_t *state)
{
    if (!meltdown_can_file(state)) return 0;
    return state->record.days[state->today.day - 1];
}

bool meltdown_can_file(const meltdown_state_t *state)
{
    if (!state || !state->clock_trusted || state->rollback || !state->record.active_valid) return false;
    if (!meltdown_date_valid(&state->today)) return false;
    return state->record.year == state->today.year && state->record.month == state->today.month;
}

void meltdown_note_unfiled(meltdown_state_t *state, meltdown_effect_t *effect)
{
    effect_clear(effect);
    if (!state) return;
    record_pending(state, effect);
}

void meltdown_apply_input(meltdown_state_t *state, meltdown_input_t input, meltdown_effect_t *effect)
{
    effect_clear(effect);
    if (!state) return;
    state->mute_flash = false;

    if (input == MELTDOWN_IN_UP_LONG && state->page == MELTDOWN_PAGE_SYNC) {
        effect->clear_credentials = true;
        return;
    }
    if (input == MELTDOWN_IN_OK_LONG) {
        toggle_mute(state, effect);
        return;
    }

    if (state->page == MELTDOWN_PAGE_ASSIGN) {
        if (input == MELTDOWN_IN_OK && meltdown_can_file(state)) {
            uint32_t *slot = &state->record.days[state->today.day - 1];
            const uint32_t room = UINT32_MAX - *slot;
            *slot += state->record.pending < room ? state->record.pending : room;
            state->record.pending = 0;
            state->record.pending_kept = false;
            if (state->record.last_day < state->today.day) {
                state->record.last_day = (uint8_t)state->today.day;
            }
            state->page = MELTDOWN_PAGE_TODAY;
            effect->changed = true;
        } else if (input == MELTDOWN_IN_DOWN) {
            state->record.pending_kept = true;
            state->page = MELTDOWN_PAGE_TODAY;
            effect->changed = true;
        }
        return;
    }

    if (!meltdown_can_file(state)) {
        if (state->clock_trusted && !state->rollback && state->record.active_valid &&
            meltdown_date_valid(&state->today) &&
            (state->record.year != state->today.year ||
             state->record.month != state->today.month)) {
            effect->needs_rollover = true;
            return;
        }
        if (input == MELTDOWN_IN_OK) record_pending(state, effect);
        return;
    }

    if (input == MELTDOWN_IN_UP) {
        if (state->page == MELTDOWN_PAGE_MONTH) state->page = MELTDOWN_PAGE_TODAY;
        else if (state->page == MELTDOWN_PAGE_TODAY) state->page = MELTDOWN_PAGE_STATS;
        return;
    }
    if (input == MELTDOWN_IN_DOWN) {
        if (state->page == MELTDOWN_PAGE_TODAY) state->page = MELTDOWN_PAGE_MONTH;
        else if (state->page == MELTDOWN_PAGE_STATS) state->page = MELTDOWN_PAGE_TODAY;
        return;
    }
    if (input != MELTDOWN_IN_OK) return;

    if (state->record.year != state->today.year || state->record.month != state->today.month) {
        effect->needs_rollover = true;
        return;
    }
    record_today(state, effect);
}

void meltdown_on_clock(meltdown_state_t *state, const meltdown_date_t *today, meltdown_effect_t *effect)
{
    effect_clear(effect);
    if (!state || !meltdown_date_valid(today)) return;
    state->clock_trusted = true;
    state->today = *today;
    if (state->record.active_valid &&
        meltdown_year_month_key(today->year, today->month) <
            meltdown_year_month_key(state->record.year, state->record.month)) {
        state->rollback = true;
        state->page = MELTDOWN_PAGE_SYNC;
        return;
    }
    state->rollback = false;
    if (!state->record.active_valid ||
        state->record.year != today->year || state->record.month != today->month) {
        effect->needs_rollover = true;
        return;
    }
    open_after_sync(state);
}

void meltdown_prepare_month(const meltdown_record_t *current, const meltdown_date_t *today,
                            meltdown_record_t *staged)
{
    *staged = *current;
    staged->year = (uint16_t)today->year;
    staged->month = (uint8_t)today->month;
    staged->last_day = 0;
    staged->active_valid = true;
    memset(staged->days, 0, sizeof(staged->days));
}

void meltdown_commit_month(meltdown_state_t *state, const meltdown_record_t *staged)
{
    const meltdown_page_t page = state->page;
    state->record = *staged;
    state->page = page;
    state->rollback = false;
    open_after_sync(state);
}

size_t meltdown_blob_pack(const meltdown_record_t *record, uint8_t *out, size_t out_len)
{
    if (!record || !out || out_len < MELTDOWN_BLOB_BYTES) return 0;
    memset(out, 0, MELTDOWN_BLOB_BYTES);
    out[0] = MELTDOWN_SCHEMA;
    out[1] = record->muted ? 1u : 0u;
    out[2] = record->month;
    out[3] = record->pending_kept ? 1u : 0u;
    write_u16(out + 4, record->year);
    out[6] = record->last_day;
    write_u32(out + 8, record->pending);
    for (int day = 0; day < 31; day++) write_u32(out + 12 + day * 4, record->days[day]);
    return MELTDOWN_BLOB_BYTES;
}

bool meltdown_blob_unpack(const uint8_t *in, size_t len, meltdown_record_t *record)
{
    if (!in || !record || len < MELTDOWN_BLOB_BYTES || in[0] != MELTDOWN_SCHEMA) return false;
    if (in[2] < 1 || in[2] > 12) return false;
    const int year = read_u16(in + 4);
    if (year < 2024 || year > 2099) return false;
    meltdown_record_clear(record);
    record->muted = in[1] != 0;
    record->month = in[2];
    record->pending_kept = in[3] != 0;
    record->year = (uint16_t)year;
    record->last_day = in[6];
    record->pending = read_u32(in + 8);
    for (int day = 0; day < 31; day++) record->days[day] = read_u32(in + 12 + day * 4);
    if (record->last_day > meltdown_days_in_month(record->year, record->month)) return false;
    record->active_valid = true;
    return true;
}

bool meltdown_slots_load(const meltdown_slots_t *slots, meltdown_record_t *out)
{
    if (!slots || !out || slots->active > 1) return false;
    if (slots->valid[slots->active] &&
        meltdown_blob_unpack(slots->blob[slots->active], MELTDOWN_BLOB_BYTES, out)) {
        return true;
    }
    const uint8_t other = (uint8_t)(1u - slots->active);
    return slots->valid[other] &&
           meltdown_blob_unpack(slots->blob[other], MELTDOWN_BLOB_BYTES, out);
}

bool meltdown_slots_stage(meltdown_slots_t *slots, const meltdown_record_t *record)
{
    if (!slots || !record || slots->active > 1) return false;
    const uint8_t inactive = (uint8_t)(1u - slots->active);
    if (meltdown_blob_pack(record, slots->blob[inactive], MELTDOWN_BLOB_BYTES) == 0) return false;
    slots->valid[inactive] = true;
    slots->generation[inactive] = slots->generation[slots->active] + 1u;
    return true;
}

bool meltdown_slots_publish(meltdown_slots_t *slots)
{
    if (!slots || slots->active > 1) return false;
    const uint8_t inactive = (uint8_t)(1u - slots->active);
    if (!slots->valid[inactive]) return false;
    slots->active = inactive;
    return true;
}

void meltdown_view_date(const meltdown_state_t *state, char *out, size_t out_len)
{
    if (!out || out_len == 0) return;
    if (!state || !meltdown_date_valid(&state->today) || !state->clock_trusted) {
        snprintf(out, out_len, "--.--");
        return;
    }
    snprintf(out, out_len, "%02d.%02d %s", state->today.month, state->today.day,
             meltdown_weekday_en(meltdown_weekday_mon0(&state->today)));
}

void meltdown_view_month(const meltdown_state_t *state, char *out, size_t out_len)
{
    if (!out || out_len == 0) return;
    if (!meltdown_can_file(state)) {
        snprintf(out, out_len, "----.--");
        return;
    }
    snprintf(out, out_len, "%04d.%02d", state->today.year, state->today.month);
}

const char *meltdown_weekday_en(int mon0)
{
    if (mon0 < 0 || mon0 > 6) return WEEKDAY_EN[0];
    return WEEKDAY_EN[mon0];
}

const char *meltdown_weekday_zh(int mon0)
{
    if (mon0 < 0 || mon0 > 6) return WEEKDAY_ZH[0];
    return WEEKDAY_ZH[mon0];
}
