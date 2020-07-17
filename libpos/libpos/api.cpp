#include "pch.h"
#include "PosServiceImpl.h"
using namespace std;

namespace libpos
{
    shared_ptr<PosService> PosService::create(const shared_ptr<SecurityService>& securityService)
    {
        return make_shared<PosServiceImpl>(securityService);
    }
}
