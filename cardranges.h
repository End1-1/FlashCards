#ifndef CARDRANGES_H
#define CARDRANGES_H

#include "dialog.h"

namespace Ui {
class CardRanges;
}

class CardRanges : public Dialog
{
    Q_OBJECT

public:
    explicit CardRanges(int type);

    ~CardRanges();

    static bool getRange(int type, QString &first, QString &last);

private slots:
    void on_tbl_cellDoubleClicked(int row, int column);

private:
    Ui::CardRanges *ui;

    QString fFirst;
};

#endif // CARDRANGES_H
