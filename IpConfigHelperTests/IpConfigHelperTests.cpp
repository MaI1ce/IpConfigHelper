#include "pch.h"
#include "CppUnitTest.h"
#include "../IpConfigHelper/IpHelper.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace IpConfigHelperTests
{
	TEST_CLASS(IpConfigHelperTests)
	{
	private:
		std::string func(NetworkParams ob) {
			return ob.getHostName();
		}
	public:
		
		TEST_METHOD(NetworkParamsConstructorInitTest)
		{
			NetworkParams ob;
			ob.init();
			Assert::AreEqual(NO_ERROR, ob.getRetVal());
		}

		TEST_METHOD(NetworkParamsCopyConstructorTest)
		{
			NetworkParams ob;
			ob.init();
			std::string res = func(ob);
			Assert::AreEqual(res, ob.getHostName());
		}

		TEST_METHOD(NetworkParamsgetDnsServerAddresTest1)
		{
			int index = 10;
			std::string def = "0.0.0.0";
			NetworkParams ob;
			ob.init();
			std::string dns = ob.getDnsServerAddres(index);
			Assert::AreEqual(def, dns);
		}
		TEST_METHOD(NetworkParamsgetDnsServerAddresTest2)
		{
			NetworkParams ob;
			ob.init();
			int index = 0;
			std::string def = "8.8.8.8";
			std::string dns = ob.getDnsServerAddres(index);
			Assert::AreEqual(def, dns);
		}
	};
}
