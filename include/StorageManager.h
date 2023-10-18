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

        ipAddressArray.addValue(ipAddress);
    }

    vector<string> GetIPAddressList()
    {
        CJon ipAddressArray = CJon("ipAddressList", false);
        return ipAddressArray.getValues();
    }
}