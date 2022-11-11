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
};

int main()
{
    reflection::registry::register_type<void>("void");
    reflection::registry::register_type<std::unique_ptr<void>>("unique_ptr");
    reflection::registry::register_type<std::string>("string");
    reflection::registry::register_type<int>("int");

    reflection::registry::register_type<test_type>("test_type")
            .conversion<int>()
            .ctor<const std::string &, int>()
            .data<&test_type::value>("value");


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
    d.set(instance, 32);
    std::cout << instance.value << "\n";

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