#pragma once

class touch_control
{
public:
    void test_msg();

    // Singleton touch_control instance
    static touch_control& instance()
    {
        static touch_control touch_ctrl;
        return touch_ctrl;
    }
};