#include "networkcombobox.h"

#include <QMouseEvent>

NetworkComboBox::NetworkComboBox(QWidget *parent) : QComboBox(parent)
{

}

void NetworkComboBox::mousePressEvent(QMouseEvent* E)
{
    //检测鼠标左键
    if(E->button() == Qt::LeftButton) {
        emit on_ComboBox_clicked();
    }
    QComboBox::mousePressEvent(E);
}
