#include "tcp_ts_server.h"
#include "common.h"
#include "tcp_session.h"
#include "touch_structure.h"

void tcp_ts_server::listen()
{
    tcp_session::pointer new_session;
    new_session = tcp_session::create(session_acceptor.get_io_service(),
        boost::bind(&tcp_ts_server::on_read_finish, this, boost::placeholders::_1, boost::placeholders::_2),
        boost::bind(&tcp_ts_server::on_write_finish, this, boost::placeholders::_1),
        boost::bind(&tcp_ts_server::on_error, this, boost::placeholders::_1, boost::placeholders::_2, boost::placeholders::_3));

    session_acceptor.async_accept(new_session->socket(),
        boost::bind(&tcp_ts_server::accept_handler, this, new_session, boost::asio::placeholders::error));
}

void tcp_ts_server::accept_handler(tcp_session::pointer new_session, const boost::system::error_code& error)
{
    if (!error)
    {
        get_std_io_mutex().lock();
        std::cout << "[Client: "
            << new_session->socket().remote_endpoint().address().to_string()
            << ":"
            << new_session->socket().remote_endpoint().port()
            << "] Connection established."
            << std::endl;
        get_std_io_mutex().unlock();

        new_session->read_touch_packets();
    }

    listen();
}

void tcp_ts_server::on_read_finish(boost::shared_ptr<tcp_session> session, const std::vector<touch_data>& container)
{
    // Handle received touch data
    get_std_io_mutex().lock();
    std::cout << "[Client: "
        << session->socket().remote_endpoint().address().to_string()
        << ":" 
        << session->socket().remote_endpoint().port()
        << "]"
        << std::endl;
    for (size_t i = 0; i < container.size(); i++)
    {
        touch_data data = container[i];
        std::cout << "{ slot = "
        << data.slot
        << ", tracking_id = "
        << data.tracking_id
        << ", x = "
        << data.x
        << ", y = "
        << data.y
        << " }"
        << std::endl;
    }
    get_std_io_mutex().unlock();

    // Continue receiving packets
    session->read_touch_packets();
}

void tcp_ts_server::on_error(boost::shared_ptr<tcp_session> session, const boost::system::error_code & error, tcp_session::error_origin origin)
{
    if (origin == tcp_session::error_origin::read)
    {
        get_std_io_mutex().lock();
        std::cout << "[Client: "
            << session->socket().remote_endpoint().address().to_string()
            << ":"
            << session->socket().remote_endpoint().port()
            << "] Disconnected ("
            << error.value()
            << "): "
            << error.message()
            << std::endl;
        get_std_io_mutex().unlock();
    }
    else if (origin == tcp_session::error_origin::bad_packet_small)
    {
        get_std_io_mutex().lock();
        std::cout << "[Client: "
            << session->socket().remote_endpoint().address().to_string()
            << ":"
            << session->socket().remote_endpoint().port()
            << "] Bad packet (packet too small to gain any valuable information)"
            << std::endl;
        get_std_io_mutex().unlock();

        // Continue receiving packets
        session->read_touch_packets();
    }
}