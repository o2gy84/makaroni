
#include "colscounter.h"


ColsCounter::ColsCounter(QWidget* _par) :
        QWidget(_par)
{

    m_SpinBox = new QSpinBox(this);

    m_SpinBox->setRange(10, 23);
    m_SpinBox->setWrapping(true);
    //m_SpinBox->setButtonSymbols(QSpinBox::PlusMinus);
    //m_SpinBox->resize(50, 30);

    m_SpinBox->move(20, 0);

    connect(m_SpinBox, SIGNAL(valueChanged(int)), this, SLOT(setNewColsCounter()));
}

ColsCounter::~ColsCounter()
{

}

void ColsCounter::setNewColsCounter()
{
    int new_value = m_SpinBox->value();
    emit colsChanged(new_value);
}
