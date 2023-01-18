#include "mirr/data.h"

#include <utility>
#include "mirr/type_data.h"
#include "mirr/registry.h"

namespace mirr
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
               && type_data::instance().types[_type_id].data.find(_id)
                  != type_data::instance().types[_type_id].data.end();
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

    mirr::type data::type() const
    {
        return registry::resolve(_type_id);
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

    any data::get(mirr::handle handle) const
    {
        if (!valid())
            return any{};
        return type_data::instance().types[_type_id].data[_id].get(std::move(handle));
    }

    bool data::set(mirr::handle handle, const any &value) const
    {
        if (!valid())
            return false;
        return type_data::instance().types[_type_id].data[_id].set(std::move(handle), value);
    }

    bool data::operator==(const data &rhs) const
    {
        return _id == rhs._id && _type_id == rhs._type_id;
    }

    mirr::type data::data_type() const
    {
        if (valid())
            return registry::resolve(type_data::instance().types[_type_id].data[_id].type_id);
        return registry::resolve<void>();
    }

    any data::user_data(uint32_t key) const
    {
        if (valid())
        {
            return type_data::instance().types[_type_id].data[_id].user_data[key];
        }
        return any{};
    }
	
	any data::user_data(const std::string& key) const
    {
    	return user_data(basic_hash<uint32_t>(key));
    }

	bool data::has_user_data(uint32_t hash) const
    {
    	return valid() && type_data::instance().types[_type_id].data[_id].user_data.find(hash) != type_data::instance().types[_type_id].data[_id].user_data.end();
    }

	bool data::has_user_data(const std::string& key) const
    {
    	return has_user_data(basic_hash<uint32_t>(key));
    }


}