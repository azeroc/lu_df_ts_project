#pragma once
#include "common.h"

enum touch_ev_type
{
    ev_syn = 0,
    ev_key = 1,
    ev_abs = 3
};

enum touch_ev_key_code
{
    btn_touch = 330
};

enum touch_ev_abs_code
{
    abs_x = 0,
    abs_y = 1,
    abs_mt_slot = 47,
    abs_mt_position_x = 53,
    abs_mt_position_y = 54,
    abs_mt_tracking_id = 57
};

struct touch_data
{
    enum { touch_data_size = 4 * sizeof(uint32_t) };

    std::uint32_t slot;
    std::uint32_t tracking_id;
    std::uint32_t x;
    std::uint32_t y;
};