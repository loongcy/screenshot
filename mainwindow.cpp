#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<windows.h>
#include<winuser.h>
#include<QDateTime>
#include<qdebug.h>
#include<QScreen>
#include<QDesktopWidget>
#include<QClipboard>
#include<QDialog>
#include <QMessageBox>
#include <QCloseEvent>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_pCapture = new CaptureScreenWidget();
    connect(m_pCapture, SIGNAL(signalFinishCaptureScreen()),this, SLOT(finishScreenShot()) );

    //注册windows消息处理函数
    qApp->installNativeEventFilter(this);
    //注册热键
    ATOM atom = GlobalAddAtomA("screenshot");
    bool ret = RegisterHotKey(reinterpret_cast<HWND>(this->winId()), atom, MOD_CONTROL|MOD_SHIFT, 65);
    if(ret){
        qDebug() << "register hotkey success";
    }else{
        UnregisterHotKey(reinterpret_cast<HWND>(this->winId()), atom);
    }
    GlobalDeleteAtom(atom);

    initSystemTray();
}

void MainWindow::initSystemTray(){
    setWindowTitle(qApp->applicationDisplayName());
    pTrayIcon = new QSystemTrayIcon(this);
    connect(pTrayIcon,&QSystemTrayIcon::activated,this,[&](QSystemTrayIcon::ActivationReason reason){
        switch(reason){
        case QSystemTrayIcon::Trigger:
            break;
        case QSystemTrayIcon::DoubleClick:
        case QSystemTrayIcon::MiddleClick:
            if(isHidden()){
                show();
                activateWindow();
            } else {
                hide();
            }
        }
    });

    auto menu = new QMenu(this);
    connect(menu->addAction("exit program"),&QAction::triggered,this,[&]{
        QMessageBox about(QMessageBox::NoIcon,windowTitle(),"<FONT size=4><div><b>exit program</b></div></FONT>",QMessageBox::Ok|QMessageBox::Cancel,this);
        //about.setIconPixmap(QPixmap(":/logo.png"));
        if(about.exec()==QMessageBox::Cancel){
            return;
        }
        qApp->quit();
    });
    pTrayIcon->setContextMenu(menu);
    pTrayIcon->setToolTip(qApp->applicationDisplayName());
    pTrayIcon->setIcon(QIcon(":/23.ico"));
    pTrayIcon->show();
//    pTrayIcon->showMessage(qApp->applicationDisplayName(), "Start");
}

bool MainWindow::nativeEventFilter(const QByteArray &eventType, void *message, long *result){
    if(eventType == "windows_generic_MSG")
     {
         MSG *msg = static_cast<MSG *>(message);
         if(msg->message == WM_HOTKEY)
         {
             Sleep(300);
             m_pCapture->initCaptureScreen();
             m_pCapture->show();
             return true;
         }
     }
    return false;
}


MainWindow::~MainWindow()
{
    if (m_pCapture){
        m_pCapture->close();
        delete m_pCapture;
    }
    delete pTrayIcon;
    delete ui;
}


void MainWindow::on_screenShotBtn_clicked()
{
    this->hide();
    Sleep(300);
    m_pCapture->initCaptureScreen();
    m_pCapture->show();
//    this->show();
}

void MainWindow::finishScreenShot(){
    //this->show();
    //qDebug() << "bbbbbbbbbbbbbbbbbbb";
    //hide();
    //pTrayIcon->show();
}

void MainWindow::closeEvent(QCloseEvent *pEvent){
    QMessageBox button(QMessageBox::Warning, QString(""),QString(tr("Are you sure exit?")));
    button.setStandardButtons (QMessageBox::Yes|QMessageBox::No);
    button.setSizeIncrement(250,120);
    button.setButtonText (QMessageBox::Yes,QString("OK"));
    button.setButtonText (QMessageBox::No,QString("system tray"));

    if(button.exec() != QMessageBox::Yes)
    {
        this->hide();
        pEvent->ignore();
    }else{
        pEvent->accept();
    }
}
