#include "progress.h"
#include "ui_progress.h"

Progress::Progress(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Progress)
{
    ui->setupUi(this);

    isCanced = false;

    //加载配置文件
    Json jsonConfig;
    dbPath =jsonConfig.getValue("db");
    theme = jsonConfig.getValue("theme");
    currentId = jsonConfig.getValue("id").toInt();

    qDebug()<<"dbPath is: "<<dbPath<<" \ntheme name is: "<<theme;

    QString styleSheet = loadTheme(theme);
    this->setStyleSheet(styleSheet);
    this->setWindowTitle(tr("Export"));
    ui->progressBar->setValue(0);

    db_ptr = new Database("progress",dbPath);
    ui->comboBoxSubject->addItems(db_ptr->getListData("subject"));
    ui->comboBoxType->addItems(db_ptr->getListData("type"));
    ui->comboBoxLevel->addItems(db_ptr->getListData("level"));
    ui->comboBoxCatalog->addItems(db_ptr->getListData("catalog"));

    m_printer = new QPrinter;
    m_x = 0;
    m_y = 0;

    generateList(generateSQL());
}

Progress::~Progress()
{
    delete m_printer;
    delete db_ptr;
    delete ui;
}

//************************************
// 函数名称:setPdfName
// 函数说明：设置PDF文件名字并启动绘制
// 作    者：ISmileLi
//************************************
void Progress::setPdfName(QString fileName)
{
    m_printer->setPageSize(QPrinter::A4);
    m_printer->setOutputFormat(QPrinter::PdfFormat);
    m_printer->setOutputFileName(fileName);
    m_painter->begin(m_printer);
}
//************************************
// 函数名称:writeTextToPdf
// 函数说明：写入文字到PDF
// 作    者：ISmileLi
//************************************
void Progress::writeTextToPdf(QString text)
{
    if(m_printer == nullptr)
        return;

    int height = 10;
    int width = 300;
    m_painter->drawText(m_x,m_y,width,height,0,text);
    m_y += height;
}
//************************************
// 函数名称:insertPictureToPdf
// 函数说明：插入图片到PDF
// 作    者：ISmileLi
//************************************
void Progress::insertPictureToPdf(QString picFile)
{
    if(m_printer == nullptr)
        return;

    QPixmap *pixmap = new QPixmap(picFile);

    int width = pixmap->width();
    int height = pixmap->height();
    m_painter->drawPixmap(m_x,m_y,width,height,*pixmap);
    m_y += height;
}
//************************************
// 函数名称:insertPictureToPdf
// 函数说明：结束PDF绘制操作
// 作    者：ISmileLi
//************************************
void Progress::endPainter()
{
    m_painter->end();
}
//************************************
// 函数名称:saveHtmlToPDF
// 函数说明：html方法生成PDF
// 作    者：ISmileLi
//************************************
QString Progress::saveHtmlToPDF()
{
    int count = 0;
    QString html = "";
    html += "<html>";
    html += "<head>";
    html += "<title>NJUCS-exercise配套题库</title>";
    html += "<head>";
    html += "<body bgcolor=\"#ffffff\">";

    QList<int> idList;
    qDebug()<<"sql is: "<<m_sql;
    db_ptr->ptr_query->prepare(m_sql);
    if(db_ptr->ptr_query->exec()){
        while(db_ptr->ptr_query->next()){
            idList.append(db_ptr->ptr_query->value("id").toInt());
        }
    }

    //根据设置选择适合的SQL语句
    m_sql.clear();
    //只有问题题目
    m_sql = QString("select question from data where id='%1'");
    if(ui->checkBoxAnswer->isChecked()){
        m_sql=QString("select question,answer from data where id = '%1'");
    }
    if(ui->checkBoxTips->isChecked()){
        m_sql=QString("select question,answer,tip from data where id='%1'");
    }

    foreach(int id,idList){
        QString sqltemp = m_sql.arg(id);
        db_ptr->ptr_query->prepare(sqltemp);
        if(db_ptr->ptr_query->exec()){
            while(db_ptr->ptr_query->next()&&!isCanced){
                html += QString::number(id)+". " + db_ptr->ptr_query->value("question").toString();
                html += "<hr>";
                ui->lcdNumberDone->display(++count);
                ui->progressBar->setValue((count*100)/total);
                sleep(1000);
            }
        }
    }

    html += "<hr>";
    html += "</body>";
    html += "</html>";

    return html;
}

