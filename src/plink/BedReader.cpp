#include "BedReader.h"
#include <iostream>

BedReader::BedReader()
    : sampleCount(0),
      snpCount(0),
      currentSnpIndex(0),
      blockSize(4096)
{
}

bool BedReader::open(const std::string& path, int samples, int snps)
{
    sampleCount = samples;
    snpCount = snps;
    currentSnpIndex = 0;

    bedFile.open(path, std::ios::binary);

    if (!bedFile)
    {
        std::cerr << "Failed to open BED file\n";
        return false;
    }

    // Skip PLINK header (3 bytes)
    bedFile.seekg(3);

    return true;
}

bool BedReader::readNextBlock(GenotypeBlock& block)
{
    if (!bedFile || currentSnpIndex >= snpCount)
        return false;

    int snpsToRead = std::min(blockSize, snpCount - currentSnpIndex);

    block.resize(snpsToRead, sampleCount);

    int bytesPerSnp = (sampleCount + 3) / 4;

    for (int snp = 0; snp < snpsToRead; ++snp)
    {
        for (int byteIndex = 0; byteIndex < bytesPerSnp; ++byteIndex)
        {
            unsigned char byte;
            bedFile.read(reinterpret_cast<char*>(&byte), 1);

            for (int bit = 0; bit < 4; ++bit)
            {
                int sample = byteIndex * 4 + bit;

                if (sample >= sampleCount)
                    break;

                unsigned char twoBits = (byte >> (bit * 2)) & 0b11;

                uint8_t genotype;

                switch (twoBits)
                {
                    case 0b00: genotype = 0; break;
                    case 0b10: genotype = 1; break;
                    case 0b11: genotype = 2; break;
                    case 0b01: genotype = 3; break;
                    default: genotype = 3;
                }

                block.set(snp, sample, genotype);
            }
        }
    }

    currentSnpIndex += snpsToRead;

    return true;
}

bool BedReader::isOpen() const
{
    return bedFile.is_open();
}