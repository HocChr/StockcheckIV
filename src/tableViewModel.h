#ifndef TABLEVIEWMODEL_H
#define TABLEVIEWMODEL_H

#include <QAbstractTableModel>
#include <qqml.h>
#include <QtQml>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QMenuBar>

#include "stockcheck/stockcheck.h"
#include "stockcheck/idatabase.h"
#include "configHandler/confighandler.h"
#include "sqliteAccess/sqliteAccess.h"

QT_CHARTS_USE_NAMESPACE

// This is a sample Model, taken from the qt-website
class TableModel : public QAbstractTableModel
{
    Q_OBJECT

public:

    explicit TableModel();

    int rowCount(const QModelIndex & = QModelIndex()) const override;
    int columnCount(const QModelIndex & = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role) const override;

    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE virtual void sort(int column, Qt::SortOrder order = Qt::AscendingOrder) override;

    Q_INVOKABLE void getLineSeriesPerShare(int rowId,
                                           QLineSeries* lineSeriesEarnings,
                                           QLineSeries* scatterSeriesEarnings,
                                           QLineSeries* lineSeriesDividends,
                                           QLineSeries* scatterSeriesDividends,
                                           QValueAxis* xAxis,
                                           QValueAxis* yAxis);

    Q_INVOKABLE void getLineSeriesRevenue(int rowId,
                                          QLineSeries* lineSeriesRevenues,
                                          QLineSeries* scatterSeriesReveues,
                                          QValueAxis* yAxis);

    Q_INVOKABLE QString getTableName(int rowId);

    Q_INVOKABLE void onDatabaseChanged(QString databaseNameNew);

    Q_INVOKABLE void onActualize();

private:

    void databaseChanged(QString databaseNameNew);

private:
    std::vector<StockEntity> _data;

    enum Role {
        Name=Qt::UserRole,
        Year,
        Rate,
        Percentage,
        RevenueGrowth,
        EarningGrowth,
        DividendGrowth,
        PayoutRatio,
        Remark
    };

    std::unique_ptr<SqliteAccess> _dataBase;
};

#endif // TABLEVIEWMODEL_H
