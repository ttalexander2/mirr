#pragma once

#include <cstdint>
#include <string>
#include "type_flags.h"
#include "type_hash.h"

namespace reflection
{

    // Forward declarations
    class type_data;
    class func_data;
    class registry;
    class any;
    class handle;
    struct type_info;
    class data;
    class function;
    class data_container;
    class function_container;

    template<typename T>
    class type_factory;

    class type
    {
        friend class type_data;
        friend class func_data;
        friend class registry;
        friend class any;
        friend class handle;
        friend class data;
        friend class function;
        friend class type_container;
        friend class argument_container;

        template<typename T>
        friend
        class type_factory;

    public:
        type();

        type(const type &) = default;

        inline bool operator==(const type &rhs) const
        { return id() == rhs.id(); }

        [[nodiscard]] bool valid() const;

        explicit operator bool() const
        { return valid(); }

        [[nodiscard]] uint32_t id() const;

        [[nodiscard]] std::string name() const;

        [[nodiscard]] uint32_t hash() const;


        [[nodiscard]] bool is_integral() const;

        [[nodiscard]] bool is_array() const;

        [[nodiscard]] bool is_enum() const;

        [[nodiscard]] bool is_class() const;

        [[nodiscard]] bool is_pointer() const;

        [[nodiscard]] bool is_pointer_like() const;

        [[nodiscard]] bool is_sequence_container() const;

        [[nodiscard]] bool is_associative_container() const;

        [[nodiscard]] bool is_template_specialization() const;

        [[nodiscard]] bool is_abstract() const;

        [[nodiscard]] type underlying_type() const;

        [[nodiscard]] reflection::data data(const std::string& name) const;
        [[nodiscard]] reflection::data data(uint32_t id) const;
        [[nodiscard]] reflection::data_container data() const;

        [[nodiscard]] reflection::function function(const std::string& name) const;
        [[nodiscard]] reflection::function function(uint32_t id) const;
        [[nodiscard]] reflection::function_container function() const;


        [[nodiscard]] bool is_convertible(uint32_t type_id) const;

        template <typename To>
        [[nodiscard]] bool is_convertible() const
        {
            return is_convertible(type_hash_v<To>);
        }

        // Provides the type flags, containing the data for the boolean properties of this type.
        [[nodiscard]] type_flags flags() const;

        [[nodiscard]] type_info *info() const;


    private:
        explicit type(type_id id);
        type_id _id;

    };
}
