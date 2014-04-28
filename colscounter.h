#ifndef COLSCOUNTER_H
#define COLSCOUNTER_H

#include <QSpinBox>
#include <QWidget>

class ColsCounter: public QWidget
{
    Q_OBJECT
public:

    ColsCounter(QWidget* _par);

    ~ColsCounter();

protected:

private:

    QSpinBox* m_SpinBox;

signals:

    void colsChanged(int _cols);

public slots:

    void setNewColsCounter();

};


#endif // COLSCOUNTER_H
