#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<capturescreen.h>
#include<QAbstractNativeEventFilter>
#include <QSystemTrayIcon>
#include <QCloseEvent>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow, public QAbstractNativeEventFilter
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    //windows消息处理
    virtual bool nativeEventFilter(const QByteArray &eventType, void *message, long *result) override;

public:
    void closeEvent(QCloseEvent *pEvent) override;

private:
    void initSystemTray();

private slots:
    void on_screenShotBtn_clicked();
    void finishScreenShot();

private:
    Ui::MainWindow *ui;
    CaptureScreenWidget* m_pCapture;
    QSystemTrayIcon* pTrayIcon;
};

#endif // MAINWINDOW_H
