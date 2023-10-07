#include "operators_tree_item.hpp"

operators_tree_item::operators_tree_item(const std::string& name, int mcc, int mnc, QWidget* parent) : QWidget(parent) {
	std::filesystem::path op_logo_path;
	for (const auto& p : std::filesystem::directory_iterator("Operators")) {
		if (p.path().stem() == std::format("{}_{}", mcc, mnc)) {
			op_logo_path = p.path();
			break;
		}
	}

	if (std::filesystem::exists(op_logo_path)) {
		layout_ = new QHBoxLayout();
		setLayout(layout_);

		op_name_label = new QLabel(name.c_str());
		op_photo_lbl_ = new QLabel;
		op_photo_lbl_->setPixmap(QPixmap(op_logo_path.string().c_str()));
		layout_->addWidget(op_photo_lbl_);
		layout_->addWidget(op_name_label);

		plus_button_ = new QPushButton("+");
		plus_button_->setStyleSheet("background-color: rgba(255, 255, 255, 0);");
		plus_button_->setVisible(false);
		layout_->addWidget(plus_button_, 0, Qt::AlignLeft);
		layout_->addStretch(1);
	}

	setWindowFlags(Qt::FramelessWindowHint);
	setAttribute(Qt::WA_NoSystemBackground);
	setAttribute(Qt::WA_TranslucentBackground);
	setAttribute(Qt::WA_TransparentForMouseEvents);
}

bool operators_tree_item::event(QEvent* event) {
	//qDebug() << "Operator item mouse move";

	if (event->type() == QEvent::Type::MouseMove) {
		qDebug() << "Mouse move";
	}

	return QWidget::event(event);
}