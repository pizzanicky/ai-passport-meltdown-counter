#include "meltdown_model.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

static meltdown_date_t date(int year, int month, int day)
{
    meltdown_date_t value = {.year = year, .month = month, .day = day};
    return value;
}

static void trust(meltdown_state_t *state, int year, int month, int day)
{
    meltdown_effect_t effect;
    meltdown_date_t today = date(year, month, day);
    state->record.year = (uint16_t)year;
    state->record.month = (uint8_t)month;
    state->record.active_valid = true;
    meltdown_on_clock(state, &today, &effect);
    assert(!effect.needs_rollover);
    assert(meltdown_can_file(state));
}

static void press_ok(meltdown_state_t *state)
{
    meltdown_effect_t effect;
    meltdown_apply_input(state, MELTDOWN_IN_OK, &effect);
}

static int find_month(int weekday, int days, int *year, int *month)
{
    for (int y = 2024; y <= 2099; y++) {
        for (int m = 1; m <= 12; m++) {
            if (meltdown_days_in_month(y, m) != days) continue;
            meltdown_date_t first = date(y, m, 1);
            if (meltdown_weekday_mon0(&first) == weekday) {
                *year = y;
                *month = m;
                return 1;
            }
        }
    }
    return 0;
}

int main(void)
{
    assert(meltdown_days_in_month(2024, 2) == 29);
    assert(meltdown_days_in_month(2025, 2) == 28);
    assert(meltdown_days_in_month(2100, 2) == 28);
    assert(meltdown_days_in_month(2000, 2) == 29);
    assert(meltdown_days_in_month(2026, 4) == 30);
    assert(meltdown_days_in_month(2026, 10) == 31);

    meltdown_date_t epoch = date(2024, 1, 1);
    assert(meltdown_weekday_mon0(&epoch) == 0);
    assert(strcmp(meltdown_weekday_en(0), "MON") == 0);
    assert(strcmp(meltdown_weekday_en(4), "FRI") == 0);

    /* Direction belongs to the triangle, never a prefixed arithmetic sign. */
    for (int direction = MELTDOWN_DELTA_UP; direction <= MELTDOWN_DELTA_DOWN; direction++) {
        meltdown_delta_t d = {.kind = direction, .magnitude = 2};
        char value[16];
        meltdown_format_delta(&d, value, sizeof value);
        assert(strcmp(value, "2") == 0);
    }
    meltdown_delta_t flat = {.kind = MELTDOWN_DELTA_FLAT};
    char flat_value[16];
    meltdown_format_delta(&flat, flat_value, sizeof flat_value);
    assert(strcmp(flat_value, "0") == 0);

    /* 2024-01-01 00:00 CST is 2023-12-31 16:00 UTC. */
    meltdown_date_t shanghai;
    assert(meltdown_date_from_unix(1704038400, &shanghai));
    assert(shanghai.year == 2024 && shanghai.month == 1 && shanghai.day == 1);
    assert(!meltdown_date_from_unix(0, &shanghai));

    uint16_t seen[5];
    for (int level = 0; level < 5; level++) seen[level] = meltdown_rgb565(meltdown_heat_hex(level));
    assert(seen[0] == meltdown_rgb565(0xB1B6AF));
    for (int i = 0; i < 5; i++) {
        for (int j = i + 1; j < 5; j++) assert(seen[i] != seen[j]);
    }
    assert(meltdown_rgb565(0x454D4E) != seen[0]);
    assert(meltdown_heat_level(0) == 0);
    assert(meltdown_heat_level(1) == 1 && meltdown_heat_level(2) == 1);
    assert(meltdown_heat_level(3) == 2 && meltdown_heat_level(5) == 2);
    assert(meltdown_heat_level(6) == 3 && meltdown_heat_level(9) == 3);
    assert(meltdown_heat_level(10) == 4 && meltdown_heat_level(11) == 4);

    char text[16];
    meltdown_format_count(0, text, sizeof text);
    assert(strcmp(text, "00") == 0);
    meltdown_format_count(8, text, sizeof text);
    assert(strcmp(text, "08") == 0);
    meltdown_format_count(100, text, sizeof text);
    assert(strcmp(text, "100") == 0);
    meltdown_format_count(100000, text, sizeof text);
    assert(strcmp(text, "99999+") == 0);

    assert(meltdown_sound_for_count(1, false, true) == MELTDOWN_SOUND_C3);
    assert(meltdown_sound_for_count(5, false, true) == MELTDOWN_SOUND_C3);
    assert(meltdown_sound_for_count(6, false, true) == MELTDOWN_SOUND_EGG);
    assert(meltdown_sound_for_count(7, false, true) == MELTDOWN_SOUND_EGG);
    assert(meltdown_sound_for_count(9, false, false) == MELTDOWN_SOUND_C3);
    assert(meltdown_sound_for_count(6, true, true) == MELTDOWN_SOUND_NONE);
    assert(meltdown_sound_for_count(0, false, true) == MELTDOWN_SOUND_NONE);

    int year = 0, month = 0;
    assert(find_month(3, 31, &year, &month));
    meltdown_grid_t grid;
    meltdown_month_grid(year, month, &grid);
    assert(grid.columns == 5);
    assert(grid.cell == 26 && grid.gap == 4);
    assert(grid.origin_x == 64 && grid.origin_y == 75);
    assert(grid.header_day[0] == 1);
    assert(grid.header_day[1] == 5);
    assert(grid.header_day[2] == 12);
    assert(grid.header_day[3] == 19);
    assert(grid.header_day[4] == 26);

    assert(find_month(0, 28, &year, &month));
    meltdown_month_grid(year, month, &grid);
    assert(grid.columns == 4 && grid.cell == 26);
    const int width4 = 4 * 26 + 3 * 4;
    assert(grid.origin_x == 64 + (5 * 26 + 4 * 4 - width4) / 2);

    assert(find_month(6, 31, &year, &month));
    meltdown_month_grid(year, month, &grid);
    assert(grid.columns == 6 && grid.cell == 22 && grid.gap == 4);
    assert(grid.header_day[0] == 1);
    assert(grid.header_day[1] == 2);

    meltdown_state_t state;
    meltdown_state_init(&state);
    trust(&state, 2026, 10, 1);
    meltdown_delta_t delta;
    meltdown_compare_yesterday(&state, &delta);
    assert(delta.kind == MELTDOWN_DELTA_NONE);
    meltdown_format_delta(&delta, text, sizeof text);
    assert(strcmp(text, "--") == 0);

    trust(&state, 2026, 10, 4);
    state.record.days[0] = 2;
    state.record.days[1] = 0;
    state.record.days[2] = 4;
    state.record.days[3] = 1;
    meltdown_week_slot_t slots[7];
    uint32_t scale = 99;
    meltdown_week_bars(&state, slots, &scale);
    /* 2026-10-01 is Thursday, so the 4th is Sunday and Monday..Wednesday are September. */
    assert(meltdown_weekday_mon0(&state.today) == 6);
    assert(slots[0].outside && slots[1].outside && slots[2].outside);
    assert(slots[3].present && slots[3].count == 2);
    assert(slots[4].present && slots[4].count == 0);
    assert(slots[5].present && slots[5].count == 4);
    assert(slots[6].present && slots[6].count == 1);
    assert(scale == 4);
    assert(meltdown_bar_height_px(0, scale) == 0);
    assert(meltdown_bar_height_px(4, scale) == 27);
    assert(meltdown_bar_height_px(2, scale) == 13);
    assert(meltdown_bar_height_px(1, scale) == 6);

    meltdown_cell_t cell;
    meltdown_cell_at(&state, 0, 0, &cell);
    assert(cell.kind == MELTDOWN_CELL_OUTSIDE);
    meltdown_cell_at(&state, 0, 3, &cell);
    assert(cell.day == 1 && cell.kind == MELTDOWN_CELL_HEAT && cell.heat == 1 && !cell.today);
    meltdown_cell_at(&state, 0, 6, &cell);
    assert(cell.day == 4 && cell.today && cell.kind == MELTDOWN_CELL_HEAT);
    meltdown_cell_at(&state, 1, 0, &cell);
    assert(cell.day == 5 && cell.kind == MELTDOWN_CELL_FUTURE);

    meltdown_effect_t effect;
    meltdown_apply_input(&state, MELTDOWN_IN_OK_LONG, &effect);
    assert(state.record.muted && effect.stop_audio && effect.changed && !effect.play);
    assert(meltdown_today_count(&state) == 1);
    meltdown_apply_input(&state, MELTDOWN_IN_OK, &effect);
    assert(!effect.play && meltdown_today_count(&state) == 2);
    meltdown_apply_input(&state, MELTDOWN_IN_OK_LONG, &effect);
    assert(!state.record.muted && !effect.play);
    for (int i = 0; i < 2; i++) press_ok(&state);
    assert(meltdown_today_count(&state) == 4);
    meltdown_apply_input(&state, MELTDOWN_IN_OK, &effect);
    assert(effect.play && effect.sound == MELTDOWN_SOUND_C3);
    assert(meltdown_today_count(&state) == 5);
    meltdown_apply_input(&state, MELTDOWN_IN_OK, &effect);
    assert(effect.play && effect.sound == MELTDOWN_SOUND_EGG);
    assert(meltdown_today_count(&state) == 6);

    state.page = MELTDOWN_PAGE_MONTH;
    meltdown_apply_input(&state, MELTDOWN_IN_OK, &effect);
    assert(state.page == MELTDOWN_PAGE_TODAY);
    assert(meltdown_today_count(&state) == 7);
    assert(effect.sound == MELTDOWN_SOUND_EGG);
    meltdown_apply_input(&state, MELTDOWN_IN_DOWN, &effect);
    assert(state.page == MELTDOWN_PAGE_MONTH);
    meltdown_apply_input(&state, MELTDOWN_IN_DOWN, &effect);
    assert(state.page == MELTDOWN_PAGE_MONTH);
    meltdown_apply_input(&state, MELTDOWN_IN_UP, &effect);
    assert(state.page == MELTDOWN_PAGE_TODAY);
    meltdown_apply_input(&state, MELTDOWN_IN_UP, &effect);
    assert(state.page == MELTDOWN_PAGE_STATS);
    meltdown_apply_input(&state, MELTDOWN_IN_UP, &effect);
    assert(state.page == MELTDOWN_PAGE_STATS);
    meltdown_apply_input(&state, MELTDOWN_IN_DOWN, &effect);
    assert(state.page == MELTDOWN_PAGE_TODAY);

    state.page = MELTDOWN_PAGE_STATS;
    const uint32_t before_stats_press = meltdown_today_count(&state);
    meltdown_apply_input(&state, MELTDOWN_IN_OK_LONG, &effect);
    assert(state.page == MELTDOWN_PAGE_STATS);
    assert(meltdown_today_count(&state) == before_stats_press);
    meltdown_apply_input(&state, MELTDOWN_IN_OK, &effect);
    assert(state.page == MELTDOWN_PAGE_TODAY);
    assert(meltdown_today_count(&state) == before_stats_press + 1);

    state.record.days[3] = UINT32_MAX;
    press_ok(&state);
    assert(meltdown_today_count(&state) == UINT32_MAX);
    meltdown_format_count(meltdown_today_count(&state), text, sizeof text);
    assert(strcmp(text, "99999+") == 0);

    meltdown_state_init(&state);
    meltdown_apply_input(&state, MELTDOWN_IN_OK, &effect);
    assert(effect.play && effect.sound == MELTDOWN_SOUND_C3);
    assert(state.record.pending == 1);
    for (int i = 0; i < 8; i++) press_ok(&state);
    meltdown_apply_input(&state, MELTDOWN_IN_OK, &effect);
    assert(state.record.pending == 10);
    assert(effect.sound == MELTDOWN_SOUND_C3);
    meltdown_date_t synced = date(2026, 10, 4);
    meltdown_on_clock(&state, &synced, &effect);
    assert(effect.needs_rollover);
    assert(!meltdown_can_file(&state));
    meltdown_record_t staged;
    meltdown_prepare_month(&state.record, &synced, &staged);
    assert(staged.pending == 10 && staged.days[3] == 0 && staged.month == 10);
    assert(state.record.pending == 10);
    meltdown_commit_month(&state, &staged);
    assert(state.page == MELTDOWN_PAGE_ASSIGN);
    assert(meltdown_today_count(&state) == 0);
    meltdown_apply_input(&state, MELTDOWN_IN_OK, &effect);
    assert(!effect.play);
    assert(state.record.pending == 0);
    assert(meltdown_today_count(&state) == 10);
    assert(state.page == MELTDOWN_PAGE_TODAY);

    meltdown_state_init(&state);
    state.record.pending = 4;
    trust(&state, 2026, 10, 8);
    assert(state.page == MELTDOWN_PAGE_ASSIGN);
    meltdown_apply_input(&state, MELTDOWN_IN_DOWN, &effect);
    assert(state.record.pending_kept && state.record.pending == 4);
    assert(meltdown_today_count(&state) == 0);
    assert(state.page == MELTDOWN_PAGE_TODAY);
    press_ok(&state);
    assert(meltdown_today_count(&state) == 1);
    assert(state.record.pending == 4);

    uint32_t kept_days[31];
    memcpy(kept_days, state.record.days, sizeof kept_days);
    meltdown_date_t backward = date(2026, 9, 30);
    meltdown_on_clock(&state, &backward, &effect);
    assert(state.rollback);
    assert(memcmp(state.record.days, kept_days, sizeof kept_days) == 0);
    press_ok(&state);
    assert(state.record.pending == 5);
    assert(state.record.days[7] == 1);

    uint8_t blob[MELTDOWN_BLOB_BYTES];
    state.rollback = false;
    state.record.muted = true;
    assert(meltdown_blob_pack(&state.record, blob, sizeof blob) == MELTDOWN_BLOB_BYTES);
    meltdown_record_t unpacked;
    assert(meltdown_blob_unpack(blob, sizeof blob, &unpacked));
    assert(unpacked.muted && unpacked.pending == 5 && unpacked.days[7] == 1);
    assert(unpacked.year == 2026 && unpacked.month == 10);
    blob[0] = 9;
    assert(!meltdown_blob_unpack(blob, sizeof blob, &unpacked));

    meltdown_slots_t store;
    memset(&store, 0, sizeof store);
    state.record.days[0] = 3;
    assert(meltdown_slots_stage(&store, &state.record));
    assert(meltdown_slots_load(&store, &unpacked));
    assert(unpacked.days[0] == 3);
    assert(meltdown_slots_publish(&store));
    assert(meltdown_slots_load(&store, &unpacked));
    assert(unpacked.days[0] == 3);
    meltdown_record_t newer = unpacked;
    newer.days[0] = 9;
    assert(meltdown_slots_stage(&store, &newer));
    assert(meltdown_slots_load(&store, &unpacked));
    assert(unpacked.days[0] == 3);
    assert(meltdown_slots_publish(&store));
    assert(meltdown_slots_load(&store, &unpacked));
    assert(unpacked.days[0] == 9);

    trust(&state, 2026, 11, 1);
    state.record = unpacked;
    state.record.year = 2026;
    state.record.month = 10;
    state.record.active_valid = true;
    state.clock_trusted = true;
    state.today = date(2026, 11, 1);
    state.rollback = false;
    meltdown_apply_input(&state, MELTDOWN_IN_OK, &effect);
    assert(effect.needs_rollover);
    assert(state.record.days[0] == 9);
    state.record.pending = 0;
    state.record.muted = false;
    meltdown_note_unfiled(&state, &effect);
    assert(state.record.pending == 1);
    assert(effect.play);
    assert(effect.sound == MELTDOWN_SOUND_C3);
    assert(state.record.days[0] == 9);

    assert(meltdown_bar_height_px(1, 1000) == 1);
    puts("meltdown model tests passed");
    return 0;
}
