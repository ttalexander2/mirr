#pragma once

#include <cstdint>
#include "type.h"
#include "any.h"

namespace reflection
{

    class type_data;
    class registry;
    class any;
    class handle;
    struct func_info;
    struct type_info;

    template <typename T>
    class type_factory;

    class function
    {
        friend class type_data;
        friend class registry;
        friend class any;
        friend class handle;
        friend class type;

        template<typename T>
        friend class type_factory;

    public:
        function();
        function(const function&) = default;

        bool operator==(const function& rhs) const;

        [[nodiscard]] bool valid() const;
        explicit operator bool() const { return valid(); }

        [[nodiscard]] uint32_t id() const;
        [[nodiscard]] std::string name() const;
        [[nodiscard]] reflection::type type() const;

        [[nodiscard]] size_t arity() const;
        [[nodiscard]] reflection::type return_type() const;
        [[nodiscard]] reflection::type arg_type(size_t index) const;

        [[maybe_unused]] any invoke_internal([[maybe_unused]] reflection::handle, any* args, size_t count) const;

        template <typename... Args>
        [[maybe_unused]] any invoke([[maybe_unused]] reflection::handle handle, Args&& ...args)
        {
            any arguments[sizeof...(Args) + 1u]{std::forward<Args>(args)...};
            return invoke_internal(std::move(handle), arguments, sizeof...(Args));
        }

    private:
        explicit function(uint32_t id, uint32_t type_id);
        uint32_t _id;
        uint32_t _type_id;
    };
}
