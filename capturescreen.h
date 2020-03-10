#ifndef CAPTURESCREEN_H
#define CAPTURESCREEN_H

#include <QWidget>
#include <QPainter>
#include <QPixmap>
#include "capturetool.h"

enum{
    mouse_state_none = 1,
    mouse_state_start_capture_press =2,
};

class CaptureScreenWidget : public QWidget
{
    Q_OBJECT

public:
    CaptureScreenWidget(QWidget *parent = nullptr);
    ~CaptureScreenWidget() override;
public:

    void initCaptureScreen();
Q_SIGNALS:
    void signalFinishCaptureScreen();//完成后发送信号给主窗口

public slots:
    void saveCapturePixmapSlot();//保存槽函数
    void copyClipboardSlot();//拷贝到剪切板槽函数
    void cancelCapturePixmapSlot();//撤销这次截图内容重新在截取的槽函数
private:
    void mousePressEvent(QMouseEvent *event) override;//鼠标按下事件处理
    void mouseReleaseEvent(QMouseEvent *event) override;//鼠标松开事件触发
    void mouseMoveEvent(QMouseEvent *event) override;//鼠标移动事件处理
    void keyPressEvent(QKeyEvent *event) override;//快捷键按下处理
    void paintEvent(QPaintEvent *event) override;//绘图处理

    QRect getRect(const QPoint &startPoint, const QPoint &endPoint);

private:
    int m_mouseCaptureState;//整个截图鼠标的状态 1：开始状态 2：鼠标按下 3：截图区域选择完毕 4：移动截取区域
    QPixmap m_fullScreenPixmap;//截图时全屏图片数据
    QPixmap m_capturePixmap;//选择下的图片数据
    QPoint m_startPoint;//记录鼠标初始位置
    QPoint m_endPoint;//记录鼠标结束的位置，用于计算选择的矩形框
    QPainter m_rectPainter;//矩形框画刷

    CaptureTool* toolBar;
};

#endif // CAPTURESCREEN_H
