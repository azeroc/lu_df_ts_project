#include "tcp_session.h"
#include "common.h"
#include "packet_structure.h"
#include "touch_structure.h"

void tcp_session::read_touch_packets()
{
    read_header();
}

void tcp_session::read_header()
{
    boost::shared_ptr<network_packet> packet = boost::make_shared<network_packet>(4);

    session_socket.async_read_some(*packet,
        boost::bind(&tcp_session::read_header_callback,
            this->shared_from_this(),
            packet,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
}

void tcp_session::read_header_callback(boost::shared_ptr<network_packet> packet, const boost::system::error_code& error, size_t bytes_transferred)
{
    if (!error)
    {
        std::uint32_t item_size = network_packet::deserialize_size(*packet);
        boost::shared_ptr<network_packet> packet2 = boost::make_shared<network_packet>(item_size * touch_data::touch_data_size);

        session_socket.async_read_some(*packet2,
            boost::bind(&tcp_session::read_touch_packet_callback,
                this->shared_from_this(),
                packet2,
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred));
    }
    else
    {
        on_error(this->shared_from_this(), error, tcp_session::error_origin::read);
    }
}

void tcp_session::read_touch_packet_callback(boost::shared_ptr<network_packet> packet, const boost::system::error_code& error, size_t bytes_transferred)
{
    if (!error)
    {
        if (bytes_transferred < (touch_data::touch_data_size))
        {
            on_error(this->shared_from_this(), error, tcp_session::error_origin::bad_packet_small);
        }
        else
        {
            std::vector<touch_data> container = network_packet::deserialize_touch_data_container(*packet);
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