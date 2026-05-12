#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <QWidget>
#include <QKeyEvent>
#include <QTimer>
#include <QPainter>
#include <QPixmap>
#include <QMessageBox>
#include <QUrl>
#include <QList>
#include "player.h"
#include "bullet.h"

class GameScene : public QWidget
{
    Q_OBJECT
public:
    explicit GameScene(QWidget *parent = nullptr, bool isPVP = true);
    ~GameScene() override;

signals:
    // 关键：游戏结束时，发送信号，通知主界面
    void gameFinished();

protected:
    void paintEvent(QPaintEvent *) override;
    void keyPressEvent(QKeyEvent *) override;
    void keyReleaseEvent(QKeyEvent *) override;

private slots:
    void gameLoop();  // 游戏主循环
    void aiLogic();    // 人机AI逻辑
    void showGameOver(bool p1Win);

private:
    Player *player1;
    Player *player2;
    QList<Bullet*> bullets;
    QTimer *timer;
    bool isPVPMode;

    bool keyW, keyS, keyA, keyD, keySpace;  // 玩家1按键
    bool keyUp, keyDown, keyLeft, keyRight, keyEnter; // 玩家2按键

    QPixmap bgPixmap;
};

#endif // GAMESCENE_H
