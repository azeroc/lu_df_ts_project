#include "config_mgr.h"
#include "common.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

void config_mgr::load(std::string config_file, std::string section, read_keys keys_to_fetch, bool clear)
{
    if (clear)
    {
        storage.clear();
    }

    try
    {
        boost::property_tree::ptree pt;
        boost::property_tree::ini_parser::read_ini(config_file, pt);

        for (std::string key : keys_to_fetch)
        {
            storage[key] = pt.get<std::string>(boost::property_tree::path(section + "/" + key, '/'));
        }
    }
    catch (const boost::property_tree::ptree_error &e)
    {
        std::cout << e.what() << std::endl;
    }
}

std::string config_mgr::get_string(std::string key)
{
    std::string map_val = storage[key];
    return map_val;
}

std::uint32_t config_mgr::get_uint32(std::string key)
{
    std::string map_val = storage[key];
    try
    {
        return std::strtoul(map_val.c_str(), nullptr, 10);
    }
    catch (std::exception e)
    {
        return 0;
    }
}