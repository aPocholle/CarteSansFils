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

private:
    Ui::Fenetre *ui;
};
#endif // FENETRE_H
