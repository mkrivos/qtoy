#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <array>
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow :
    public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void on_actionExit_triggered();

    int getScale();
    bool getParallelRun();
    int getFunction();
    float getGamma();
    float getWeight();
    int getFontSize();

    std::array<float, 3> getParams();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
