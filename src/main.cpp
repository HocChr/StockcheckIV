#include <QApplication>
#include <QQmlApplicationEngine>

#include "stockentity.h"


int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    StockEntity stockEntity("test");
    stockEntity.GetYearData();

    return app.exec();
}
