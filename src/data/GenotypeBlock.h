#ifndef GENOTYPEBLOCK_H
#define GENOTYPEBLOCK_H

#include <vector>
#include <cstdint>

class GenotypeBlock
{
public:
    GenotypeBlock();

    void resize(int snps, int samples);

    uint8_t get(int snp, int sample) const;
    void set(int snp, int sample, uint8_t value);

    int getSnpCount() const;
    int getSampleCount() const;

private:
    std::vector<uint8_t> data;

    int snpCount;
    int sampleCount;
};

#endif