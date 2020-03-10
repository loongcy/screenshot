#include "capturetool.h"
#include "ui_capturetool.h"


CaptureTool::CaptureTool(QWidget *parent) : QWidget(parent), ui(new Ui::CaptureTool)
{
    ui->setupUi(this);
}

CaptureTool::~CaptureTool(){
    delete ui;
}

void CaptureTool::on_painterRectBtn_clicked()
{

}

void CaptureTool::on_painterArrowBtn_clicked()
{

}

void CaptureTool::on_painterPenBtn_clicked()
{

}

void CaptureTool::on_cancalBtn_clicked()
{
    emit cancelCapturePixmapSignal();
    hide();
}

void CaptureTool::on_pixmapPegDeskopBtn_clicked()
{

}

void CaptureTool::on_saveBtn_clicked()
{
    emit saveCapturePixmapSignal();
    hide();
}

void CaptureTool::on_closeBtn_clicked()
{

}

void CaptureTool::on_clipboardBtn_clicked()
{
    emit copyClipboardSignal();
    hide();

}
