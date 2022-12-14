#pragma once

#include <iostream>
#include "type.h"
#include "type_data.h"
#include "type_hash.h"
#include "type_factory.h"


namespace mirr
{
    // Forward declarations
    class type_container;

    /**
     *
     */
    class registry
    {
    public:
        using id_hash = basic_hash<uint32_t>;

        template<typename T>
        static type_factory<T> register_type()
        {
            return type_factory<T>();
        }

        template<typename T>
        static type_factory<T> register_type(const std::string &name)
        {
            return type_factory<T>(name);
        }

        static type resolve(const std::string &name) noexcept;

        static type resolve(uint32_t id) noexcept;

        static type_container resolve() noexcept;

        template<typename T>
        static type resolve() noexcept
        {
            uint32_t hash = internal::type_hash<std::decay_t<T>>::value();
            auto val = type_data::instance().types.find(hash);
            if (val == type_data::instance().types.end())
            {
                auto factory = type_factory<T>(typeid(T).name());
                return factory._type;
            }
            return type(val->first);
        }

        static bool valid(const std::string &name);

        static bool valid(uint32_t id);

    private:
        static uint32_t id_from_name(const std::string &name);
    };

} // mirr
