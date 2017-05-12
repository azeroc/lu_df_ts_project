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
        std::cout << "successful!" << std::endl;
        std::cout << "Client ready! Sending touch event packets now..." << std::endl;

        // Enable keep-alive for better sustained connection
        boost::asio::socket_base::keep_alive option(true);
        server_session->socket().set_option(option);

        send_touch_event();
    }
    else
    {
        std::cout << "failed!" << std::endl;
        std::cout << "Error code: " << error.value() << std::endl;
        std::cout << "Error message: " << error.message() << std::endl;
        exit(2);
    }
}

void tcp_ts_client::send_touch_event()
{
    // Build next touch event data struct from input events

    // Quick test
    
    touch_data data;
    data.slot = std::rand() % 10;
    data.tracking_id = std::rand() % 65565;
    data.x = std::rand() % 4096;
    data.y = std::rand() % 4096;

    server_session->write_touch_packet(data);
}

void tcp_ts_client::on_write_finish(boost::shared_ptr<tcp_session> session)
{
    std::this_thread::sleep_for(std::chrono::seconds(1));
    send_touch_event();
}

void tcp_ts_client::on_error(boost::shared_ptr<tcp_session> session, const boost::system::error_code & error, tcp_session::error_origin origin)
{

}