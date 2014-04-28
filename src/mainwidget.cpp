
#include <QRect>

#include "mainwidget.h"

MainWidget::MainWidget(QWidget* _parent)
    : QWidget(_parent)
{

    /*
    QSize size;
    size.setHeight(200);
    size.setWidth(200);
*/

    m_About = new AboutProgramm(this);

    m_FileManager = new FileManager(this);
    //m_FileManager->resize(size);
    //m_FileManager->show();
    connect(m_FileManager, SIGNAL(textLoaded(int)), this, SLOT(startGame(int)));
    connect(m_FileManager, SIGNAL(textSaved()), this, SLOT(saveTotalText()));

    m_FileManager->move(0,0);

    m_ColsCounter = new ColsCounter(this);
    //m_ColsCounter->resize(size);
    //m_ColsCounter->show();
    connect(m_ColsCounter, SIGNAL(colsChanged(int)), this, SLOT(startGame(int)));
    m_ColsCounter->move(300, 1);

    /*
    QGridLayout* layout = new QGridLayout;
    layout->addWidget(m_FileManager, 0 ,1);
    layout->addWidget(m_ColsCounter, 0 ,6);
    this->setLayout(layout);
*/


    /*
    QHBoxLayout* layout = new QHBoxLayout;

    layout->addWidget(m_FileManager);
    layout->addWidget(m_ColsCounter);
    this->setLayout(layout);
    */

    setAcceptDrops(true);
}


MainWidget::~MainWidget()
{
    //qDebug() << "WTF    !!!";
}

void MainWidget::mousePressEvent(QMouseEvent* event)
{

    QRect square = targetSquare(event->pos());
    update(square);


    FileManager* maybe_its_file_manager = dynamic_cast<FileManager*>(childAt(event->pos()));
    if(maybe_its_file_manager != 0)
    {
        return;
    }

    ColsCounter* maybe_its_cols_counter = dynamic_cast<ColsCounter*>(childAt(event->pos()));
    if(maybe_its_cols_counter != 0)
    {
        return;
    }

    Pasta* maybe_its_pasta = dynamic_cast<Pasta*>(childAt(event->pos()));
    if(maybe_its_pasta != 0)
    {
        //qDebug() << "its Pasta!!!";
    }
    else
    {
        //qDebug() << "its no Pasta!!!";
        return;
    }

    Pasta* child = static_cast<Pasta*>(childAt(event->pos()));

    if (!child)
    {
        return;
    }

    QPoint hotSpot = event->pos() - child->pos();
    QPoint event_pos = event->pos();

    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);

    //dataStream << child->getText() << child->getNumber() << QPoint(hotSpot);
    dataStream << child->getText() << child->getNumber() << child->getColsNumber() << QPoint(event_pos);

    QMimeData *mimeData = new QMimeData;
    mimeData->setData(Holy::kMimeTypeName, itemData);
    mimeData->setText(child->getText());

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);

    drag->setPixmap(*child->pixmap());

    drag->setHotSpot(hotSpot);

    child->hide();

    if (drag->exec(Qt::MoveAction | Qt::CopyAction, Qt::CopyAction) == Qt::MoveAction)
    {
        update(targetSquare(event->pos()));
        //qDebug() << "1 here she failed...";
        //child->close();
        //qDebug() << "2";
    }
    else
    {
        update(targetSquare(event->pos()));
        child->show();
    }
}


void MainWidget::dragEnterEvent(QDragEnterEvent* event)
{
    //QMessageBox::information(0, "MainWidget::dragEnterEvent", "Function begin");

    if (event->mimeData()->hasFormat(Holy::kMimeTypeName))
    {
        if (children().contains(event->source()))
        {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        }
        else
        {
            event->acceptProposedAction();
        }

    }
    else if (event->mimeData()->hasText())
    {
        event->acceptProposedAction();
    }
    else
    {
        event->ignore();
    }
}


void MainWidget::dragMoveEvent(QDragMoveEvent* event)
{
    //QRect updateRect = m_highlightedRect.unite(targetSquare(event->pos()));
    QRect updateRect = m_highlightedRect.united(targetSquare(event->pos()));

    if (event->mimeData()->hasFormat(Holy::kMimeTypeName))
    {
        if (children().contains(event->source()))
        {
            m_highlightedRect = targetSquare(event->pos());
            event->setDropAction(Qt::MoveAction);
            event->accept();
        }
        else
        {
            bool isValidRect = false;
            for(int i = 0; i < m_pieceRects.size(); i++)
            {
                if(m_pieceRects[i].first.contains(event->pos()))
                {
                    //qDebug() << "contains";
                    m_highlightedRect = m_pieceRects[i].first;
                    isValidRect = true;
                }
            }

            if(!isValidRect)
            {
                m_highlightedRect = targetSquare(event->pos());
            }

            event->acceptProposedAction();
        }
    }
    else if (event->mimeData()->hasText())
    {
        event->acceptProposedAction();
    }
    else
    {
        event->ignore();
    }

    update(updateRect);
}


