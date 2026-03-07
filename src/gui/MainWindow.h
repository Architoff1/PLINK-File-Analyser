#pragma once

#include <QMainWindow>

#include "../plink/PlinkLoader.h"

class QTextEdit;
class QLabel;

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
    QLabel *datasetInfo;

    PlinkDataset dataset;
};