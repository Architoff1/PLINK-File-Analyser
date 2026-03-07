#include "GenomeDetector.h"

#include <fstream>
#include <sstream>
#include <limits>

std::string GenomeDetector::inferGenomeBuild(const PlinkDataset& dataset)
{
    std::ifstream bim(dataset.bimFile);

    if (!bim.is_open())
        return "Unknown";

    std::string line;
    std::string chr;
    long position;

    long maxPosition = 0;

    while (std::getline(bim, line))
    {
        std::stringstream ss(line);

        ss >> chr;     // chromosome
        ss >> chr;     // SNP ID
        ss >> chr;     // genetic distance
        ss >> position;

        if (position > maxPosition)
            maxPosition = position;
    }

    bim.close();

    // ---- Simple genome build heuristic ----

    if (maxPosition > 248956000)
        return "GRCh37 / hg19";

    if (maxPosition > 240000000)
        return "GRCh38 / hg38";

    return "Unknown";
}