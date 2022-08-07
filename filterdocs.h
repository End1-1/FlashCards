#ifndef FILTERDOCS_H
#define FILTERDOCS_H

#include "c5filterwidget.h"

namespace Ui {
class FilterDocs;
}

class FilterDocs : public C5FilterWidget
{
    Q_OBJECT

public:
    explicit FilterDocs(QWidget *parent = nullptr);
    ~FilterDocs();
    virtual QString condition();
    virtual QString conditionText();
    virtual QString filterString();

private:
    Ui::FilterDocs *ui;
};

#endif // FILTERDOCS_H
