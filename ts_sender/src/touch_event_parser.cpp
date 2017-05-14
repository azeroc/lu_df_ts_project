#include "touch_event_parser.h"
#include "common.h"
#include "tcp_ts_client.h"
#include "touch_structure.h"

void touch_event_parser::on_touch_data_receive(std::vector<touch_data> container)
{
    tcp_client->send_touch_data_container(container);
}