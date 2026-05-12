#ifndef BULLET_H
#define BULLET_H

#include <QPainter>
#include <QList>
#include <QPoint>

class Bullet
{
public:
    int x, y;
    int dir; // 1=右，-1=左
    bool isHit=false;
    Bullet(int x_, int y_, int d);
    void move();
    void draw(QPainter &p);
private:
    QList<QPoint> trail;
};

#endif // BULLET_H
