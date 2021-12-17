#include "stdafx.h"

//friend function
template<class APItype>
void allocator(IpHelper<APItype>* ob) {
    if (ob->pBuffer == NULL) {
        ob->dwRetVal = ob->APICALL();
        if (ob->Error(ob->dwRetVal) == NO_ERROR) {
            ob->pBuffer = (APItype*)malloc(ob->size);
            if (ob->pBuffer == NULL) {
                throw IpHelperError(ERROR_BUFFER_OVERFLOW);
            }
            ob->dwRetVal = ob->APICALL();
            if (ob->Error(ob->dwRetVal) == NO_ERROR) {
                if (ob->pBuffer) {
                    free(ob->pBuffer);
                }
                throw IpHelperError(ob->dwRetVal);
            }
        }
    }
}

//-----------------------------------------------------------------------NetworkParams

void NetworkParams::init() 
{
    allocator(this);
    IP_ADDR_STRING* pIPAddr = &(pBuffer->DnsServerList);
    int i = 0;
    while (pIPAddr) {
        pIPAddr = pIPAddr->Next;
        i++;
    }
    DnsNum = i;
}


void NetworkParams::info() {
    if (pBuffer) {
        IP_ADDR_STRING* pIPAddr;
        std::cout << "\tHost Name: " << std::string(pBuffer->HostName) << "\n";
        std::cout << "\tDomain Name: " << std::string(pBuffer->DomainName) << "\n";
        std::cout << "\tDNS Servers:\n";

        pIPAddr = &(pBuffer->DnsServerList);
        while (pIPAddr) {
            std::cout << "\t\t" << std::string(pIPAddr->IpAddress.String) << "\n";
            pIPAddr = pIPAddr->Next;
        }

        std::cout << "\tNode Type: ";
        switch (pBuffer->NodeType) {
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

        std::cout << "\tNetBIOS Scope ID: " << pBuffer->ScopeId <<"\n";

        if (pBuffer->EnableRouting)
            std::cout << "\tIP Routing Enabled: Yes\n";
        else
            std::cout << "\tIP Routing Enabled: No\n";

        if (pBuffer->EnableProxy)
            std::cout << "\tWINS Proxy Enabled: Yes\n";
        else
            std::cout << "\tWINS Proxy Enabled: No\n";

        if (pBuffer->EnableDns)
            std::cout << "\tNetBIOS Resolution Uses DNS: Yes\n";
        else
            std::cout << "\tNetBIOS Resolution Uses DNS: No\n";
    }
}

std::string NetworkParams::getDnsServerAddres(int index) {
    if (pBuffer) {
        IP_ADDR_STRING* pIPAddr = &(pBuffer->DnsServerList);
        int i = 0;
        while (pIPAddr && (i < index)) {
            i++;
            pIPAddr = pIPAddr->Next;
        }
        if (pIPAddr)
            return std::string(pIPAddr->IpAddress.String);
        else return std::string("0.0.0.0");
    }
    else throw IpHelperError(ERROR_NULL_POINTER_ACCESS);
}

std::string NetworkParams::getHostName() {
    if (pBuffer)
        return std::string(pBuffer->HostName);
    else throw IpHelperError(ERROR_NULL_POINTER_ACCESS);
}

std::string NetworkParams::getDomainName() {
    if (pBuffer)
        return std::string(pBuffer->DomainName);
    else throw IpHelperError(ERROR_NULL_POINTER_ACCESS);
}

std::string NetworkParams::getNetBIOSScopeID() {
    if (pBuffer)
        return std::string(pBuffer->ScopeId);
    else throw IpHelperError(ERROR_NULL_POINTER_ACCESS);
}

int NetworkParams::getNodeType() {
    if (pBuffer)
        return pBuffer->NodeType;
    else throw IpHelperError( ERROR_NULL_POINTER_ACCESS);
}

bool NetworkParams::getEnableRouting() {
    if (pBuffer)
        return pBuffer->EnableRouting;
    else throw IpHelperError(ERROR_NULL_POINTER_ACCESS);
}

bool NetworkParams::getEnableProxy() {
    if (pBuffer)
        return pBuffer->EnableProxy;
    else throw IpHelperError( ERROR_NULL_POINTER_ACCESS);
}

bool NetworkParams::getNetBIOSResolution() {
    if (pBuffer)
        return pBuffer->EnableDns;
    else throw IpHelperError( ERROR_NULL_POINTER_ACCESS);
}



//-----------------------------------------------------------------------NetworkParams

//-----------------------------------------------------------------------AdaptersInfo
void AdaptersInfo::init() {
    allocator(this);
    PIP_ADAPTER_INFO pAdapter = pBuffer;
    int i = 0;
    while (pAdapter) {
        pAdapter = pAdapter->Next;
        i++;
    }
    AdapterNum = i;
}


void AdaptersInfo::info() {
    printf("empty");
}

//-----------------------------------------------------------------------AdaptersInfo
