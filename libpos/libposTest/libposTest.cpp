#include "pch.h"
#include <libpos/libpos.h>
using namespace std;
using namespace std::chrono;
using namespace libpos;
using namespace fakeit;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace libposTest
{
    TEST_CLASS(基本功能驗證)
    {
    public:

        struct Context
        {
            shared_ptr<PosService> makeService(int weekDay = 0)
            {
                if (weekDay < 0 || weekDay > 6)
                    throw runtime_error("invalid week day.");
                auto datetimeFunc = [=]
                {
                    tm tm{ 0 };
                    tm.tm_year = 120;
                    tm.tm_mon = 6;
                    tm.tm_mday = 12 + weekDay;
                    return system_clock::from_time_t(mktime(&tm));
                };
                return PosService::create({ nullptr, datetimeFunc });
            }
        };

        TEST_METHOD(測試一般男生價格)
        {
            Context ctx;
            auto target = ctx.makeService();
            auto expect = (size_t)300;
            auto actual = target->calcPrice({ {}, {}, Genders::male });
            Assert::AreEqual(expect, actual);
        }

        TEST_METHOD(測試一般女生價格)
        {
            Context ctx;
            auto target = ctx.makeService();
            auto expect = (size_t)600;
            auto actual = target->calcPrice({ {}, {}, Genders::female });
            Assert::AreEqual(expect, actual);
        }

        TEST_METHOD(測試淑女之夜男生價格)
        {
            Context ctx;
            auto target = ctx.makeService(3);
            auto expect = (size_t)600;
            auto actual = target->calcPrice({ {}, {}, Genders::male });
            Assert::AreEqual(expect, actual);
        }

        TEST_METHOD(測試淑女之夜女生價格)
        {
            Context ctx;
            auto target = ctx.makeService(3);
            auto expect = (size_t)0;
            auto actual = target->calcPrice({ {}, {}, Genders::female });
            Assert::AreEqual(expect, actual);
        }

        TEST_METHOD(測試一般男生價格異步執行)
        {
            Context ctx;
            auto target = ctx.makeService();
            auto expect = (size_t)300;
            auto actual = promise<size_t>();
            auto token = target->calcPrice({ {}, {}, Genders::male }, [&](shared_future<size_t>& fut)
                {
                    try
                    {
                        actual.set_value(fut.get());
                    }
                    catch (const exception&)
                    {
                        actual.set_exception(current_exception());
                    }
                });
            Assert::AreEqual(expect, actual.get_future().get());
        }

        TEST_METHOD(測試一般女生價格異步執行)
        {
            Context ctx;
            auto target = ctx.makeService();
            auto expect = (size_t)600;
            auto actual = promise<size_t>();
            auto token = target->calcPrice({ {}, {}, Genders::female }, [&](shared_future<size_t>& fut)
                {
                    try
                    {
                        actual.set_value(fut.get());
                    }
                    catch (const exception&)
                    {
                        actual.set_exception(current_exception());
                    }
                });
            Assert::AreEqual(expect, actual.get_future().get());
        }

        TEST_METHOD(測試淑女之夜男生價格異步執行)
        {
            Context ctx;
            auto target = ctx.makeService(3);
            auto expect = (size_t)600;
            auto actual = promise<size_t>();
            auto token = target->calcPrice({ {}, {}, Genders::male }, [&](shared_future<size_t>& fut)
                {
                    try
                    {
                        actual.set_value(fut.get());
                    }
                    catch (const exception&)
                    {
                        actual.set_exception(current_exception());
                    }
                });
            Assert::AreEqual(expect, actual.get_future().get());
        }

        TEST_METHOD(測試淑女之夜女生價格異步執行)
        {
            Context ctx;
            auto target = ctx.makeService(3);
            auto expect = (size_t)0;
            auto actual = promise<size_t>();
            auto token = target->calcPrice({ {}, {}, Genders::female }, [&](shared_future<size_t>& fut)
                {
                    try
                    {
                        actual.set_value(fut.get());
                    }
                    catch (const exception&)
                    {
                        actual.set_exception(current_exception());
                    }
                });
            Assert::AreEqual(expect, actual.get_future().get());
        }
    };

    TEST_CLASS(外部安全模組測試)
    {
    public:

        struct Context
        {
            Mock<SecurityService> securitySvcMock;

            shared_ptr<SecurityService> securityService{ shared_ptr<SecurityService>(&securitySvcMock.get(), [](...) {}) };

            shared_ptr<PosService> makeService(unique_ptr<PosServiceConfiguration>&& config = nullptr) const
            {
                if (config)
                    return PosService::create(*config);
                PosServiceConfiguration defaultConfig;
                defaultConfig.securityService = securityService;
                return PosService::create(defaultConfig);
            }

            Context()
            {
                When(ConstOverloadedMethod(securitySvcMock, issued, bool(const UserItem&))).AlwaysReturn(false);
                When(ConstOverloadedMethod(securitySvcMock, issued, void(const UserItem&, std::weak_ptr<std::function<void(bool)>>)))
                    .AlwaysDo([](const UserItem& user, weak_ptr<function<void(bool)>> completeEvent)
                        {
                            auto e = completeEvent.lock();
                            if (!e)
                                return;
                            (*e)(false);
                        });
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
            When(ConstOverloadedMethod(ctx.securitySvcMock, issued, bool(const UserItem&))).Do([](const UserItem& user)
                {
                    if (user.id == "A123456789")
                        return true;
                    return false;
                });
            auto target = ctx.makeService();
            Assert::ExpectException<IssuedUserException>([&] { target->calcPrice({ "A123456789", {}, {} }); });
        }

        TEST_METHOD(對象安全驗證通過異步執行)
        {
            Context ctx;
            auto target = ctx.makeService();
            auto expect = (size_t)300;
            auto actual = promise<size_t>();
            auto token = target->calcPrice({}, [&](shared_future<size_t>& fut)
                {
                    try
                    {
                        actual.set_value(fut.get());
                    }
                    catch (const exception&)
                    {
                        actual.set_exception(current_exception());
                    }
                });
            Assert::AreEqual(expect, actual.get_future().get());
        }

        TEST_METHOD(對象安全驗證不通過異步執行)
        {
            Context ctx;
            When(ConstOverloadedMethod(ctx.securitySvcMock, issued, void(const UserItem&, weak_ptr<function<void(bool)>>))).Do([](const UserItem& user, weak_ptr<function<void(bool)>> completeEvent)
                {
                    auto e = completeEvent.lock();
                    if (!e)
                        return;
                    if (user.id == "A123456789")
                        return (*e)(true);
                    return (*e)(false);
                });
            auto target = ctx.makeService();
            auto actual = promise<size_t>();
            auto token = target->calcPrice({ "A123456789", {}, {} }, [&](shared_future<size_t>& fut)
                {
                    try
                    {
                        actual.set_value(fut.get());
                    }
                    catch (const exception&)
                    {
                        actual.set_exception(current_exception());
                    }
                });
            Assert::ExpectException<IssuedUserException>([&] { actual.get_future().get(); });
        }

        TEST_METHOD(操作逾時)
        {
            Context ctx;
            future<void> fut;
            When(ConstOverloadedMethod(ctx.securitySvcMock, issued, void(const UserItem&, weak_ptr<function<void(bool)>>))).Do([&](const UserItem& user, weak_ptr<function<void(bool)>> completeEvent)
                {
                    fut = async([&, completeEvent = move(completeEvent)]
                        {
                            auto e = completeEvent.lock();
                            if (!e)
                                return;
                            if (user.id == "A123456789")
                                return (*e)(true);
                            return (*e)(false);
                        });
                });
            auto config = make_unique<PosServiceConfiguration>();
            config->securityService = ctx.securityService;
            auto target = ctx.makeService(move(config));
            auto actual = promise<size_t>();
            target->calcPrice({ "A123456789", {}, {} }, [&](shared_future<size_t>& fut)
                {
                    try
                    {
                        actual.set_value(fut.get());
                    }
                    catch (const exception&)
                    {
                        actual.set_exception(current_exception());
                    }
                });
            fut.get();
            Assert::AreEqual((underlying_type<future_status>::type)future_status::timeout, (underlying_type<future_status>::type)actual.get_future().wait_for(10ms));
        }
    };
}
