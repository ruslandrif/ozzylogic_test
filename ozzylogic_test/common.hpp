#pragma once

#include <string>
#include <vector>
#include <filesystem>
#include <QIcon>
#include <QPushButton>
#include <QTreeView>
#include <QScrollBar>

struct mob_operator {
public:
	mob_operator() = delete;

	mob_operator(int mcc, int mnc, const std::string& name) : mcc_(mcc), mnc_(mnc), name_(name) {
		icon_ = QIcon(search_icon(mcc_, mnc_).string().c_str());
	}

	QIcon icon() const { return icon_.isNull() ? QIcon(QPixmap(":/ozzylogic_test/question.png")) : icon_; }
	int mcc() const { return mcc_; }
	int mnc() const { return mnc_; }
	std::string name() const { return name_; }

	static std::filesystem::path search_icon(int mcc, int mnc) {
		std::filesystem::path res = std::filesystem::path("Operators") / std::format("{}_{}.png", mcc, mnc);
		
		return std::filesystem::exists(res) ? res : std::filesystem::path();
	}

private:
	int mcc_{ 0 };
	int mnc_{ 0 };
	std::string name_;
	QIcon icon_;
};

struct country {
	country() = delete;

	country(const std::string& name, const std::string& country_code, int mcc) : name_(name), code_(country_code)
	{
		icon_ = QIcon(search_icon(code_).string().c_str());
		mcc_.push_back(mcc);
	}

	void add_operator(const mob_operator& op) {
		operators_.emplace_back(op);
	}

	bool has_mcc(int m) const {
		return std::find(mcc_.begin(), mcc_.end(), m) != mcc_.end();
	}

	void add_mcc(int m) { mcc_.push_back(m); }

	std::string code() const { return code_; }
	std::string name() const { return name_; }
	std::vector<int> mcc() const { return mcc_; }
	
	std::vector<mob_operator> operators() const { return operators_; }

	QIcon icon() const { return icon_.isNull() ? QIcon(QPixmap(":/ozzylogic_test/question.png")) : icon_; }

	static std::filesystem::path search_icon(const std::string& country_code) {
		std::filesystem::path res = std::filesystem::path("Countries") / std::format("{}.png", country_code);
		
		return std::filesystem::exists(res) ? res : std::filesystem::path();
	}

private:
	std::string name_;
	std::string code_;
	std::vector<int> mcc_;
	std::vector<mob_operator> operators_;

	QIcon icon_;
};

enum class operator_action {
	add,
	edit,
	unk
};

enum class countries_data_roles {
	country_code = Qt::UserRole + 1,
	mcc,
	mnc,
	operator_name
};
