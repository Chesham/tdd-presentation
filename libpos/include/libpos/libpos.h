#pragma once
#include "useritem.h"
#include "securityservice.h"
#include <cstddef>
#include <memory>
#include <functional>
#include <chrono>

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

        virtual std::size_t calcPrice(const UserItem& user) const = 0;

        static LIBPOSAPI std::shared_ptr<PosService> create(const std::shared_ptr<SecurityService>& securityService, const std::function<std::chrono::system_clock::time_point()>& datetimeFunc = nullptr);
    };
}
