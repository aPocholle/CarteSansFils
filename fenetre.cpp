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

//---------------------------------------------------------------------------------------------------------------------//
void Fenetre::on_But_Con_clicked()
{
    if(!sT){
        MonLecteur.Type = ReaderCDC;
        MonLecteur.device = 0;
        status = OpenCOM(&MonLecteur);

        if(status == 0){
            sT = true;
            status = RF_Power_Control(&MonLecteur, TRUE, 0);

            status = Version(&MonLecteur);
            ui->Affichage->setText(MonLecteur.version);
            ui->Affichage->update();

            status = Version(&MonLecteur);
                if (status == MI_OK){
                    qDebug() << "Reader firwmare is " << MonLecteur.version;
                    qDebug() << "Reader serial is " << MonLecteur.serial[0] << MonLecteur.serial[1] << MonLecteur.serial[2] << MonLecteur.serial[3];
                    qDebug() << "Reader stack is " << MonLecteur.stack;
                }
                qDebug() << "Connecté";
             status = LEDBuzzer(&MonLecteur, LED_RED_ON);
        }
    }
}
void Fenetre::on_but_Disc_clicked()
{

    status = RF_Power_Control(&MonLecteur, FALSE, 0);
    if(status == 0){
        sT = false;
        buzzer();
        status = CloseCOM(&MonLecteur);
        qDebug() << "Deconnecté";
        ui->Affichage->setText("");
        ui->Affichage->update();
        status = LEDBuzzer(&MonLecteur, LED_GREEN_ON);

    }

}
//---------------------------------------------------------------------------------------------------------------------//
void Fenetre::on_But_Quit_clicked()
{

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
//---------------------------------------------------------------------------------------------------------------------//
void Fenetre::on_buton_ID_clicked()
{
    status = ISO14443_3_A_PollCard(&MonLecteur, atq, sak, uid, &uid_len);
    if (status != MI_OK){
            qDebug() << "Pas de carte à lire";
        }
    else{
        qDebug() << "Mise à jour des informations de la carte.";
        buzzer();
        writing();

    }

}
//---------------------------------------------------------------------------------------------------------------------//
void Fenetre::on_buton_Payer_clicked()
{
    uint32_t value = ui->nb_unit_decr->value();
    if(ui->nb_unit_decr->value() !=0){
        status = ISO14443_3_A_PollCard(&MonLecteur, atq, sak, uid, &uid_len);
        if (status != MI_OK){
                qDebug() << "Pas de carte à lire";
            }
        else{
            qDebug() << "Décrément de" << value << "voyages.";
            buzzer();
            decrement(value);
        }
    }

}
//---------------------------------------------------------------------------------------------------------------------//
void Fenetre::on_but_Carte_clicked()
{

    status = ISO14443_3_A_PollCard(&MonLecteur, atq, sak, uid, &uid_len);
    if (status != MI_OK){
            qDebug() << "Pas de carte à lire";
        }
    else{
        qDebug() << "Carte numéro: " << uid[0] << uid[1] << uid[2] << uid[3];
        buzzer();
        reading();
    }
}
//---------------------------------------------------------------------------------------------------------------------//
void Fenetre::on_buton_Charger_clicked()
{
    uint32_t value = ui->nb_unit_incr->value();
    if(ui->nb_unit_incr->value() !=0){
        status = ISO14443_3_A_PollCard(&MonLecteur, atq, sak, uid, &uid_len);
        if (status != MI_OK){
                qDebug() << "Pas de carte à lire";
            }
        else{
            qDebug() << "Ajout de" << value << "voyages.";
            buzzer();
            increment(value);
        }
    }
}

void Fenetre::on_but_Restore_clicked()
{
    status = ISO14443_3_A_PollCard(&MonLecteur, atq, sak, uid, &uid_len);
    if (status != MI_OK){
            qDebug() << "Pas de carte à lire";
        }
    else{
        qDebug() << "Informations restorées";
        buzzer();
        backup();
    }
}
//---------------------------------------------------------------------------------------------------------------------//
void Fenetre::buzzer(){
    status = LEDBuzzer(&MonLecteur, LED_GREEN_ON+LED_YELLOW_ON+LED_RED_ON+LED_GREEN_ON);
    DELAYS_MS(10);
    status = LEDBuzzer(&MonLecteur, LED_RED_ON);
}

//---------------------------------------------------------------------------------------------------------------------//

void Fenetre::writing(){
    char DataIn[16];
    sprintf(DataIn,ui->Nom->toPlainText().toUtf8().data(),16);
    status = Mf_Classic_Write_Block(&MonLecteur, TRUE, 10, (uint8_t*)DataIn, AuthKeyB, 2);
    sprintf(DataIn,ui->Prenom->toPlainText().toUtf8().data(),16);
    status = Mf_Classic_Write_Block(&MonLecteur, TRUE, 9, (uint8_t*)DataIn, AuthKeyB, 2);

    QString val = "Identité";
    sprintf(DataIn,val.toUtf8().data(),16);
    status = Mf_Classic_Write_Block(&MonLecteur, TRUE, 8, (uint8_t*)DataIn, AuthKeyB, 2);

    val = "Porte Monnaie";
    sprintf(DataIn,val.toUtf8().data(),16);
    status = Mf_Classic_Write_Block(&MonLecteur, TRUE, 12, (uint8_t*)DataIn, AuthKeyB, 3);

    qDebug() << "Informations écritent sur la carte.";
}
//---------------------------------------------------------------------------------------------------------------------//
void Fenetre::reading(){
    char dataText[240] = {0};
    status = Mf_Classic_Read_Block(&MonLecteur, TRUE, 10, (uint8_t*)dataText, AuthKeyA, 2);
    ui->Nom->setText((char*)dataText);
    status = Mf_Classic_Read_Block(&MonLecteur, TRUE, 9, (uint8_t*)dataText, AuthKeyA, 2);
    ui->Prenom->setText((char*)dataText);
    uint32_t dataNum = 0;
    status = Mf_Classic_Read_Value(&MonLecteur, TRUE, 14, &dataNum, AuthKeyA, 3);
    ui->NB_unit->setText(QString::number(dataNum));
    qDebug() << "Informations lues depuis la carte.";
}

//---------------------------------------------------------------------------------------------------------------------//

void Fenetre::decrement(uint32_t value){
    //Recuperation de la valeur backup
    uint32_t dataNum = 0;
    status = Mf_Classic_Read_Value(&MonLecteur, TRUE, 14, &dataNum, AuthKeyA, 3);
    status = Mf_Classic_Write_Value(&MonLecteur, TRUE, 13, dataNum, AuthKeyB, 3);

    // Decrementation
    status = Mf_Classic_Decrement_Value(&MonLecteur, TRUE, 14, value, 14, AuthKeyB, 3);
    if(status == 0){
        ui->NB_unit->setText(QString::number(ui->NB_unit->text().toInt() - value));
    }
}

void Fenetre::increment(uint32_t value){
    //Recuperation de la valeur backup
    uint32_t dataNum = 0;
    status = Mf_Classic_Read_Value(&MonLecteur, TRUE, 14, &dataNum, AuthKeyA, 3);
    status = Mf_Classic_Write_Value(&MonLecteur, TRUE, 13, dataNum, AuthKeyB, 3);

    // Incrementation
    status = Mf_Classic_Increment_Value(&MonLecteur, TRUE, 14, value, 14, AuthKeyB, 3);
    if(status == 0){
        ui->NB_unit->setText(QString::number(ui->NB_unit->text().toInt() + value));
    }
}

void Fenetre::backup(){
    status = Mf_Classic_Restore_Value(&MonLecteur, TRUE, 13, 14, AuthKeyB, 3);
    if(status == 0){
        uint32_t dataNum = 0;
        Mf_Classic_Read_Value(&MonLecteur, TRUE, 14, &dataNum, AuthKeyA, 3);
        ui->NB_unit->setText(QString::number(dataNum));
    }
}
//---------------------------------------------------------------------------------------------------------------------//
