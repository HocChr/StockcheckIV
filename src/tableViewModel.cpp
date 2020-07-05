#include "tableViewModel.h"

#include <QDebug>

TableViewModel::TableViewModel(QObject* parent) : QAbstractTableModel(parent) {

    //SimpleData* m_simpledata;
    //
    //// Create junk model data
    //m_simpledata = new SimpleData(true, "a1", "b1");
    //m_data_list.append(m_simpledata);
    //m_simpledata = new SimpleData(false, "a2", "b2");
    //m_data_list.append(m_simpledata);
}

int TableViewModel::rowCount(const QModelIndex& /*parent*/) const {
    return 3;
    //return m_data_list.length();
}

int TableViewModel::columnCount(const QModelIndex& /*parent*/) const {
    return 3;
}

QVariant TableViewModel::data(const QModelIndex& index, int role) const {

    //switch (role)
    //{
    //case RoleOne:
    //    return m_data_list.at(index.row())->enable;
    //case RoleTwo:
    //    return m_data_list.at(index.row())->m_one;
    //case RoleThree:
    //    return m_data_list.at(index.row())->m_two;
    //}
    //
    return QVariant();
}

bool TableViewModel::setData(const QModelIndex& index, const QVariant& value, int role) {

    qDebug() << "setData() called with:" << value;

    //switch (role)
    //{
    //case RoleOne:
    //    m_data_list[index.row()]->enable = value.toBool();
    //case RoleTwo:
    //    m_data_list[index.row()]->m_one = value.toString();
    //case RoleThree:
    //    m_data_list[index.row()]->m_two = value.toString();
    //}

    return true;
}

QHash<int, QByteArray> TableViewModel::roleNames() const {

    QHash<int, QByteArray> roles;
    roles[RoleOne] = "role_enable";
    roles[RoleTwo] = "role_m_one";
    roles[RoleThree] = "role_m_two";
    return roles;
}

void TableViewModel::print_rows() {

    qDebug() << "Model Data:";

    //for (int i = 0; i < m_data_list.length(); i++) {
    //    qDebug() << "At" << i << ":" << m_data_list.at(i)->enable << m_data_list.at(i)->m_one << m_data_list.at(i)->m_two;
    //}

}
