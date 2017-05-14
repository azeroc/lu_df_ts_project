#pragma once
#include "common.h"
#include "touch_structure.h"
#include "tcp_ts_client.h"

#ifdef linux
    #include "linux_impl/dev_monitor.h"
#endif 

class touch_event_parser
{
public:
    enum input_type
    {
        device,
        test_file
    };

    touch_event_parser(boost::asio::io_service& io_service, 
        std::string input, 
        touch_event_parser::input_type type, 
        boost::shared_ptr<tcp_ts_client> tcp_client) 
    {
        this->tcp_client = tcp_client;

        if (type == touch_event_parser::input_type::device)
        {
#ifdef linux
            initialize_dev_mon(io_service, input);
#else
            std::cerr << "Device monitor initialization is only supported on linux-type operating systems" << std::endl;
#endif
        }
        else
        {
            initialize_file_parser(io_service, input);
        }
    }

private:
    boost::shared_ptr<tcp_ts_client> tcp_client;

    void on_touch_data_receive(std::vector<touch_data> container);

    void initialize_file_parser(boost::asio::io_service& io_service, std::string input)
    {
    }

#ifdef linux
    boost::shared_ptr<dev_monitor> dev_mon;

    void initialize_dev_mon(boost::asio::io_service& io_service, std::string input)
    {
        dev_mon = boost::make_shared<dev_monitor>(io_service, input,
                boost::bind(&touch_event_parser::on_touch_data_receive,
                this,
                boost::placeholders::_1));
    }
#endif
};