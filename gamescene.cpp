#include "gamescene.h"
#include "player.h"
#include "bullet.h"
#include <QKeyEvent>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QDialog>

GameScene::GameScene(QWidget *parent, bool isPVP)
    : QWidget(parent), isPVPMode(isPVP)
{
    this->setFixedSize(800, 400);
    this->setWindowTitle("战斗中");

    bgPixmap.load(":/game.png");

    // 初始化玩家
    player1 = new Player(50, height()/2 - 25, Qt::blue, true);
    player2 = new Player(700, height()/2 - 25, Qt::red, false);

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

GameScene::~GameScene()
{
    // 停止并释放定时器
    if (timer) {
        timer->stop();
        delete timer;
        timer = nullptr;
    }

    // 释放所有子弹对象
    qDeleteAll(bullets);
    bullets.clear();

    // 释放玩家对象
    delete player1;
    delete player2;
}

void GameScene::paintEvent(QPaintEvent *)
{
    QPainter p(this);

    // 游戏开始提示
    if(timer->remainingTime() > 2500){ // 前2.5秒显示
        p.setPen(QColor(255,80,80));
        QFont f = p.font();
        f.setBold(true);
        f.setPointSize(40);
        p.setFont(f);
        p.drawText(rect(), Qt::AlignCenter, "FIGHT!");
    }

    p.setRenderHint(QPainter::Antialiasing);

    if(!bgPixmap.isNull()){
        p.drawPixmap(0, 0, width(), height(), bgPixmap);
    } else {
        p.fillRect(rect(), Qt::lightGray); // 没背景图就用灰色
    }

    // 画玩家
    player1->draw(p);
    player2->draw(p);

    for(Bullet *b : bullets){
        if(b->isHit){
            p.setBrush(Qt::red);
            p.drawEllipse(b->x-10, b->y-10, 20, 20);
        } else {
            b->draw(p);
        }
    }

    // 玩家1血条
    p.setPen(Qt::white);
    p.setBrush(QColor(50,50,50));
    p.drawRect(20, 20, 200, 18);
    p.setBrush(Qt::red);
    p.drawRect(20, 20, player1->hp * 2, 18);
    p.setPen(Qt::black);
    p.drawText(20, 20, 200, 18, Qt::AlignCenter, "玩家1");

    // 玩家2血条
    p.setPen(Qt::white);
    p.setBrush(QColor(50,50,50));
    p.drawRect(this->width() - 220, 20, 200, 18);
    p.setBrush(Qt::red);
    p.drawRect(this->width() - 220, 20, player2->hp * 2, 18);
    p.setPen(Qt::black);
    p.drawText(this->width() - 220, 20, 200, 18, Qt::AlignCenter, isPVPMode ? "玩家2" : "AI");
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
            b->isHit=true;
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
            b->isHit=true;
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
    timer->stop();

    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("游戏结束");
    dialog->setFixedSize(300, 220);
    dialog->setStyleSheet("background-color: #222; color: white;");

    QLabel *label = new QLabel(dialog);
    QString msg;
    if(p1Win){
        if(isPVPMode) msg = "玩家1 胜利！";
        else msg = "你击败了AI！";
    } else {
        if(isPVPMode) msg = "玩家2 胜利！";
        else msg = "你被AI击败了...";
    }
    label->setText(msg);
    label->setStyleSheet("font-size: 18px; color: #4CAF50;");
    label->setAlignment(Qt::AlignCenter);

    QPushButton *btnRestart = new QPushButton("再来一局", dialog);
    QPushButton *btnBackMenu = new QPushButton("返回主菜单", dialog);

    btnRestart->setStyleSheet("background-color: #4CAF50; color: white; padding: 8px; border-radius: 4px;");
    btnBackMenu->setStyleSheet("background-color: #f44336; color: white; padding: 8px; border-radius: 4px;");

    QVBoxLayout *layout = new QVBoxLayout(dialog);
    layout->addWidget(label);
    layout->addWidget(btnRestart);
    layout->addWidget(btnBackMenu);
    dialog->setLayout(layout);

    // 再来一局
    connect(btnRestart, &QPushButton::clicked, dialog, [=](){
        dialog->accept();
        this->close();
        this->deleteLater();

        GameScene *newScene = new GameScene(nullptr, isPVPMode);
        connect(newScene, &GameScene::gameFinished, qobject_cast<QWidget*>(this->parent()), &QWidget::show);
        newScene->show();
    });

    // 返回主菜单
    connect(btnBackMenu, &QPushButton::clicked, dialog, [=](){
        dialog->accept();
        emit gameFinished();  // 通知主窗口回来
        this->close();        // 关闭游戏场景
    });

    dialog->exec();
    delete dialog;
}