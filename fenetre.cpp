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
#include <string.h>
#include <stdio.h>


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
    MonLecteur.Type = ReaderCDC;
    MonLecteur.device = 0;
    status = OpenCOM(&MonLecteur);
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
     status = LEDBuzzer(&MonLecteur, LED_GREEN_ON);

}

void Fenetre::on_But_Quit_clicked()
{
    int16_t status = MI_OK;
    RF_Power_Control(&MonLecteur, TRUE, 0);
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
    nom = ui->Nom->toPlainText();
    prenom = ui->Prenom->toPlainText();


}

void Fenetre::on_buton_Payer_clicked()
{
    QString unite = ui->NB_unit->toPlainText();
    nb_unite = unite.toInt();


}



void Fenetre::on_but_Carte_clicked()
{
    //RF_Power_Control(&MonLecteur, TRUE, 0);
    while(ISO14443_3_A_PollCard(&MonLecteur, atq, sak, uid, &uid_len)){
        qDebug() << "En attente d'une carte";
    };
    if (status != MI_OK){
            qDebug() << "No available tag in RF field";
        }

        qDebug() << "Tag found: UID= ";
        for (i = 0; i < uid_len; i++)
             qDebug() << uid[i];
        qDebug() << "ATQ= " << atq[1] << atq[0];
        qDebug() << "SAK= " << sak[0];
        qDebug() << "\n";
        status = LEDBuzzer(&MonLecteur, LED_GREEN_ON+LED_YELLOW_ON+LED_RED_ON+LED_GREEN_ON);
        DELAYS_MS(10);
        status = LEDBuzzer(&MonLecteur, LED_RED_ON);
        reading();
}

void Fenetre::reading(){
    for (sect = 0; sect < sect_count; sect++){

        bloc = 0;

            memset(data, 0x00, 240);
            status = Mf_Classic_Read_Sector(&MonLecteur, TRUE, sect, data, AuthKeyA, 0);
                    if (sect < 32)
                        bloc_count = 3;
                    else

                        bloc_count = 15;
                    for (bloc = 0; bloc < bloc_count; bloc++){
                        QString Val = "";
                        QString Val2 = "";
                        if(bloc == 0){
                            Val += "Nom: ";
                            Val2 += "Nom: ";
                        }
                        else if(bloc == 1){
                            Val += "Prenom: ";
                            Val2 += "Prenom: ";
                        }
                        else{
                            Val += "Identité: ";
                            Val2 += "Identité: ";
                        }
                        for (offset = 0; offset < 16; offset++){
                            char val_int[5] = "";
                            sprintf(val_int,"%u ",data[16 * bloc + offset]);
                            Val += val_int;

                        }
                        for (offset = 0; offset < 16; offset++){
                            if (data[16 * bloc + offset] >= ' '){
                                char val_int[5] = "";
                                sprintf(val_int,"%u ",data[16 * bloc + offset]);
                                Val2 += val_int;
                            }
                            else
                                Val2 += ". ";

                        }
                        qDebug() << Val ;
                        qDebug() << Val2 ;

            }

        }
}
