#include "about.h"


AboutProgramm::AboutProgramm(QWidget* _par) :
        QWidget(_par)
{

    QSize size (50, 50);

    m_About = new QPushButton("about", _par);

    connect(m_About, SIGNAL(clicked()), this, SLOT(showAboutInfo()));

    m_About->move(900, 0);


}

AboutProgramm::~AboutProgramm()
{

}

void AboutProgramm::showAboutInfo()
{
    //QDialog* dlg = new QDialog;
    //dlg->setModal(true);

    QString window_header = "vertical permutations";
    QString about_info = "todo\r\n";
    about_info += "Developer: o2gy.";

    QString tmp1 = window_header.fromLocal8Bit(window_header.toStdString().c_str(), window_header.size());
    QString tmp2 = about_info.fromLocal8Bit(about_info.toStdString().c_str(), about_info.size());

    QMessageBox::about(0, tmp1, tmp2 );
}

