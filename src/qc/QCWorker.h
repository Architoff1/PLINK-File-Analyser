#pragma once

#include <QObject>
#include "../plink/BedReader.h"
#include "../data/GenotypeBlock.h"
#include "QCEngine.h"

class QCWorker : public QObject
{
    Q_OBJECT

public:
    QCWorker(const std::string& bedFile,
             int samples,
             int snps);

signals:
    void progressUpdated(int percent);
    void finished(QCEngine::QCStats stats);

public slots:
    void run();

private:
    std::string bedFilePath;
    int sampleCount;
    int snpCount;
};