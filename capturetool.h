#ifndef CAPTURETOOL_H
#define CAPTURETOOL_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class CaptureTool; }
QT_END_NAMESPACE


class CaptureTool : public QWidget
{
    Q_OBJECT
public:
    explicit CaptureTool(QWidget *parent = nullptr);
    ~CaptureTool();

signals:
    void saveCapturePixmapSignal();//保存信号
    void copyClipboardSignal();//拷贝到剪切板
    void cancelCapturePixmapSignal();//取消此次截屏内容 重新截屏
public slots:

public:
    Ui::CaptureTool *ui;
private slots:
    void on_painterRectBtn_clicked();
    void on_painterArrowBtn_clicked();
    void on_painterPenBtn_clicked();
    void on_cancalBtn_clicked();
    void on_pixmapPegDeskopBtn_clicked();
    void on_saveBtn_clicked();
    void on_closeBtn_clicked();
    void on_clipboardBtn_clicked();
};

#endif // CAPTURETOOL_H
