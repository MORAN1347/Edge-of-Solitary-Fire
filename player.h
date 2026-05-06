#ifndef PLAYER_H
#define PLAYER_H

#include <QPainter>
#include <QColor>

class Player
{
public:
    int x, y;
    int hp;
    QColor color;

    Player(int x_, int y_, QColor c);
    void draw(QPainter &p);
};

#endif // PLAYER_H
