#include <runtypes/Type.hpp>
#include <runtypes/Data.hpp>

#include <string>
#include <vector>
#include <map>

#include <iostream>

int main()
{
    rt::Type c_int = rt::CType<int>();
    rt::Type c_uint = rt::CType<unsigned int>();
    rt::Type c_uint8 = rt::CType<uint8_t>();
    rt::Type c_uint32 = rt::CType<uint32_t>();
    rt::Type c_str = rt::CType<std::string>();
    rt::Type c_v_uint32 = rt::CType<std::vector<uint32_t>>();
    rt::Type c_m_str_uint32 = rt::CType<std::map<std::string, uint32_t>>();

    rt::Struct t_bb = rt::Struct("t_bb");
    t_bb.add_member("c_uint", c_uint);
    t_bb.add_member("c_uint8", c_uint8);
    t_bb.add_member("c_uint32", c_uint32);
    t_bb.add_member("c_str", c_str);
    t_bb.add_member("c_v_uint32", c_v_uint32);
    t_bb.add_member("c_m_str_uint32", c_m_str_uint32);

    std::cout << c_int.memory_size() << ": " << c_int.name() << std::endl;
    std::cout << c_uint.memory_size() << ": " << c_uint.name() << std::endl;
    std::cout << c_uint8.memory_size() << ": " << c_uint8.name() << std::endl;
    std::cout << c_uint32.memory_size() << ": " << c_uint32.name() << std::endl;
    std::cout << c_str.memory_size() << ": " << c_str.name() << std::endl;
    std::cout << c_v_uint32.memory_size() << ": " << c_v_uint32.name() << std::endl;
    std::cout << c_m_str_uint32.memory_size() << ": " << c_m_str_uint32.name() << std::endl;
    std::cout << t_bb.memory_size() << ": " << t_bb.name() << std::endl;

    rt::Data d1(c_v_uint32);
    rt::Data d2(t_bb);

    //t_bb_d["c_str"].as<std::string>() = "Example";
    //std::cout << t_bb_d["c_str"].as<std::string>() << std::endl;*/

    return 0;
}

