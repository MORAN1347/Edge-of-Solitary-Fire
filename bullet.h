#ifndef BULLET_H
#define BULLET_H

#include <QPainter>

class Bullet
{
public:
    int x, y;
    int dir; // 1=右，-1=左

    Bullet(int x_, int y_, int d);
    void move();
    void draw(QPainter &p);
};

#endif // BULLET_H
