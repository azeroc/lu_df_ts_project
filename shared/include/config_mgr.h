#pragma once
#include "common.h"

class config_mgr
{
public:
    // First pair value is key to search for, second is default value
    typedef std::vector<std::string> read_keys;
    typedef std::map <std::string, std::string> storage_map;

    static config_mgr& instance()
    {
        static config_mgr cfg_mgr;
        return cfg_mgr;
    }
    
    void load(std::string config_file, std::string section, read_keys keys_to_fetch, bool clear = false);
    std::string get_string(std::string key);
    std::uint32_t get_uint32(std::string key);
private:
    storage_map storage;

    config_mgr()
    {
    }
};