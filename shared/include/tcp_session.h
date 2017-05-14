#pragma once
#include <common.h>
#include "packet_structure.h"

// TCP session class for proper touch-signal packet communication
class tcp_session : public boost::enable_shared_from_this<tcp_session>
{
public:
    enum error_origin
    {
        write,
        read,
        bad_packet_small,
        unknown
    };

    // Pointer shared_ptr type of TCP session
    typedef boost::shared_ptr<tcp_session> pointer;

    // Shortening function types
    typedef boost::function<void(boost::shared_ptr<tcp_session>, const std::vector<touch_data>&)> read_callback_function;
    typedef boost::function<void(boost::shared_ptr<tcp_session>)> write_callback_function;
    typedef boost::function<void(boost::shared_ptr<tcp_session>, const boost::system::error_code&, tcp_session::error_origin)> error_function;

    // Factory-pattern instance creation
    static pointer create(boost::asio::io_service& io_service,
        const read_callback_function& on_read,
        const write_callback_function& on_write,
        const error_function& on_error)
    {
        return pointer(new tcp_session(io_service, on_read, on_write, on_error));
    }

    boost::asio::ip::tcp::socket& socket()
    {
        return session_socket;
    }

    void read_touch_packets();
    void write_touch_packet_container(const std::vector<touch_data>& container);

private:
    tcp_session(boost::asio::io_service& io_service, 
        const read_callback_function& on_read_finish,
        const write_callback_function& on_write_finish,
        const error_function& on_error)
        : session_socket(io_service)
    {        
        this->on_read_finish = on_read_finish;
        this->on_write_finish = on_write_finish;
        this->on_error = on_error;
    }

    void read_touch_packet_callback(const network_packet& packet, const boost::system::error_code& error, size_t bytes_transferred);
    void write_touch_packet_container_callback(const boost::system::error_code& error);

    // Variables
    read_callback_function on_read_finish;
    write_callback_function on_write_finish;
    error_function on_error;
    boost::asio::ip::tcp::socket session_socket;
};