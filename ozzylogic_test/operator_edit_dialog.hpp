#pragma once
#include <QDialog>
#include <QLayout>
#include <QLineEdit>
#include <QLabel>
#include <QFont>
#include <QValidator>
#include <QPushButton>
#include "common.hpp"


class operator_edit_dialog : public QDialog {
	Q_OBJECT
public:
	operator_edit_dialog(const std::string& name, const std::vector<country>& recs, QWidget* parent);

	void edit_op(const mob_operator& op);

	void add_operator();
signals:
	void operator_action_sig(mob_operator, operator_action);
private slots:
	void emit_new_operator();

private:
	void search_icons();

	QHBoxLayout *top_label_l_{ nullptr }, *name_l_{ nullptr }, *mcc_l_{ nullptr }, *mnc_l_{ nullptr }, *buttons_l{nullptr};
	QVBoxLayout* main_layout_{ nullptr };
	QLabel *name_lbl_, *mcc_lbl_, *mnc_lbl_, *main_lbl, *country_flag_lbl, *op_logo_lbl;
	QLineEdit *name_edit_, *mnc_edit_, *mcc_edit;
	QPushButton *ok_b, *cancel_b;

	operator_action action_{ operator_action::unk };

	std::vector<country> records_;

	int mcc_top_border = 1000;
	int mnc_top_border = 1000;
};