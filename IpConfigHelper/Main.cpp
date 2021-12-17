#include "stdafx.h"

using namespace std;

string foo(NetworkParams ob) {
	ob.info();
	return ob.getHostName();
}



int main(int argc, char* argv[]) {
	try {
		NetworkParams ob;
		ob.init();
		string res = foo(ob);
		cout << res;
	}
	catch (IpHelperError ob) {
		ob.info();
	}
	//DWORD(__stdcall*fp)(/*PFIXED_INFO*/ void *p, PULONG b) = GetNetworkParams;
	return 0;
}