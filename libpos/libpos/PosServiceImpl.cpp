#include "pch.h"
#include <stdexcept>
#include "PosServiceImpl.h"
using namespace std;

namespace libpos
{
    size_t PosServiceImpl::calcPrice(Genders gender) const
    {
        switch (gender)
        {
        case libpos::Genders::male:
            return 300;
        case libpos::Genders::female:
            return 600;
        default:
            throw runtime_error("invalid operation.");
        }
    }
}
