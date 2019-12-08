#ifndef EXPORT_H
#define EXPORT_H

#include <QWidget>
#include <QIcon>

namespace Ui {
class Export;
}

class Export : public QWidget
{
    Q_OBJECT

public:
    explicit Export(QWidget *parent = nullptr);
    ~Export();

private:
    Ui::Export *ui;
};

#endif // EXPORT_H
