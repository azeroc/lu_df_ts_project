#include "tcp_ts_client.h"
#include "common.h"
#include "tcp_session.h"
#include "packet_structure.h"
#include "touch_structure.h"

void tcp_ts_client::connect(boost::asio::io_service& io_service, boost::asio::ip::tcp::resolver::iterator endpoint_iterator)
{
    server_session = tcp_session::create(io_service,
        boost::bind(&tcp_ts_client::on_read_finish, this, boost::placeholders::_1, boost::placeholders::_2),
        boost::bind(&tcp_ts_client::on_write_finish, this, boost::placeholders::_1),
        boost::bind(&tcp_ts_client::on_error, this, boost::placeholders::_1, boost::placeholders::_2, boost::placeholders::_3));

    std::cout << "Making connection to server [" 
        << endpoint_iterator->endpoint().address().to_string() 
        << ":" 
        << endpoint_iterator->endpoint().port() << "] ... ";
    std::cout.flush();

    boost::asio::async_connect(
        server_session->socket(), 
        endpoint_iterator,
        boost::bind(&tcp_ts_client::on_connection, this, boost::asio::placeholders::error));
}

void tcp_ts_client::on_connection(const boost::system::error_code& error)
{
    if (!error)
    {
        get_std_io_mutex().lock();
        std::cout << "successful!" << std::endl;

        // Enable keep-alive for better sustained connection
        boost::asio::socket_base::keep_alive option(true);
        server_session->socket().set_option(option);

        std::cout << "Client ready! Accepting touch input send requests..." << std::endl;
        get_std_io_mutex().unlock();
    }
    else
    {
        get_std_io_mutex().lock();
        std::cout << "failed!" << std::endl;
        std::cout << "Error code: " << error.value() << std::endl;
        std::cout << "Error message: " << error.message() << std::endl;
        get_std_io_mutex().unlock();
        exit(2);
    }
}

void tcp_ts_client::send_touch_data_container(const std::vector<touch_data>& container)
{
    // Output sent touch data
    get_std_io_mutex().lock();
    std::cout << "[Connection to: "
        << server_session->socket().remote_endpoint().address().to_string()
        << ":" 
        << server_session->socket().remote_endpoint().port()
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

    server_session->write_touch_packet_container(container);
}

void tcp_ts_client::on_write_finish(boost::shared_ptr<tcp_session> session)
{
}

void tcp_ts_client::on_error(boost::shared_ptr<tcp_session> session, const boost::system::error_code & error, tcp_session::error_origin origin)
{
    if (origin == tcp_session::error_origin::write)
    {
        get_std_io_mutex().lock();
        std::cerr << "[Connection to: "
            << session->socket().remote_endpoint().address().to_string()
            << ":"
            << session->socket().remote_endpoint().port()
            << "] ERROR ("
            << error.value()
            << "): "
            << error.message()
            << std::endl;
        get_std_io_mutex().unlock();
        exit(2);
    }
}