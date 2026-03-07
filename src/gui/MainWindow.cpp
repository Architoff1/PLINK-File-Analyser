#include "MainWindow.h"

#include <vector>
#include <algorithm>
#include <map>
#include <fstream>
#include <sstream>

#include <QFrame>
#include <QComboBox>
#include <QFileInfo>
#include <QMenuBar>
#include <QStatusBar>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QLabel>
#include <QWidget>
#include <QTableWidget>
#include <QHeaderView>

#include <QFileDialog>
#include <QMessageBox>

#include "../plink/PlinkLoader.h"
#include "../genome/GenomeDetector.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("PLINK PCA Analyzer");
    resize(1000, 700);

    createMenus();
    createWorkspace();

    statusBar()->showMessage("Ready");
}


void MainWindow::createMenus()
{
    QMenu *fileMenu = menuBar()->addMenu("File");

    QAction *loadAction = new QAction("Load PLINK Dataset", this);
    connect(loadAction, &QAction::triggered, this, &MainWindow::loadDataset);
    fileMenu->addAction(loadAction);

    QMenu *qcMenu = menuBar()->addMenu("Quality Control");
    qcMenu->addAction("Run QC");

    QMenu *ldMenu = menuBar()->addMenu("LD Pruning");
    ldMenu->addAction("Run LD Pruning");

    QMenu *pcaMenu = menuBar()->addMenu("PCA");
    pcaMenu->addAction("Compute PCA");

    QMenu *visMenu = menuBar()->addMenu("Visualization");
    visMenu->addAction("Show PCA Plot");

    QMenu *exportMenu = menuBar()->addMenu("Export");
    exportMenu->addAction("Export Results");
}


void MainWindow::createWorkspace()
{
    QWidget *central = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout;

    QLabel *title = new QLabel("Dataset Summary");
    title->setStyleSheet("font-weight: bold; font-size: 16px;");

    datasetNameLabel = new QLabel("Dataset: None");
    sampleCountLabel = new QLabel("Samples: 0");
    snpCountLabel = new QLabel("SNPs: 0");
    chromosomeLabel = new QLabel("Chromosomes: Unknown");

    QLabel *refLabel = new QLabel("Reference Genome:");

    referenceGenomeDropdown = new QComboBox;
    referenceGenomeDropdown->addItem("Unknown");
    referenceGenomeDropdown->addItem("GRCh37 / hg19");
    referenceGenomeDropdown->addItem("GRCh38 / hg38");
    referenceGenomeDropdown->addItem("T2T-CHM13");

    // Separator
    healthSeparator = new QFrame;
    healthSeparator->setFrameShape(QFrame::HLine);
    healthSeparator->setFrameShadow(QFrame::Sunken);
    healthSeparator->setStyleSheet("color:#555;");

    // ---- Dataset Health ----
    healthTitleLabel = new QLabel("Dataset Health");
    healthTitleLabel->setStyleSheet("font-weight:bold; font-size:16px;");

    missingChromosomesLabel = new QLabel("Missing Chromosomes: Unknown");
    sexChromosomeLabel = new QLabel("Sex Chromosomes: Unknown");
    mitochondrialLabel = new QLabel("Mitochondrial Variants: Unknown");
    assemblyConsistencyLabel = new QLabel("Genome Assembly: Unknown");
    datasetTypeLabel = new QLabel("Dataset Type: Unknown");

    // ---- Dataset Overview ----
    overviewTitleLabel = new QLabel("Dataset Overview");
    overviewTitleLabel->setStyleSheet("font-weight:bold; font-size:16px;");

    chromosomeTable = new QTableWidget;
    chromosomeTable->setColumnCount(3);

    QStringList headers;
    headers << "Chromosome" << "SNP Count" << "Density";

    chromosomeTable->setHorizontalHeaderLabels(headers);
    chromosomeTable->horizontalHeader()->setStretchLastSection(true);
    chromosomeTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    chromosomeTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    chromosomeTable->setSelectionMode(QAbstractItemView::NoSelection);

    logPanel = new QTextEdit;
    logPanel->setReadOnly(true);

    layout->addWidget(title);
    layout->addWidget(datasetNameLabel);
    layout->addWidget(sampleCountLabel);
    layout->addWidget(snpCountLabel);
    layout->addWidget(chromosomeLabel);

    layout->addWidget(refLabel);
    layout->addWidget(referenceGenomeDropdown);

    layout->addSpacing(10);
    layout->addWidget(healthSeparator);
    layout->addSpacing(10);

    layout->addWidget(healthTitleLabel);
    layout->addWidget(missingChromosomesLabel);
    layout->addWidget(sexChromosomeLabel);
    layout->addWidget(mitochondrialLabel);
    layout->addWidget(assemblyConsistencyLabel);
    layout->addWidget(datasetTypeLabel);

    layout->addSpacing(20);

    layout->addWidget(overviewTitleLabel);
    layout->addWidget(chromosomeTable);

    layout->addSpacing(20);
    layout->addWidget(logPanel);

    central->setLayout(layout);
    setCentralWidget(central);

    // Hide health + overview initially
    healthSeparator->hide();
    healthTitleLabel->hide();
    missingChromosomesLabel->hide();
    sexChromosomeLabel->hide();
    mitochondrialLabel->hide();
    assemblyConsistencyLabel->hide();
    datasetTypeLabel->hide();

    overviewTitleLabel->hide();
    chromosomeTable->hide();
}


