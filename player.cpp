#include "player.h"

Player::Player(int x_, int y_, QColor c)
{
    x = x_;
    y = y_;
    color = c;
    hp = 100;
}

void Player::draw(QPainter &p)
{
    p.setBrush(color);
    p.drawRect(x, y, 50, 50);  // 用矩形代表弓箭手
}