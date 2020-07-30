#include "pch.h"
#include "PosServiceImpl.h"
using namespace std;
using namespace std::chrono;

namespace libpos
{
    shared_ptr<PosService> PosService::create(const PosServiceConfiguration& config)
    {
        return make_shared<PosServiceImpl>(config);
    }
}
