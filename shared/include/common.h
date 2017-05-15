#pragma once

// STD LIBS 
#include <chrono>
#include <thread>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <cstdint> // portable fixed-size data types
#include <mutex>
#include <cmath>

// BOOST LIBS
#ifdef _WIN32 // Boost wants to know what is _WIN32_WINNT if compiling on Windows
    #include <sdkddkver.h>    
#endif

#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp> // TCP/IP communication

// UTILITY FUNCTIONS
namespace ts_util
{
    inline std::uint32_t bytes_to_uint32(const std::vector<std::uint8_t>& bytes, std::uint32_t start_pos = 0)
    {        
        std::uint32_t a = 0;

        if (start_pos < bytes.size())
        {
            int current_pos = start_pos;

            for (std::uint32_t i = 0; i < 4 && (current_pos + i) < bytes.size(); i++)
            {
                a = a | (bytes[current_pos] << (8 * (3 - i)));
                current_pos++;
            }
        }

        return a;
    }

    inline std::int32_t bytes_to_int32(const std::vector<std::uint8_t>& bytes, std::uint32_t start_pos = 0)
    {        
        std::int32_t a = 0;

        if (start_pos < bytes.size())
        {
            int current_pos = start_pos;

            for (std::uint32_t i = 0; i < 4 && (current_pos + i) < bytes.size(); i++)
            {
                a = a | (bytes[current_pos] << (8 * (3 - i)));
                current_pos++;
            }
        }

        return a;
    }

    inline std::vector<std::uint8_t> uint32_to_bytes(std::uint32_t a)
    {
        std::vector<std::uint8_t> bytes(4);
        bytes[0] = (a >> 24) & 0xff;
        bytes[1] = (a >> 16) & 0xff;
        bytes[2] = (a >> 8) & 0xff;
        bytes[3] = a & 0xff;
        return bytes;
    }

    inline std::vector<std::uint8_t> int32_to_bytes(std::int32_t a)
    {
        std::vector<std::uint8_t> bytes(4);
        bytes[0] = (a >> 24) & 0xff;
        bytes[1] = (a >> 16) & 0xff;
        bytes[2] = (a >> 8) & 0xff;
        bytes[3] = a & 0xff;
        return bytes;
    }

    inline void fill_byte_vector(const std::vector<std::uint8_t>& src, std::vector<std::uint8_t>& dst, std::uint32_t start_pos = 0)
    {
        std::uint32_t current_pos = start_pos;
        for (auto src_it = src.begin(); src_it != src.end() && current_pos < dst.size(); src_it++)
        {
            dst[current_pos] = *src_it;
            current_pos++;
        }
    }

    inline bool file_exists(std::string filename)
    {
        std::ifstream f(filename.c_str());
        return f.good();
    }
}