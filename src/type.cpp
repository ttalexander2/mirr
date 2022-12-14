#include "mirr/type.h"

#include "mirr/registry.h"
#include "mirr/data.h"
#include "mirr/function.h"
#include "mirr/iterators/data_container.h"
#include "mirr/iterators/function_container.h"
#include "mirr/iterators/constructor_container.h"

namespace mirr
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

    type::type() : _id(internal::type_hash_v<void>)
    {

    }

    mirr::data type::data(const std::string &name) const
    {
        return mirr::data(basic_hash<uint32_t>(name), _id);
    }

    mirr::data type::data(uint32_t id) const
    {
        return mirr::data(id, _id);
    }

    mirr::function type::func(const std::string &name) const
    {
        return mirr::function(basic_hash<uint32_t>(name), _id);
    }

    mirr::function type::func(uint32_t id) const
    {
        return mirr::function(id, _id);
    }

    bool type::is_convertible(uint32_t type_id) const
    {
        if (!valid())
            return false;
        return type_data::instance().types[_id].conversions.find(type_id) !=
               type_data::instance().types[_id].conversions.end();
    }

    mirr::data_container type::data() const
    {
        return mirr::data_container(_id);
    }

    mirr::function_container type::func() const
    {
        return mirr::function_container(_id);
    }

    any type::user_data(const std::string& key) const
    {
        uint32_t hash = basic_hash<uint32_t>(key);
        if (!valid() || type_data::instance().types[_id].user_data.find(hash) == type_data::instance().types[_id].user_data.end())
            return any{};

        return any{type_data::instance().types[_id].user_data[hash]};
    }

    mirr::constructor type::constructor(uint32_t id) const
    {
        return {id, _id};
    }

    mirr::constructor_container type::constructor() const
    {
        return mirr::constructor_container(_id);
    }
}