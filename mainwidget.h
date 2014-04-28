#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <iostream>

#include <QWidget>

#include <vector>
using std::vector;

#include <QFrame>

#include "pasta.h"
#include "filemanager.h"
#include "colscounter.h"
#include "about.h"

class MainWidget: public QWidget
{
    Q_OBJECT
public:

    MainWidget(QWidget* _parent = 0);

    virtual ~MainWidget();

protected:

    void mousePressEvent(QMouseEvent *event);

    void dragEnterEvent(QDragEnterEvent *event);

    void dragMoveEvent(QDragMoveEvent *event);

    void dropEvent(QDropEvent *event);

    void paintEvent(QPaintEvent *event);

private:

    const QRect targetSquare(const QPoint& position) const;

    int findPiece(const QPoint& _point) const;

    bool isPointOnValidRect(const QPoint& point) const;

private:

    int m_PastaCount;

    AboutProgramm* m_About;
    FileManager* m_FileManager;
    ColsCounter* m_ColsCounter;
    QRect m_highlightedRect;

    typedef std::pair<QRect, int> PiceRect;
    QList<PiceRect> m_pieceRects;

    QList<Pasta*> m_Makaronen;
    QList<Pasta*>  m_MovedMakaronen;


public slots:

    int startGame(int _num_cols);
    int saveTotalText();
};



#endif // MAINWIDGET_H
