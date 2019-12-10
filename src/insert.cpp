#include "insert.h"
#include "ui_insert.h"

Insert::Insert(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Insert)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("Insertion"));

    //加载配置文件
    Json jsonConfig;
    dbPath =jsonConfig.getValue("db");
    theme = jsonConfig.getValue("theme");

    this->setStyleSheet(theme);
    db_ptr=new Database("insert",dbPath);

    maxId = db_ptr->getMaxId("data");
    ui->opLineId->setText(QString::number(maxId));

    ui->opComboType->addItems(db_ptr->getListData("type"));
    ui->opComboLevel->addItems(db_ptr->getListData("level"));
    ui->opComboSubject->addItems(db_ptr->getListData("subject"));
}

Insert::~Insert()
{
    delete ui;
}

void Insert::operation(int id)
{
    ui->opLineId->setText(QString::number(id));

    m_sql = QString("select * from data where id= %1").arg(id);
    db_ptr->ptr_query->prepare(m_sql);
    if(db_ptr->ptr_query->exec()){
        while(db_ptr->ptr_query->next()){
            ui->opTextQ->setPlainText(db_ptr->ptr_query->value("question").toString());
            ui->opTextA->setPlainText(db_ptr->ptr_query->value("answer").toString());
            ui->opPlainTips->setPlainText(db_ptr->ptr_query->value("tip").toString());
        }
    }
    else {
        qDebug()<<"Cannot query detail from data table :"<<db_ptr->ptr_query->lastError();
    }

    m_sql = QString("select * from property where id=%1").arg(id);
    db_ptr->ptr_query->prepare(m_sql);
    if(db_ptr->ptr_query->exec()){
        while(db_ptr->ptr_query->next()){
            ui->opPlainCata->setPlainText(db_ptr->ptr_query->value("keyword").toString());
            ui->opComboType->setCurrentText(db_ptr->ptr_query->value("type").toString());
            ui->opComboLevel->setCurrentText(db_ptr->ptr_query->value("level").toString());
            ui->opComboSubject->setCurrentText(db_ptr->ptr_query->value("subject").toString());
        }
    }
    else {
        qDebug()<<"Cannot query detail from property table :"<<db_ptr->ptr_query->lastError();
    }
}

void Insert::on_opButtonUpdate_clicked()
{
    m_sql.clear();
    m_sql = QString("update data set question=:question,answer=:answer,tip=:tip where id=:id");
    db_ptr->ptr_query->prepare(m_sql);
    db_ptr->ptr_query->bindValue(":question",ui->opTextQ->toPlainText());
    db_ptr->ptr_query->bindValue(":answer",ui->opTextA->toPlainText());
    db_ptr->ptr_query->bindValue(":tip",ui->opPlainTips->toPlainText());
    db_ptr->ptr_query->bindValue(":id",ui->opLineId->text().toInt());
    if(db_ptr->ptr_query->exec()){
        qDebug()<<"Update data in data table";
    }
    else{
        QMessageBox::information(this,tr("Update"),QString("Cannot update data into db: %1").arg(db_ptr->ptr_query->lastError().text())
                                 ,QMessageBox::Ok);
    }

    m_sql = QString("update property set type=:type,subject=:subject,keyword=:keyword,status=:status,level=:level where id=:id");
    db_ptr->ptr_query->prepare(m_sql);
    db_ptr->ptr_query->bindValue(":type",ui->opComboType->currentText());
    db_ptr->ptr_query->bindValue(":subject",ui->opComboSubject->currentText());
    db_ptr->ptr_query->bindValue(":keyword",ui->opPlainCata->toPlainText());
    db_ptr->ptr_query->bindValue(":status",0);
    db_ptr->ptr_query->bindValue(":level",ui->opComboLevel->currentText());
    db_ptr->ptr_query->bindValue(":id",ui->opLineId->text().toInt());
    if(db_ptr->ptr_query->exec()){
        qDebug()<<"Update data in property table";
        on_opButtonClear_clicked();
    }
    else{
        QMessageBox::information(this,tr("Update"),QString("Cannot update data into db: %1").arg(db_ptr->ptr_query->lastError().text())
                                 ,QMessageBox::Ok);
    }
    on_opButtonClose_clicked();
}

void Insert::on_opButtonInsert_clicked()
{
    m_sql.clear();
    m_sql=QString("insert into data values(:id,:question,:answer,:tip)");
    db_ptr->ptr_query->prepare(m_sql);
    db_ptr->ptr_query->bindValue(":id",++maxId);
    db_ptr->ptr_query->bindValue(":question",ui->opTextQ->toPlainText());
    db_ptr->ptr_query->bindValue(":answer",ui->opTextA->toPlainText());
    db_ptr->ptr_query->bindValue(":tip",ui->opPlainTips->toPlainText());
    if(db_ptr->ptr_query->exec()){
        maxId++;
        qDebug()<<"Inserted data into db";
    }

    m_sql=QString("insert into property values(:id,:type,:subject,:keyword,:status,:level)");
    db_ptr->ptr_query->prepare(m_sql);
    db_ptr->ptr_query->bindValue(":id",maxId);
    db_ptr->ptr_query->bindValue(":type",ui->opComboType->currentText());
    db_ptr->ptr_query->bindValue(":subject",ui->opComboSubject->currentText());
    db_ptr->ptr_query->bindValue(":keyword",ui->opPlainCata->toPlainText());
    db_ptr->ptr_query->bindValue(":status",ui->opSpinStatus->value());
    db_ptr->ptr_query->bindValue(":level",ui->opComboLevel->currentText());
    if(db_ptr->ptr_query->exec()){
        qDebug()<<"Inserted data into db";
    }

    on_opButtonClear_clicked();
}

void Insert::on_opButtonClear_clicked()
{
    ui->opTextA->clear();
    ui->opTextQ->clear();
    ui->opPlainCata->clear();
    ui->opPlainTips->clear();

    maxId = db_ptr->getMaxId("data");
    ui->opLineId->setText(QString::number(maxId));

    qDebug()<<"Clear data done";
}

void Insert::on_opButtonClose_clicked()
{
    this->close();
}
