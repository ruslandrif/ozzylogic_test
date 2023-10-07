#include "tree_item_delegate.hpp"
#include "common.hpp"
#include <filesystem>
#include <QStandardItemModel>
#include <QPainter>
#include <QStandardItem>
#include <QTreeView>
#include <QIcon>
#include <QEvent>

tree_item_delegate::tree_item_delegate(const std::vector<country>& recs, QObject* parent) : QStyledItemDelegate(parent), recs_(recs) {

}

QSize tree_item_delegate::sizeHint(const QStyleOptionViewItem& option,
    const QModelIndex& index) const {

    auto ret = QStyledItemDelegate::sizeHint(option, index);

    if (index.parent().isValid() && index.column() == 1) { //  + button 
        ret.setWidth(30);
    }
    return ret;
}

bool tree_item_delegate::editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option,
    const QModelIndex& index) {
    QStandardItemModel* standard_model = (QStandardItemModel*)index.model();
    if (index.parent().isValid()) {
        if (event->type() == QEvent::Type::MouseMove) {
            disable_all_plus_buttons(standard_model);

            flash_plus_button(index);
        }
    }
    else {
        disable_all_plus_buttons(standard_model);
    }
    return QStyledItemDelegate::editorEvent(event, model, option, index);
}

void tree_item_delegate::flash_plus_button(const QModelIndex& index) {
    QStandardItemModel* standard_model = (QStandardItemModel*)index.model();
    QStandardItem* item = standard_model->itemFromIndex(index);

    const int country_rows_cnt = standard_model->itemFromIndex(index.parent())->rowCount();

    for (int i = 0; i < country_rows_cnt; ++i) {
        const auto idx = standard_model->index(i, 1, index.parent());
        QStandardItem* item = standard_model->itemFromIndex(idx);

        if (i == index.row()) {
            item->setText("+");
            item->setTextAlignment(Qt::AlignHCenter);
            item->setBackground(Qt::gray);
        }
        else {
            item->setText("");
            item->setBackground(Qt::transparent);
        }
    }
}

void tree_item_delegate::disable_all_plus_buttons(QStandardItemModel* model) {
    for (int i = 0; i < model->rowCount(); ++i) {
        const auto idx = model->index(i, 0);
        auto* itm = model->itemFromIndex(idx);

        for (int j = 0; j < itm->rowCount(); ++j) {
            const auto chuld_idx = model->index(j, 1, idx);
            auto* itm_child = model->itemFromIndex(chuld_idx);
            itm_child->setText("");
            itm_child->setBackground(Qt::transparent);
        }

    }

}