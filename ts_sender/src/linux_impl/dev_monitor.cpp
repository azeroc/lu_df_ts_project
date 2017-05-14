#include "linux_impl/dev_monitor.h"
#include "common.h"

void dev_monitor::read_events()
{
    event_vec.resize(dev_monitor::max_event_buf_size);
    sd.async_read_some(boost::asio::buffer(event_vec), 
        boost::bind(&dev_monitor::handle_events, 
        this, 
        boost::asio::placeholders::error, 
        boost::asio::placeholders::bytes_transferred));
}

void dev_monitor::handle_events(boost::system::error_code error, size_t bytes_transferred)
{
    if (!error) 
    {
        std::vector<touch_data> data_vec;
        touch_data current_touch_data(current_touch_slot, 0, 0, 0, 
            touch_data::update_flags::update_none);
        auto const n = bytes_transferred / sizeof(input_event);

        for (input_event& ev : event_vec | boost::adaptors::sliced(0, n)) 
        {
            switch(ev.type)
            {
                case EV_SYN:
                    data_vec.push_back(current_touch_data);
                    on_touch_data(data_vec);
                    data_vec.clear();
                    break;
                case EV_ABS:
                    switch(ev.code)
                    {
                        case ABS_MT_SLOT:
                            current_touch_slot = ev.value;

                            if (current_touch_data.upd_flags != touch_data::update_flags::update_none)
                            {
                                data_vec.push_back(current_touch_data);
                            }

                            current_touch_data = touch_data(current_touch_slot, 0, 0, 0, 
                                touch_data::update_flags::update_none);
                            break;
                        case ABS_MT_TRACKING_ID: 
                            current_touch_data.tracking_id = ev.value;
                            current_touch_data.upd_flags |= touch_data::update_flags::update_tracking_id;
                            break;
                        case ABS_MT_POSITION_X:
                            current_touch_data.x = ev.value;
                            current_touch_data.upd_flags |= touch_data::update_flags::update_x;
                            break;
                        case ABS_MT_POSITION_Y:
                            current_touch_data.y = ev.value;
                            current_touch_data.upd_flags |= touch_data::update_flags::update_y;
                            break;
                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }
        }

        read_events();
    } else {
        std::cerr << error.message() << "\n";
        exit(0);
    }
}