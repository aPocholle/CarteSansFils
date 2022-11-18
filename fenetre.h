#ifndef FENETRE_H
#define FENETRE_H

#include <QMainWindow>

#include "MfErrNo.h"
#include "Core.h"
#include "Sw_Device.h"
#include "Sw_Mf_Classic.h"
#include "Sw_Poll.h"
#include "Sw_ISO14443A-3.h"
#include "TypeDefs.h"
#include "Tools.h"

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

<<<<<<< Updated upstream
=======
    void on_buton_ID_clicked();

    void on_buton_Payer_clicked();

>>>>>>> Stashed changes
private:
    Ui::Fenetre *ui;
};
#endif // FENETRE_H
