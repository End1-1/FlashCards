#ifndef DOWNLOAD_H
#define DOWNLOAD_H

#include <QObject>

class Download : public QObject
{
    Q_OBJECT
public:
    explicit Download(QObject *parent = nullptr);

public slots:
    void downloadData();

signals:
    void newServer(const QString &name);

private slots:
    void parseResponse(bool error, const QString &str);
    void parseFile(bool error, const QString &str);

};

#endif // DOWNLOAD_H
