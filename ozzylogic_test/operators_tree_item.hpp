#pragma once

#include <QWidget>
#include <QEvent>
#include <QLabel>
#include <QLayout>
#include <QPushButton>
#include <filesystem>
#include "common.hpp"
class operators_tree_item : public QWidget {
	Q_OBJECT
public:
	operators_tree_item(const std::string& name, int mcc, int mnc, QWidget* parent = nullptr);

	bool event(QEvent* event) override;

	inline void set_button_visible(bool visible) { plus_button_->setVisible(true); }
private:
	QHBoxLayout* layout_{ nullptr };
	QLabel* op_photo_lbl_{ nullptr };
	QLabel* op_name_label{ nullptr };
	QPushButton* plus_button_{ nullptr };
};
