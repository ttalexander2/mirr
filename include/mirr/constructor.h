#pragma once

#include <cstdint>
#include "type.h"
#include "any.h"

namespace mirr
{

    class type_data;

    class registry;

    struct ctor_info;
    struct type_info;

    class constructor_container;

    class argument_container;

    template<typename T>
    class type_factory;

    class constructor
    {
        friend class type_data;

        friend class registry;

        friend class any;

        friend class handle;

        friend class type;

        friend class argument_container;

        friend class constructor_container;

        template<typename T>
        friend
        class type_factory;

    public:
        constructor();

        constructor(const constructor &) = default;

        bool operator==(const constructor &rhs) const;

        [[nodiscard]] bool valid() const;

        explicit operator bool() const
        { return valid(); }

        [[nodiscard]] uint32_t id() const;

        [[nodiscard]] mirr::type type() const;

        [[nodiscard]] size_t arity() const;

        [[nodiscard]] mirr::type args(size_t index) const;

        template<typename... Args>
        any invoke(Args &&...args)
        {
            any arguments[sizeof...(Args) + 1u]{std::forward<Args>(args)...};
            return invoke_internal(arguments, sizeof...(Args));
        }

    private:
        constructor(uint32_t id, uint32_t type_id);

        [[maybe_unused]] any invoke_internal([[maybe_unused]]any *args, size_t count) const;

        uint32_t _id;
        uint32_t _type_id;

    };
}

