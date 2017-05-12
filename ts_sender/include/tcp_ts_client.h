#pragma once
#include "common.h"
#include "tcp_session.h"
#include "packet_structure.h"
#include "touch_structure.h"

class tcp_ts_client
{
public:
    tcp_ts_client(boost::asio::io_service& io_service, boost::asio::ip::tcp::resolver::iterator endpoint_iterator)
    {
        connect(io_service, endpoint_iterator);
    }
private:
    void connect(boost::asio::io_service& io_service, boost::asio::ip::tcp::resolver::iterator endpoint_iterator);
    void send_touch_event();

    void on_connection(const boost::system::error_code& error);
    void on_read_finish(boost::shared_ptr<tcp_session> session, const touch_data& data) {} // Not used for client-side
    void on_write_finish(boost::shared_ptr<tcp_session> session);
    void on_error(boost::shared_ptr<tcp_session> session, const boost::system::error_code& error, tcp_session::error_origin origin);

    tcp_session::pointer server_session;
};