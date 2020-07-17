#include "confighandler.h"

#include <experimental/filesystem>
#include <fstream>
#include <unordered_map>

using namespace ConfigurationHandler;

using keyValueVector = std::vector<std::pair<std::string, std::string>>;

std::string getConfigFilePath()
{
    std::experimental::filesystem::path p = std::experimental::filesystem::current_path();
    p.append("config");
    p.append("config.txt");
    return p.string();
}

keyValueVector readConfigFile()
{
    keyValueVector configuration;

    std::string key, value;
    std::ifstream infile(getConfigFilePath());

    std::string line;
    while (std::getline(infile, line))
    {
        std::istringstream iss(line);

        // trenne die Line an den ":"
        std::string segment;
        std::vector<std::string> seglist;
        while(std::getline(iss, segment, ':'))
        {
            seglist.push_back(segment);
        }

        // füge die getrennten Einträge nur hinzu, wenn sie durch genau 1 ":" getrennt wurden und entferne Leerzeichen
        if (seglist.size() == 2)
        {
            auto key = seglist[0];
            auto value = seglist[1];
            key.erase(std::remove_if(key.begin(), key.end(), ::isspace), key.end());
            value.erase(std::remove_if(value.begin(), value.end(), ::isspace), value.end());
            configuration.push_back(std::pair<std::string, std::string>(key, value));
        }
        else {
            // es wird sich um ein Kommentar handeln. Schreibe das auch mit auf.
            configuration.push_back(std::pair<std::string, std::string>(line, ""));
        }
    }

    return configuration;
}

std::string getValueFromKey(const std::string& key, const keyValueVector& vec)
{
    auto it = std::find_if(vec.begin(), vec.end(),
                           [&key](const std::pair<std::string, std::string>& pair) { return pair.first == key; });

    if(it != vec.end())
        return it->second;
    return "";
}

void setValueFromKey(const std::string& key, const std::string& value, keyValueVector& vec)
{
    auto it = std::find_if(vec.begin(), vec.end(),
                           [&key](const std::pair<std::string, std::string>& pair) { return pair.first == key; });

    if(it != vec.end())
        it->second = value;
}

ConfigurationHandler::Configuration ConfigurationHandler::getConfiguration()
{
    Configuration configuration;

    auto configMap = readConfigFile();

    configuration.CURRENT_YEAR =    std::stoi(getValueFromKey("CurrentYear", configMap));
    configuration.CURRENT_DATABASE = getValueFromKey("CurrentDatabase", configMap);

    for (const auto& entry : configMap)
    {
        if (entry.first.rfind("Database") == 0)
        {
            configuration.AVAILABLE_DATABASES.push_back(entry.second);
        }
    }

    return configuration;
}

void ConfigurationHandler::setConfiguration(const Configuration &configuration)
{
    // --- read the original settings (to write also non used setting back to file)
    auto originalSettings = readConfigFile();

    // --- override the original settings
    setValueFromKey("CurrentDatabase", configuration.CURRENT_DATABASE, originalSettings);
    setValueFromKey("CurrentYear", std::to_string(configuration.CURRENT_YEAR), originalSettings);

    int cnt = 1;
    for (const auto& availableDatabase : configuration.AVAILABLE_DATABASES)
    {
        std::string key = "Database" + std::to_string(cnt);
        setValueFromKey(key, availableDatabase, originalSettings);
        cnt++;
    }

    // --- write the settings back to file
    std::ofstream file (getConfigFilePath());
    if (file.is_open())
    {
        for(const auto& item : originalSettings)
        {
            if (item.second != "")
                file << item.first << " : " << item.second << std::endl;
            else
                file << item.first << std::endl;
        }
        file.close();
    }
}
