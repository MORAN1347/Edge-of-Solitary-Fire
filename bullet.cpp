#include "bullet.h"
#include <QPainter>
#include <QRadialGradient>

Bullet::Bullet(int x_, int y_, int d)
{
    x = x_;
    y = y_;
    dir = d;
}

void Bullet::move()
{
    // 移动前先记录当前位置作为轨迹点
    trail.append(QPoint(x, y));
    // 限制轨迹长度，避免太长
    if(trail.size() > 5) {
        trail.removeFirst();
    }

    // 子弹移动
    x += dir * 10;
}

void Bullet::draw(QPainter &p)
{
    // 先画轨迹（半透明渐变，越靠近子弹越亮）
    for(int i=0; i<trail.size(); i++) {
        // 透明度和大小随轨迹点动态变化，越新的点越亮越大
        int alpha = 50 + i * 30; // 50, 80, 110, 140, 170
        int size = 2 + i;        // 2, 3, 4, 5, 6
        p.setBrush(QColor(255, 150, 0, alpha));
        p.setPen(Qt::NoPen);
        p.drawEllipse(trail[i].x() - size/2, trail[i].y() - size/2, size, size);
    }

    // 1. 绘制子弹主体（渐变填充）
    QRadialGradient gradient(x, y, 8);
    gradient.setColorAt(0, Qt::yellow);    // 中心亮黄
    gradient.setColorAt(0.7,QColor(255, 165, 0));  // 中间橙色
    gradient.setColorAt(1, Qt::transparent); // 边缘透明
    p.setBrush(gradient);
    p.setPen(Qt::NoPen);
    p.drawEllipse(x-4, y-4, 8, 8); // 8x8的圆形子弹

    // 2. 加一点拖尾效果
    p.setBrush(QColor(255, 150, 0, 100)); // 半透明橙色
    if(dir == 1) {
        p.drawEllipse(x-10, y-2, 6, 4); // 向右飞行的拖尾
    } else {
        p.drawEllipse(x+4, y-2, 6, 4);  // 向左飞行的拖尾
    }
}