/**
        === DROP EVENT ===
  */
void MainWidget::dropEvent(QDropEvent* event)
{
    if (event->mimeData()->hasFormat(Holy::kMimeTypeName) && isPointOnValidRect(event->pos()))
    {
        const QMimeData* mime = event->mimeData();

        QByteArray itemData = mime->data(Holy::kMimeTypeName);
        QDataStream dataStream(&itemData, QIODevice::ReadOnly);

        QString text;
        QPoint offset;
        int number;
        int cols_number;
        dataStream >> text >> number >> cols_number >> offset;
        qDebug() << "moved " << number << " item";

        int distance = abs(event->pos().x() - offset.x());

        int direction = event->pos().x() - offset.x();
        //qDebug() << "distance = " << distance;
        //qDebug() << "distance = " << direction;

        if(distance <= Holy::kMinDistance)
            return;

        for(int i = 0; i < m_MovedMakaronen.size(); i++)
        {
            if(number == m_MovedMakaronen[i]->getNumber())
            {
                qDebug() << "removing from Moved Makaronen: " << number;
                delete m_MovedMakaronen[i];
                m_MovedMakaronen.removeAt(i);
                break;
            }
        }

        for(int i = 0; i < m_Makaronen.size(); i++)
        {
            if(number == m_Makaronen[i]->getNumber())
            {
                qDebug() << "removing from Static Makaronen: " << number;
                delete m_Makaronen[i];
                m_Makaronen.removeAt(i);
                break;
            }
        }

        Pasta* nPasta;
        QPoint point(0,0);
        // need to find where the drop makaronen
        int new_pos = findPiece(event->pos());
        if(new_pos != -1)
        {
            point.setY(Holy::kInitMakaronPositionY);

            if(direction > 0)
            {

                point.setX(m_pieceRects[new_pos].first.x() - Holy::kPastaWidth);

                qDebug() << "num of dragged = " << number ;
                for(int num = number; num < new_pos; num++)
                {
                    for(int i = 0; i < m_Makaronen.size(); i++)
                    {
                        if (m_Makaronen[i]->getNumber() == num)
                        {
                            qDebug() <<"; num of moved = " << num;
                            int xPos = m_Makaronen[i]->x();
                            xPos = xPos - Holy::kMakaronSpacing - Holy::kPastaWidth;
                            m_Makaronen[i]->setNumber(num-1);
                            m_Makaronen[i]->move(xPos, Holy::kInitMakaronPositionY);

                        }
                    }

                    for(int i = 0; i < m_MovedMakaronen.size(); i++)
                    {
                        if (m_MovedMakaronen[i]->getNumber() == num)
                        {
                            qDebug() <<"; num of moved = " << num;
                            int xPos = m_MovedMakaronen[i]->x();
                            xPos = xPos - Holy::kMakaronSpacing - Holy::kPastaWidth;
                            m_MovedMakaronen[i]->setNumber(num-1);
                            m_MovedMakaronen[i]->move(xPos, Holy::kInitMakaronPositionY);
                        }
                    }
                }

                qDebug() << "new_pos = " << new_pos;
                nPasta = new Pasta(text, this, new_pos-1, cols_number);
                qDebug() << "pasta = new Pasta (DROPPED:)" << nPasta;
                nPasta->move(point);
                nPasta->show();

            }
            else // turn left
            {
                point.setX(m_pieceRects[new_pos].first.x() + Holy::kMakaronSpacing);
                qDebug() << "num of dragged = " << number;

                for(int num = number; num >= new_pos; num--)
                {
                    if(num != number)
                    {
                        for(int i = m_Makaronen.size() -1 ; i >= 0 ; i--)
                        {
                            if (m_Makaronen[i]->getNumber() == num)
                            {
                                qDebug() <<"; num of moved (static) = " << num;
                                int xPos = m_Makaronen[i]->x();
                                xPos = xPos + Holy::kMakaronSpacing + Holy::kPastaWidth;
                                m_Makaronen[i]->setNumber(num+1);
                                m_Makaronen[i]->move(xPos, Holy::kInitMakaronPositionY);

                            }
                        }

                        for(int i = m_MovedMakaronen.size()-1; i >= 0 ; i--)
                        {
                            if (m_MovedMakaronen[i]->getNumber() == num)
                            {
                                qDebug() <<"; num of moved (moved) = " << num;
                                int xPos = m_MovedMakaronen[i]->x();
                                xPos = xPos + Holy::kMakaronSpacing + Holy::kPastaWidth;
                                m_MovedMakaronen[i]->setNumber(num+1);
                                m_MovedMakaronen[i]->move(xPos, Holy::kInitMakaronPositionY);
                            }
                        }
                    }
                }

                qDebug() << "new_pos = " << new_pos;
                nPasta = new Pasta(text, this, new_pos, cols_number);
                //qDebug() << "pasta = new Pasta (DROPPED:)" << nPasta;
                nPasta->move(point);
                nPasta->show();

            }

        }



        //nPasta->setAttribute(Qt::WA_DeleteOnClose);

        //qDebug() << this->baseSize() << " " << this->height() << " " << this->rect() << " " << this->size()
        //        << " " << this->width() <<" " << this->x()<< " " << this->y();

        m_MovedMakaronen.push_back(nPasta);

        qDebug() << "MovedMakaronen size: " << m_MovedMakaronen.size();
        qDebug() << "Makaronen size: " << m_Makaronen.size();

        if (event->source() == this)
        {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        }
        else
        {
            event->acceptProposedAction();
        }

    }
    else if (event->mimeData()->hasText())
    {
        /*
        QStringList pieces = event->mimeData()->text().split(QRegExp("\\s+"), QString::SkipEmptyParts);
        QPoint position = event->pos();

        foreach (QString piece, pieces) {
            Pasta* newPasta = new Pasta(piece, this);
            newPasta->move(position);
            newPasta->show();
            newPasta->setAttribute(Qt::WA_DeleteOnClose);

            position += QPoint(newPasta->width(), 0);
        }
        */
        event->acceptProposedAction();
    }
    else
    {
        event->ignore();
    }

    qDebug() << "=============================";

}


