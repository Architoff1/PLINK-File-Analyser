#pragma once

#include "QCEngine.h"
#include "../data/GenotypeBlock.h"

#include <vector>
#include <thread>
#include <mutex>

class QCParallelEngine
{
public:

    static QCEngine::QCStats computeParallelStats(
            const std::vector<GenotypeBlock>& blocks);

private:

    static void processBlocks(
            const std::vector<GenotypeBlock>& blocks,
            size_t start,
            size_t end,
            QCEngine::QCStats& stats,
            std::mutex& mergeMutex);
};