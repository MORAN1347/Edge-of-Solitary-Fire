#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QPainter>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onPVPClicked();   // 双人模式
    void onPVEClicked();   // 人机模式

protected:
    // 关键：画背景图
    void paintEvent(QPaintEvent *) override;

private:
    QPushButton *btn_pvp;
    QPushButton *btn_pve;
};

#endif // MAINWINDOW_H
