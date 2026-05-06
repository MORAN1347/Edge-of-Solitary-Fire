#include "bullet.h"

Bullet::Bullet(int x_, int y_, int d)
{
    x = x_;
    y = y_;
    dir = d;
}

void Bullet::move()
{
    x += dir * 10;
}

void Bullet::draw(QPainter &p)
{
    p.setBrush(Qt::yellow);
    p.drawEllipse(x, y, 8, 8);
}