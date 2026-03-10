#include "QCWorker.h"
#include "QCParallelEngine.h"

QCWorker::QCWorker(const std::string& bedFile,
                   int samples,
                   int snps)
    : bedFilePath(bedFile),
      sampleCount(samples),
      snpCount(snps)
{
}

void QCWorker::run()
{
    BedReader reader;
    reader.open(bedFilePath, sampleCount, snpCount);

    std::vector<GenotypeBlock> blocks;

    GenotypeBlock block;

    int processedSnps = 0;

    while (reader.readNextBlock(block))
    {
        blocks.push_back(block);

        processedSnps += block.getSnpCount();

        int progress = (processedSnps * 100) / snpCount;

        emit progressUpdated(progress);
    }

    auto totalStats = QCParallelEngine::computeParallelStats(blocks);

    emit finished(totalStats);
}