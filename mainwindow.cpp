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
            background-color: rgba(20,20,20,180);
            color: white;
            border: 2px solid white;
            border-radius: 12px;
            font-size: 22px;
        }
        QPushButton:hover {
            background-color: rgba(50,100,255,200);
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
    scene->show();
    this->close();
}

void MainWindow::onPVEClicked()
{
    GameScene *scene = new GameScene(nullptr, false);
    scene->show();
    this->close();
}