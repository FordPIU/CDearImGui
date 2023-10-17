#pragma once

#ifndef STORAGE_MANAGER_H
#include <iostream>
#include <fstream>
#include <vector>
#include <json/json.h>
#define STORAGE_MANAGER_H
#endif

using namespace std;

namespace StorageManager
{
    void SaveIPAddress(const char *ipAddress)
    {
        // Load Json
        Json::Value root;

        ifstream file("bridgeConnections.json");
        if (file.is_open())
        {
            file >> root;
            file.close();
        }
        else
        {
            cout << "Warning: File did not exist, creating from a empty slate." << endl;
        }

        // Set Value
        const auto &memberNames = root.getMemberNames();
        int index = (memberNames.empty() ? -1 : memberNames.size()) + 1;
        root["BridgeStorage"][index] = ipAddress;

        // Save Json
        ofstream outputFile("bridgeConnections.json");
        if (outputFile.is_open())
        {
            outputFile << root;
            outputFile.close();
        }
        else
        {
            cerr << "Could not write to bridgeConnections.json" << endl;
        }
    }

    vector<const char *> GetIPAddressList()
    {
        Json::Value root;

        // File Check
        ifstream file("CDearImGui.json");
        if (file.is_open())
        {
            file >> root;
            file.close();
        }
        else
        {
            return {};
        }

        const Json::Value bridgeStorage = root["BridgeStorage"];
        vector<const char *> ipAddresss;

        // this isnt working for some reason.
        // TODO: FIX THIS!
        for (int i = 0; i < ipAddresss.size(); ++i)
        {
            cout << bridgeStorage[i] << endl;
            ipAddresss.push_back(bridgeStorage[i].asCString());
        }

        return ipAddresss;
    }
}