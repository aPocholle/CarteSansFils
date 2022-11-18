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



void Fenetre::on_But_Con_clicked()
{
    int16_t status = MI_OK;
    MonLecteur.Type = ReaderCDC;
    MonLecteur.device = 0;
    status = OpenCOM(&MonLecteur);
    qDebug() << "OpenCOM" << status;


    status = Version(&MonLecteur);
    ui->Affichage->setText(MonLecteur.version);
    ui->Affichage->update();


    status = Version(&MonLecteur);

    ui->Affichage->setText(MonLecteur.version);
    ui->Affichage->update();

}

void Fenetre::on_But_Quit_clicked()
{
    int16_t status = MI_OK;
    RF_Power_Control(&MonLecteur, FALSE, 0);
    status = LEDBuzzer(&MonLecteur, LED_OFF);
    status = CloseCOM(&MonLecteur);
    qApp->quit();
}
<<<<<<< Updated upstream
=======

void Fenetre::on_buton_ID_clicked()
{
    QString Nom = ui->Nom->toPlainText();
    QString Prenom = ui->Prenom->toPlainText();
}

void Fenetre::on_buton_Payer_clicked()
{
    QString Unite = ui->NB_unit->toPlainText();
    int nb_unite = Unite.toInt();
    int decr = ui->nb_unit_decr->value();
    nb_unite=nb_unite-decr;
    ui->NB_unit->setText(QString::number(nb_unite));

}
>>>>>>> Stashed changes
