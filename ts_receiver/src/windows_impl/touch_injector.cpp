#include "windows_impl/touch_injector.h"
#include "common.h"
#include "touch_structure.h"
#include <windows.h>

touch_injector::touch_injector()
{
    max_contact_size = MAX_TOUCH_COUNT;
    free_contact_size = max_contact_size;

    if (!InitializeTouchInjection(1, TOUCH_FEEDBACK_INDIRECT))
    {
        get_std_io_mutex().lock();
        std::cerr << "Touch injection initialization failed: InitializeTouchInjection returned false with error " << GetLastError() << std::endl;
        get_std_io_mutex().unlock();
        exit(3);        
    }
}

void touch_injector::remove_contact(std::uint32_t pointer_id)
{
    if (active_contacts.find(pointer_id) != active_contacts.end())
    {
        POINTER_TOUCH_INFO contact = active_contacts[pointer_id];
        active_contacts.erase(pointer_id);
        contact.pointerInfo.pointerFlags = POINTER_FLAG_UP;

        if (!InjectTouchInput(1, &contact)) // Injecting new touch event
        {
            get_std_io_mutex().lock();
            std::cerr << "Touch injection [remove] failed: InjectTouchInput returned false with error " << GetLastError() << std::endl;
            get_std_io_mutex().unlock();
        } 
    }
}

void touch_injector::update_contact(const touch_data& data)
{
    if (active_contacts.find(data.slot) == active_contacts.end())
    { // New contact
        // We don't want to execute touch with incomplete touch_data
        if (data.upd_flags == touch_data::update_flags::update_all)
        {
            // Initialize new contact info and add it in active_contacts
            active_contacts[data.slot] = POINTER_TOUCH_INFO();
            POINTER_TOUCH_INFO& contact = active_contacts[data.slot];
            memset(&contact, 0, sizeof(POINTER_TOUCH_INFO));

            // Set main touch information
            contact.pointerInfo.pointerType = PT_TOUCH;
            contact.pointerInfo.pointerId = data.slot;          //contact 0            
            contact.pointerInfo.ptPixelLocation.x = data.x; // X co-ordinate of touch on screen
            contact.pointerInfo.ptPixelLocation.y = data.y; // Y co-ordinate of touch on screen

            // Set flags
            contact.touchFlags = POINTER_FLAG_DOWN | POINTER_FLAG_INRANGE | POINTER_FLAG_INCONTACT;
            contact.touchMask = TOUCH_MASK_CONTACTAREA | TOUCH_MASK_ORIENTATION | TOUCH_MASK_PRESSURE;

            // Set other touch characteristics
            contact.orientation = touch_injector::default_orientation; 
            contact.pressure = touch_injector::default_pressure;

            // Defining contact area
            contact.rcContact.left = contact.pointerInfo.ptPixelLocation.x - touch_injector::touch_region_add_w;
            contact.rcContact.right = contact.pointerInfo.ptPixelLocation.x + touch_injector::touch_region_add_w;
            contact.rcContact.top = contact.pointerInfo.ptPixelLocation.y - touch_injector::touch_region_add_h;
            contact.rcContact.bottom = contact.pointerInfo.ptPixelLocation.y + touch_injector::touch_region_add_h;

            if (!InjectTouchInput(1, &contact)) // Injecting new touch event
            {
                get_std_io_mutex().lock();
                std::cerr << "Touch injection [new] failed: InjectTouchInput returned false with error " << GetLastError() << std::endl;
                get_std_io_mutex().unlock();
            }
        }
    }
    else
    { // Update existing
        POINTER_TOUCH_INFO& contact = active_contacts[data.slot];

        if (data.upd_flags != touch_data::update_flags::update_none
            && data.upd_flags != touch_data::update_flags::update_tracking_id)
        {
            if (data.upd_flags | touch_data::update_flags::update_x)
            {
                contact.pointerInfo.ptPixelLocation.x = data.x; // X co-ordinate of touch on screen
            }

            if (data.upd_flags | touch_data::update_flags::update_y)
            {
                contact.pointerInfo.ptPixelLocation.y = data.y; // Y co-ordinate of touch on screen
            }

            contact.pointerInfo.pointerFlags = POINTER_FLAG_UPDATE | POINTER_FLAG_INRANGE | POINTER_FLAG_INCONTACT;

            if (!InjectTouchInput(1, &contact)) // Injecting update request to active touch contact
            {
                get_std_io_mutex().lock();
                std::cerr << "Touch injection [update] failed: InjectTouchInput returned false with error " << GetLastError() << std::endl;
                get_std_io_mutex().unlock();
            } 
        }

    }
}

void touch_injector::update_touch_data(const std::vector<touch_data>& container)
{
    for (touch_data data : container)
    {
        if (data.tracking_id == -1)
            remove_contact(data.slot);
        else
            update_contact(data);
    }
}