void MainWidget::paintEvent(QPaintEvent* event)
{
    QPainter painter;
    painter.begin(this);

    //painter.fillRect(event->rect(), Qt::green);
    painter.fillRect(event->rect(), "#ffffdd");

    if(m_FileManager->isTextPresence())
    {
            for(int i = 0; i < m_pieceRects.size(); i++)
            {

                painter.setBrush(QColor("#ddddcb"));
                painter.setPen(Qt::NoPen);
                //painter.drawRect(m_pieceRects[i].adjusted(0, 0, -1, -1));
                painter.drawRect(m_pieceRects[i].first);
            }

            if (m_highlightedRect.isValid())
            {

                //painter.setBrush(QColor("#ffcccc"));
                painter.setBrush(QColor(Qt::red));
                painter.setPen(Qt::NoPen);
                painter.drawRect(m_highlightedRect);
                //painter.drawRect(m_highlightedRect.adjusted(0, 0, -1, -1));
            }
            else
            {

            }
    }
    painter.end();
}

const QRect MainWidget::targetSquare(const QPoint &position) const
{
    return QRect(position.x()/10 * 10 , position.y()/10 * 10, 0, 0);
}



int MainWidget::findPiece(const QPoint& _point) const
{
    for (int i = 0; i < m_pieceRects.size(); i++)
    {
        if (m_pieceRects[i].first.contains(_point))
        {
            return i;
        }
    }
    return -1;
}


bool MainWidget::isPointOnValidRect(const QPoint& _point) const
{
    for(int i = 0; i < m_pieceRects.size(); i++)
    {
        if(m_pieceRects[i].first.contains(_point))
        {
            return true;
        }
    }
    return false;
}



/**
       ========== START GAME !!!  ===============
  */

