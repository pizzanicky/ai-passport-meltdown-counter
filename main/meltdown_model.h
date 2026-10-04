#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Pure calendar, counting, and persistence rules for the meltdown counter.
 * Beijing time is fixed UTC+8. This header does not include ESP-IDF or LVGL.
 */

#define MELTDOWN_BLOB_BYTES 136
#define MELTDOWN_DISPLAY_MAX 99999u
#define MELTDOWN_AUDIO_C3_MAX 5u
#define MELTDOWN_SCHEMA 1u

typedef enum {
    MELTDOWN_SOUND_NONE = 0,
    MELTDOWN_SOUND_C3,
    MELTDOWN_SOUND_EGG,
} meltdown_sound_t;

typedef enum {
    MELTDOWN_PAGE_TODAY = 0,
    MELTDOWN_PAGE_MONTH,
    MELTDOWN_PAGE_SYNC,
    MELTDOWN_PAGE_ASSIGN,
    MELTDOWN_PAGE_STATS,
} meltdown_page_t;

typedef enum {
    MELTDOWN_IN_UP = 1,
    MELTDOWN_IN_DOWN,
    MELTDOWN_IN_OK,
    MELTDOWN_IN_OK_LONG,
    MELTDOWN_IN_UP_LONG,
} meltdown_input_t;

typedef enum {
    MELTDOWN_CELL_OUTSIDE = 0,
    MELTDOWN_CELL_FUTURE,
    MELTDOWN_CELL_ZERO,
    MELTDOWN_CELL_HEAT,
} meltdown_cell_kind_t;

typedef enum {
    MELTDOWN_DELTA_NONE = 0,
    MELTDOWN_DELTA_FLAT,
    MELTDOWN_DELTA_UP,
    MELTDOWN_DELTA_DOWN,
} meltdown_delta_kind_t;

typedef struct {
    int year;
    int month;
    int day;
} meltdown_date_t;

typedef struct {
    uint16_t year;
    uint8_t month;
    uint8_t last_day;
    uint32_t days[31];
    uint32_t pending;
    bool pending_kept;
    bool muted;
    bool active_valid;
} meltdown_record_t;

typedef struct {
    meltdown_record_t record;
    bool clock_trusted;
    bool rollback;
    meltdown_date_t today;
    meltdown_page_t page;
    bool mute_flash;
} meltdown_state_t;

typedef struct {
    bool changed;
    bool play;
    meltdown_sound_t sound;
    bool stop_audio;
    bool needs_rollover;
    bool clear_credentials;
} meltdown_effect_t;

typedef struct {
    uint8_t blob[2][MELTDOWN_BLOB_BYTES];
    bool valid[2];
    uint32_t generation[2];
    uint8_t active;
} meltdown_slots_t;

typedef struct {
    meltdown_delta_kind_t kind;
    uint32_t magnitude;
    bool overflow;
} meltdown_delta_t;

typedef struct {
    bool outside;
    bool future;
    bool present;
    uint32_t count;
} meltdown_week_slot_t;

typedef struct {
    int columns;
    int cell;
    int gap;
    int origin_x;
    int origin_y;
    int header_day[6];
} meltdown_grid_t;

typedef struct {
    meltdown_cell_kind_t kind;
    int day;
    uint32_t count;
    int heat;
    bool today;
} meltdown_cell_t;

void meltdown_state_init(meltdown_state_t *state);
void meltdown_record_clear(meltdown_record_t *record);

bool meltdown_date_valid(const meltdown_date_t *date);
bool meltdown_date_from_unix(int64_t unix_seconds, meltdown_date_t *out);
int meltdown_days_in_month(int year, int month);
int meltdown_weekday_mon0(const meltdown_date_t *date);
int meltdown_year_month_key(int year, int month);

int meltdown_heat_level(uint32_t count);
uint32_t meltdown_heat_hex(int level);
uint16_t meltdown_rgb565(uint32_t hex);

meltdown_sound_t meltdown_sound_for_count(uint32_t count_after, bool muted,
                                          bool filing_trusted);

void meltdown_format_count(uint32_t count, char *out, size_t out_len);
void meltdown_format_delta(const meltdown_delta_t *delta, char *out, size_t out_len);
void meltdown_compare_yesterday(const meltdown_state_t *state, meltdown_delta_t *out);
void meltdown_week_bars(const meltdown_state_t *state, meltdown_week_slot_t slots[7],
                        uint32_t *scale_max);
int meltdown_bar_height_px(uint32_t count, uint32_t scale_max);

void meltdown_month_grid(int year, int month, meltdown_grid_t *grid);
void meltdown_cell_at(const meltdown_state_t *state, int column, int row,
                      meltdown_cell_t *cell);
uint32_t meltdown_today_count(const meltdown_state_t *state);
bool meltdown_can_file(const meltdown_state_t *state);

void meltdown_apply_input(meltdown_state_t *state, meltdown_input_t input,
                          meltdown_effect_t *effect);
/* Count one press as pending when a month rollover could not be committed. */
void meltdown_note_unfiled(meltdown_state_t *state, meltdown_effect_t *effect);
void meltdown_on_clock(meltdown_state_t *state, const meltdown_date_t *today,
                       meltdown_effect_t *effect);
void meltdown_prepare_month(const meltdown_record_t *current, const meltdown_date_t *today,
                            meltdown_record_t *staged);
void meltdown_commit_month(meltdown_state_t *state, const meltdown_record_t *staged);

size_t meltdown_blob_pack(const meltdown_record_t *record, uint8_t *out, size_t out_len);
bool meltdown_blob_unpack(const uint8_t *in, size_t len, meltdown_record_t *record);
bool meltdown_slots_load(const meltdown_slots_t *slots, meltdown_record_t *out);
bool meltdown_slots_stage(meltdown_slots_t *slots, const meltdown_record_t *record);
bool meltdown_slots_publish(meltdown_slots_t *slots);

void meltdown_view_date(const meltdown_state_t *state, char *out, size_t out_len);
void meltdown_view_month(const meltdown_state_t *state, char *out, size_t out_len);
const char *meltdown_weekday_en(int mon0);
const char *meltdown_weekday_zh(int mon0);
