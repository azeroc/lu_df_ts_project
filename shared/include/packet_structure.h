#pragma once
#include "Common.h"
#include "touch_structure.h"

// This packet class implements and uses buffers directly,
// which will keep data reference alive until boost::asio::async_* operation callbacks are executed
class network_packet
{
public:
    typedef std::uint8_t data_unit;
    typedef std::vector<network_packet::data_unit> data_array;

    network_packet(const data_array& raw_data)
        : data(boost::make_shared<data_array>(raw_data)),
          mutable_buffer(boost::asio::buffer(*this->data))
    {
    }

    network_packet(size_t init_size)
        : data(boost::make_shared<data_array>(data_array(init_size))),
          mutable_buffer(boost::asio::buffer(*this->data))
    {
    }

    size_t size() const
    {
        return data->size();
    }

    // Direct, unrestricted access to mutable buffer, used for async_read
    boost::asio::mutable_buffer& buffer() { return mutable_buffer; }

    // Implement mutable buffer, needed for async_send
    typedef boost::asio::mutable_buffer value_type;
    typedef const boost::asio::mutable_buffer* const_iterator;
    const boost::asio::mutable_buffer* begin() const { return &mutable_buffer; }
    const boost::asio::mutable_buffer* end() const { return &mutable_buffer + 1; }

    
public: // Static helper methods for serialization and deserialization
    static network_packet serialize_touch_data(const touch_data& data);
    static touch_data deserialize_touch_data(const network_packet& packet);

private:
    boost::shared_ptr<std::vector<network_packet::data_unit>> data;
    boost::asio::mutable_buffer mutable_buffer;
};