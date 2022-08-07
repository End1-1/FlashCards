#ifndef DOWNLOADDIALOG_H
#define DOWNLOADDIALOG_H

#include <QDialog>

namespace Ui {
class DownloadDialog;
}

class DownloadDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DownloadDialog(QWidget *parent = nullptr);
    ~DownloadDialog();
    void startDownload();
    static DownloadDialog *instance();

private:
    Ui::DownloadDialog *ui;
    static DownloadDialog *fInstance;

private slots:
    void newServerName(const QString &name);
};

#endif // DOWNLOADDIALOG_H
