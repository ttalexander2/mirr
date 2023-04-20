#include "mirr/data.h"

#include <utility>
#include "mirr/type_data.h"
#include "mirr/registry.h"

namespace mirr
{
    data::data() : _id(0), _owner(registry::resolve<void>().id())
    {

    }

    data::data(uint32_t id, uint32_t owner) : _id(id), _owner(owner)
    {

    }

    bool data::valid() const
    {
        return registry::valid(_owner)
               && type_data::instance().types[_owner].data.find(_id)
                  != type_data::instance().types[_owner].data.end();
    }

    uint32_t data::id() const
    {
        return _id;
    }

    std::string data::name() const
    {
        if (valid())
            return type_data::instance().types[_owner].data[_id].name;
        return std::string{};
    }

    mirr::type data::type() const
    {
    	if (valid())
    		return mirr::type(type_data::instance().types[_owner].data[_id].type_id);
    	return registry::resolve<void>();
    }

    bool data::is_const() const
    {
        if (valid())
            return internal::has_flag(type_data::instance().types[_owner].data[_id].flags, data_flags::is_const);
        return false;
    }

    bool data::is_static() const
    {
        if (valid())
            return internal::has_flag(type_data::instance().types[_owner].data[_id].flags, data_flags::is_static);
        return false;
    }

    any data::get(mirr::handle handle) const
    {
        if (!valid())
            return any{};
        return type_data::instance().types[_owner].data[_id].get(std::move(handle));
    }

    bool data::set(mirr::handle& handle, any value) const
    {
        if (!valid())
            return false;
        return type_data::instance().types[_owner].data[_id].set(handle, std::move(value));
    }

    bool data::operator==(const data &rhs) const
    {
        return _id == rhs._id && _owner == rhs._owner;
    }

    mirr::type data::owner() const
    {
    	return mirr::type(_owner);
    }

    any data::user_data(uint32_t key) const
    {
        if (valid())
        {
            return type_data::instance().types[_owner].data[_id].user_data[key];
        }
        return any{};
    }
	
	any data::user_data(const std::string& key) const
    {
    	return user_data(basic_hash<uint32_t>(key));
    }

	bool data::has_user_data(uint32_t hash) const
    {
    	return valid() && type_data::instance().types[_owner].data[_id].user_data.find(hash) != type_data::instance().types[_owner].data[_id].user_data.end();
    }

	bool data::has_user_data(const std::string& key) const
    {
    	return has_user_data(basic_hash<uint32_t>(key));
    }


}