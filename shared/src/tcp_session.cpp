#include "tcp_session.h"
#include "common.h"
#include "packet_structure.h"
#include "touch_structure.h"

void tcp_session::read_touch_packets()
{
    network_packet packet(4 + network_packet::max_packet_size);

    session_socket.async_read_some(packet,
        boost::bind(&tcp_session::read_touch_packet_callback,
            this->shared_from_this(),
            packet,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
}

void tcp_session::read_touch_packet_callback(const network_packet& packet, const boost::system::error_code& error, size_t bytes_transferred)
{
    if (!error)
    {
        if (bytes_transferred < (4 + touch_data::touch_data_size))
        {
            on_error(this->shared_from_this(), error, tcp_session::error_origin::bad_packet_small);
        }
        else
        {
            std::vector<touch_data> container = network_packet::deserialize_touch_data_container(packet);
            on_read_finish(this->shared_from_this(), container);
        }
    }
    else
    {
        on_error(this->shared_from_this(), error, tcp_session::error_origin::read);
    }
}

void tcp_session::write_touch_packet_container(const std::vector<touch_data>& container)
{
    network_packet packet = network_packet::serialize_touch_data_container(container);
    session_socket.async_send(packet,
        boost::bind(&tcp_session::write_touch_packet_container_callback,
            this->shared_from_this(),
            boost::asio::placeholders::error));
}

void tcp_session::write_touch_packet_container_callback(const boost::system::error_code& error)
{
    if (!error)
    {
        on_write_finish(this->shared_from_this());
    }
    else
    {
        on_error(this->shared_from_this(), error, tcp_session::error_origin::write);
    }
}