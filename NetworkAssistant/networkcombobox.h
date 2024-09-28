#ifndef NETWORKCOMBOBOX_H
#define NETWORKCOMBOBOX_H

#include <QComboBox>
#include <QWidget>

class NetworkComboBox : public QComboBox
{
    Q_OBJECT
public:
    NetworkComboBox(QWidget* parent);

protected:
    void mousePressEvent(QMouseEvent *E) override;

signals:
    void on_ComboBox_clicked();

};

#endif // NETWORKCOMBOBOX_H
