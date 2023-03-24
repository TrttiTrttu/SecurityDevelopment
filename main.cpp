#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQmlApplicationEngine>

#include <authmanager.h>
#include <listcontroller.h>
#include <mymodel.h>
#include <securemanager.h>

int main(int argc, char *argv[])
{
//    qmlRegisterType<listModel>("LM", 1, 0, "MyListModel");
//    qmlRegisterUncreatableType<listController>("LM", 1, 0, "ListController",
//            QStringLiteral("listController should not be created in QML"));

    QGuiApplication app(argc, argv);

    QString path = QCoreApplication::applicationDirPath() + "/data.json";
    SecureManager securemanager(path);
    securemanager.ParseJson();

    QQmlApplicationEngine engine;
    QQmlContext *context = engine.rootContext();

    authManager auth;
    context->setContextProperty("auth", &auth);

    ListController listcontroller;
    listcontroller.appendItem(securemanager.ParseJson());
    context->setContextProperty("ListController", &listcontroller);

    MyModel mymodel;
    mymodel.setList(&listcontroller);
    context->setContextProperty("MyModel", &mymodel);

    const QUrl url(u"qrc:/main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    QObject *authPage = engine.rootObjects().constFirst()->children().at(2);
    QObject::connect(authPage, SIGNAL(checkPin(QString)), &auth, SLOT(onCheckPin(QString)));

    QObject *dataPage = engine.rootObjects().constFirst()->children().at(3);
    QObject::connect(dataPage, SIGNAL(entryDeleteClicked()), &auth, SLOT(onEntryDeleteClicked()));

    QObject *formPage = engine.rootObjects().constFirst()->children().at(4);
    QObject::connect(formPage, SIGNAL(checkCreateEntry(QString, QString, QString)), &listcontroller, SLOT(onCheckCreateEntry(QString, QString, QString)));

    return app.exec();
}
