#pragma once

#include "../data/GenotypeBlock.h"

class QCEngine
{
public:
    struct QCStats
    {
        int totalGenotypes = 0;
        int missingGenotypes = 0;
        int homozygousRef = 0;
        int heterozygous = 0;
        int homozygousAlt = 0;
    };

    static QCStats computeBlockStats(const GenotypeBlock& block);
};