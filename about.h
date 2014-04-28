
#include <QPushButton>
#include <QWidget>
#include <QDialog>
#include <QMessageBox>

class AboutProgramm: public QWidget
{
    Q_OBJECT
public:

    AboutProgramm(QWidget* _par);

    ~AboutProgramm();

protected:

private:

    QPushButton* m_About;

public slots:

    void showAboutInfo();

};
