#include <iostream>
#include <unordered_map>

#include "../include/reflection.h"


struct test_type
{
    test_type(const std::string &name, int val) : value(val), name(name)
    {

    }

    int value;
    std::string name;

    operator int()
    { return value; }

    int get_value() const { return value; }
    void set_value(int val) { value = val;}

    void foo(const std::string& a, const std::string& b)
    {
        std::cout << "test_type::foo called\n";
        std::cout << a << "\n";
        std::cout << b << "\n";
    }
};

std::string c_str_to_string(const char* str)
{
    return std::string{str};
}


int main()
{
    reflection::registry::register_type<void>("void");
    reflection::registry::register_type<std::unique_ptr<void>>("unique_ptr");
    reflection::registry::register_type<std::string>("string")
            .conversion<const char*, &std::string::c_str>();
    reflection::registry::register_type<const char*>("cstring")
            .conversion<std::string, &c_str_to_string>();
    reflection::registry::register_type<int>("int");

    reflection::registry::register_type<test_type>("test_type")
            .conversion<int>()
            .ctor<const std::string &, int>()
            .data<&test_type::get_value, &test_type::set_value>("value")
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


    std::cout << "string type id: " << reflection::registry::resolve<std::string>().id() << "\n";
    auto f = type.function("foo");
    //f.invoke(instance, "ayyy", "beeee");

    reflection::any string = "instance";
    std::cout << "try_cast: " << *string.try_cast<std::string>() << "\n";






    //reflection::registry::register_type<std::unique_ptr<test_type>>("std::unique_ptr<test_type>");

    //auto test = std::make_unique<test_type>("test name", 76);

    //auto any = reflection::any(std::move(test));

    //std::cout << "type name: " << any.type().name() << "\n";
    //std::cout << "pointer?: " << any.type().is_pointer() << "\n";
    //std::cout << "pointer like?: " << any.type().is_pointer_like() << "\n";
    //std::cout << "underlying type: " << any.type().underlying_type().name() << "\n";

    //test_type* result = *static_cast<test_type**>(any.data());
    //std::cout << result->name << "\n";
    //std::cout << result->value << "\n";
    //
    //test.name = "changed name";
    //std::cout << result->name << "\n";

    //int x = 4;
    //reflection::handle h = reflection::handle(&x);
    //int** ptr = h.try_cast<int*>();
    //if (ptr)
    //{
    //    **ptr = 8;
    //    std::cout << x << "\n";
    //}

    return 0;
}