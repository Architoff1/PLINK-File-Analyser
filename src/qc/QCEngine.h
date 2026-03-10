#pragma once

#include <cstdint>
#include "../data/GenotypeBlock.h"

class QCEngine
{
public:
    struct QCStats
    {
        uint64_t totalGenotypes = 0;
        uint64_t missingGenotypes = 0;
        uint64_t homozygousRef = 0;
        uint64_t heterozygous = 0;
        uint64_t homozygousAlt = 0;
    };

    static QCStats computeBlockStats(const GenotypeBlock& block);

    static void accumulateStats(QCStats& total, const QCStats& block);

};