int MainWidget::startGame(int _num_cols)
{
    qDebug() << "Start Game > cols = " << _num_cols;

    m_PastaCount = _num_cols;

    if( !m_FileManager->isTextPresence())
        return 0;

    string text = m_FileManager->getEncryptedText();
    int size = text.size();

    if(size == 0)
        return 0;

    /*
    for(int i = 0; i < ::qMin(m_Makaronen.size(), m_MovedMakaronen.size()); i++)
    {
        qDebug() <<" makaronen[" <<i<<"]"<< m_Makaronen[i] <<"   moved_makarona["<<i<<"]"<<m_MovedMakaronen[i];
    }
    */

    while(!m_Makaronen.isEmpty())
    {
        m_Makaronen[m_Makaronen.size()-1]->hide();
        qDebug() <<" deling Makaron from m_Makaronen..." << m_Makaronen[m_Makaronen.size()-1];
        delete m_Makaronen[m_Makaronen.size()-1];
        m_Makaronen.pop_back();
    }


    while(!m_pieceRects.isEmpty())
    {
        m_pieceRects.pop_back();
    }

    while(!m_MovedMakaronen.isEmpty())
    {
        qDebug() << "deling Makaron from m_MovedMakaronen...: " << m_MovedMakaronen[m_MovedMakaronen.size()-1];

        m_MovedMakaronen[m_MovedMakaronen.size()-1]->hide();
        delete m_MovedMakaronen[m_MovedMakaronen.size()-1];
        m_MovedMakaronen.pop_back();
    }

    qDebug()<<"delete excess makaronen: OK";
    //update(QRect());

    int num_of_column = _num_cols;

    while(text.size() % num_of_column != 0)
    {
        text += " ";
        size++;
    }

    vector<QString> vec(num_of_column);

    int j = 0;
    int i = 0;
    while(j != size)
    {
        vec[i] += text[j];

        i++;
        j++;
        if(i == num_of_column)
            i = 0;
    }


    int step = Holy::kInitMakaronPositionX - Holy::kMakaronSpacing;
    int xPos = Holy::kInitMakaronPositionX;
    for(int i = 0; i < num_of_column; i++)
    {
        QString str = vec[i];
        Pasta* pasta = new Pasta(str, this, i, (i+1));

        pasta->setLeftPasta(i-1);

        if( (i+1) ==  num_of_column)
            pasta->setRightPasta(-11);
        else
            pasta->setRightPasta(i+1);

        pasta->move(xPos, Holy::kInitMakaronPositionY);
        pasta->show();
        m_Makaronen.push_back(pasta);
        xPos += step;
    }

    m_highlightedRect = QRect();

    int xPoss = Holy::kInitMakaronPositionX - Holy::kMakaronSpacing;
    for(int i = 0; i < num_of_column + 1; i++)
    {
        //QRect rect (xPoss, Holy::kInitMakaronPositionY - 15, 10, 100);
        QRect rect (xPoss-3, Holy::kInitMakaronPositionY - 15, 16, 100);
        PiceRect pr;
        pr.first = rect;

        if( (i+1) == (num_of_column + 1))
            pr.second = -1;
        else
            pr.second = i;
        m_pieceRects.push_back(pr);
        xPoss += step;
    }

    int y_scale = m_Makaronen[0]->height();
    //qDebug() << "height: " << y_scale;
    QSize new_size(this->size());
    new_size.setHeight(y_scale + 100);
    this->resize(new_size);

    return 0;
}


int MainWidget::saveTotalText()
{
    if( !m_FileManager->isTextPresence())
        return 0;

    int num_of_cols = m_Makaronen.size() + m_MovedMakaronen.size();

    QVector<QString> vec;

    for(int num = 0; num < num_of_cols; num++)
    {
            for(int i = 0 ; i < m_Makaronen.size() ; i++)
            {
                if (m_Makaronen[i]->getNumber() == num)
                {
                    QString tmp = m_Makaronen[i]->getText();
                    vec.push_back(tmp);
                }
            }

            for(int i = 0; i < m_MovedMakaronen.size() ; i++)
            {
                if (m_MovedMakaronen[i]->getNumber() == num)
                {
                    QString tmp = m_MovedMakaronen[i]->getText();
                    vec.push_back(tmp);
                }
            }

    }

    int text_size = vec[0].size() * vec.size();
    QString total_text;
    for(int i = 0, j = 0, k = 0; i < text_size; i++)
    {
        total_text.push_back(vec[j][k]);
        j++;
        if(j == vec.size())
        {
            j=0;
            k++;
        }
    }

    QString name = QFileDialog::getSaveFileName(0, "Saving file", "", "*.txt");
    ofstream os(name.toStdString().c_str(), std::ios::out | std::ios::trunc);

    //QFile file(name);
    //if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate ))
    //         return -1;

    //QString tmp = QString::fromLocal8Bit(total_text.toStdString().c_str(), total_text.size());

    //qDebug() << "text:\n" << total_text;

    QString tmp = total_text.toLocal8Bit();
    char* data = tmp.toLatin1().data();
    //char* data = tmp.toutf8().data();

    string std_text(data);

    //std::cerr<< "std text:\n" << data << "\n";

    os << std_text;

    return 0;
}



