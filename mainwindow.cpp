#include "mainwindow.h"
#include "gamescene.h"
#include <QVBoxLayout>
#include <QPainter>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // 统一窗口大小 800x400
    this->setFixedSize(800, 400);
    this->setWindowTitle("孤锋弹道");

    // 布局
    QWidget *central = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(central);
    setCentralWidget(central);

    // 按钮
    btn_pvp = new QPushButton("双人对战", this);
    btn_pve = new QPushButton("人机对战", this);
    btn_pvp->setFixedSize(220, 70);
    btn_pve->setFixedSize(220, 70);

    // 按钮美化样式
    btn_pvp->setStyleSheet(R"(
    QPushButton {
        background-color: rgba(30,30,45,180);
        color: white;
        border: 2px solid #5090ff;
        border-radius: 16px;
        font-size: 24px;
        padding: 10px;
    }
    QPushButton:hover {
        background-color: #5090ff;
        border-color: white;
        padding-left: 15px;
    }
)");
    btn_pve->setStyleSheet(btn_pvp->styleSheet());

    layout->addWidget(btn_pvp, 0, Qt::AlignCenter);
    layout->addWidget(btn_pve, 0, Qt::AlignCenter);
    layout->setSpacing(40);
    layout->setAlignment(Qt::AlignCenter);

    connect(btn_pvp, &QPushButton::clicked, this, &MainWindow::onPVPClicked);
    connect(btn_pve, &QPushButton::clicked, this, &MainWindow::onPVEClicked);
}

// 画背景图（最重要！）
void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.drawPixmap(0, 0, width(), height(), QPixmap(":/menu.png"));
}

MainWindow::~MainWindow()
{
}

void MainWindow::onPVPClicked()
{
    GameScene *scene = new GameScene(nullptr, true);

    // 游戏结束 → 回到主菜单
    connect(scene, &GameScene::gameFinished, this, [=](){
        this->show();       // 重新显示主界面
        scene->deleteLater(); // 销毁游戏场景
    });

    scene->show();
    this->hide(); // 只隐藏，不关闭！
}

void MainWindow::onPVEClicked()
{
    GameScene *scene = new GameScene(nullptr, false);

    // 游戏结束 → 回到主菜单
    connect(scene, &GameScene::gameFinished, this, [=](){
        this->show();       // 重新显示主界面
        scene->deleteLater(); // 销毁游戏场景
    });

    scene->show();
    this->hide(); // 只隐藏，不关闭！
}