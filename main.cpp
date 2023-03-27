#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQmlApplicationEngine>

#include <authmanager.h>
#include <listcontroller.h>
#include <mymodel.h>
#include <securemanager.h>
#include <searchfilter.h>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QString path = QCoreApplication::applicationDirPath() + "/data.json";
    SecureManager securemanager(path);
//    securemanager.ParseJson();

    QQmlApplicationEngine engine;
    QQmlContext *context = engine.rootContext();

    authManager auth(path);
    context->setContextProperty("auth", &auth);
     QObject::connect(&auth, &authManager::keyCreated , &securemanager, &SecureManager::onKeyCreated);

    ListController listcontroller;
//    listcontroller.appendItem(securemanager.ParseJson());
    securemanager.mItems = &listcontroller.mItems;
    context->setContextProperty("ListController", &listcontroller);
    QObject::connect(&listcontroller, &ListController::entryCreated, &securemanager, &SecureManager::onEntryCreated);
    QObject::connect(&listcontroller, &ListController::entryDeleted, &securemanager, &SecureManager::onEntryDeleted);
    QObject::connect(&securemanager, &SecureManager::JSONparsed, &listcontroller, &ListController::onJSONparsed);

    MyModel mymodel;
    mymodel.setList(&listcontroller);

    SearchFilter searchFilter(&mymodel, mymodel.siteRole);
    context->setContextProperty("searchFilterModel", &searchFilter);

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
    QObject::connect(dataPage, SIGNAL(entryDeleteClicked()), &listcontroller, SLOT(onEntryDeleteClicked()));

    QObject *formPage = engine.rootObjects().constFirst()->children().at(4);
    QObject::connect(formPage, SIGNAL(checkCreateEntry(QString, QString, QString)), &listcontroller, SLOT(onCheckCreateEntry(QString, QString, QString)));

    return app.exec();
}
