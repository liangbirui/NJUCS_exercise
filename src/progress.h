#ifndef PROGRESS_H
#define PROGRESS_H

#include <base.hpp>
#include "database.hpp"
#include "json.hpp"

namespace Ui {
class Progress;
}

class Progress : public QWidget
{
    Q_OBJECT

public:
    explicit Progress(QWidget *parent = nullptr);
    ~Progress();

    /// 设置PDF文件名字
    void setPdfName(QString fileName);
    /// 写入文字到pdf上
    void writeTextToPdf(QString text);
    /// 插入图片到PDF上
    void insertPictureToPdf(QString picFile);
    /// 结束绘制
    void endPainter();
    /// html转化为PDF
    QString saveHtmlToPDF();

private slots:
    void on_buttonChoose_clicked();

    void on_buttonStart_clicked();

private:
    Ui::Progress *ui;
    Database *db_ptr;

    QPrinter *m_printer;
    QPainter *m_painter;

    QString pdfPath;
    QString dbPath;
    QString theme;
    int currentId;
    QString m_sql;
    int m_x;
    int m_y;
};

#endif // PROGRESS_H
