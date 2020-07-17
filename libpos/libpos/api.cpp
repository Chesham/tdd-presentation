#include "pch.h"
#include "PosServiceImpl.h"
using namespace std;

namespace libpos
{
    shared_ptr<PosService> PosService::create()
    {
        return make_shared<PosServiceImpl>();
    }
}
