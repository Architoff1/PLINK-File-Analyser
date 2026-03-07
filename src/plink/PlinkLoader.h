#pragma once

#include <string>
#include <set>
struct PlinkDataset
{
    std::string bedFile;
    std::string bimFile;
    std::string famFile;

    int snpCount = 0;
    int sampleCount = 0;
    std::set<std::string> chromosomes;
};

class PlinkLoader
{
public:
    static bool loadDataset(const std::string &bedPath, PlinkDataset &dataset);
};