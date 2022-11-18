#include "fenetre.h"
#include "ui_fenetre.h"
#include "MfErrNo.h"
#include "Core.h"
#include "Sw_Device.h"
#include "Sw_Mf_Classic.h"
#include "Sw_Poll.h"
#include "Sw_ISO14443A-3.h"
#include "TypeDefs.h"
#include "Tools.h"
#include <QtGui>


Fenetre::Fenetre(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Fenetre)
{
    ui->setupUi(this);
}

Fenetre::~Fenetre()
{
    delete ui;
}

ReaderName MonLecteur;
boolean sT = false;


void Fenetre::on_But_Con_clicked()
{
    int16_t status = MI_OK;

    MonLecteur.Type = ReaderCDC;
    MonLecteur.device = 0;
    status = OpenCOM(&MonLecteur);
    if(status == 0){
        sT = true;
    }
    if(sT){
        qDebug() << "Connecté";
    }
    else{
        qDebug() << "Deconnecté";
    }
    status = Version(&MonLecteur);

    ui->Affichage->setText(MonLecteur.version);
    ui->Affichage->update();
}

void Fenetre::on_But_Quit_clicked()
{
    int16_t status = MI_OK;
    RF_Power_Control(&MonLecteur, FALSE, 0);
    for(int i = 0; i < 3; i++){
        status = LEDBuzzer(&MonLecteur, LED_GREEN_ON+LED_YELLOW_ON+LED_RED_ON+LED_GREEN_ON);
        DELAYS_MS(75);
        status = LEDBuzzer(&MonLecteur, LED_GREEN_ON);
        DELAYS_MS(75);
    }
    status = CloseCOM(&MonLecteur);
    qApp->quit();
}

void Fenetre::on_buton_ID_clicked()
{
    QString Nom = ui->Nom->toPlainText();
    QString Prenom = ui->Prenom->toPlainText();
}

void Fenetre::on_buton_Payer_clicked()
{
    QString Unite = ui->NB_unit->toPlainText();
    int nb_unite = Unite.toInt();


}
