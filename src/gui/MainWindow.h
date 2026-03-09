#pragma once

#include <QMainWindow>
#include <QFrame>
#include <QTableWidget>

#include "../plink/PlinkLoader.h"
#include "../plink/BedReader.h"
#include "../data/GenotypeBlock.h"

class QTextEdit;
class QLabel;
class QComboBox;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private:
    void createMenus();
    void createWorkspace();
    void loadDataset();   // SLOT for menu action

    // ---- Log Panel ----
    QTextEdit *logPanel;

    // ---- Dataset Summary ----
    QLabel *datasetNameLabel;
    QLabel *sampleCountLabel;
    QLabel *snpCountLabel;
    QLabel *chromosomeLabel;

    // ---- Reference Genome ----
    QComboBox *referenceGenomeDropdown;

    // ---- Dataset Health ----
    QFrame *healthSeparator;
    QLabel *healthTitleLabel;
    QLabel *missingChromosomesLabel;
    QLabel *sexChromosomeLabel;
    QLabel *mitochondrialLabel;
    QLabel *assemblyConsistencyLabel;
    QLabel *datasetTypeLabel;

    // ---- Dataset Overview Table ----
    QLabel *overviewTitleLabel;
    QTableWidget *chromosomeTable;

    // ---- Dataset Storage ----
    PlinkDataset dataset;

    // ---- Genotype Streaming ----
    BedReader bedReader;
    GenotypeBlock genotypeBlock;
};