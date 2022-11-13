#include "../include/type.h"

#include "../include/registry.h"
#include "../include/data.h"
#include "../include/function.h"
#include "../include/data_container.h"
#include "../include/function_container.h"

namespace reflection
{

    bool type::valid() const
    {
        return registry::valid(_id);
    }

    type_id type::id() const
    {
        return _id;
    }

    std::string type::name() const
    {
        if (valid())
            return type_data::instance().types[_id].name;
        return {};
    }

    uint32_t type::hash() const
    {
        return basic_hash<uint32_t>::hash(name());
    }

    bool type::is_integral() const
    {
        if (valid())
            return internal::has_flag(type_data::instance().types[_id].flags, type_flags::is_integral);
        return false;
    }

    bool type::is_array() const
    {
        if (valid())
            return internal::has_flag(type_data::instance().types[_id].flags, type_flags::is_array);
        return false;
    }

    bool type::is_enum() const
    {
        if (valid())
            return internal::has_flag(type_data::instance().types[_id].flags, type_flags::is_enum);
        return false;
    }

    bool type::is_class() const
    {
        if (valid())
            return internal::has_flag(type_data::instance().types[_id].flags, type_flags::is_class);
        return false;
    }

    bool type::is_pointer() const
    {
        if (valid())
            return internal::has_flag(type_data::instance().types[_id].flags, type_flags::is_pointer);
        return false;
    }

    bool type::is_pointer_like() const
    {
        if (valid())
            return internal::has_flag(type_data::instance().types[_id].flags, type_flags::is_pointer_like);
        return false;
    }

    bool type::is_sequence_container() const
    {
        if (valid())
            return internal::has_flag(type_data::instance().types[_id].flags, type_flags::is_sequence_container);
        return false;
    }

    bool type::is_associative_container() const
    {
        if (valid())
            return internal::has_flag(type_data::instance().types[_id].flags, type_flags::is_associative_container);
        return false;
    }

    bool type::is_template_specialization() const
    {
        if (valid())
            return internal::has_flag(type_data::instance().types[_id].flags, type_flags::is_template_specialization);
        return false;
    }

    bool type::is_abstract() const
    {
        if (valid())
            return internal::has_flag(type_data::instance().types[_id].flags, type_flags::is_abstract);
        return false;
    }

    type::type(uint32_t id)
            : _id(id)
    {
    }

    type_flags type::flags() const
    {
        if (valid())
            return type_data::instance().types[_id].flags;
        return type_flags::none;
    }

    type_info *type::info() const
    {
        return &type_data::instance().types[_id];
    }

    type type::underlying_type() const
    {
        if (!valid())
            return registry::resolve<void>();
        return type(type_data::instance().types[_id].underlying_type_id);
    }

    type::type() : _id(type_hash_v<void>)
    {

    }

    reflection::data type::data(const std::string &name) const
    {
        return reflection::data(basic_hash<uint32_t>(name), _id);
    }

    reflection::data type::data(uint32_t id) const
    {
        return reflection::data(id, _id);
    }

    reflection::function type::function(const std::string &name) const
    {
        return reflection::function(basic_hash<uint32_t>(name), _id);
    }

    reflection::function type::function(uint32_t id) const
    {
        return reflection::function(id, _id);
    }

    bool type::is_convertible(uint32_t type_id) const
    {
        if (!valid())
            return false;
        return type_data::instance().types[_id].conversions.find(type_id) !=
               type_data::instance().types[_id].conversions.end();
    }

    reflection::data_container type::data() const
    {
        return reflection::data_container(_id);
    }

    reflection::function_container type::function() const
    {
        return reflection::function_container(_id);
    }
}