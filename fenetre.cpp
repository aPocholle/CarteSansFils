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

