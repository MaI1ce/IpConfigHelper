#pragma once
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <winsock2.h>
#include <iphlpapi.h>

#include <string>
#endif
#define ERROR_NULL_POINTER_ACCESS 101
#define UNEXPECTED_API_ERROR 9999

class IpHelperError {
private:
    DWORD dwRetVal;
public:
    /*
    error codes:
    ERROR_BUFFER_OVERFLOW
    ERROR_INSUFFICIENT_BUFFER
    ERROR_NULL_POINTER_ACCESS (101)
    ERROR_SUCCESS
    UNEXPECTED_API_ERROR
    */
    IpHelperError(int error_code) : dwRetVal(error_code) { }
    void info() { fprintf(stderr, "error code = %d", dwRetVal); };
};


template<class APItype>
class IpHelper {
protected:
    DWORD dwRetVal;
    ULONG size;
    APItype* pBuffer;

    virtual DWORD APICALL() = 0;

    virtual DWORD Error(DWORD val) = 0;

    template<class APItype>

    friend void allocator(IpHelper<APItype>* ob);

    IpHelper() :pBuffer(NULL), size(0), dwRetVal(0) {}

    IpHelper(const IpHelper& ob) :pBuffer(NULL), size(0), dwRetVal(0)
    {
        if (ob.pBuffer != NULL) {
            pBuffer = (APItype*)malloc(ob.size);
            if (pBuffer != NULL) {
                memcpy(pBuffer, ob.pBuffer, ob.size);
            }
            dwRetVal = ob.dwRetVal;
            size = ob.size;
        }
    }
public:

    ~IpHelper() {
        if (pBuffer) {
            free(pBuffer);
            pBuffer = NULL;
        }
    }

    virtual void info() = 0;

    virtual void init() = 0;

    
};


//-----------------------------------------------------------------------NetworkParams
class NetworkParams: public IpHelper<FIXED_INFO> {
    int DnsNum;
    
    DWORD APICALL() {
        return GetNetworkParams(pBuffer, &size);
    }

    DWORD Error(DWORD val) {
        if (val == ERROR_BUFFER_OVERFLOW)
            return NO_ERROR;
        else return UNEXPECTED_API_ERROR;
    }

    template<class APItype> friend void allocator(IpHelper<APItype>* ob);
public:
    void init();

    NetworkParams() :IpHelper<FIXED_INFO>(), DnsNum(0) {};

    NetworkParams(const NetworkParams& ob) :IpHelper<FIXED_INFO>(ob), DnsNum(ob.DnsNum) {};

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
class AdaptersInfo : public IpHelper<IP_ADAPTER_INFO> {
    int AdapterNum;

    DWORD APICALL() {
        return GetAdaptersInfo(pBuffer, &size);
    }

    DWORD Error(DWORD val) {
        if (val != ERROR_SUCCESS)
            return NO_ERROR;
        else return UNEXPECTED_API_ERROR;
    }

    template<class APItype> friend void allocator(IpHelper<APItype>* ob);
public:
    AdaptersInfo() :IpHelper<IP_ADAPTER_INFO>(), AdapterNum(0) {};

    AdaptersInfo(const AdaptersInfo& ob) :IpHelper<IP_ADAPTER_INFO>(ob), AdapterNum(ob.AdapterNum) {};

    void init();

    void info();
};
//-----------------------------------------------------------------------AdaptersInfo