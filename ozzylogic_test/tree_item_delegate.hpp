#pragma once
#include <QStyledItemDelegate>
#include <QStandardItemModel>
#include "common.hpp"

namespace tst::view {
    class tree_item_delegate : public QStyledItemDelegate
    {
        Q_OBJECT
    public:
        tree_item_delegate(const std::vector<country>& recs, QObject* parent);

        QSize sizeHint(const QStyleOptionViewItem& option,
            const QModelIndex& index) const override;

        bool editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option,
            const QModelIndex& index) override;

        void disable_all_plus_buttons(QStandardItemModel* model);
    private:
        void flash_plus_button(const QModelIndex& index);
        std::vector<country> recs_;
    };
}
