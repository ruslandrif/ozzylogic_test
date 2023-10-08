#include "operators_model.h"

using namespace tst::model;

Qt::ItemFlags operators_model::flags(const QModelIndex& index) const {
    return Qt::ItemFlag::ItemIsEnabled;
}

QModelIndex operators_model::index(int row, int column, const QModelIndex& parent) const {
    return QStandardItemModel::index(row,column, parent);
}

QModelIndex operators_model::parent(const QModelIndex& child) const  {
    return QStandardItemModel::parent(child);
}

int operators_model::rowCount(const QModelIndex& parent) const {
    return QStandardItemModel::rowCount(parent);
}

int operators_model::columnCount(const QModelIndex& parent) const {
    return QStandardItemModel::columnCount(parent);
}

QVariant operators_model::data(const QModelIndex& index, int role) const {
   
    return QStandardItemModel::data(index, role);
}


