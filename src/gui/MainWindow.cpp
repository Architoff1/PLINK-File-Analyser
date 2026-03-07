#include "MainWindow.h"

#include <QMenuBar>
#include <QStatusBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("PLINK PCA Analyzer");
    resize(1000, 700);

    createMenus();

    statusBar()->showMessage("Ready");
}

void MainWindow::createMenus()
{
    QMenu *fileMenu = menuBar()->addMenu("File");
    fileMenu->addAction("Load PLINK Dataset");

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