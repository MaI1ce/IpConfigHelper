#pragma once
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <winsock2.h>
#include <iphlpapi.h>

#include <string>
#endif
#define ERROR_NULL_POINTER_ACCESS 101

class IpHelper {
protected:
    DWORD dwRetVal;
public:
    virtual void info() = 0;
};

class IpHelperError: public IpHelper {
private:
    IpHelper* p_ob;
public:
    /*
    error codes:
    ERROR_BUFFER_OVERFLOW
    ERROR_INSUFFICIENT_BUFFER
    ERROR_NULL_POINTER_ACCESS (101)
    ERROR_SUCCESS
    */
    IpHelperError(IpHelper* ob, int error_code) :p_ob(ob) {
        dwRetVal = error_code;
    }
    void info() { fprintf(stderr,"error code = %d", dwRetVal); };
};

//-----------------------------------------------------------------------NetworkParams
class NetworkParams: public IpHelper {
    FIXED_INFO* pFixedInfo;
    ULONG       ulOutBufLen;
    int         DnsNum;
public:
    NetworkParams();

    NetworkParams(const NetworkParams& ob);

    ~NetworkParams();

    void info();

    long  getRetVal() { return dwRetVal; }

    std::string getHostName();

    std::string getDomainName();

    std::string getNetBIOSScopeID();

    int getNodeType();

    bool getEnableRouting();

    bool getEnableProxy();

    bool getNetBIOSResolution();

    int getNumDNS() {return DnsNum;}

    std::string getDnsServerAddres(int index);

};
//-----------------------------------------------------------------------NetworkParams

//-----------------------------------------------------------------------AdaptersInfo
class AdaptersInfo : public IpHelper {
    IP_ADAPTER_INFO* pAdapterInfo;
    ULONG            ulOutBufLen;
    int              AdapterNum;
public:
    AdaptersInfo();
    AdaptersInfo(const AdaptersInfo& ob);
    ~AdaptersInfo();
    void info();
};
//-----------------------------------------------------------------------AdaptersInfo