#pragma once

#include <vector>
#include <string>

namespace ConfigurationHandler {

struct Configuration
{
    std::string              CURRENT_DATABASE{""};
    int                      CURRENT_YEAR{0};
    std::vector<std::string> AVAILABLE_DATABASES;
};

Configuration getConfiguration();

void setConfiguration(const Configuration& configuration);

} // ConfigurationHandler
