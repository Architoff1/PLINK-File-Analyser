#pragma once

#include <string>
#include "../plink/PlinkLoader.h"

class GenomeDetector
{
public:
    static std::string inferGenomeBuild(const PlinkDataset& dataset);
};