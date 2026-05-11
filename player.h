#ifndef PLAYER_H
#define PLAYER_H

#include <QPainter>
#include <QPixmap>

class Player
{
public:
    // 改这里：多加一个参数 isPlayer1 区分玩家1/2
    Player(int x_, int y_, QColor c, bool isPlayer1);

    void draw(QPainter &p);

    int x, y;
    int hp;
    QColor color;

private:
    QPixmap pix; // 人物图片
};

#endif // PLAYER_H
