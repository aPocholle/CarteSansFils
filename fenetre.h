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
    void reading();
    void writing();
    void buzzer();
    void decrement(uint32_t value);
    void increment(uint32_t value);
    void backup();

private slots:
    void on_But_Con_clicked();

    void on_But_Quit_clicked();

    void on_buton_ID_clicked();

    void on_buton_Payer_clicked();

    void on_but_Carte_clicked();

    void on_buton_Charger_clicked();

    void on_but_Restore_clicked();

    void on_but_Disc_clicked();

private:
    ReaderName MonLecteur;
    boolean sT = false;
    Ui::Fenetre *ui;
    int16_t status = MI_OK;
    char s_buffer[64];
    uint8_t atq[2];
    uint8_t sak[1];
    uint8_t uid[12];
    uint16_t uid_len = 12;
};
#endif // FENETRE_H
