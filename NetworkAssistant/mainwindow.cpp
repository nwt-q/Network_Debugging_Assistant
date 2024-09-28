#include "mainwindow.h"
#include "NetworkComboBox.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setLayout(ui->verticalLayout);

    server = new QTcpServer(); // 完成初始化
    connect(server, &QTcpServer::newConnection, this, &MainWindow::on_newClient_connect);

    connect(ui->comboBoxChildren, &NetworkComboBox::on_ComboBox_clicked,this,&MainWindow::Network_ComboBox_refresh);
    ui->Stopconnect->setEnabled(false);
    ui->breakconnect->setEnabled(false);

    //获取所有网络接口
    QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();

    // 遍历所有网络接口
    for(const QNetworkInterface& interface : interfaces) {
        //检查接口是否处于激活状态
        if (interface.flags().testFlag(QNetworkInterface::IsUp) && !interface.flags().testFlag(QNetworkInterface::IsLoopBack)) { //排除回环接口
            // 获取所有地址
            for (const QNetworkAddressEntry& entry : interface.addressEntries()) {
                //只处理IPv4地址
                if(entry.ip().protocol() == QAbstractSocket::IPv4Protocol) {
                    ui->comboBoxIP->addItem(entry.ip().toString());
                    qDebug() << "Interface:" << interface.name();
                    qDebug() << "IP Address:" << entry.ip().toString();
                }
            }
        }
    }
}


MainWindow::~MainWindow()
{
    delete ui;
}

//检测客户端是否连接
void MainWindow::mdisconnected()
{
    qDebug() << "OUT Line";
    QTcpSocket *tmpSock = qobject_cast<QTcpSocket*>(sender());
    ui->ServertextEdit->insertPlainText("客户端断开! ");
    tmpSock->deleteLater();
}

//用于监听客户端连接
void MainWindow::Network_ComboBox_refresh()
{
    ui->comboBoxChildren->clear();
    QList<QTcpSocket*> tcpSocketClients = server->findChildren<QTcpSocket*>();

    for(QTcpSocket* tmp:tcpSocketClients) {
        if(tmp != nullptr && tmp->peerPort() != 0) {
            ui->comboBoxChildren->addItem(QString::number(tmp->peerPort()));
        }
    }
    ui->comboBoxChildren->addItem("ALL");
}


void MainWindow::on_newClient_connect()
{
    if(server->hasPendingConnections()) {
        std::cout << "NewConnect\n";
        //从 QTcpServer 中获取一个新的等待连接的 QTcpSocke
        QTcpSocket *connection = server->nextPendingConnection();

        qDebug() << "Cilent Add: " << connection->peerAddress().toString() << "port:" << connection->peerPort();
        ui->ServertextEdit->setText( "\n客户端地址: " + connection->peerAddress().toString() + "\n客户端端口号" + QString::number(connection->peerPort()));

        connect(connection,QTcpSocket::readyRead ,this, &MainWindow::on_readyRead_handler);

        //建立信号与槽
        connect(connection, &QTcpSocket::disconnected, this, &MainWindow::mdisconnected);
    }
}

void MainWindow::on_Startconnect_clicked()
{
    //QHostAddress add("192.168.37.167");
    qDebug() << "is OK to Listen";
    int port = ui->lineEdit->text().toInt();
    if(!server->listen(QHostAddress(ui->comboBoxIP->currentText()),port)) {
        //端口号被占用
        QMessageBox mesgBox;
        mesgBox.setWindowTitle("监听失败");
        mesgBox.setText("端口号被占用");
        mesgBox.exec();
        return ;
    }
    ui->Stopconnect->setEnabled(true);
    ui->breakconnect->setEnabled(true);
    ui->Startconnect->setEnabled(false);
}

void MainWindow::on_Stopconnect_clicked()
{
    QList<QTcpSocket*> tcpSocketClients = server->findChildren<QTcpSocket*>();

    //循环遍历断开端口号
    for(QTcpSocket* tmp:tcpSocketClients) {
        tmp->close();
    }
    server->close();

    //恢复按钮
    ui->Stopconnect->setEnabled(false);
    ui->breakconnect->setEnabled(false);
    ui->Startconnect->setEnabled(true);
}

//数据到来读取数据
void MainWindow::on_readyRead_handler()
{
    QTcpSocket * tmpSocker = qobject_cast<QTcpSocket *>(sender());

    QByteArray revData = tmpSocker->readAll();
    //将读取的数据显示在文本框中
    ui->ServertextEdit->insertPlainText(revData);
}


//向指定目标发送信息
void MainWindow::on_SendButton_clicked()
{
    QList<QTcpSocket*> tcpSocketClients = server->findChildren<QTcpSocket*>();

    if(ui->comboBoxChildren->currentText() != "ALL") {
        tcpSocketClients[ChildIndex]->write(ui->ClienttextEdit->toPlainText().toStdString().c_str());
    } else {
        //循环遍历给端口号发送信息
        for(QTcpSocket* tmp:tcpSocketClients) {
            tmp->write(ui->ClienttextEdit->toPlainText().toStdString().c_str());
        }
    }
    //tcpSocketClients[0]->write(ui->ClienttextEdit->toPlainText().toStdString().c_str());
}


void MainWindow::on_comboBoxChildren_activated(int index)
{
    ChildIndex = index;
}


void MainWindow::on_breakconnect_clicked()
{
    on_Stopconnect_clicked();
    delete server;
    this->close();
}

