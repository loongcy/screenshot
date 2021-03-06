#include "capturescreen.h"
#include <QMouseEvent>
#include <QApplication>
#include <QPixmap>
#include <QScreen>
#include <QDesktopWidget>
#include "capturetool.h"
#include <QFileDialog>
#include <QDateTime>
#include <qdebug.h>
#include <QClipboard>

CaptureScreenWidget::CaptureScreenWidget(QWidget *parent)
    : QWidget(parent)
{
    m_mouseCaptureState = 1;
    toolBar = new CaptureTool(this);
    toolBar->setWindowFlag(Qt::FramelessWindowHint);
    toolBar->hide();
    connect(toolBar, SIGNAL(saveCapturePixmapSignal()), this, SLOT(saveCapturePixmapSlot()));
    connect(toolBar, SIGNAL(copyClipboardSignal()), this, SLOT(copyClipboardSlot()));
    connect(toolBar, SIGNAL(cancelCapturePixmapSignal()), this, SLOT(cancelCapturePixmapSlot()));
}

void CaptureScreenWidget::initCaptureScreen(){
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);//设置无边框和标题
    this->setMouseTracking(true);//设置鼠标可以移动
    setWindowState(Qt::WindowActive | Qt::WindowFullScreen);//设置窗口激活和全屏显示
    //获取应用程序的屏幕
    QScreen* pScreen = QGuiApplication::primaryScreen();
    QDesktopWidget *pDestop = QApplication::desktop();
    //抓取真个屏幕图片的一个截图
    m_fullScreenPixmap = pScreen->grabWindow(pDestop->winId(), 0, 0, pDestop->width(), pDestop->height());
}

CaptureScreenWidget::~CaptureScreenWidget(){
    delete  toolBar;
}

//鼠标按下事件处理
void CaptureScreenWidget::mousePressEvent(QMouseEvent *pEvent) {

    if(pEvent->button() == Qt::LeftButton && m_mouseCaptureState == 1)
    {
        m_startPoint =  pEvent->pos();
        m_mouseCaptureState = 2;
    }

    QWidget::mousePressEvent(pEvent);
}

//鼠标松开事件触发
void CaptureScreenWidget::mouseReleaseEvent(QMouseEvent *event){
    if (m_mouseCaptureState == 2){
        m_mouseCaptureState = 3;
        m_endPoint = event->pos();
    }

    //调用基类的鼠标松开的方法
    QWidget::mouseReleaseEvent(event);

    toolBar->setContentsMargins(0,0,0,0);
    toolBar->setGeometry(m_endPoint.x() - 500, m_endPoint.y(), 500, 50);
    toolBar->show();

}

//鼠标移动事件处理
void CaptureScreenWidget::mouseMoveEvent(QMouseEvent *event){

    if(m_mouseCaptureState == 2){
        //如果鼠标处于选中移动的时候，需要绘制矩形，并将矩形的其他区域添加一层阴影
        m_endPoint = event->pos();
        update();
    }

    QWidget::mouseMoveEvent(event);
}

//快捷键按下处理
void CaptureScreenWidget::keyPressEvent(QKeyEvent *event){
    if(event->key() == Qt::Key_Escape)
    {
        m_mouseCaptureState = 1;
        emit signalFinishCaptureScreen();
        this->hide();
        toolBar->hide();
    }

    //按回车键和小键盘enter键，完成截图，并拷贝到剪切板中
    if(event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
    {
        copyClipboardSlot();
        emit signalFinishCaptureScreen();//发出信号通知
        this->hide();
        toolBar->hide();
    }

}

//绘图处理
void CaptureScreenWidget::paintEvent(QPaintEvent *){
    m_rectPainter.begin(this);

    //获取阴影颜色
    QColor shadowColor = QColor(0, 0, 0, 100);
    m_rectPainter.setPen(QPen(Qt::blue, 1, Qt::SolidLine, Qt::FlatCap));
    m_rectPainter.drawPixmap(0, 0, m_fullScreenPixmap);//绘制背景图片
    m_rectPainter.fillRect(m_fullScreenPixmap.rect(), shadowColor);

    if(m_mouseCaptureState == 2)
    {
        QRect selectedRect = getRect(m_startPoint, m_endPoint);
        m_capturePixmap = m_fullScreenPixmap.copy(selectedRect);
        m_rectPainter.drawPixmap(selectedRect.topLeft(), m_capturePixmap);
        m_rectPainter.drawRect(selectedRect);
    }

    m_rectPainter.end();

    qDebug() << "CaptureScreenWidget::paintEvent painter rect";

}

QRect CaptureScreenWidget::getRect(const QPoint &beginPoint, const QPoint &endPoint)
{
    int x, y, width, height;
    width = qAbs(beginPoint.x() - endPoint.x());
    height = qAbs(beginPoint.y() - endPoint.y());
    x = beginPoint.x() < endPoint.x() ? beginPoint.x() : endPoint.x();
    y = beginPoint.y() < endPoint.y() ? beginPoint.y() : endPoint.y();

    QRect selectedRect = QRect(x, y, width, height);
    if (selectedRect.width() == 0)
    {
        selectedRect.setWidth(1);
    }
    if (selectedRect.height() == 0)
    {
        selectedRect.setHeight(1);
    }

    return selectedRect;
}

//保存槽函数
void CaptureScreenWidget::saveCapturePixmapSlot(){
    QString caption = QDir::currentPath()+ "/" + QDateTime::currentDateTime().toString("yyyyMMddHHmmss") + ".jpg";
    QString fileName = QFileDialog::getSaveFileName(this, "save", caption, "Image Files(.jpg .png .jpeg)");

    if(!fileName.isNull() && !fileName.isEmpty()){
        bool ret = m_capturePixmap.save(fileName);
        if(ret){
            qDebug() << fileName;
        }
    }

    emit signalFinishCaptureScreen();//发出信号通知
    toolBar->hide();
    this->hide();
    m_mouseCaptureState = 1;
}

//拷贝到剪切板槽函数
void CaptureScreenWidget::copyClipboardSlot(){
    QClipboard* pClipboard = QApplication::clipboard();
    pClipboard->setPixmap(m_capturePixmap);

    emit signalFinishCaptureScreen();//发出信号通知
    toolBar->hide();
    this->hide();
    m_mouseCaptureState = 1;
}

//撤销这次截图内容重新在截取的槽函数
void CaptureScreenWidget::cancelCapturePixmapSlot(){
     m_mouseCaptureState = 1;
}
