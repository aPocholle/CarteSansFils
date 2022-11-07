#ifndef FENETRE_H
#define FENETRE_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class Fenetre; }
QT_END_NAMESPACE

class Fenetre : public QMainWindow
{
    Q_OBJECT

public:
    Fenetre(QWidget *parent = nullptr);
    ~Fenetre();

private slots:
    void on_But_Con_clicked();

    void on_But_Quit_clicked();

private:
    Ui::Fenetre *ui;
};
#endif // FENETRE_H
