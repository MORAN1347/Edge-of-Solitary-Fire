#include "player.h"
#include <QImage>

Player::Player(int x_, int y_, QColor c, bool isPlayer1)
{
    x = x_;
    y = y_;
    color = c;
    hp = 100;

    QImage img;

    // 1. 加载图片并强制转成带透明通道的格式
    if(isPlayer1) {
        img.load(":/t.png");
    } else {
        img.load(":/ct.png");
    }
    // 关键：转换为带alpha通道的格式，才能真正支持透明
    img = img.convertToFormat(QImage::Format_ARGB32);

    // 2. 把所有接近白色的像素，设为完全透明
    for (int y = 0; y < img.height(); y++) {
        for (int x = 0; x < img.width(); x++) {
            QRgb pixel = img.pixel(x, y);
            int r = qRed(pixel);
            int g = qGreen(pixel);
            int b = qBlue(pixel);

            // 调整阈值，把接近白色的都设为透明
            if (r > 230 && g > 230 && b > 230) {
                img.setPixel(x, y, qRgba(0, 0, 0, 0)); // 全透明
            }
        }
    }

    // 3. 转成QPixmap并缩放
    pix = QPixmap::fromImage(img);
    pix = pix.scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation);
}

void Player::draw(QPainter &p)
{
    // 启用抗锯齿，边缘更柔和
    p.setRenderHint(QPainter::Antialiasing);
    p.drawPixmap(x, y, pix);
}