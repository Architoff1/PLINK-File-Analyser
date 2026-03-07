#pragma once

#include <QMainWindow>
#include "../plink/PlinkLoader.h"

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

    QTextEdit *logPanel;

    QLabel *datasetNameLabel;
    QLabel *sampleCountLabel;
    QLabel *snpCountLabel;
    QLabel *chromosomeLabel;

    QComboBox *referenceGenomeDropdown;

    PlinkDataset dataset;
};