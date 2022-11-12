#include "../include/any.h"

#include "../include/type_data.h"

namespace reflection
{
    any any::try_conversion(uint32_t type_id)
    {
        if (type().valid())
            return type_data::instance().types[type_info._id].conversions[type_id].helper_func(*this);
        return any{};
    }
}