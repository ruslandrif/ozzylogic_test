#pragma once

#include <QLayout>
#include <QTreeView>
#include <QPushButton>
#include <QStandardItemModel>
#include <memory>
#include "ui_ozzylogic_test.h"
#include "c_database.hpp"
#include "operators_model.h"
#include "tree_item_delegate.hpp"


class ozzylogic_test : public QWidget
{
    Q_OBJECT

public:
    ozzylogic_test(QWidget *parent = nullptr);
    ~ozzylogic_test();

private slots:
    void on_treeview_double_click(const QModelIndex& idx);
    void on_operator_clicked(int mcc, int mnc);
private:
    void update_ui();
    

    void check_folder_existence(std::filesystem::path p);

    void connect_slots();

    QTreeView* treeview_{ nullptr };

    operators_model* standardModel{ nullptr };

    QVBoxLayout* layout_{ nullptr };

    QPushButton* add_op_button_;

    std::vector<country> list_operators;

    c_database* p_db_{ nullptr };
    std::unique_ptr<tree_item_delegate> delegate_;
};