#include "operator_edit_dialog.hpp"
#include <QMessageBox>

using namespace tst::control;

operator_edit_dialog::operator_edit_dialog(const std::string& name, const std::vector<country>& recs, QWidget* parent) : QDialog(parent), records_(recs) {
	setWindowTitle(name.c_str());

	main_layout_ = new QVBoxLayout(this);

	top_label_l_ = new QHBoxLayout;
	name_l_ = new QHBoxLayout;
	mcc_l_ = new QHBoxLayout;
	mnc_l_ = new QHBoxLayout;
	buttons_l = new QHBoxLayout;

	main_layout_->addLayout(top_label_l_);
	main_layout_->addLayout(name_l_);
	main_layout_->addLayout(mcc_l_);
	main_layout_->addLayout(mnc_l_);
	main_layout_->addLayout(buttons_l);

	name_lbl_ = new QLabel("Name");
	mcc_lbl_ = new QLabel("MCC");
	mnc_lbl_ = new QLabel("MNC");
	main_lbl = new QLabel("Operator editor");
	main_lbl->setStyleSheet("font-weight: bold;");

	country_flag_lbl = new QLabel;
	op_logo_lbl = new QLabel;

	name_edit_ = new QLineEdit;
	mnc_edit_ = new QLineEdit;
	mcc_edit = new QLineEdit;

	mnc_edit_->setValidator(new QIntValidator(0, mnc_top_border, this));
	mcc_edit->setValidator(new QIntValidator(0, mcc_top_border, this));

	ok_b = new QPushButton("Save");
	cancel_b = new QPushButton("Cancel");


	top_label_l_->insertWidget(0, main_lbl, 1);
	top_label_l_->insertWidget(0, op_logo_lbl, 1);
	top_label_l_->setAlignment(main_lbl, Qt::AlignLeft);
	top_label_l_->setAlignment(op_logo_lbl, Qt::AlignLeft);
	top_label_l_->addStretch(1);
	mcc_l_->addWidget(mcc_lbl_);
	mcc_l_->addWidget(mcc_edit);
	mcc_l_->addWidget(country_flag_lbl);

	mnc_l_->addWidget(mnc_lbl_);
	mnc_l_->addWidget(mnc_edit_);

	name_l_->addWidget(name_lbl_);
	name_l_->addWidget(name_edit_);

	buttons_l->addWidget(ok_b);
	buttons_l->addWidget(cancel_b);

	search_icons();

	connect(ok_b, &QPushButton::clicked, this, &operator_edit_dialog::emit_new_operator);
	connect(cancel_b, &QPushButton::clicked, this, &QDialog::reject);

	connect(mcc_edit, &QLineEdit::textChanged, this, [&](const QString& s) {
		search_icons();
	});

	connect(mnc_edit_, &QLineEdit::textChanged, this, [&](const QString& s) {
		search_icons();
	});
}

void operator_edit_dialog::edit_op(const mob_operator& op) {
	name_edit_->setText(op.name().c_str());
	mcc_edit->setText(QString::number(op.mcc()));
	mnc_edit_->setText(QString::number(op.mnc()));

	mnc_edit_->setEnabled(false);
	mcc_edit->setEnabled(false);

	action_ = operator_action::edit;

	QDialog::exec();
}

void operator_edit_dialog::add_operator() { //add new operator
	mnc_edit_->setEnabled(true);
	mcc_edit->setEnabled(true);

	action_ = operator_action::add;

	QDialog::exec();
}

void operator_edit_dialog::emit_new_operator() {
	if (values_entered()) {
		mob_operator oper(mcc_edit->text().toInt(), mnc_edit_->text().toInt(), name_edit_->text().toStdString());

		emit operator_action_sig(oper, action_);

		accept();
	}
	else {
		QMessageBox::warning(this, "Error", "Please, enter all required values", QMessageBox::Ok);
	}
}

void operator_edit_dialog::search_icons() {
	const QPixmap question_pix = QPixmap(":/ozzylogic_test/question.png");
	bool ok{ false };
	const int mcc = mcc_edit->text().toInt(&ok);
	const int mnc = mnc_edit_->text().toInt(&ok);

	QPixmap country_flag;
	QPixmap op_logo = QPixmap(mob_operator::search_icon(mcc, mnc).string().c_str());

	const auto iter = std::find_if(records_.begin(), records_.end(), [mcc](const auto& cntry) { return cntry.has_mcc(mcc); });
	if (iter != records_.end()) {
		country_flag = QPixmap(iter->icon().pixmap(iter->icon().availableSizes().first()));
	}
	else {
		country_flag = question_pix;
	}

	country_flag = country_flag.isNull() ? question_pix : country_flag;
	op_logo = op_logo.isNull() ? question_pix : op_logo;

	country_flag_lbl->setPixmap(country_flag);
	op_logo_lbl->setPixmap(op_logo);

}