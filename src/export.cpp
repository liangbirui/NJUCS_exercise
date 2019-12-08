#include "export.h"
#include "ui_export.h"

Export::Export(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Export)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("Export"));
    this->setWindowIcon(QIcon(":/resources/icons/export.svg"));
}

Export::~Export()
{
    delete ui;
}
