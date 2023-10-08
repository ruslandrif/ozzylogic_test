#pragma once
#include <QAbstractProxyModel>
#include <QSqlQueryModel>
#include <QStandardItemModel>

namespace tst::model {
    class operators_model : public QStandardItemModel
    {
        Q_OBJECT
    public:
        operators_model(QObject* parent = 0) : QStandardItemModel(parent) {}

        QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const;
        virtual QModelIndex parent(const QModelIndex& child) const override;
        int rowCount(const QModelIndex& parent) const;
        int columnCount(const QModelIndex& parent) const;
        QVariant data(const QModelIndex& index, int role) const;
        Qt::ItemFlags flags(const QModelIndex& index) const;
    };
}
