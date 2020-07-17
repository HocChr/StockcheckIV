#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtQuickControls2/QQuickStyle>
#include <QQuickStyle>

#include <memory>

#include "tableViewModel.h"

QT_CHARTS_USE_NAMESPACE

int main(int argc, char** argv)
{
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);

    QQuickStyle::setStyle("Material");

    QQmlApplicationEngine engine;
    qmlRegisterType<TableModel>("TableModel", 0, 1, "TableModel");
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    app.setWindowIcon(QIcon("Stock-icon.png"));

    auto configuration = ConfigurationHandler::getConfiguration();

    // prepare the database-list for setting it to gui
    QVariantList databaseList;
    for(const auto& item : configuration.AVAILABLE_DATABASES)
    {
        databaseList << item.c_str();
    }
    // prepare current database
    QVariant currentDatabase = configuration.CURRENT_DATABASE.c_str();

    // init gui with available databases and current database
    QObject *rootObject = qobject_cast<QObject*>(engine.rootObjects().first());
    QMetaObject::invokeMethod(rootObject, "addToMenu",
                              Q_ARG(QVariant, QVariant::fromValue(databaseList)),
                              Q_ARG(QVariant, QVariant::fromValue(currentDatabase)));

    return app.exec();
}
