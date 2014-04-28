#ifndef PASTA_H
#define PASTA_H

#include <QtGui>
#include <QPalette>
#include <QLabel>

#include "specifications.h"

#include <sstream>
using std::stringstream;

class Pasta: public QLabel
{
public:
    Pasta(const QString& _text, QWidget* _parent, int _pasta_number, int _cols_number);
    ~Pasta();

    QString getText() const;

    void setNumber(int _num)        {m_PastaNumber = _num;}
    int getNumber()  const;

    int getColsNumber() const       {return m_ColsNumber;}

    int getLeftPasta() const        {return m_LeftPastaNum;}
    int getRightPasta() const        {return m_RightPastaNum;}

    void setLeftPasta(int _num)        {m_LeftPastaNum = _num;}
    void setRightPasta(int _num)       {m_RightPastaNum = _num;}

protected:

    void mouseDoubleClickEvent(QMouseEvent* _event);

private:

    bool is_PastaDoubleClicked;

    QString m_Text;

    int m_PastaNumber;
    int m_ColsNumber;
    int m_LeftPastaNum;
    int m_RightPastaNum;

    QImage m_CurrentImage;      // current pasta image
};




#endif // PASTA_H
