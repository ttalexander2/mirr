//
// Created by Thomas on 1/17/2023.
//

#pragma once

#include "any.h"
#include "type.h"

namespace mirr
{

    template<typename KeyType, typename>
    auto type::user_data(KeyType &&key) const
    {
        return nullptr;
    }

}
