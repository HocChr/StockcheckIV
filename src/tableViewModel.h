#ifndef TABLEVIEWMODEL_H
#define TABLEVIEWMODEL_H

#include <QAbstractTableModel>
#include <qqml.h>
#include <QtQml>

#include "stockcheck/stockcheck.h"
#include "stockcheck/idatabase.h"

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

private:
    std::vector<StockEntity> _data;
};

#endif // TABLEVIEWMODEL_H
