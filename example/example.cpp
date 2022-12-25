#include <iostream>
#include <unordered_map>

#include <mirr/mirr.h>


struct test_type
{
    REFLECT(test_type);
private:

public:

    test_type(const std::string &name, int val) : value(val), name(name), other(0)
    {

    }

    int value;
    std::string name;
    const int other;

    operator int()
    { return value; }

    [[nodiscard]] int get_value() const
    { return value; }

    void set_value(int val)
    { value = val; }

    static void foo(const std::string &a, const std::string &b)
    {
        std::cout << "test_type::foo called\n";
        std::cout << a << "\n";
        std::cout << b << "\n";
    }

    void some_func(){}
    void some_func(int arg){}


    enum class my_custom_flags : uint32_t
    {
        none = 0,
        serialize = 1 << 0,
        font = 2 << 0
    };

    static inline type_factory register_type()
    {
        std::cout << "called register_type\n";
        return mirr::register_type<test_type>("test_type")
                .ctor<const std::string &, int>()
                .conversion<int>()
                .data<&test_type::name>("name")
                .data<&test_type::get_value, &test_type::set_value>("value")
                .data<&test_type::other>("other")
                .function<static_cast<void (test_type::*)(int)>(&test_type::some_func)>("some_func1")
                .user_data("something", 4);
    }
};

void foo()
{
    auto val = test_type("aahh", 0);
}

test_type create_test_type(char f)
{
    return test_type("aaah", 0);
}


int main()
{
    mirr::register_type<void>("void");
    mirr::register_type<uint8_t>("uint8_t");
    mirr::register_type<uint16_t>("uint16_t");
    mirr::register_type<uint32_t>("uint32_t");
    mirr::register_type<uint64_t>("uint64_t");
    mirr::register_type<int8_t>("int8_t");
    mirr::register_type<int16_t>("int16_t");
    mirr::register_type<int32_t>("int32_t");
    mirr::register_type<int64_t>("int64_t");
    mirr::register_type<float>("float");
    mirr::register_type<double>("double");
    mirr::register_type<char>("char");
    mirr::register_type<std::unique_ptr<void>>("unique_ptr");
    mirr::register_type<std::string>("std::string")
            .conversion<const char *, &std::string::c_str>();
    mirr::register_type<const char *>("cstring")
            .conversion<std::string>();


    auto type = mirr::resolve("test_type");
    std::cout << "id: " << type.id() << "\n";
    std::cout << "name: " << type.name() << "\n";
    std::cout << "is_integral: " << std::boolalpha << type.is_integral() << "\n";
    std::cout << "is_class: " << std::boolalpha << type.is_class() << "\n";
    std::cout << "is_associative_container: " << std::boolalpha << type.is_associative_container() << "\n";

   //for (auto &[id, ctor]: type.constructors())
   //{
   //    std::cout << "Constructor: " << mirr::type_name_from_id(ctor.type_id) << "\nargs: ";
   //    for (int i = 0; i < ctor.arity; i++)
   //    {
   //        std::cout << mirr::type_name_from_id(ctor.arg(i)) << ((i == ctor.arity - 1) ? "" : ", ");
   //    }
   //    std::cout << "\n";

   //}

    auto something = type.user_data("something").cast<int>();

    std::cout << "user_data(something): " << something << "\n";

    test_type instance("bob", 42);

    auto d = type.data("value");
    std::cout << d.type().id() << "\n";
    std::cout << instance.value << "\n";
    bool set_result = d.set(instance, 32);
    std::cout << set_result << "\n";
    auto val = d.get(instance);
    std::cout << val.cast<int>() << "\n";


    auto f = type.func("foo");
    f.invoke(instance, "a", "b");

    std::cout << "\n";

    for (auto data: type.data())
    {
        std::cout << data.name() << "\n";
    }

    std::cout << "\n";

    for (auto func: type.func())
    {
        std::cout << func.name() << "\n";
        for (auto arg: func.args())
        {
            std::cout << "\t" << arg.name() << "\n";
        }
    }

    std::cout << "\n";

    for (auto t: mirr::resolve())
    {
        std::cout << t.name() << "\n";
    }


    return 0;
}