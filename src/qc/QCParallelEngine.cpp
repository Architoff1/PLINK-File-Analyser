#include "QCParallelEngine.h"

QCEngine::QCStats QCParallelEngine::computeParallelStats(
        const std::vector<GenotypeBlock>& blocks)
{
    QCEngine::QCStats globalStats;

    if (blocks.empty())
        return globalStats;

    unsigned int threadCount = std::thread::hardware_concurrency();

    if (threadCount == 0)
        threadCount = 4;

    size_t totalBlocks = blocks.size();
    size_t blocksPerThread = totalBlocks / threadCount;

    std::vector<std::thread> threads;
    std::mutex mergeMutex;

    for (unsigned int t = 0; t < threadCount; ++t)
    {
        size_t start = t * blocksPerThread;
        size_t end = (t == threadCount - 1)
                     ? totalBlocks
                     : start + blocksPerThread;

        threads.emplace_back(
            processBlocks,
            std::ref(blocks),
            start,
            end,
            std::ref(globalStats),
            std::ref(mergeMutex)
        );
    }

    for (auto &thread : threads)
        thread.join();

    return globalStats;
}


void QCParallelEngine::processBlocks(
        const std::vector<GenotypeBlock>& blocks,
        size_t start,
        size_t end,
        QCEngine::QCStats& globalStats,
        std::mutex& mergeMutex)
{
    QCEngine::QCStats localStats;

    for (size_t i = start; i < end; ++i)
    {
        auto stats = QCEngine::computeBlockStats(blocks[i]);
        QCEngine::accumulateStats(localStats, stats);
    }

    std::lock_guard<std::mutex> lock(mergeMutex);
    QCEngine::accumulateStats(globalStats, localStats);
}