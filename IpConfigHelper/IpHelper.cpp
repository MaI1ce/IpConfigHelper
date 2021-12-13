#include "stdafx.h"


//-----------------------------------------------------------------------NetworkParams
NetworkParams::NetworkParams() 
	:pFixedInfo(NULL), ulOutBufLen(0), DnsNum(0)
{
    dwRetVal = 0;
	if (pFixedInfo == NULL) {
        dwRetVal = GetNetworkParams(pFixedInfo, &ulOutBufLen);
        if (dwRetVal == ERROR_BUFFER_OVERFLOW) {
            pFixedInfo = (FIXED_INFO*)malloc(ulOutBufLen);
            if (pFixedInfo == NULL) {
                throw IpHelperError(this, ERROR_BUFFER_OVERFLOW);
            }
            if (dwRetVal = GetNetworkParams(pFixedInfo, &ulOutBufLen) != NO_ERROR) {
                if (pFixedInfo) {
                    free(pFixedInfo);
                }
                throw IpHelperError(this, dwRetVal);
            }
            IP_ADDR_STRING* pIPAddr = pFixedInfo->DnsServerList.Next;
            int i = 0;
            while (pIPAddr) {
                pIPAddr = pIPAddr->Next;
                i++;
            }
            DnsNum = i;
        }
	}
}

NetworkParams::NetworkParams(const NetworkParams& ob)
    :pFixedInfo(NULL), ulOutBufLen(ob.ulOutBufLen), DnsNum(ob.DnsNum)
{
    dwRetVal = ob.dwRetVal;
    if (ob.pFixedInfo != NULL) {
        pFixedInfo = (FIXED_INFO*)malloc(ob.ulOutBufLen);
        if (pFixedInfo != NULL) {
            memcpy(pFixedInfo, ob.pFixedInfo, ob.ulOutBufLen);
        }
    }
}

NetworkParams::~NetworkParams() {
    if (pFixedInfo) {
        free(pFixedInfo);
        pFixedInfo = NULL;
    }
};

void NetworkParams::info() {
    if (pFixedInfo) {
        IP_ADDR_STRING* pIPAddr;
        std::cout << "\tHost Name: " << std::string(pFixedInfo->HostName) << "\n";
        std::cout << "\tDomain Name: " << std::string(pFixedInfo->DomainName) << "\n";
        std::cout << "\tDNS Servers:\n";

        pIPAddr = &(pFixedInfo->DnsServerList);
        while (pIPAddr) {
            std::cout << "\t\t" << std::string(pIPAddr->IpAddress.String) << "\n";
            pIPAddr = pIPAddr->Next;
        }

        std::cout << "\tNode Type: ";
        switch (pFixedInfo->NodeType) {
        case 1:
            std::cout << "Broadcast\n";
            break;
        case 2:
            std::cout << "Peer to peer\n";
            break;
        case 4:
            std::cout << "Mixed\n";
            break;
        case 8:
            std::cout << "Hybrid\n";
            break;
        default:
            printf("\n");
        }

        std::cout << "\tNetBIOS Scope ID: " << pFixedInfo->ScopeId <<"\n";

        if (pFixedInfo->EnableRouting)
            std::cout << "\tIP Routing Enabled: Yes\n";
        else
            std::cout << "\tIP Routing Enabled: No\n";

        if (pFixedInfo->EnableProxy)
            std::cout << "\tWINS Proxy Enabled: Yes\n";
        else
            std::cout << "\tWINS Proxy Enabled: No\n";

        if (pFixedInfo->EnableDns)
            std::cout << "\tNetBIOS Resolution Uses DNS: Yes\n";
        else
            std::cout << "\tNetBIOS Resolution Uses DNS: No\n";
    }
}

std::string NetworkParams::getDnsServerAddres(int index) {
    if (pFixedInfo) {
        IP_ADDR_STRING* pIPAddr = &(pFixedInfo->DnsServerList);
        int i = 0;
        while (pIPAddr && (i < index)) {
            i++;
            pIPAddr = pIPAddr->Next;
        }
        if (pIPAddr)
            return std::string(pIPAddr->IpAddress.String);
        else return std::string("0.0.0.0");
    }
    else throw IpHelperError(this, ERROR_NULL_POINTER_ACCESS);
}

