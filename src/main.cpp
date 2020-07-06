#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <memory>

#include "tableViewModel.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    QQmlApplicationEngine engine;

    qmlRegisterType<TableModel>("TableModel", 0, 1, "TableModel");

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
