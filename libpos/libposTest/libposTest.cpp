#include "pch.h"
#include <libpos/libpos.h>
using namespace std;
using namespace libpos;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace libposTest
{
    TEST_CLASS(基本功能驗證)
    {
    public:

        TEST_METHOD(測試男生價格)
        {
            auto target = PosService::create();
            auto expect = (size_t)300;
            auto actual = target->calcPrice(Genders::male);
            Assert::AreEqual(expect, actual);
        }

        TEST_METHOD(測試女生價格)
        {
            auto target = PosService::create();
            auto expect = (size_t)600;
            auto actual = target->calcPrice(Genders::female);
            Assert::AreEqual(expect, actual);
        }
    };
}
