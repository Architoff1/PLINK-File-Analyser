#ifndef BEDREADER_H
#define BEDREADER_H

#include <fstream>
#include <string>
#include "data/GenotypeBlock.h"

class BedReader
{
public:
    BedReader();

    bool open(const std::string& path, int samples, int snps);

    bool readNextBlock(GenotypeBlock& block);

    bool isOpen() const;

private:
    std::ifstream bedFile;

    int sampleCount;
    int snpCount;

    int currentSnpIndex;

    int blockSize;
};

#endif