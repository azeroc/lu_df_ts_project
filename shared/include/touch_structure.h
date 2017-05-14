#pragma once
#include "common.h"

struct touch_data
{
    enum { touch_data_size = 5 * sizeof(uint32_t) };

    // Update flags to determine what touch_data should be used
    enum update_flags
    {
        update_none = 0x0,
        update_x = 0x1,
        update_y = 0x2,
        update_tracking_id = 0x4,
        update_all = update_x | update_y | update_tracking_id
    };

    std::uint32_t slot;
    std::int32_t tracking_id;
    std::uint32_t x;
    std::uint32_t y;
    std::uint32_t upd_flags;

    touch_data()
    {
        this->slot = 0;
        this->tracking_id = 0;
        this->x = 0;
        this->y = 0;
        this->upd_flags = update_flags::update_none;
    }

    touch_data(std::uint32_t slot, std::int32_t tracking_id, std::uint32_t x, std::uint32_t y, update_flags upd_flags)
    {
        this->slot = slot;
        this->tracking_id = tracking_id;
        this->x = x;
        this->y = y;
        this->upd_flags = upd_flags;
    }
};