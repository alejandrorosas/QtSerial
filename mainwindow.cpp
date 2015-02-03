#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtCore/QDebug>
#include <QScrollBar>

#include <QtSerialPort/QSerialPortInfo>
#include <QtSerialPort/QSerialPort>
QT_USE_NAMESPACE

QSerialPortInfo port[100];

#define BAUD_RATE 9600



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    serial = new QSerialPort(this);

    ui->comboBox->clear();
    int i = 0;
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
           ui->comboBox->addItem(""+ info.portName());
           port[i++] = info;
    }
    //Test GitHub
    connect(serial, SIGNAL(readyRead()), this, SLOT(readData()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::portConnect()
{
    if(ui->butConnect->text().startsWith("Connect"))
    {
        serial->setPort(port[ui->comboBox->currentIndex()]);
        serial->open(QIODevice::ReadWrite);
        serial->setBaudRate(BAUD_RATE);
        ui->butConnect->setText("Close");
    } else
    {
        serial->close();
        ui->butConnect->setText("Connect");
    }

}

void MainWindow::readData()
{
    QByteArray data = serial->readAll();
    QString old_data = ui->textEdit->toPlainText();
    ui->textEdit->setText(old_data+data);
    ui->textEdit->verticalScrollBar()->setValue(ui->textEdit->verticalScrollBar()->maximum());
}
