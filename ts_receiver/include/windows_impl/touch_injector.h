#include "common.h"
#include "touch_structure.h"
#include <windows.h>

class touch_injector
{
public:
    enum
    {
        default_pressure = 32000,
        default_orientation = 90, // Orientation of 90 means touching perpendicular to screen
        touch_region_add_w = 2,
        touch_region_add_h = 2
    };

    touch_injector();

    void update_touch_data(const std::vector<touch_data>& container);
    std::uint32_t get_max_touch_contacts() { return max_contact_size; }

    std::mutex& get_std_io_mutex()
    {
        static std::mutex std_io_mtx;
        return std_io_mtx;
    }
private:
    void remove_contact(std::uint32_t pointer_id);
    void update_contact(const touch_data& data);

    std::uint32_t max_contact_size;
    std::uint32_t free_contact_size;
    std::map<std::uint32_t, POINTER_TOUCH_INFO> active_contacts;
};