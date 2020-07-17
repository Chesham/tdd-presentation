#pragma once
#include <libpos/libpos.h>

namespace libpos
{
    class PosServiceImpl : public PosService
    {
    public:

        virtual std::size_t calcPrice(Genders gender) const override;
    };
}
