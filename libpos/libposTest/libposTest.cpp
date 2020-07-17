#include "pch.h"
#include <libpos/libpos.h>
using namespace std;
using namespace libpos;
using namespace fakeit;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace libposTest
{
    TEST_CLASS(基本功能驗證)
    {
    public:

        TEST_METHOD(測試男生價格)
        {
            auto target = PosService::create(nullptr);
            auto expect = (size_t)300;
            auto actual = target->calcPrice({ {}, {}, Genders::male });
            Assert::AreEqual(expect, actual);
        }

        TEST_METHOD(測試女生價格)
        {
            auto target = PosService::create(nullptr);
            auto expect = (size_t)600;
            auto actual = target->calcPrice({ {}, {}, Genders::female });
            Assert::AreEqual(expect, actual);
        }
    };

    TEST_CLASS(外部安全模組測試)
    {
    public:

        struct Context
        {
            Mock<SecurityService> securitySvcMock;

            shared_ptr<SecurityService> securityService{ shared_ptr<SecurityService>(&securitySvcMock.get(), [](...) {}) };

            shared_ptr<PosService> makeService() const
            {
                return PosService::create(securityService);
            }

            Context()
            {
                When(Method(securitySvcMock, issued)).AlwaysReturn(false);
            }
        };

        TEST_METHOD(對象安全驗證通過)
        {
            Context ctx;
            auto target = ctx.makeService();
            auto expect = (size_t)300;
            auto actual = target->calcPrice({});
            Assert::AreEqual(expect, actual);
        }

        TEST_METHOD(對象安全驗證不通過)
        {
            Context ctx;
            When(Method(ctx.securitySvcMock, issued)).Do([](const UserItem& user)
                {
                    if (user.id == "A123456789")
                        return true;
                    return false;
                });
            auto target = ctx.makeService();
            Assert::ExpectException<IssuedUserException>([&] { target->calcPrice({ "A123456789", {}, {} }); });
        }
    };
}
