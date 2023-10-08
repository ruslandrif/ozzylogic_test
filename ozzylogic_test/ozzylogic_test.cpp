#include "ozzylogic_test.hpp"
#include "operator_edit_dialog.hpp"
#include <QMessageBox>
#include <QHeaderView>

using namespace tst::view;

ozzylogic_test::ozzylogic_test(QWidget *parent)
    : QWidget(parent), treeview_(new QTreeView(this))
{
    setLayout(layout_ = new QVBoxLayout());
    treeview_->viewport()->setMouseTracking(true);
    add_op_button_ = new QPushButton("+");
    add_op_button_->setToolTip("Add new operator");

    layout_->addWidget(treeview_);
    layout_->addWidget(add_op_button_);

    p_db_ = std::make_unique<tst::db::c_database>("system.db");
    if (!p_db_->init()) {
        QMessageBox::critical(this, "Error", p_db_->last_error().c_str(), QMessageBox::Ok);
        exit(-1);
    }

    check_folder_existence("Countries");
    check_folder_existence("Operators");

    standardModel = new tst::model::operators_model(this);
    standardModel->setColumnCount(3);
    setMinimumSize(500, 700);

    update_ui();

    connect_slots();
}

void ozzylogic_test::update_ui() {
    list_operators = p_db_->load_vector_countries();
    standardModel->clear();

    delegate_ = std::make_unique<tree_item_delegate>(list_operators, treeview_);

    treeview_->header()->setVisible(false);
    treeview_->setItemDelegate(delegate_.get());
    treeview_->setModel(standardModel);

    QStandardItem* rootNode = standardModel->invisibleRootItem();

    for (const auto& c : list_operators) {
        auto* country_item = get_country_st_item(c);

        rootNode->appendRow({ country_item, new QStandardItem("")});
    }

    treeview_->header()->resizeSections(QHeaderView::ResizeMode::ResizeToContents);
    treeview_->header()->setStretchLastSection(false);
}

QStandardItem* ozzylogic_test::get_country_st_item(const tst::country& c) {
    auto* itm = new QStandardItem(c.name().c_str());

    itm->setIcon(c.icon());
    itm->setEditable(false);
    itm->setData(QVariant(QString::fromStdString(c.code())), countries_data_roles::country_code);

    for (const auto& op : c.operators()) {
        auto* itm_op = new QStandardItem(std::format("{} ({}_{})", op.name().c_str(), op.mcc(), op.mnc()).c_str());

        itm_op->setTextAlignment(Qt::AlignmentFlag::AlignLeft);
        itm_op->setToolTip(itm_op->text());

        itm_op->setIcon(op.icon());

        itm_op->setData(op.mcc(), countries_data_roles::mcc);
        itm_op->setData(op.mnc(), countries_data_roles::mnc);
        itm_op->setData(QString::fromStdString(op.name()), countries_data_roles::operator_name);
        itm_op->setData(QString::fromStdString(c.code()), countries_data_roles::country_code);

        itm->appendRow({ itm_op, new QStandardItem("") });
    }

    return itm;
}

void ozzylogic_test::connect_slots() {
    connect(treeview_, &QAbstractItemView::clicked, this, [&](const QModelIndex& idx) {
        if (idx.parent().isValid() && idx.column() == tst::plus_button) {
            auto* item = standardModel->itemFromIndex(standardModel->index(idx.row(), tst::element, idx.parent()));
            const int mcc = item->data(static_cast<int>(countries_data_roles::mcc)).toInt();
            const int mnc = item->data(static_cast<int>(countries_data_roles::mnc)).toInt();
            on_operator_clicked(mcc, mnc);
        }
    });

    connect(p_db_.get(), &tst::db::c_database::table_updated, this, &ozzylogic_test::update_ui); //TODO update not all tree view, but only needed row

    connect(p_db_.get(), &tst::db::c_database::error, this, [&]() {
        QMessageBox::critical(this, "Database error", p_db_->last_error().c_str(), QMessageBox::Ok, QMessageBox::Close);
    });

    connect(treeview_, &QAbstractItemView::doubleClicked, this, &ozzylogic_test::on_treeview_double_click);
    connect(add_op_button_, &QPushButton::clicked, this, [&]() {
        auto* dialog = new tst::control::operator_edit_dialog("Add operator", list_operators, this);
        connect(dialog, &tst::control::operator_edit_dialog::operator_action_sig, p_db_.get(), &tst::db::c_database::operator_perform_action);
        dialog->add_operator();
    });
}

void ozzylogic_test::on_treeview_double_click(const QModelIndex& idx) {
    if (idx.parent().isValid() && idx.column() == 0) {
        QStandardItemModel* model = (QStandardItemModel*)idx.model();
        auto* item = model->itemFromIndex(idx);

        const int mcc = item->data(static_cast<int>(countries_data_roles::mcc)).toInt();
        const int mnc = item->data(static_cast<int>(countries_data_roles::mnc)).toInt();
        const std::string oper_name = item->data(static_cast<int>(countries_data_roles::operator_name)).toString().toStdString();

        mob_operator oper{ mcc, mnc, oper_name };

        auto* dialog = new tst::control::operator_edit_dialog("Edit operator", list_operators, this);
        connect(dialog, &tst::control::operator_edit_dialog::operator_action_sig, p_db_.get(), &tst::db::c_database::operator_perform_action);
        dialog->edit_op(oper);
    }
}

void ozzylogic_test::on_operator_clicked(int mcc, int mnc) {
    qDebug() << "Operator with mcc =" << mcc << "and mnc =" << mnc << "clicked. Empty slot has been called.";
}

void ozzylogic_test::check_folder_existence(std::filesystem::path p) {
    if (!std::filesystem::exists(p)) {
        QMessageBox::critical(this, "Error", QString::fromStdString("Folder \"" + p.string() + "\" does not exist"), QMessageBox::Ok);
        exit(-1);
    }
}

ozzylogic_test::~ozzylogic_test()
{

}

void ozzylogic_test::leaveEvent(QEvent* event) {
    delegate_->disable_all_plus_buttons(standardModel);

    QWidget::leaveEvent(event);
}
