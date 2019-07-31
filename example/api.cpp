int main()
{
    // CREATION
    rt::Struct inner("inner_type");
    inner.add_member("uint", rt::CType<int>(5));
    inner.add_member("str", rt::CType<std::string>("hello"));
    inner.add_member("vec", rt::CType<std::vector<int>({1, 4, 3, 5}));
    inner.add_member("map", rt::CType<std::map<std::string, uint32_t>>({{"a": 3}, {"b": 8}, {"c": 8}));

    rt::Struct my_type;
    my_type.add_member("inner", inner);
    my_type.add_member("float", rt::CType<float>(3.0f));

    rt::Data my_data(my_type);

    // READ / WRITE
    int my_elem = my_data["inner"]["vec"].as<std::vector<int>>[1];
    std::cout << my_elem << std::endl;

    my_data["inner"]["uint"].as<int>() = 8;
    std::cout << my_data["inner"]["uint"].as<int>() << std::endl;

    for(auto&& member: my_data)
    {
        std::cout << member.name() << std::endl;
        std::cout << member.type() << std::endl;
        //std::cout << member.value<TYPE_HERE>() << std::endl;
    }
}
