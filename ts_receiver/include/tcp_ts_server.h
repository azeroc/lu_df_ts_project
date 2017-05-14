#pragma once
#include "common.h"
#include "tcp_session.h"

class tcp_ts_server
{
public:
    tcp_ts_server(boost::asio::io_service& io_service, unsigned short port)
        : session_acceptor(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
    {
        listen();
    }

private:
    void listen();

    void accept_handler(tcp_session::pointer new_session, const boost::system::error_code& error);

    void on_read_finish(boost::shared_ptr<tcp_session> session, const std::vector<touch_data>& container);
    void on_write_finish(boost::shared_ptr<tcp_session> session) {}; // Not used for server-side
    void on_error(boost::shared_ptr<tcp_session> session, const boost::system::error_code & error, tcp_session::error_origin origin);
    std::mutex& get_std_io_mutex()
    {
        static std::mutex std_io_mtx;
        return std_io_mtx;
    }
private:
    boost::asio::ip::tcp::acceptor session_acceptor;

};