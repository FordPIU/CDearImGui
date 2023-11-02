#pragma once

#ifndef STORAGE_MANAGER_H
#include "CJon.h"
#define STORAGE_MANAGER_H
#endif

namespace StorageManager
{
    void SaveIPAddress(const char *ipAddress)
    {
        CJon ipAddressArray = CJon("ipAddressList");

        ipAddressArray.addValueV2(ipAddress);
    }

    void RemoveIPAddress(std::string ipAddress)
    {
        CJon ipAddressArray = CJon("ipAddressList");

        ipAddressArray.removeValue(ipAddress);
    }

    vector<string> GetIPAddressList()
    {
        CJon ipAddressArray = CJon("ipAddressList", false);
        return ipAddressArray.getValues();
    }
}