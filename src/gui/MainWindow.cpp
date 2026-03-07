#include "MainWindow.h"

#include <QComboBox>
#include <QFileInfo>
#include <QMenuBar>
#include <QStatusBar>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QLabel>
#include <QWidget>
//-----------------------
#include <QFileDialog>
#include <QMessageBox>
#include "../plink/PlinkLoader.h"

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
    // -------- File Menu --------
    QMenu *fileMenu = menuBar()->addMenu("File");

    QAction *loadAction = new QAction("Load PLINK Dataset", this);
    connect(loadAction, &QAction::triggered, this, &MainWindow::loadDataset);
    fileMenu->addAction(loadAction);

    // -------- QC Menu --------
    QMenu *qcMenu = menuBar()->addMenu("Quality Control");
    qcMenu->addAction("Run QC");

    // -------- LD Menu --------
    QMenu *ldMenu = menuBar()->addMenu("LD Pruning");
    ldMenu->addAction("Run LD Pruning");

    // -------- PCA Menu --------
    QMenu *pcaMenu = menuBar()->addMenu("PCA");
    pcaMenu->addAction("Compute PCA");

    // -------- Visualization Menu --------
    QMenu *visMenu = menuBar()->addMenu("Visualization");
    visMenu->addAction("Show PCA Plot");

    // -------- Export Menu --------
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

    logPanel = new QTextEdit;
    logPanel->setReadOnly(true);

    layout->addWidget(title);
    layout->addWidget(datasetNameLabel);
    layout->addWidget(sampleCountLabel);
    layout->addWidget(snpCountLabel);
    layout->addWidget(chromosomeLabel);
    layout->addWidget(refLabel);
    layout->addWidget(referenceGenomeDropdown);
    layout->addSpacing(20);
    layout->addWidget(logPanel);

    central->setLayout(layout);
    setCentralWidget(central);
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

    // ---- Update Dataset Summary Panel ----

    QString datasetName = QFileInfo(bedFile).baseName();

    datasetNameLabel->setText(
        QString("Dataset: %1").arg(datasetName)
    );

    sampleCountLabel->setText(
        QString("Samples: %1").arg(dataset.sampleCount)
    );

    snpCountLabel->setText(
        QString("SNPs: %1").arg(dataset.snpCount)
    );

    chromosomeLabel->setText("Chromosomes: Detecting...");

    // ---- Reset genome dropdown ----

    referenceGenomeDropdown->setCurrentIndex(0); // Unknown

    // ---- Log Output ----

    logPanel->append("Dataset loaded successfully.");
    logPanel->append(QString("Dataset: %1").arg(datasetName));
    logPanel->append(QString("Samples: %1").arg(dataset.sampleCount));
    logPanel->append(QString("SNPs: %1").arg(dataset.snpCount));
}