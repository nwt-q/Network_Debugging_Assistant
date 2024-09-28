#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QDebug>
#include <QString>
#include <iostream>

#include <QTcpServer>
#include <QTcpSocket>
#include <QCoreApplication>
#include <QNetworkInterface>
#include "networkcombobox.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QTcpServer *server;

public slots:
    void mdisconnected();

    void Network_ComboBox_refresh();

private slots:
    void on_newClient_connect();

    void on_Startconnect_clicked();

    void on_Stopconnect_clicked();

    void on_readyRead_handler();

    void on_SendButton_clicked();


    void on_comboBoxChildren_activated(int index);

    void on_breakconnect_clicked();

private:
    Ui::MainWindow *ui;
    int ChildIndex;
};
#endif // MAINWINDOW_H
