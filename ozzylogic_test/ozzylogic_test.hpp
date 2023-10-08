#pragma once

#include <QLayout>
#include <QTreeView>
#include <QPushButton>
#include <QStandardItemModel>
#include <memory>
#include "c_database.hpp"
#include "operators_model.h"
#include "tree_item_delegate.hpp"

namespace tst::view {
    class ozzylogic_test : public QWidget
    {
        Q_OBJECT

    public:
        ozzylogic_test(QWidget* parent = nullptr);
        ~ozzylogic_test();

    protected:
        void leaveEvent(QEvent* event);
    private slots:
        void on_treeview_double_click(const QModelIndex& idx);
        void on_operator_clicked(int mcc, int mnc);
        void update_operator(mob_operator, operator_action);
    private:
        void update_ui();

        void check_folder_existence(std::filesystem::path p);

        void connect_slots();

        QStandardItem* get_country_st_item(const tst::country& c);

        QTreeView* treeview_{ nullptr };

        tst::model::operators_model* standardModel{ nullptr };

        QVBoxLayout* layout_{ nullptr };

        QPushButton* add_op_button_;

        std::vector<country> list_operators;

        std::unique_ptr<tst::db::c_database> p_db_;

        std::unique_ptr<tree_item_delegate> delegate_;
    };
}