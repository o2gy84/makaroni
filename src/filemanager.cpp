
#include "filemanager.h"

#include <QtGui>

FileManager::FileManager(QWidget* _parent) :
        QWidget(_parent)
{
    QSize size (50, 50);

    m_Open = new QPushButton("open", _parent);
    //m_Open->resize(size);
    //m_Open->show();

    m_Save = new QPushButton("save", _parent);
    //m_Save->resize(size);

    m_Close = new QPushButton("close", _parent);
    //m_Close->resize(size);
    //m_Close->show();

    connect(m_Open, SIGNAL(clicked()), this, SLOT(slotButtonOpenClicked()));
    connect(m_Save, SIGNAL(clicked()), this,  SLOT(slotButtonSaveClicked()));
    connect(m_Close, SIGNAL(clicked()), qApp, SLOT(quit()));

    m_Open->move(0,0);
    m_Save->move(100, 0);
    m_Close->move(200, 0);

    /*
    QHBoxLayout* layout = new QHBoxLayout;

    layout->addWidget(m_Open);
    layout->addWidget(m_Save);
    layout->addWidget(m_Close);
    this->setLayout(layout);
    */

    m_EncryptedText = "";
    m_TextPresence = false;

}

FileManager::~FileManager()
{

}

void FileManager::slotButtonOpenClicked()
{

    QString name = QFileDialog::getOpenFileName(0, "Choosing file", "", "*.txt");

    ifstream is(name.toStdString().c_str(), std::ios::in );
    string file ( (std::istreambuf_iterator<char>(is)), std::istreambuf_iterator<char>() );

    m_EncryptedText = file;


//#ifdef WIN32
  //  qDebug() << "WINDOWS!!!";
    // QString tmp = QString::toLocal8Bit(file.c_str(), file.size());
    // m_EncryptedText = tmp.toStdString();
//#else
  //  qDebug() << "LINUX!!!\n";
    QString tmp = QString::fromLocal8Bit(file.c_str(), file.size());
    m_EncryptedText = tmp.toStdString();
//#endif


    m_TextPresence = true;

    emit textLoaded(15);
}

void FileManager::slotButtonSaveClicked()
{

    //QString name = QFileDialog::getSaveFileName(0, "Saving file", "", "*.txt");
    //ofstream os(name.toStdString().c_str(), std::ios::out | std::ios::trunc);

    emit textSaved();


}


