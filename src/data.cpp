#include <utility>

#include "../include/data.h"

#include "../include/type_data.h"
#include "../include/registry.h"

namespace reflection
{
    data::data() : _id(0), _type_id(registry::resolve<void>().id())
    {

    }

    data::data(uint32_t id, uint32_t type_id) : _id(id), _type_id(type_id)
    {

    }

    bool data::valid() const
    {
        return registry::valid(_type_id)
        && type_data::instance().types[_type_id].data.find(_id) != type_data::instance().types[_type_id].data.end();
    }

    uint32_t data::id() const
    {
        return _id;
    }

    std::string data::name() const
    {
        if (valid())
            return type_data::instance().types[_type_id].data[_id].name;
        return std::string{};
    }

    reflection::type data::type() const
    {
        return registry::resolve(_type_id);
    }

    bool data::is_serialized() const
    {
        if (valid())
            return internal::has_flag(type_data::instance().types[_type_id].data[_id].flags, data_flags::is_serialized);
        return false;
    }

    bool data::is_const() const
    {
        if (valid())
            return internal::has_flag(type_data::instance().types[_type_id].data[_id].flags, data_flags::is_const);
        return false;
    }

    bool data::is_static() const
    {
        if (valid())
            return internal::has_flag(type_data::instance().types[_type_id].data[_id].flags, data_flags::is_static);
        return false;
    }

    any data::get(reflection::handle handle) const
    {
        if (!valid())
            return any{};
        return type_data::instance().types[_type_id].data[_id].get(std::move(handle));
    }

    void data::set(reflection::handle handle, const any& value) const
    {
        if (!valid())
            return;
        type_data::instance().types[_type_id].data[_id].set(std::move(handle), value);
    }


}