std::string NetworkParams::getHostName() {
    if (pFixedInfo)
        return std::string(pFixedInfo->HostName);
    else throw IpHelperError(this, ERROR_NULL_POINTER_ACCESS);
}

std::string NetworkParams::getDomainName() {
    if (pFixedInfo)
        return std::string(pFixedInfo->DomainName);
    else throw IpHelperError(this, ERROR_NULL_POINTER_ACCESS);
}

std::string NetworkParams::getNetBIOSScopeID() {
    if (pFixedInfo)
        return std::string(pFixedInfo->ScopeId);
    else throw IpHelperError(this, ERROR_NULL_POINTER_ACCESS);
}

int NetworkParams::getNodeType() {
    if (pFixedInfo)
        return pFixedInfo->NodeType;
    else throw IpHelperError(this, ERROR_NULL_POINTER_ACCESS);
}

bool NetworkParams::getEnableRouting() {
    if (pFixedInfo)
        return pFixedInfo->EnableRouting;
    else throw IpHelperError(this, ERROR_NULL_POINTER_ACCESS);
}

bool NetworkParams::getEnableProxy() {
    if (pFixedInfo)
        return pFixedInfo->EnableProxy;
    else throw IpHelperError(this, ERROR_NULL_POINTER_ACCESS);
}

bool NetworkParams::getNetBIOSResolution() {
    if (pFixedInfo)
        return pFixedInfo->EnableDns;
    else throw IpHelperError(this, ERROR_NULL_POINTER_ACCESS);
}

/*


NetworkParams NetworkParams::operator=(const NetworkParams& ob){
    ulOutBufLen = ob.ulOutBufLen;
    dwRetVal = ob.dwRetVal;
    if (ob.pFixedInfo != NULL) {
        pFixedInfo = (FIXED_INFO*)malloc(ob.ulOutBufLen);
        if (pFixedInfo == NULL) {
            memcpy(pFixedInfo, ob.pFixedInfo, ob.ulOutBufLen);
        }
    }
    return *this;
}
*/
//-----------------------------------------------------------------------NetworkParams

//-----------------------------------------------------------------------AdaptersInfo
AdaptersInfo::AdaptersInfo()
    :pAdapterInfo(NULL), ulOutBufLen(0), AdapterNum(0)
{
    dwRetVal = 0;
    if (pAdapterInfo == NULL) {
        dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen);
        if (dwRetVal != ERROR_SUCCESS) {
            pAdapterInfo = (IP_ADAPTER_INFO*)malloc(ulOutBufLen);
            if (pAdapterInfo == NULL) {
                throw IpHelperError(this, ERROR_BUFFER_OVERFLOW);
            }
            if (dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_SUCCESS) {
                if (pAdapterInfo) {
                    free(pAdapterInfo);
                }
                throw IpHelperError(this, dwRetVal);
            }
            PIP_ADAPTER_INFO pAdapter = pAdapterInfo;
            int i = 0;
            while (pAdapter) {
                pAdapter = pAdapter->Next;
                i++;
            }
            AdapterNum = i;
        }
    }
}

AdaptersInfo::AdaptersInfo(const AdaptersInfo& ob)
    :pAdapterInfo(NULL), ulOutBufLen(ob.ulOutBufLen), AdapterNum(ob.AdapterNum)
{
    dwRetVal = ob.dwRetVal;
    if (ob.pAdapterInfo != NULL) {
        pAdapterInfo = (IP_ADAPTER_INFO*)malloc(ulOutBufLen);
        if (pAdapterInfo != NULL) {
            memcpy(pAdapterInfo, ob.pAdapterInfo, ob.ulOutBufLen);
        }
    }
}

AdaptersInfo::~AdaptersInfo() {
    if (pAdapterInfo) {
        free(pAdapterInfo);
        pAdapterInfo = NULL;
    }
};

void AdaptersInfo::info() {
    printf("empty");
}

//-----------------------------------------------------------------------AdaptersInfo
