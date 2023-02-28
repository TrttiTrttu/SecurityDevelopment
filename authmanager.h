#ifndef AUTHMANAGER_H
#define AUTHMANAGER_H

#include <QObject>
#include <qDebug>

class authManager : public QObject
{
    Q_OBJECT
public:
    explicit authManager(QObject *parent = nullptr);
    bool checkPin(QString pin);

signals:
    void pinChecked(bool res);

public slots:
    void onCheckPin(QString pin);

private:
    const QString mPin = "123";

};

#endif // AUTHMANAGER_H
