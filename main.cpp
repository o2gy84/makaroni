
#include <QtGui>
#include <QApplication>

#include "mainwidget.h"

int main(int argc, char** argv)
{
    Q_INIT_RESOURCE(makaron);

    QApplication app(argc, argv);

    QIcon icon(":/images/makaron.jpg");

    app.setWindowIcon(icon);

    //QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    //QTextCodec::setCodecForTr(QTextCodec::codecForName("IBM-866"));
    //QTextCodec::setCodecForTr(QTextCodec::codecForName("Windows-1251"));

    //QTextCodec *codec = QTextCodec::codecForName("cp1251");
    //QTextCodec::setCodecForCStrings(codec);
    //QTextCodec::setCodecForTr(codec);

    MainWidget mw;
    mw.setWindowTitle("VerticalPermutations");

    QSize size;
    size.setWidth(1000);
    size.setHeight(300);



    mw.setMinimumSize(size);
    //mw.setMaximumSize(size);

    mw.show();


    return app.exec();
}
