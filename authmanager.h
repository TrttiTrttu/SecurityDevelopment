#ifndef AUTHMANAGER_H
#define AUTHMANAGER_H

#include <QObject>
#include <qDebug>

#include <securemanager.h>

class authManager : public QObject
{
    Q_OBJECT
public:
    authManager(const QString path);

    bool checkPin(QByteArray key);
    SecureManager SM;

signals:
    void pinChecked(bool res);
    void keyCreated(QByteArray key);
    void alertPopup(QString text);

public slots:
    void onCheckPin(QString pin);

private:
    QFile jFile;
    const QString mPin = "123";

};

#endif // AUTHMANAGER_H
