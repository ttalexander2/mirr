#include <limits>
#include "../include/registry.h"
#include "../include/type_factory.h"
#include "../include/type_container.h"

namespace reflection
{

    type registry::resolve(const std::string &name) noexcept
    {
        return type(id_from_name(name));
    }

    type registry::resolve(uint32_t id) noexcept
    {
        return type(id);
    }

    type_container registry::resolve() noexcept
    {
        return type_container();
    }

    bool registry::valid(uint32_t id)
    {
        return type_data::instance().types.find(id) != type_data::instance().types.end();
    }

    bool registry::valid(const std::string &name)
    {
        auto id = id_from_name(name);
        return valid(id);
    }

    uint32_t registry::id_from_name(const std::string &name)
    {
        auto val = type_data::instance().type_aliases.find(id_hash::hash(name));
        if (val != type_data::instance().type_aliases.end())
        {
            return val->second;
        }
        return std::numeric_limits<uint32_t>::max();
    }

} // reflection