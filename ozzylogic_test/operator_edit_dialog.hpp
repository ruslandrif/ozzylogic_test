#pragma once
#include <QDialog>
#include <QLayout>
#include <QLineEdit>
#include <QLabel>
#include <QFont>
#include <QValidator>
#include <QPushButton>
#include "common.hpp"

namespace tst::control {
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

		bool values_entered() const { 
			return !mcc_edit->text().isEmpty() && !mnc_edit_->text().isEmpty() && !name_edit_->text().isEmpty(); 
		}

		QHBoxLayout* top_label_l_{ nullptr };
		QHBoxLayout* name_l_{ nullptr };
		QHBoxLayout* mcc_l_{ nullptr };
		QHBoxLayout* mnc_l_{ nullptr };
		QHBoxLayout* buttons_l{ nullptr };

		QVBoxLayout* main_layout_{ nullptr };

		QLabel* name_lbl_{ nullptr };
		QLabel* mcc_lbl_{ nullptr };
		QLabel* mnc_lbl_{ nullptr };
		QLabel* main_lbl{ nullptr };
		QLabel* country_flag_lbl{ nullptr };
		QLabel* op_logo_lbl{ nullptr };

		QLineEdit* name_edit_{ nullptr };
		QLineEdit* mnc_edit_{ nullptr };
		QLineEdit* mcc_edit{ nullptr };

		QPushButton* ok_b{ nullptr };
		QPushButton* cancel_b{ nullptr };

		operator_action action_{ operator_action::unk };

		std::vector<country> records_;

		int mcc_top_border = 1000;
		int mnc_top_border = 1000;
	};

}