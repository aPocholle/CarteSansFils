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

int card_read(uint8_t sect_count);

BOOL bench = FALSE;

boolean sT = false;



void Fenetre::on_But_Con_clicked()
{
    MonLecteur.Type = ReaderCDC;
    MonLecteur.device = 0;
    status = OpenCOM(&MonLecteur);

    qDebug() << "OpenCOM" << status;


    status = Version(&MonLecteur);
    ui->Affichage->setText(MonLecteur.version);
    ui->Affichage->update();


    qDebug() << "status du reader" << status;
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

    status = Version(&MonLecteur);
        if (status == MI_OK){
            qDebug() << "Reader firwmare is " << MonLecteur.version;
            qDebug() << "Reader serial is " << MonLecteur.serial[0] << MonLecteur.serial[1] << MonLecteur.serial[2] << MonLecteur.serial[3];
            qDebug() << "Reader stack is " << MonLecteur.stack;
        }
     status = LEDBuzzer(&MonLecteur, LED_YELLOW_ON);

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


void Fenetre::on_but_Carte_clicked()
{
    RF_Power_Control(&MonLecteur, TRUE, 0);
    while(ISO14443_3_A_PollCard(&MonLecteur, atq, sak, uid, &uid_len)){
        qDebug() << "En attente d'une carte";
    }; // en attente d'une carte
    if (status != MI_OK){
            qDebug() << "No available tag in RF field";
        }

        qDebug() << "Tag found: UID= ";
        for (i = 0; i < uid_len; i++)
             qDebug() << uid[i];
        qDebug() << "ATQ= " << atq[1] << atq[0];
        qDebug() << "SAK= " << sak[0];
}


void Fenetre::on_buton_Payer_clicked()
{
    QString Unite = ui->NB_unit->toPlainText();
    int nb_unite = Unite.toInt();
    int decr = ui->nb_unit_decr->value();
    nb_unite=nb_unite-decr;
    ui->NB_unit->setText(QString::number(nb_unite));
    writing();
}

void Fenetre::writing(){

    char DataIn[16];
    sprintf(DataIn, ui->Nom->toPlainText().toUtf8().data(), 16);

    Mf_Classic_Write_Block(&MonLecteur, TRUE, 10, (uint8_t*)DataIn, AuthKeyA, 2);

}



void Fenetre::on_buton_ID_clicked()
{   ISO14443_3_A_PollCard(&MonLecteur, atq, sak, uid, &uid_len);
    char DataIn[16];
    //Nom
    sprintf(DataIn, ui->Nom->toPlainText().toUtf8().data(), 16);
    Mf_Classic_Write_Block(&MonLecteur, TRUE, 10, (uint8_t*)DataIn, AuthKeyA, 2);

    //Prenom
    sprintf(DataIn, ui->Prenom->toPlainText().toUtf8().data(), 16);
    Mf_Classic_Write_Block(&MonLecteur, TRUE, 9, (uint8_t*)DataIn, AuthKeyA, 2);

    //Identité
    sprintf(DataIn, ui->Identite->title().toUtf8().data(), 16);
    Mf_Classic_Write_Block(&MonLecteur, TRUE, 8, (uint8_t*)DataIn, AuthKeyA, 2);

    qDebug() << "info";

}
