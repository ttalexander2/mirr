#pragma once

#include "util.h"
#include "type_factory.h"

namespace mirr
{
    /**
     * @brief Helper class used to automatically register a type at the beginning of runtime.
     * @tparam Type - Containing type to register.
     *
     * By default, the object assumes the type contains a function called "register_type" which will be called
     * during static initialization. A pointer to a custom function can also be provided in the constructor.
     *
     * Example:
     * @code
     *  struct my_type
     *  {
     *      static inline type_initializer<my_type> initializer{};
     *      static type_factory<my_type> register_type();
     *  }
     * @endcode
     */
    template<typename Type>
    class type_initializer
    {
    public:
        /**
         * Constructs a type initializer, which on construction, registers the type anonymously.
         */
        type_initializer()
        {
            Type::register_type();
        }

        /**
         * Constructs a type initializer, which registers the type with the given name on construction.
         * @param name
         */
        explicit type_initializer(const std::string &name)
        {
            type_factory<Type> factory(name);
            Type::register_type();
        }


        /**
         * Constructs a type initializer, which calls the provided function on construction.
         * @param initialization_function User provided function to be called in place of "register_type".
         */
        explicit type_initializer(std::function<type_factory<Type>(void)> initialization_function)
        {
            initialization_function();
        }
    };
}