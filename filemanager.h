#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QPushButton>
#include <QWidget>
#include <QFileDialog>

#include <string>
using std::string;

#include <fstream>
using std::ifstream;
using std::ofstream;


class FileManager: public QWidget
{
    Q_OBJECT

public:

    FileManager(QWidget* parent);
    ~FileManager();

    string getEncryptedText()       {return m_EncryptedText;}
    bool isTextPresence()           {return m_TextPresence;}

protected:



private:

    QPushButton* m_Open;
    QPushButton* m_Save;
    QPushButton* m_Close;

    string m_EncryptedText;

    bool m_TextPresence;

signals:
    void textLoaded(int _num_cols);
    void textSaved();

public slots:
    void slotButtonOpenClicked();
    void slotButtonSaveClicked();

};


#endif // FILEMANAGER_H
