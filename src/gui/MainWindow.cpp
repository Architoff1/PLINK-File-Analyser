#include "MainWindow.h"

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

    datasetInfo = new QLabel("No dataset loaded.");
    datasetInfo->setStyleSheet("font-weight: bold;");

    logPanel = new QTextEdit;
    logPanel->setReadOnly(true);

    layout->addWidget(datasetInfo);
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
        QMessageBox::critical(this, "Error",
                              "Could not find matching .bim or .fam files.");
        return;
    }

    datasetInfo->setText(
        QString("Dataset Loaded\nSamples: %1\nSNPs: %2")
            .arg(dataset.sampleCount)
            .arg(dataset.snpCount)
    );

    logPanel->append("Dataset loaded successfully.");
    logPanel->append(QString("Samples: %1").arg(dataset.sampleCount));
    logPanel->append(QString("SNPs: %1").arg(dataset.snpCount));
}