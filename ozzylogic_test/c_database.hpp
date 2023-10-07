#pragma once
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <filesystem>
#include "common.hpp"

class c_database : public QObject
{
	Q_OBJECT
public:
	explicit c_database(const std::filesystem::path& db_path, QObject* parent = nullptr);
	bool init();

	std::string last_error() const { return last_error_; }

	std::vector<country> load_vector_countries();

public slots:
	void operator_perform_action(mob_operator, operator_action);

signals:
	void error();
	void table_updated();
private:
	void add_operator(mob_operator);
	void update_operator(mob_operator);

	bool execute_query(QSqlQuery& q, const std::string& s);

	std::filesystem::path db_path_;

	std::string last_error_;

	QSqlDatabase db_;
};

