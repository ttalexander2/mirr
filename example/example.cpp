#include <iostream>
#include <unordered_map>

#include "../include/reflection.h"


struct test_type
{
    test_type(const std::string &name, int val) : value(val), name(name), other(0)
    {

    }

    int value;
    std::string name;
    const int other;

    operator int()
    { return value; }

    [[nodiscard]] int get_value() const { return value; }
    void set_value(int val) { value = val;}

    static void foo(const std::string& a, const std::string& b)
    {
        std::cout << "test_type::foo called\n";
        std::cout << a << "\n";
        std::cout << b << "\n";
    }
};


int main()
{
    reflection::registry::register_type<void>("void");
    reflection::registry::register_type<uint8_t>("uint8");
    reflection::registry::register_type<uint16_t>("uint16");
    reflection::registry::register_type<uint32_t>("uint32");
    reflection::registry::register_type<uint64_t>("uint64");
    reflection::registry::register_type<int8_t>("int8");
    reflection::registry::register_type<int16_t>("int16");
    reflection::registry::register_type<int32_t>("int32");
    reflection::registry::register_type<int64_t>("int64");
    reflection::registry::register_type<float>("float");
    reflection::registry::register_type<double>("double");
    reflection::registry::register_type<char>("char");
    reflection::registry::register_type<std::unique_ptr<void>>("unique_ptr");
    reflection::registry::register_type<std::string>("std::string")
            .conversion<const char*, &std::string::c_str>();
    reflection::registry::register_type<const char*>("cstring")
            .conversion<std::string>();

    reflection::registry::register_type<test_type>("test_type")
            .conversion<int>()
            .ctor<const std::string &, int>()
            .data<&test_type::get_value, &test_type::set_value>("value")
            .data<&test_type::name>("name")
            .data<&test_type::other>("other")
            .function<&test_type::foo>("foo");


    auto type = reflection::registry::resolve("test_type");
    std::cout << "id: " << type.id() << "\n";
    std::cout << "name: " << type.name() << "\n";
    std::cout << "is_integral: " << std::boolalpha << type.is_integral() << "\n";
    std::cout << "is_class: " << std::boolalpha << type.is_class() << "\n";
    std::cout << "is_associative_container: " << std::boolalpha << type.is_associative_container() << "\n";
    auto info = type.info();
    for (auto &constructor: info->constructors)
    {
        std::cout << constructor.arity << "\n";

        for (auto &arg: constructor.args)
        {
            auto arg_type = reflection::registry::resolve(arg);
            std::cout << arg_type.name() << "\n";
        }

    }

    for (auto &[id, data]: info->data)
    {
        std::cout << data.name << "\n";
    }

    test_type instance("bill", 42);

    auto d = type.data("value");
    std::cout << d.type().id() << "\n";
    std::cout << instance.value << "\n";
    bool set_result = d.set(instance, 32);
    std::cout << set_result << "\n";
    auto val = d.get(instance);
    std::cout << val.cast<int>() << "\n";


    auto f = type.function("foo");
    f.invoke(instance, "a", "b");

    std::cout << "\n";

    for (auto data : type.data())
    {
        std::cout << data.name() << "\n";
    }

    std::cout << "\n";

    for (auto func : type.function())
    {
        std::cout << func.name() << "\n";
        for (auto arg : func.args())
        {
            std::cout << "\t" << arg.name() << "\n";
        }
    }



    //for (auto t : reflection::registry::resolve())
    //{
    //    std::cout << t.name() << "\n";
    //}


    return 0;
}