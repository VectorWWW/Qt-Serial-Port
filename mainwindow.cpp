#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<string>
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initPort();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::readCom()
{
    QByteArray temp = serial.readAll();
    if(!temp.isEmpty()){
        if(ui->hexStringButton->text() == "String"){
            ui->uartRecvText->insertPlainText(temp);
            ui->uartRecvText->insertPlainText("\n");
        }else{
//            temp = serial.readAll();
            ui->uartRecvText->insertPlainText("0x");
            ui->uartRecvText->insertPlainText(temp.toHex());
            ui->uartRecvText->insertPlainText("\n");
        }

    }
}


void MainWindow::on_uartOpenCloseBtn_clicked()
{
    if(ui->portNameCombo->isEnabled()){
        ui->uartOpenCloseBtn->setText("Close");
        ui->portNameCombo->setDisabled(true);
        serial.setPortName(ui->portNameCombo->currentText());
        serial.setBaudRate(QSerialPort::BaudRate(ui->baudRateCombo->currentText().toInt()), QSerialPort::AllDirections);
        serial.setDataBits(QSerialPort::DataBits(ui->dataBitsCombo->currentText().toInt()));
        serial.setFlowControl(QSerialPort::NoFlowControl);
        serial.setParity(QSerialPort::Parity(ui->parityCombo->currentIndex() == 0 ?
                                                 0 : ui->parityCombo->currentIndex() + 1));
        serial.setStopBits(QSerialPort::StopBits(ui->stopBitsComb->currentText().toFloat() == 1.5 ?
                                                    3 : ui->stopBitsComb->currentText().toInt()));
        serial.close(); // 先关串口，再打开，可以保证串口不被其他函数占用
        if(serial.open(QIODevice::ReadWrite)){
            connect(&serial, SIGNAL(readyRead()), this, SLOT(readCom()));   // 把串口的readRead()信号绑定到readCom()这个槽函数上
        }
    }else{
        ui->uartOpenCloseBtn->setText("Open");
        ui->portNameCombo->setEnabled(true);
        serial.close();
    }
}

void MainWindow::on_uartSendBtn_clicked()
{
    int numOfInput = serial.write(ui->uartSendText->toPlainText().toLatin1());   // 以ASCII码的形式通过串口发送出去
    string statusShow = "the number of input is" + to_string(numOfInput);

    ui->statusBar->showMessage(QString::fromLocal8Bit(statusShow.c_str()), 2000);
}

void MainWindow::initPort()
{
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts()){
       QSerialPort serialtmp;
       serialtmp.setPort(info);
       /* 判断端口是否能打开 */
       if(serialtmp.open(QIODevice::ReadWrite))
       {
           int isHaveItemInList = 0;
           /* 判断是不是已经在列表中了 */
           for(int i=0; i<ui->portNameCombo->count(); i++)
           {
               /* 如果，已经在列表中了，那么不添加这一项了就 */
               if(ui->portNameCombo->itemData(i) == serialtmp.portName())
               {
                   isHaveItemInList++;
               }
           }

           if(isHaveItemInList == 0)
           {
               ui->portNameCombo->addItem(serialtmp.portName());
           }
           serialtmp.close();
       }
    }
}


void MainWindow::on_recvClearBtn_clicked()
{
    ui->uartRecvText->clear();
}

void MainWindow::on_hexStringButton_clicked()
{
    if(ui->hexStringButton->text() == "String")
        ui->hexStringButton->setText("Hex");
    else
        ui->hexStringButton->setText("String");
}
