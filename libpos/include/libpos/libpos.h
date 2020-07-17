#pragma once
#include "genders.h"
#include <cstddef>
#include <memory>

#ifdef LIBPOS
#define LIBPOSAPI __declspec(dllexport)
#else
#define LIBPOSAPI __declspec(dllimport)
#endif // LIBPOS

namespace libpos
{
    struct PosService
    {
        virtual ~PosService() { }

        virtual std::size_t calcPrice(Genders gender) const = 0;

        static LIBPOSAPI std::shared_ptr<PosService> create();
    };
}
