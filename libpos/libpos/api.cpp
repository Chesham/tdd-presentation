#include "pch.h"
#include "PosServiceImpl.h"
using namespace std;
using namespace std::chrono;

namespace libpos
{
    shared_ptr<PosService> PosService::create(const shared_ptr<SecurityService>& securityService, const function<system_clock::time_point()>& datetimeFunc)
    {
        return make_shared<PosServiceImpl>(securityService, datetimeFunc);
    }
}
