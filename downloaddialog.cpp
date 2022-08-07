#include "downloaddialog.h"
#include "ui_downloaddialog.h"
#include "download.h"
#include <QThread>

DownloadDialog *DownloadDialog::fInstance = nullptr;

DownloadDialog::DownloadDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DownloadDialog)
{
    ui->setupUi(this);
    ui->tbl->setColumnWidths(ui->tbl->columnCount(), 50, 200);
}

DownloadDialog::~DownloadDialog()
{
    delete ui;
}

void DownloadDialog::startDownload()
{
    Download *d = new Download();
    connect(d, &Download::newServer, this, &DownloadDialog::newServerName);
    QThread *t = new QThread();
    connect(t, &QThread::started, d, &Download::downloadData);
    d->moveToThread(t);
    t->start();
}

DownloadDialog *DownloadDialog::instance()
{
    if (fInstance == nullptr) {
        fInstance = new DownloadDialog();
    }
    return fInstance;
}

void DownloadDialog::newServerName(const QString &name)
{
    int r = ui->tbl->addEmptyRow();
    ui->tbl->setString(r, 1, name);
}
