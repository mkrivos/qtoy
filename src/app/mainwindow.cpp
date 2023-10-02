#include "mainwindow.h"

#include "centralwidget.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->scaleSpinBox->setValue(4);
    ui->functionSpinBox->setValue(3);

    resize(1200, 800);
    setCentralWidget(new CentralWidget(this));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}

int MainWindow::getScale()
{
    return ui->scaleSpinBox->value();
}

bool MainWindow::getParallelRun()
{
    return ui->parallelCheckBox->checkState();
}

int MainWindow::getFunction()
{
    return ui->functionSpinBox->value();
}

float MainWindow::getGamma()
{
    return ui->gammaDoubleSpinBox->value();
}
float MainWindow::getWeight()
{
    return ui->weightDoubleSpinBox->value();
}

int MainWindow::getFontSize()
{
    return ui->sizeSpinBox->value();
}

std::array<float, 3> MainWindow::getParams()
{
    std::array<float, 3> params;

    params[0] = ui->t0DoubleSpinBox->value();
    params[1] = ui->t1DoubleSpinBox->value();
    params[2] = 0;

    return params;
}
