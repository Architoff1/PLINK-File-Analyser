#include "PlinkLoader.h"

#include <fstream>
#include <filesystem>
#include <sstream>
#include <set>

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

    // ---- Count Samples (.fam file) ----
    std::ifstream fam(dataset.famFile);
    std::string line;

    dataset.sampleCount = 0;

    while (std::getline(fam, line))
        dataset.sampleCount++;

    fam.close();

    // ---- Read SNPs and Chromosomes (.bim file) ----
    std::ifstream bim(dataset.bimFile);

    dataset.snpCount = 0;
    dataset.chromosomes.clear();

    std::string chromosome;

    while (std::getline(bim, line))
    {
        std::stringstream ss(line);

        ss >> chromosome;

        dataset.chromosomes.insert(chromosome);

        dataset.snpCount++;
    }

    bim.close();

    return true;
}