#include "common.h"
#include "packet_structure.h"

network_packet network_packet::serialize_touch_data_container(const std::vector<touch_data>& container)
{
    std::uint32_t size = container.size();
    network_packet packet(4 + size * touch_data::touch_data_size);
    ts_util::fill_byte_vector(ts_util::uint32_to_bytes(htonl(size)), *packet.data, 0);

    for (std::uint32_t i = 0; i < size; i++)
    {
        touch_data data = container[i];
        ts_util::fill_byte_vector(ts_util::uint32_to_bytes(htonl(data.slot)), *packet.data, i*20 + 4);
        ts_util::fill_byte_vector(ts_util::int32_to_bytes(htonl(data.tracking_id)), *packet.data, i*20 + 8);
        ts_util::fill_byte_vector(ts_util::uint32_to_bytes(htonl(data.x)), *packet.data, i*20 + 12);
        ts_util::fill_byte_vector(ts_util::uint32_to_bytes(htonl(data.y)), *packet.data, i*20 + 16);
        ts_util::fill_byte_vector(ts_util::uint32_to_bytes(htonl(data.upd_flags)), *packet.data, i*20 + 20);
    }

    return packet;
}

std::vector<touch_data> network_packet::deserialize_touch_data_container(const network_packet& packet)
{
    std::vector<touch_data> container;
    std::uint32_t size = ntohl(ts_util::bytes_to_uint32(*packet.data, 0));

    for (std::uint32_t i = 0; i < size; i++)
    {
        touch_data data;
        data.slot = ntohl(ts_util::bytes_to_uint32(*packet.data, i*20 + 4));
        data.tracking_id = ntohl(ts_util::bytes_to_int32(*packet.data, i*20 + 8));
        data.x = ntohl(ts_util::bytes_to_uint32(*packet.data, i*20 + 12));
        data.y = ntohl(ts_util::bytes_to_uint32(*packet.data, i*20 + 16));
        data.upd_flags = ntohl(ts_util::bytes_to_uint32(*packet.data, i*20 + 20));
        container.push_back(data);
    }

    return container;
}
