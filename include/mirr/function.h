#pragma once

#include <cstdint>
#include "type.h"
#include "any.h"

namespace mirr
{

    class type_data;

    class registry;

    struct func_info;
    struct type_info;

    class argument_container;

    template<typename T>
    class type_factory;

    class function
    {
        friend class type_data;

        friend class registry;

        friend class any;

        friend class handle;

        friend class type;

        friend class function_container;

        template<typename T>
        friend
        class type_factory;

    public:
        function();

        function(const function &) = default;

        bool operator==(const function &rhs) const;

        [[nodiscard]] bool valid() const;

        explicit operator bool() const
        { return valid(); }

        [[nodiscard]] uint32_t id() const;

        [[nodiscard]] std::string name() const;

        [[nodiscard]] mirr::type type() const;

        [[nodiscard]] size_t arity() const;

        [[nodiscard]] mirr::type return_type() const;

        [[nodiscard]] mirr::type args(size_t index) const;

        [[nodiscard]] argument_container args() const;

        template<typename... Args>
        [[maybe_unused]] any invoke([[maybe_unused]] mirr::handle handle, Args &&...args)
        {
            any arguments[sizeof...(Args) + 1u]{std::forward<Args>(args)...};
            return invoke_internal(std::move(handle), arguments, sizeof...(Args));
        }

    private:
        explicit function(uint32_t id, uint32_t type_id);

        [[maybe_unused]] any invoke_internal([[maybe_unused]] mirr::handle, any *args, size_t count) const;

        uint32_t _id;
        uint32_t _type_id;
    };
}
