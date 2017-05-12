#include "tcp_session.h"

void tcp_session::read_touch_packet()
{
    network_packet packet(touch_data::touch_data_size);

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
        touch_data data = network_packet::deserialize_touch_data(packet);
        on_read_finish(this->shared_from_this(), data);
    }
    else
    {
        on_error(this->shared_from_this(), error, tcp_session::error_origin::read);
    }
}

void tcp_session::write_touch_packet(const touch_data& data)
{
    network_packet packet = network_packet::serialize_touch_data(data);
    session_socket.async_send(packet,
        boost::bind(&tcp_session::write_touch_packet_callback,
            this->shared_from_this(),
            boost::asio::placeholders::error));
}

void tcp_session::write_touch_packet_callback(const boost::system::error_code& error)
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