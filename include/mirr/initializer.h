#pragma once

#include "util.h"
#include "type_factory.h"

namespace mirr
{
    /**
     * @brief Class to be declared statically as a member
     * @tparam Type
     */
    template<typename Type>
    class type_initializer
    {
    public:
        type_initializer()
        {
            Type::register_type();
        }

        explicit type_initializer(const std::string &name)
        {
            type_factory<Type> factory(name);
            Type::register_type();
        }


        explicit type_initializer(std::function<type_factory<Type>(void)> initialization_function)
        {
            initialization_function();
        }
    };
}