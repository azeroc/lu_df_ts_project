#pragma once
#include "common.h"
#include "constants.h"
#include "touch_structure.h"
#include <boost/asio/posix/stream_descriptor.hpp>
#include <iomanip>
#include <linux/input.h> // for input_event
#include <boost/range/adaptor/sliced.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

// Monitors/reads input_event of a device and converts them into touch_data
class dev_monitor
{
public:
    typedef boost::function<void(std::vector<touch_data>)> on_touch_data_function;

    // Device monitor configuration constants
    // TODO: move to .ini config file
    enum
    {
        max_event_buf_size = 128,
        resolution_x = 1920,
        resolution_y = 1080
    };

    dev_monitor(boost::asio::io_service& io_service, std::string dev_path, on_touch_data_function on_touch_data) 
        : sd(io_service, open(dev_path.c_str(), O_RDONLY)), on_touch_data(on_touch_data)
    {
        current_touch_slot = 0;
        read_events();
    }
private:
    boost::asio::posix::stream_descriptor sd;
    std::vector<input_event> event_vec;
    on_touch_data_function on_touch_data;
    std::uint32_t current_touch_slot;

    void read_events();
    void handle_events(boost::system::error_code error, size_t bytes_transferred);
};