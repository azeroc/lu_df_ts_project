#include "common.h"
#include "packet_structure.h"

network_packet network_packet::serialize_touch_data(const touch_data & data)
{
    network_packet packet(touch_data::touch_data_size);
    ts_util::fill_byte_vector(ts_util::uint32_to_bytes(htonl(data.slot)), *packet.data, 0);
    ts_util::fill_byte_vector(ts_util::uint32_to_bytes(htonl(data.tracking_id)), *packet.data, 4);
    ts_util::fill_byte_vector(ts_util::uint32_to_bytes(htonl(data.x)), *packet.data, 8);
    ts_util::fill_byte_vector(ts_util::uint32_to_bytes(htonl(data.y)), *packet.data, 12);

    return packet;
}

touch_data network_packet::deserialize_touch_data(const network_packet & packet)
{
    touch_data data;

    if (packet.size() == touch_data::touch_data_size)
    {
        data.slot = ntohl(ts_util::bytes_to_uint32(*packet.data, 0));
        data.tracking_id = ntohl(ts_util::bytes_to_uint32(*packet.data, 4));
        data.x = ntohl(ts_util::bytes_to_uint32(*packet.data, 8));
        data.y = ntohl(ts_util::bytes_to_uint32(*packet.data, 12));
    }

    return data;
}
