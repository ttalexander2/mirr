#include "mirr/function.h"

#include "mirr/type_data.h"
#include "mirr/registry.h"
#include "mirr/iterators/argument_container.h"

namespace mirr
{

    function::function() : _id(0), _type_id(registry::resolve<void>().id())
    {

    }


    function::function(uint32_t id, uint32_t type_id) : _id(id), _type_id(type_id)
    {

    }

    bool function::valid() const
    {
        return registry::valid(_type_id)
               && type_data::instance().types[_type_id].functions.find(_id)
                  != type_data::instance().types[_type_id].functions.end();
    }

    bool function::operator==(const function &rhs) const
    {
        return _id == rhs._id && _type_id == rhs._type_id;
    }

    uint32_t function::id() const
    {
        return _id;
    }

    std::string function::name() const
    {
        if (valid())
            return type_data::instance().types[_type_id].functions[_id].name;
        return std::string{};
    }

    mirr::type function::type() const
    {
        return registry::resolve(_type_id);
    }

    size_t function::arity() const
    {
        if (valid())
            return type_data::instance().types[_type_id].functions[_id].arity;
        return 0;
    }

    mirr::type function::return_type() const
    {
        if (valid())
            return mirr::type(type_data::instance().types[_type_id].functions[_id].return_type);
        return registry::resolve<void>();
    }

    mirr::type function::args(size_t index) const
    {
        if (valid())
            return mirr::type(type_data::instance().types[_type_id].functions[_id].arg(index));
        return registry::resolve<void>();
    }

    any function::invoke_internal(mirr::handle handle, any *args, size_t count) const
    {
        if (valid())
            return type_data::instance().types[_type_id].functions[_id].invoke(std::move(handle), args);
        return any{};
    }

    argument_container function::args() const
    {
        return argument_container(_type_id, _id);
    }
}