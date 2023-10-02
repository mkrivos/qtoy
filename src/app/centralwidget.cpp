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

#include "centralwidget.h"
#include "shaders/shaders.h"

#include "glm/vec4.hpp"
#include "glm/vec2.hpp"

#include <QTimer>
#include <QPainter>
#include <QImage>
#include <QWidget>

CentralWidget::CentralWidget(MainWindow *parent)
    : QWidget(parent),
      parent{parent}
{
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, QOverload<>::of(&CentralWidget::update));
    timer->start(1000);
}

void CentralWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    int w = width()&(~3);
    int h = height();
#if VERBOSE
    w = 16;
    h = 16;
#endif
    int count = w*h;
    int scale = parent->getScale();
    auto f = QImage::Format::Format_RGBA32FPx4;

    QImage qimage(w, h, f);

    auto p = parent->getParams();

    static QImage serif(":/resources/glyph33.png");

    ShaderParameters params =
    {
        reinterpret_cast<glm::vec4*>(qimage.bits()),
        glm::vec2(w,h),
        reinterpret_cast<uint8_t*>(serif.bits()),
        glm::vec2(serif.width(), serif.height()),

        glm::vec3(p[0], p[1], p[2]),
        parent->getFunction(),
        parent->getGamma(),
        parent->getWeight(),
        parent->getFontSize(),
        parent->getParallelRun()
    };

    computeFrame(params);

    painter.drawImage(
        QRectF(0, 0, w*scale, h*scale),
        qimage
    );
}
