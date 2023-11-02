#pragma once

#pragma region INCLUDES/NAMESPACE
#include <iostream>
#include <fstream>
#include <map>
#include <filesystem>

using std::ifstream;
using std::map;
using std::string;
using std::vector;
namespace fs = std::filesystem;
#pragma endregion

/// @brief CJon is a lightweight variant of JSON, utilizing maps.
/// @author Caleb Brodock
/// @version 1
/// @date 10/18/2023
class CJon
{
public:
#pragma region CONSTRUCTORS
    /// @brief Constructor for a CJon object.
    /// @param dataHeaderName Header name for the data. File containing this data will save saved as this.
    /// @param saveOnDeconstruct OPTIONAL if the file should save on deconstruction. Defaults to true, useful if you want to quickly access data without writing the file.
    /// @example CJon myData = CJon("myData");
    /// @author Caleb Brodock
    /// @version 1
    /// @date 10/18/2023
    CJon(const string &dataHeaderName, const bool &saveOnDeconstruct = true)
    {
        this->cjonName = dataHeaderName;
        this->filePath = "Storage/" + this->cjonName + ".cjon";
        this->saveOnDecon = saveOnDeconstruct;

        loadData();
    }

    /// @brief Deconstructor for a CJon object. Simply saves the data if save on deconstruction is on.
    /// @author Caleb Brodock
    /// @version 1
    /// @date 10/18/2023
    ~CJon()
    {
        if (this->saveOnDecon)
            saveData();
    }
#pragma endregion

#pragma region ADD
    /// @brief Add a Key Value Pair to the CJon.
    /// @param key
    /// @param value
    /// @author Caleb Brodock
    /// @version 1
    /// @date 10/18/2023
    void addKVPair(const string &key, const string &value)
    {
        this->cjonMap[key] = value;
    }

    /// @brief Add a iterating value to the CJon.
    /// @param value
    /// @author Caleb Brodock
    /// @version 1
    /// @date 10/18/2023
    /// @deprecated
    void addValue(const string &value)
    {
        int iterator = this->cjonMap.size();
        this->cjonMap[std::to_string(iterator)] = value;
    }

    /// @brief Add a iterating value to the CJon.
    /// @param value
    /// @author Caleb Brodock
    /// @version 2
    /// @date 11/1/2023
    void addValueV2(const string &value)
    {
        int iterator = 0;

        if (this->cjonMap.size() != 0)
        {
            try
            {
                int lastMember_iterator = std::stoi(this->cjonMap.rbegin()->first) + 1;

                iterator = lastMember_iterator;
            }
            catch (const std::invalid_argument &ex)
            {
                iterator = this->cjonMap.size();
            }
            catch (const std::out_of_range &ex)
            {
                iterator = this->cjonMap.size();
            }
        }
        else
        {
            iterator = 0;
        }

        this->cjonMap[std::to_string(iterator)] = value;
    }
#pragma endregion

#pragma region REMOVE
    /// @brief Remove a specific value within the CJon
    /// @author Caleb Brodock
    /// @version 2
    /// @date 11/1/2023
    void removeValue(const string &value)
    {
        auto it = this->cjonMap.begin();
        while (it != this->cjonMap.end())
        {
            if (it->second == value)
            {
                it = this->cjonMap.erase(it);
                return;
            }

            ++it;
        }
    }
#pragma endregion

#pragma region GET
    /// @brief Get all keys in the CJon
    /// @return CJon Keys
    /// @author Caleb Brodock
    /// @version 1
    /// @date 10/18/2023
    vector<string> getKeys()
    {
        vector<string> keys;

        for (const auto &pair : this->cjonMap)
        {
            keys.push_back(pair.first);
        }

        return keys;
    }

    /// @brief Get all values in the CJon
    /// @return CJon Values
    /// @author Caleb Brodock
    /// @version 1
    /// @date 10/18/2023
    vector<string> getValues()
    {
        vector<string> values;

        for (const auto &pair : this->cjonMap)
        {
            values.push_back(pair.second);
        }

        return values;
    }
#pragma endregion

private:
#pragma region VARIABLES
    map<string, string> cjonMap;
    string cjonName;
    string filePath;
    bool saveOnDecon = true;
#pragma endregion

#pragma region SAVE/LOAD
    /// @brief Save the current CJon data.
    /// @author Caleb Brodock
    /// @version 1
    /// @date 10/18/2023
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

        std::ofstream file(this->filePath);
        file << jsonString;
        file.close();

        std::cout << "Data saved to " << this->filePath << std::endl;
    }

    /// @brief Load the current CJon data.
    /// @param saveFirst OPTIONAL : default is false. If true, it'll save the current data before reloading the data.
    /// @author Caleb Brodock
    /// @version 1
    /// @date 10/18/2023
    void loadData(bool saveFirst = false)
    {
        if (saveFirst)
            saveData();

        ifstream file(this->filePath);
        if (!file.is_open())
        {
            std::cerr << "Error opening file: " << this->filePath << std::endl;
            return;
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        string jsonString = buffer.str();

        if (jsonString.empty() || jsonString == "{}")
        {
            std::cout << "File is empty or does not contain valid JSON data." << std::endl;
            return;
        }

        this->cjonMap.clear();
        size_t start = jsonString.find('{') + 1;
        size_t end = jsonString.rfind('}');
        string pairs = jsonString.substr(start, end - start);

        std::stringstream ss(pairs);
        string pair;
        while (std::getline(ss, pair, ','))
        {
            size_t colonPos = pair.find(':');
            string key = pair.substr(1, colonPos - 2);
            string value = pair.substr(colonPos + 2, pair.size() - colonPos - 3);
            this->cjonMap[key] = value;
        }

        // std::cout << "Data loaded from " << this->filePath << std::endl;
    }
#pragma endregion
};