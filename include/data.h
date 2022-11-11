#pragma once

#include <cstdint>
#include "type.h"
#include "any.h"

namespace reflection
{

    // Forward declarations
    class type_data;
    class registry;
    class any;
    struct data_info;
    struct type_info;

    template<typename T>
    class type_factory;


    class data
    {
        friend class type_data;
        friend class registry;
        friend class any;
        friend class type;

        template<typename T>
        friend class type_factory;

    public:
        data();
        data(const data&) = default;

        inline bool operator==(const data&rhs) const { return _id == rhs._id && _type_id == rhs._type_id; }

        [[nodiscard]] bool valid() const;

        explicit operator bool() const { return valid(); }

        [[nodiscard]] uint32_t id() const;
        [[nodiscard]] std::string name() const;
        [[nodiscard]] reflection::type type() const;

        [[nodiscard]] bool is_serialized() const;
        [[nodiscard]] bool is_const() const;
        [[nodiscard]] bool is_static() const;

        [[nodiscard]] any get(reflection::handle handle) const;
        void set(reflection::handle handle, const any& value) const;

    private:
        explicit data(uint32_t id, uint32_t type_id);
        uint32_t _id;
        uint32_t _type_id;
    };

}