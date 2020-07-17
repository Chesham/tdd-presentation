#pragma once
#include <string>
#include "genders.h"

namespace libpos
{
    struct UserItem
    {
        std::string id;

        std::string name;

        Genders gender;
    };
}
