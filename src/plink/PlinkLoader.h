#pragma once

#include <string>

struct PlinkDataset
{
    std::string bedFile;
    std::string bimFile;
    std::string famFile;

    int snpCount = 0;
    int sampleCount = 0;
};

class PlinkLoader
{
public:
    static bool loadDataset(const std::string &bedPath, PlinkDataset &dataset);
};