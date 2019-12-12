#ifndef EXPORT_H
#define EXPORT_H

#include "base.hpp"

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
