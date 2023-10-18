#pragma once

#ifndef CJON_H
#include <iostream>
#include <fstream>
#include <map>
#include <filesystem>
#define CJON_H
#endif

using namespace std;
namespace fs = filesystem;

class CJon
{
public:
    CJon(const string &dataHeaderName, const bool &saveOnDeconstruct = true)
    {
        this->cjonName = dataHeaderName;
        this->filePath = "Storage/" + this->cjonName + ".cjon";
        this->saveOnDecon = saveOnDeconstruct;

        loadData();
    }
    ~CJon()
    {
        if (this->saveOnDecon)
            saveData();
    }

    // KVP
    void addKVPair(const string &key, const string &value)
    {
        this->cjonMap[key] = value;
    }

    // IV
    void addValue(const string &value)
    {
        int iterator = this->cjonMap.size();
        this->cjonMap[to_string(iterator)] = value;
    }

    vector<string> getKeys()
    {
        vector<string> keys;

        for (const auto &pair : this->cjonMap)
        {
            keys.push_back(pair.first);
        }

        return keys;
    }

    vector<string> getValues()
    {
        vector<string> values;

        for (const auto &pair : this->cjonMap)
        {
            values.push_back(pair.second);
        }

        return values;
    }

private:
    map<string, string> cjonMap;
    string cjonName;
    string filePath;
    bool saveOnDecon = true;

    void saveData()
    {
        fs::create_directories(fs::path(this->filePath).parent_path());

        string jsonString = "{";
        for (const auto &pair : this->cjonMap)
        {
            jsonString += "\"" + pair.first + "\":\"" + pair.second + "\",";
        }
        jsonString.pop_back();
        jsonString += "}";

        ofstream file(this->filePath);
        file << jsonString;
        file.close();

        cout << "Data saved to " << this->filePath << endl;
    }

    void loadData()
    {
        ifstream file(this->filePath);
        if (!file.is_open())
        {
            cerr << "Error opening file: " << this->filePath << endl;
            return;
        }

        stringstream buffer;
        buffer << file.rdbuf();
        string jsonString = buffer.str();

        if (jsonString.empty() || jsonString == "{}")
        {
            cout << "File is empty or does not contain valid JSON data." << endl;
            return;
        }

        this->cjonMap.clear();
        size_t start = jsonString.find('{') + 1;
        size_t end = jsonString.rfind('}');
        string pairs = jsonString.substr(start, end - start);

        stringstream ss(pairs);
        string pair;
        while (getline(ss, pair, ','))
        {
            size_t colonPos = pair.find(':');
            string key = pair.substr(1, colonPos - 2);
            string value = pair.substr(colonPos + 2, pair.size() - colonPos - 3);
            this->cjonMap[key] = value;
        }

        cout << "Data loaded from " << this->filePath << endl;
    }
};