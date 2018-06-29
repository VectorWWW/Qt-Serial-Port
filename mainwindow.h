#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void readCom();
    void on_uartOpenCloseBtn_clicked();

    void on_uartSendBtn_clicked();

    void on_recvClearBtn_clicked();

    void on_hexStringButton_clicked();

private:
    Ui::MainWindow *ui;
    QSerialPort serial;

    void initPort();
};

#endif // MAINWINDOW_H
