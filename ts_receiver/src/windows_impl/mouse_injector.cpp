#include "windows_impl/mouse_injector.h"
#include "common.h"
#include "touch_structure.h"
#include <windows.h>

mouse_injector::mouse_injector()
{
}

void mouse_injector::release(std::uint32_t slot)
{
    if (active_inputs.find(slot) != active_inputs.end())
    {
        INPUT input = active_inputs[slot];
        active_inputs.erase(slot);
        input.mi.dwFlags = MOUSEEVENTF_LEFTUP;

        if (!::SendInput(1, &input, sizeof(INPUT))) // Injecting new touch event
        {
            get_std_io_mutex().lock();
            std::cerr << "Mouse injection [release-leftup] failed: SendInput returned false with error " << GetLastError() << std::endl;
            get_std_io_mutex().unlock();
        }
    }
}

void mouse_injector::move(const touch_data& data)
{
    if (active_inputs.find(data.slot) != active_inputs.end())
    {
        INPUT& input = active_inputs[data.slot];
        double fScreenWidth = ::GetSystemMetrics(SM_CXSCREEN) - 1;
        double fScreenHeight = ::GetSystemMetrics(SM_CYSCREEN) - 1;
        double fx = data.x * (65535.0f / fScreenWidth);
        double fy = data.y * (65535.0f / fScreenHeight);
        input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;

        if (data.upd_flags & touch_data::update_flags::update_x)
            input.mi.dx = static_cast<LONG>(fx);
        if (data.upd_flags & touch_data::update_flags::update_y)
            input.mi.dy = static_cast<LONG>(fy);
        
        if (!::SendInput(1, &input, sizeof(INPUT))) // Injecting new touch event
        {
            get_std_io_mutex().lock();
            std::cerr << "Mouse injection [move] failed: SendInput returned false with error " << GetLastError() << std::endl;
            get_std_io_mutex().unlock();
        }
    }
}

void mouse_injector::pushdown(const touch_data& data)
{
    if (data.upd_flags == touch_data::update_flags::update_all)
    {
        INPUT& input = active_inputs[data.slot];
        memset(&input, 0, sizeof(INPUT));
        double fScreenWidth = ::GetSystemMetrics(SM_CXSCREEN) - 1;
        double fScreenHeight = ::GetSystemMetrics(SM_CYSCREEN) - 1;
        double fx = data.x * (65535.0f / fScreenWidth);
        double fy = data.y * (65535.0f / fScreenHeight);
        input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;
        input.mi.dx = static_cast<LONG>(fx);;
        input.mi.dy = static_cast<LONG>(fy);;

        if (!::SendInput(1, &input, sizeof(INPUT))) // Injecting new touch event
        {
            get_std_io_mutex().lock();
            std::cerr << "Mouse injection [pushdown] failed: SendInput returned false with error " << GetLastError() << std::endl;
            get_std_io_mutex().unlock();
        }
    }
}

void mouse_injector::update_mouse_data(const std::vector<touch_data>& container)
{
    for (touch_data data : container)
    {
        if (data.tracking_id == -1)
            release(data.slot);
        else if (data.upd_flags & touch_data::update_flags::update_tracking_id)
            pushdown(data);
        else
            move(data);
    }
}