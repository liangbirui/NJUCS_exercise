#ifndef INSERT_H
#define INSERT_H

#include "database.hpp"
#include "json.hpp"
#include "base.hpp"
#include "similarity.hpp"

namespace Ui {
class Insert;
}

class Insert : public QWidget
{
    Q_OBJECT

public:
    explicit Insert(QWidget *parent = nullptr);
    ~Insert();

    void operation(int id);
    void dispalyDataById(int id);
    void setLineId(int id);//设置界面显示ID
    int getLineId();//获取界面上面的当前ID

private slots:
    void on_opButtonUpdate_clicked();

    void on_opButtonInsert_clicked();

    void on_opButtonClear_clicked();

    void on_opButtonClose_clicked();

    void on_opButtonPrevious_clicked();

    void on_opButtonNext_clicked();

private:
    Ui::Insert *ui;

    Database *db_ptr;
    Similarity *pSim;

    QString m_sql;
    QString dbPath;
    QString theme;

    int maxId;
    int currentId;
};

#endif // INSERT_H
