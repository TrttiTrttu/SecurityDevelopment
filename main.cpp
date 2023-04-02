#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQmlApplicationEngine>

#include "windows.h"
#include <authmanager.h>
#include <listcontroller.h>
#include <mymodel.h>
#include <securemanager.h>
#include <searchfilter.h>

int main(int argc, char *argv[])
{
    // Определить начало сегмента .text
    unsigned long long moduleBase = (unsigned long long)GetModuleHandle(NULL);
    unsigned long long text_segment_start = moduleBase + 0x1000;
    // Определить его длину
    PIMAGE_DOS_HEADER pIDH = reinterpret_cast<PIMAGE_DOS_HEADER>(moduleBase);
    PIMAGE_NT_HEADERS pINH = reinterpret_cast<PIMAGE_NT_HEADERS>(moduleBase + pIDH->e_lfanew);
    unsigned long long size_of_text = pINH -> OptionalHeader.SizeOfCode; // размер .text

    QByteArray text_segment_contents = QByteArray((char*)text_segment_start, size_of_text);
    QByteArray current_hash_raw = QCryptographicHash::hash(text_segment_contents, QCryptographicHash::Sha256);
    QByteArray current_hash = current_hash_raw.toBase64();

    const QByteArray hash_correct = QByteArray("CshG9QKRjVNlRkOZFP92LyexYU0LXNm7InP8uPfU63U=");

    qDebug() <<  current_hash;

    bool is_edited = (hash_correct != current_hash);
     qDebug() <<  is_edited;

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
    QObject::connect(&auth, &authManager::keyCreated , &listcontroller, &ListController::onKeyCreated);

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

    if (is_edited) {
        emit auth.alertPopup("Hash is not in the right state!");
    }

//    if (IsDebuggerPresent()) {
//        emit auth.alertPopup("Debugger is present!");
//    }

    QObject *authPage = engine.rootObjects().constFirst()->children().at(2);
    QObject::connect(authPage, SIGNAL(checkPin(QString)), &auth, SLOT(onCheckPin(QString)));

    QObject *dataPage = engine.rootObjects().constFirst()->children().at(3);
    QObject::connect(dataPage, SIGNAL(entryDeleteClicked()), &listcontroller, SLOT(onEntryDeleteClicked()));
    QObject::connect(dataPage, SIGNAL(copyToBuffer(int, QString)), &listcontroller, SLOT(onCopyToBuffer(int, QString)));

    QObject *formPage = engine.rootObjects().constFirst()->children().at(4);
    QObject::connect(formPage, SIGNAL(checkCreateEntry(QString, QString, QString)), &listcontroller, SLOT(onCheckCreateEntry(QString, QString, QString)));

    return app.exec();
}
