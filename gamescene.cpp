
#include "gamescene.h"
#include "player.h"
#include "bullet.h"
#include <QKeyEvent>
#include <QMessageBox>

GameScene::GameScene(QWidget *parent, bool isPVP)
    : QWidget(parent), isPVPMode(isPVP)
{
    this->setFixedSize(800, 400);
    this->setWindowTitle("战斗中");

    bgPixmap.load(":/bg.png");

    // 初始化玩家
    player1 = new Player(50, height()/2 - 25, Qt::blue);
    player2 = new Player(700, height()/2 - 25, Qt::red);

    // 按键初始化
    keyW=keyS=keyA=keyD=keySpace = false;
    keyUp=keyDown=keyLeft=keyRight=keyEnter = false;

    // 游戏循环定时器 30ms 刷新一次
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &GameScene::gameLoop);
    timer->start(30);

    // 人机模式启动AI
    if(!isPVPMode){
        QTimer *aiTimer = new QTimer(this);
        connect(aiTimer, &QTimer::timeout, this, &GameScene::aiLogic);
        aiTimer->start(800);
    }
}

void GameScene::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    if(!bgPixmap.isNull()){
        p.drawPixmap(0, 0, width(), height(), bgPixmap);
    } else {
        p.fillRect(rect(), Qt::lightGray); // 没背景图就用灰色
    }

    // 画玩家
    player1->draw(p);
    player2->draw(p);

    // 画子弹
    for(Bullet *b : bullets){
        b->draw(p);
    }

    // 玩家1血量条
    p.setBrush(Qt::darkRed);
    p.drawRect(20, 20, 100, 15);
    p.setBrush(Qt::green);
    p.drawRect(20, 20, player1->hp, 15);
    p.setPen(Qt::white);
    p.drawText(25, 32, "玩家1");

    // 玩家2血量条
    p.setBrush(Qt::darkRed);
    p.drawRect(width()-120, 20, 100, 15);
    p.setBrush(Qt::green);
    p.drawRect(width()-120, 20, player2->hp, 15);
    p.setPen(Qt::white);
    p.drawText(width()-115, 32, "玩家2");
}

void GameScene::keyPressEvent(QKeyEvent *e)
{
    // 玩家1：W S A D 移动，空格射击
    if(e->key() == Qt::Key_W) keyW = true;
    if(e->key() == Qt::Key_S) keyS = true;
    if(e->key() == Qt::Key_A) keyA = true;
    if(e->key() == Qt::Key_D) keyD = true;
    if(e->key() == Qt::Key_Space && !keySpace){
        keySpace = true;
        bullets.append(new Bullet(player1->x+30, player1->y+20, 1));

    }

    // 玩家2：方向键移动，回车射击（双人模式才启用）
    if(isPVPMode){
        if(e->key() == Qt::Key_Up) keyUp = true;
        if(e->key() == Qt::Key_Down) keyDown = true;
        if(e->key() == Qt::Key_Left) keyLeft = true;
        if(e->key() == Qt::Key_Right) keyRight = true;
        if(e->key() == Qt::Key_Return && !keyEnter){
            keyEnter = true;
            bullets.append(new Bullet(player2->x-10, player2->y+20, -1));
        }
    }
}

void GameScene::keyReleaseEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_W) keyW = false;
    if(e->key() == Qt::Key_S) keyS = false;
    if(e->key() == Qt::Key_A) keyA = false;
    if(e->key() == Qt::Key_D) keyD = false;
    if(e->key() == Qt::Key_Space) keySpace = false;

    if(e->key() == Qt::Key_Up) keyUp = false;
    if(e->key() == Qt::Key_Down) keyDown = false;
    if(e->key() == Qt::Key_Left) keyLeft = false;
    if(e->key() == Qt::Key_Right) keyRight = false;
    if(e->key() == Qt::Key_Return) keyEnter = false;
}

void GameScene::gameLoop()
{
    // 玩家1移动
    if(keyW && player1->y > 0) player1->y -= 5;
    if(keyS && player1->y < height()-50) player1->y +=5;
    if(keyA && player1->x > 0) player1->x -=5;
    if(keyD && player1->x < width()/2 - 30) player1->x +=5;

    // 双人模式玩家2移动
    if(isPVPMode){
        if(keyUp && player2->y >0) player2->y -=5;
        if(keyDown && player2->y < height()-50) player2->y +=5;
        if(keyLeft && player2->x > width()/2 +30) player2->x -=5;
        if(keyRight && player2->x < width()-30) player2->x +=5;
    }

    // 子弹移动 & 碰撞
    for(int i=0; i<bullets.size(); i++){
        Bullet *b = bullets[i];
        b->move();

        // 飞出屏幕删除
        if(b->x <0 || b->x > width()){
            bullets.removeAt(i);
            delete b;
            i--;
            continue;
        }

        // 击中玩家1
        if(b->dir == -1 &&
            b->x > player1->x && b->x < player1->x+50 &&
            b->y > player1->y && b->y < player1->y+50)
        {
            player1->hp -= 10;
            bullets.removeAt(i);
            delete b;
            i--;
        }
        // 击中玩家2
        if(b->dir == 1 &&
            b->x > player2->x && b->x < player2->x+50 &&
            b->y > player2->y && b->y < player2->y+50)
        {
            player2->hp -=10;
            bullets.removeAt(i);
            delete b;
            i--;
        }
    }

    // 胜负判断
    if(player1->hp <=0){
        timer->stop();
        showGameOver(false); // 玩家2胜利
        return;
    }
    if(player2->hp <=0){
        timer->stop();
        showGameOver(true); // 玩家1胜利
        return;
    }

    update();
}

// 人机AI：自动移动 + 自动射击
void GameScene::aiLogic()
{
    if(!isPVPMode && player2->hp >0){
        // 上下移动
        if(player2->y < player1->y) player2->y += 5;
        else player2->y -=5;

        // 自动射箭
        bullets.append(new Bullet(player2->x-10, player2->y+20, -1));
    }
}

// 胜负弹窗
void GameScene::showGameOver(bool p1Win)
{
    QString msg;
    if(p1Win){
        if(isPVPMode) msg = "玩家1 胜利！";
        else msg = "你击败了AI！";
    } else {
        if(isPVPMode) msg = "玩家2 胜利！";
        else msg = "你被AI击败了...";
    }

    QMessageBox::information(this, "游戏结束", msg);
    this->close();
}