QString Progress::generateSQL()
{
    m_sql = QString("select id from property ");
    int status =0;

    switch(ui->comboBoxSubject->currentIndex()){
    case 0:
        break;
    case 1:
        m_sql += QString("where subject = '%1'").arg(ui->comboBoxSubject->currentText());
        status =1;
    }

    switch(ui->comboBoxType->currentIndex()){
    case 0:
        break;
    default:
    {
        if(status){
            m_sql += QString("and type = '%1'").arg(ui->comboBoxType->currentText());
        }
        else{
            m_sql += QString("where type = '%1'").arg(ui->comboBoxType->currentText());
        }
    }
    }

    switch(ui->comboBoxLevel->currentIndex()){
    case 0:
        break;
    default:
    {
        if(status){
            m_sql += QString("and level = '%1'").arg(ui->comboBoxLevel->currentText());
        }
        else{
            m_sql += QString("where level = '%1'").arg(ui->comboBoxLevel->currentText());
        }
    }
    }

    switch(ui->comboBoxCatalog->currentIndex()){
    case 0:
        break;
    default:
    {
        if(status){
            m_sql += QString("and catalog = '%1'").arg(ui->comboBoxCatalog->currentText());
        }
        else{
            m_sql += QString("where catalog = '%1'").arg(ui->comboBoxCatalog->currentText());
        }
    }
    }

    QString keywords = ui->lineKeyword->text();
    if(!keywords.isEmpty()){
        m_sql += QString("and keywords like '%%1%'").arg(keywords);
    }

    qDebug()<<"Current sql is: "<<m_sql;

    return m_sql;
}

void Progress::generateList(QString sql)
{
    total=0;
    db_ptr->ptr_query->prepare(sql);
    if(db_ptr->ptr_query->exec()){
        while(db_ptr->ptr_query->next()){
            total++;
        }
    }

    ui->lcdNumberTotal->display(total);
}

void Progress::on_buttonChoose_clicked()
{
    pdfPath = QFileDialog::getSaveFileName(this,tr("Export"),QDir::currentPath(),
                                                    QString("*.pdf\n*.docx\n*"));
    if(pdfPath.isEmpty()){
        qDebug()<<"Select nothing";
        return;
    }
    ui->lineEdit->setText(pdfPath);
}

void Progress::on_buttonStart_clicked()
{
    /*
    setPdfName("E:/testPDF.pdf");
    QStringList testList = (QStringList()<<"测试PDF文字写入" << "测试PDF写入hello word");
    foreach(QString text,testList)
    {
        writeTextToPdf(text);
        qDebug() << "text:" <<text;
    }
    insertPictureToPdf("E:/Monitor.png");
    insertPictureToPdf("E:/map.png");
    insertPictureToPdf("E:/girl.jpg");
    endPainter();
    */
    if(pdfPath.isEmpty()) {
        QMessageBox::information(this,tr("Warning"),tr("Please choose a file first"),QMessageBox::Ok);
        return;
    }

    QPrinter printer;
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(pdfPath);
    QTextDocument document;
    QString html = saveHtmlToPDF();
    document.setHtml(html);
    document.print(&printer);
    document.end();
}

void Progress::on_buttonClose_clicked()
{
    this->close();
}

void Progress::on_comboBoxSubject_currentIndexChanged(int index)
{
    Q_UNUSED(index)
    generateList(generateSQL());
}

void Progress::on_comboBoxType_currentIndexChanged(int index)
{
    Q_UNUSED(index)
    generateList(generateSQL());
}

void Progress::on_comboBoxLevel_currentIndexChanged(int index)
{
    Q_UNUSED(index)
    generateList(generateSQL());
}

void Progress::on_comboBoxCatalog_currentIndexChanged(int index)
{
    Q_UNUSED(index)
    generateList(generateSQL());
}

void Progress::on_lineKeyword_textChanged(const QString &keyword)
{
    Q_UNUSED(keyword)
    generateList(generateSQL());
}

void Progress::on_checkBoxTips_stateChanged(int status)
{
    if(status) ui->checkBoxAnswer->setChecked(true);
    else ui->checkBoxAnswer->setChecked(false);
}

void Progress::on_buttonCance_clicked()
{
    isCanced = true;
}
