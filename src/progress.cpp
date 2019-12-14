#include "progress.h"
#include "ui_progress.h"

Progress::Progress(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Progress)
{
    ui->setupUi(this);

    //加载配置文件
    Json jsonConfig;
    dbPath =jsonConfig.getValue("db");
    theme = jsonConfig.getValue("theme");
    currentId = jsonConfig.getValue("id").toInt();

    qDebug()<<"dbPath is: "<<dbPath<<" \ntheme name is: "<<theme;

    QString styleSheet = loadTheme(theme);
    this->setStyleSheet(styleSheet);

    db_ptr = new Database("progress",dbPath);

    m_printer = new QPrinter;
    m_painter = new QPainter;
    m_x = 0;
    m_y = 0;
}

Progress::~Progress()
{
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
    QString html = "";
    html += "<html>";
    html += "<head>";
    html += "<title>NJUCS-exercise配套题库</title>";
    html += "<head>";
    html += "<body bgcolor=\"#ccccff\">";

    qDebug()<<"sql is: "<<m_sql;
    db_ptr->ptr_query->prepare(m_sql);
    if(db_ptr->ptr_query->exec()){
        while(db_ptr->ptr_query->next()){
            html += db_ptr->ptr_query->value("id").toString();
            html += "<hr>";
        }
    }

    html += "<hr>";
    html += "</body>";
    html += "</html>";

    return html;
}

void Progress::run()
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

    QPrinter printer;
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(pdfPath);
    QTextDocument document;
    QString html = saveHtmlToPDF();
    document.setHtml(html);
    document.print(&printer);
    document.end();
}

void Progress::on_pushButton_clicked()
{
    QString filePath = QFileDialog::getSaveFileName(this,tr("Export"),QDir::currentPath(),
                                                    QString("*.pdf\n*.docx\n*"));
    if(filePath.isEmpty()){
        qDebug()<<"Select nothing";
        return;
    }
    qDebug()<<"Exportion path is: "<<filePath;
}
