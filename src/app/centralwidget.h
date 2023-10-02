///
/// \package qtoy
///
/// \author Marian Krivos <marian.krivos@rsys.sk>
/// \date 9. 7. 2023 
/// \brief definicia typu
///
/// (C) Copyright 2023 R-SYS s.r.o
/// All rights reserved.
///


#pragma once

#include "mainwindow.h"
#include <QWidget>
#include <QPaintEvent>

class CentralWidget :
    public QWidget
{
    Q_OBJECT

public:
    CentralWidget(MainWindow *parent);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    MainWindow *parent;
};



