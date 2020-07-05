#ifndef TABLEVIEWMODEL_H
#define TABLEVIEWMODEL_H

#include <qqml.h>
#include <QAbstractTableModel>

// This is a sample Model, taken from the qt-website
class TableModel : public QAbstractTableModel
{
    Q_OBJECT
    QML_ELEMENT
    QML_ADDED_IN_MINOR_VERSION(1)

public:
    int rowCount(const QModelIndex & = QModelIndex()) const override
    {
        return 3;
    }

    int columnCount(const QModelIndex & = QModelIndex()) const override
    {
        return 8;
    }

    QVariant data(const QModelIndex &index, int role) const override
    {
        switch (role) {
            case Qt::DisplayRole:
                return QString("%1, %2").arg(index.column()).arg(index.row());
            default:
                break;
        }

        return QVariant();
    }

    QHash<int, QByteArray> roleNames() const override
    {
        return { {Qt::DisplayRole, "display"} };
    }
};

class TableViewModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit TableViewModel(QObject* parent = 0);

    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    int columnCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);
    QHash<int, QByteArray> roleNames() const;

    //QList<SimpleData*> m_data_list;

    enum MyRoles {
        RoleOne = Qt::UserRole + 1,
        RoleTwo,
        RoleThree
    };

signals:

public slots:
    void print_rows();
};

#endif // TABLEVIEWMODEL_H
