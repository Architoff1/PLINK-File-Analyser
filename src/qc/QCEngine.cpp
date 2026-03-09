#include "QCEngine.h"

QCEngine::QCStats QCEngine::computeBlockStats(const GenotypeBlock& block)
{
    QCStats stats;

    int snps = block.getSnpCount();
    int samples = block.getSampleCount();

    for (int snp = 0; snp < snps; ++snp)
    {
        for (int sample = 0; sample < samples; ++sample)
        {
            uint8_t genotype = block.get(snp, sample);

            stats.totalGenotypes++;

            switch (genotype)
            {
                case 0:
                    stats.homozygousRef++;
                    break;

                case 1:
                    stats.heterozygous++;
                    break;

                case 2:
                    stats.homozygousAlt++;
                    break;

                case 3:
                    stats.missingGenotypes++;
                    break;
            }
        }
    }

    return stats;
}