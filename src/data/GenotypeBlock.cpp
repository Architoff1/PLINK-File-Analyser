#include "GenotypeBlock.h"

GenotypeBlock::GenotypeBlock()
    : snpCount(0), sampleCount(0)
{
}

void GenotypeBlock::resize(int snps, int samples)
{
    snpCount = snps;
    sampleCount = samples;

    data.resize(snps * samples);
}

uint8_t GenotypeBlock::get(int snp, int sample) const
{
    return data[snp * sampleCount + sample];
}

void GenotypeBlock::set(int snp, int sample, uint8_t value)
{
    data[snp * sampleCount + sample] = value;
}

int GenotypeBlock::getSnpCount() const
{
    return snpCount;
}

int GenotypeBlock::getSampleCount() const
{
    return sampleCount;
}