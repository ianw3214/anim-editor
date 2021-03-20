#pragma once

#include <string>

class Serializer
{
public:
    Serializer();

    bool ReadFromJSONFile(const std::string& file);
    bool WriteToFile(const std::string& file);
private:

};