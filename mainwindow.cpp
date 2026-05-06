#include "mainwindow.h"
#include "gamescene.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setFixedSize(600, 400);
    this->setWindowTitle("弓箭手对战游戏");

    // 布局
    QWidget *central = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(central);
    setCentralWidget(central);

    // 按钮
    btn_pvp = new QPushButton("双人对战", this);
    btn_pve = new QPushButton("人机对战", this);
    btn_pvp->setFixedSize(200, 60);
    btn_pve->setFixedSize(200, 60);

    layout->addWidget(btn_pvp, 0, Qt::AlignCenter);
    layout->addWidget(btn_pve, 0, Qt::AlignCenter);
    layout->setSpacing(30);

    connect(btn_pvp, &QPushButton::clicked, this, &MainWindow::onPVPClicked);
    connect(btn_pve, &QPushButton::clicked, this, &MainWindow::onPVEClicked);
}

MainWindow::~MainWindow()
{
}

void MainWindow::onPVPClicked()
{
    // 打开双人模式
    GameScene *scene = new GameScene(nullptr, true);
    scene->show();
    this->close();
}

void MainWindow::onPVEClicked()
{
    // 打开人机模式
    GameScene *scene = new GameScene(nullptr, false);
    scene->show();
    this->close();
}