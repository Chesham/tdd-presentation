#pragma once
#include "useritem.h"
#include "securityservice.h"
#include <cstddef>
#include <memory>
#include <functional>
#include <future>

#ifdef LIBPOS
#define LIBPOSAPI __declspec(dllexport)
#else
#define LIBPOSAPI __declspec(dllimport)
#endif // LIBPOS

namespace libpos
{
    struct PosServiceConfiguration
    {
        std::shared_ptr<SecurityService> securityService;

        std::function<std::chrono::system_clock::time_point()> datetimeFunc{ [] { return std::chrono::system_clock::now(); } };
    };

    struct PosService
    {
        virtual ~PosService() { }

        virtual std::size_t calcPrice(const UserItem& user) const = 0;

        virtual std::shared_ptr<std::function<void(std::shared_future<std::size_t>&)>> calcPrice(const UserItem& user, const std::function<void(std::shared_future<std::size_t>&)>& completeEvent) const = 0;

        static LIBPOSAPI std::shared_ptr<PosService> create(const PosServiceConfiguration& config);
    };
}
