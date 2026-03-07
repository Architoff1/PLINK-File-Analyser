#include "PlinkLoader.h"

#include <fstream>
#include <filesystem>

bool PlinkLoader::loadDataset(const std::string &bedPath, PlinkDataset &dataset)
{
    std::filesystem::path bed(bedPath);

    std::string prefix = bed.replace_extension("").string();

    dataset.bedFile = prefix + ".bed";
    dataset.bimFile = prefix + ".bim";
    dataset.famFile = prefix + ".fam";

    if (!std::filesystem::exists(dataset.bimFile))
        return false;

    if (!std::filesystem::exists(dataset.famFile))
        return false;

    std::ifstream fam(dataset.famFile);
    std::string line;

    while (std::getline(fam, line))
        dataset.sampleCount++;

    std::ifstream bim(dataset.bimFile);

    while (std::getline(bim, line))
        dataset.snpCount++;

    return true;
}