void MainWindow::loadDataset()
{
    QString bedFile = QFileDialog::getOpenFileName(
        this,
        "Select PLINK BED file",
        "",
        "PLINK BED Files (*.bed)"
    );

    if (bedFile.isEmpty())
        return;

    if (!PlinkLoader::loadDataset(bedFile.toStdString(), dataset))
    {
        QMessageBox::critical(
            this,
            "Error",
            "Could not find matching .bim or .fam files."
        );
        return;
    }

    // Show panels
    healthSeparator->show();
    healthTitleLabel->show();
    missingChromosomesLabel->show();
    sexChromosomeLabel->show();
    mitochondrialLabel->show();
    assemblyConsistencyLabel->show();
    datasetTypeLabel->show();

    overviewTitleLabel->show();
    chromosomeTable->show();

    QString datasetName = QFileInfo(bedFile).baseName();

    datasetNameLabel->setText(QString("Dataset: %1").arg(datasetName));
    sampleCountLabel->setText(QString("Samples: %1").arg(dataset.sampleCount));
    snpCountLabel->setText(QString("SNPs: %1").arg(dataset.snpCount));

    // ---- Chromosome detection ----
    std::vector<int> numericChromosomes;
    bool hasX=false, hasY=false, hasMT=false;

    for(const auto &chr : dataset.chromosomes)
    {
        int c = std::stoi(chr);

        if(c>=1 && c<=22)
            numericChromosomes.push_back(c);
        else if(c==23)
            hasX=true;
        else if(c==24)
            hasY=true;
        else if(c==26)
            hasMT=true;
    }

    std::sort(numericChromosomes.begin(), numericChromosomes.end());

    QString chromosomeText="Chromosomes: ";

    int start=-1, prev=-1;

    for(int chr : numericChromosomes)
    {
        if(start==-1)
        {
            start=prev=chr;
            continue;
        }

        if(chr==prev+1)
        {
            prev=chr;
            continue;
        }

        if(start==prev)
            chromosomeText+=QString("%1, ").arg(start);
        else
            chromosomeText+=QString("%1–%2, ").arg(start).arg(prev);

        start=prev=chr;
    }

    if(start!=-1)
    {
        if(start==prev)
            chromosomeText+=QString("%1").arg(start);
        else
            chromosomeText+=QString("%1–%2").arg(start).arg(prev);
    }

    if(hasX) chromosomeText+=", X";
    if(hasY) chromosomeText+=", Y";
    if(hasMT) chromosomeText+=", MT";

    chromosomeLabel->setText(chromosomeText);

    // ---- Missing chromosomes ----
    std::vector<int> missing;

    for(int i=1;i<=22;i++)
        if(std::find(numericChromosomes.begin(),numericChromosomes.end(),i)==numericChromosomes.end())
            missing.push_back(i);

    QString missingText="Missing Chromosomes: ";
    if(missing.empty())
        missingText+="None";
    else
        for(int c:missing)
            missingText+=QString::number(c)+" ";

    missingChromosomesLabel->setText(missingText);

    if(hasX && hasY)
        sexChromosomeLabel->setText("Sex Chromosomes: X and Y");
    else if(hasX)
        sexChromosomeLabel->setText("Sex Chromosomes: X only");
    else if(hasY)
        sexChromosomeLabel->setText("Sex Chromosomes: Y only");
    else
        sexChromosomeLabel->setText("Sex Chromosomes: None");

    mitochondrialLabel->setText(
        hasMT ? "Mitochondrial Variants: Present"
              : "Mitochondrial Variants: None");

    if(hasMT && numericChromosomes.empty())
        datasetTypeLabel->setText("Dataset Type: Mitochondrial Only");
    else if(hasMT)
        datasetTypeLabel->setText("Dataset Type: Mixed Nuclear + Mitochondrial");
    else
        datasetTypeLabel->setText("Dataset Type: Nuclear Genome");

    // ---- Genome build detection ----
    std::string genomeBuild = GenomeDetector::inferGenomeBuild(dataset);
    QString detected = QString::fromStdString(genomeBuild);

    referenceGenomeDropdown->setCurrentIndex(0);
    int index = referenceGenomeDropdown->findText(detected);
    if(index!=-1)
        referenceGenomeDropdown->setCurrentIndex(index);

    assemblyConsistencyLabel->setText(
        detected=="Unknown" ?
        "Genome Assembly: Uncertain" :
        "Genome Assembly: Consistent");

    // ---- SNP counts per chromosome ----
    std::map<std::string,int> chromosomeCounts;

    std::ifstream bim(dataset.bimFile);
    std::string line,chr;

    while(std::getline(bim,line))
    {
        std::stringstream ss(line);
        ss>>chr;
        chromosomeCounts[chr]++;
    }

    chromosomeTable->setRowCount(chromosomeCounts.size());

    int row=0;

    for(const auto &entry:chromosomeCounts)
    {
        QString chromosome=QString::fromStdString(entry.first);
        int snpCount=entry.second;

        QString density;

        if(snpCount>30000)
            density="High";
        else if(snpCount>10000)
            density="Medium";
        else
            density="Low";

        chromosomeTable->setItem(row,0,new QTableWidgetItem(chromosome));
        chromosomeTable->setItem(row,1,new QTableWidgetItem(QString::number(snpCount)));
        chromosomeTable->setItem(row,2,new QTableWidgetItem(density));

        row++;
    }

    // ---- Logging ----
    logPanel->clear();

    logPanel->append("Dataset loaded successfully.");
    logPanel->append(QString("Dataset: %1").arg(datasetName));
    logPanel->append(QString("Samples: %1").arg(dataset.sampleCount));
    logPanel->append(QString("SNPs: %1").arg(dataset.snpCount));
    logPanel->append(QString("Genome build detected: %1").arg(detected));
}