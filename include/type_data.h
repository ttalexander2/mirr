#pragma once

#include <unordered_map>
#include <cstdint>
#include <string>
#include <vector>

#include "basic_hash.h"
#include "type_traits.h"
#include "type_flags.h"

namespace reflection
{
    // Forward declarations
    struct type_info;
    struct data_info;
    struct func_info;
    struct ctor_info;

    class any;
    class handle;

    struct data_info
    {
        std::string name;
        uint32_t id;
        uint32_t type_id;

        bool (*set)(handle, any);
        any (*get)(handle);

        data_flags flags;
    };

    struct func_info
    {
        std::string name;
        uint32_t id;

        size_t arity;
        std::vector<uint32_t> args;
        uint32_t return_type;

        bool is_const;
        bool is_static;
    };

    struct ctor_info
    {
        uint32_t id;
        uint32_t type_id;

        size_t arity;
        std::vector<uint32_t> args;
    };

    // Holds type_data information for a type.
    struct type_info
    {
        std::string name;
        uint32_t id;

        type_flags flags;

        uint32_t underlying_type_id;

        std::unordered_map<uint32_t, data_info> data;
        std::unordered_map<uint32_t, func_info> functions;
        std::vector<ctor_info> constructors;
        std::vector<uint32_t> conversions;
        std::vector<uint32_t> bases;
    };

    class type_data
    {
        friend class type;
        friend class data;
        friend class function;
        friend class registry;

        template<typename T>
        friend
        class type_factory;

        using id_hash = basic_hash<uint32_t>;

        static type_data &instance()
        {
            static type_data instance;
            return instance;
        }

        template<typename T>
        static type_flags get_flags()
        {
            type_flags flags = type_flags::none;

            if (std::is_class_v<T>)
            {
                flags |= type_flags::is_class;
            }
            if (std::is_abstract_v<T>)
            {
                flags |= type_flags::is_abstract;
            }
            if (std::is_integral_v<T>)
            {
                flags |= type_flags::is_integral;
            }
            if (std::is_array_v<T>)
            {
                flags |= type_flags::is_array;
            }
            if (std::is_enum_v<T>)
            {
                flags |= type_flags::is_enum;
            }
            if (std::is_pointer_v<T>)
            {
                flags |= type_flags::is_pointer;
            }
            if (internal::is_pointer_like_v < T >)
            {
                flags |= type_flags::is_pointer_like;
            }
            if (internal::is_sequence_container_v < T >)
            {
                flags |= type_flags::is_sequence_container;
            }
            if (internal::is_associative_container_v < T >)
            {
                flags |= type_flags::is_associative_container;
            }
            if (false)
            {
                flags |= type_flags::is_template_specialization;
            }

            return flags;
        }

        // Map of application type info by type hash
        std::unordered_map<uint32_t, type_info> types;

        // Key is name hash, value is type hash
        std::unordered_map<uint32_t, uint32_t> type_aliases;

        type_data() = default;

    };

}
