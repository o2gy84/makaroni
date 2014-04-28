

#include "pasta.h"


Pasta::Pasta(const QString &text, QWidget *parent, int _pasta_number, int _cols_number)
    : QLabel(parent)
{

    is_PastaDoubleClicked = false;

    m_PastaNumber = _pasta_number;
    m_ColsNumber = _cols_number;

    QString cols_num;
    cols_num.setNum(_cols_number);

    QFontMetrics metric(font());
    QSize size = metric.size(Qt::TextSingleLine, text);

    //QImage image(size.width() + 12, size.height() + 12, QImage::Format_ARGB32_Premultiplied);
    //QImage image(size.width() + 12, size.height() + 12, QImage::Format_RGB16);
    //QImage image(size.width() + 12, size.height() + 12, QImage::Format_ARGB4444_Premultiplied);
    //QImage image(size.width() + 12, size.height() + 60, QImage::Format_ARGB4444_Premultiplied);
    //QImage image(size.width() + 12, size.height() + text.length()*20, QImage::Format_ARGB4444_Premultiplied);
    QImage image( Holy::kPastaWidth , size.height() + (text.length()+1)*20, QImage::Format_ARGB4444_Premultiplied);
    image.fill(qRgba(0, 0, 0, 0));

    QFont font;
    font.setStyleStrategy(QFont::ForceOutline);
    //font.setStyleStrategy(QFont::PreferAntialias);

    //font.setBold(true);
    font.setPixelSize(14);

    //QString new_family = "times new roman";
    QString new_family = Holy::kFontFamily;
    font.setFamily(new_family);

    //QString fam = font.family();
    //qDebug() << "Family: " << fam;

    //QLinearGradient gradient(0, 0, 0, image.height()-1);
    QLinearGradient gradient(0, 0, 0, image.height());
    gradient.setColorAt(0.0, Qt::white);
    gradient.setColorAt(0.2, QColor(200, 200, 255));
    gradient.setColorAt(0.8, QColor(200, 200, 255));
    gradient.setColorAt(1.0, QColor(127, 127, 200));

    QPainter painter;
    painter.begin(&image);
    painter.setRenderHint(QPainter::Antialiasing);
    //painter.setBrush(gradient);
    painter.setBrush(QColor("#ef7877"));

    //painter.drawRoundedRect(QRectF(0.5, 0.5, image.width()-30, image.height()-1),  3, 3, Qt::RelativeSize);
    painter.drawRoundedRect(QRectF(0.5, 0.5, image.width()-1, image.height()-1),  3, 3, Qt::RelativeSize);

    painter.setFont(font);
    painter.setBrush(Qt::black);

    QString separator = "-----";
    int yPosition = 6;
    for(int i = -1; i < text.length(); i++)
    {
        if(i == -1 )
        {

                painter.setBrush(QColor("#aaaaaa"));
                painter.drawText(QRect(QPoint(6, yPosition     ), size), 0, cols_num);
                painter.drawText(QRect(QPoint(6, yPosition + 10), size), 0, separator);
                yPosition = yPosition + 10 + 10;
                painter.setBrush(Qt::black);
        }
        else if (i == text.length() -1)
        {
                painter.drawText(QRect(QPoint(6, yPosition     ), size), 0, text[i]);
        }
        else
        {
                //painter.drawText(QRect(QPoint(6, yPosition     ), size), Qt::AlignHCenter, text[i]);
                painter.drawText(QRect(QPoint(6, yPosition     ), size), 0, text[i]);
                if(text.length() > 1)
                {
                        //painter.drawText(QRect(QPoint(6, yPosition + 10), size), Qt::AlignHCenter, separator);
                        painter.drawText(QRect(QPoint(6, yPosition + 10), size), 0, separator);
                        yPosition = yPosition + 10 + 10;
                }
        }
    }

    painter.end();

    setPixmap(QPixmap::fromImage(image));

    m_CurrentImage = image;

    m_Text = text;

    /*
    static int s4 = 1;
    qDebug() << "pasta constructor: " << s4 << "  create: " << this;
    s4++;
    */
}

Pasta::~Pasta()
{
    /*
    static int s4 = 1;"num = " << num <<
    qDebug() << "pasta destructor: "<< s4 << "  delete: " << this;
    s4++;
    */
}

QString Pasta::getText() const
{
    return m_Text;
}

int Pasta::getNumber() const
{
    return m_PastaNumber;
}


void Pasta::mouseDoubleClickEvent(QMouseEvent* _event)
{

    QFontMetrics metric(font());
    QSize met_size = metric.size(Qt::TextSingleLine, m_Text);

    QString cols_num;
    cols_num.setNum(m_ColsNumber);

    QFont font;
    font.setStyleStrategy(QFont::ForceOutline);

    font.setPixelSize(14);

    QString new_family = Holy::kFontFamily;
    font.setFamily(new_family);

    QPainter ppainter;
    ppainter.begin(&m_CurrentImage);
    ppainter.setRenderHint(QPainter::Antialiasing);

    if(is_PastaDoubleClicked)
    {
         ppainter.setBrush(QColor("#ef7877"));
         is_PastaDoubleClicked = false;
    }
    else
    {
        ppainter.setBrush(QColor("#aaaaaa"));
        is_PastaDoubleClicked = true;
    }


    ppainter.drawRoundedRect(QRectF(0.5, 0.5, m_CurrentImage.width()-1, m_CurrentImage.height()-1),  3, 3, Qt::RelativeSize);

    ppainter.setFont(font);
    ppainter.setBrush(Qt::black);

    QString separator = "-----";
    int yPosition = 6;
    for(int i = -1; i < m_Text.length(); i++)
    {
        if(i == -1 )
        {
                ppainter.setBrush(QColor("#aaaaaa"));
                ppainter.drawText(QRect(QPoint(6, yPosition     ), met_size), 0, cols_num);
                ppainter.drawText(QRect(QPoint(6, yPosition + 10), met_size), 0, separator);
                yPosition = yPosition + 10 + 10;
                ppainter.setBrush(Qt::black);
        }
        else if (i == m_Text.length() -1)
        {
                QString tmp;
                tmp.push_back(m_Text[i]);
                ppainter.drawText(QRect(QPoint(6, yPosition     ), met_size), 0, tmp);
        }
        else
        {
                QString tmp;
                tmp.push_back(m_Text[i]);
                ppainter.drawText(QRect(QPoint(6, yPosition     ), met_size), 0, tmp);
                if(m_Text.length() > 1)
                {
                        ppainter.drawText(QRect(QPoint(6, yPosition + 10), met_size), 0, separator);
                        yPosition = yPosition + 10 + 10;
                }
        }
    }

    ppainter.end();

    setPixmap(QPixmap::fromImage(m_CurrentImage));

}
