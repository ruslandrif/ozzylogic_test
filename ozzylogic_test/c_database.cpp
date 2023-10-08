#include "c_database.hpp"
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>

c_database::c_database(const std::filesystem::path& db_path, QObject* parent) : db_path_(db_path), QObject(parent) {

}

bool c_database::init() {
	if (!std::filesystem::exists(db_path_)) {
		last_error_ = "Database file does not exist in path \"" +  db_path_.string() + "\"";
		return false;
	}

	db_ = QSqlDatabase::addDatabase("QSQLITE", "SQLITE");
	db_.setDatabaseName(db_path_.string().c_str());

	if (!db_.open()) {
		last_error_ = db_.lastError().text().toStdString();
		emit error();
	}

	return db_.isOpen();
}

void c_database::operator_perform_action(mob_operator op, operator_action act) {
	switch (act) {
	case operator_action::add:
		add_operator(op);
	case operator_action::edit:
		update_operator(op);
	case operator_action::unk:
	default:
		break;
	}
}

void c_database::add_operator(mob_operator op) {
	//for me it seems that in the "operators table" can not be two rows with the same (mcc, mnc) pair. 
	//But in the database there are not any constraints on that table, so I'm inserting new row very straightforward
	const std::string query_str = std::format(
		"INSERT INTO operators(mcc, mnc, name) VALUES("
		"{},"
		"{}, "
		"\'{}\')", op.mcc(), op.mnc(), op.name());
	QSqlQuery q(db_);															
	if (!execute_query(q, query_str)) {											
		emit error();
	}
	else {
		emit table_updated();
	}
}

void c_database::update_operator(mob_operator op) {
	const std::string query_str = std::format("UPDATE operators SET name = \'{}\' WHERE mcc = {} AND mnc = {}", op.name(), op.mcc(), op.mnc());
	
	QSqlQuery q(db_);
	if (!execute_query(q, query_str)) {
		emit error();
	}
	else {
		emit table_updated();
	}
}

bool c_database::execute_query(QSqlQuery& q, const std::string& s) {

	const bool res =  q.prepare(s.c_str()) && q.exec();
	if (!res) {
		last_error_ = q.lastError().databaseText().toStdString();
	}

	return res;
}

std::vector<country> c_database::load_vector_countries() {
	std::vector<country> res;
	auto* model = new QSqlQueryModel(this);

	QSqlQuery quer(db_);

	const std::string query_str = 
		"SELECT operators.mcc as mcc, operators.mnc as mnc, operators.name as op_name, countries.name as c_name, countries.code as c_code"
		" FROM operators"
		" JOIN countries ON operators.mcc = countries.mcc"
		" ORDER BY countries.name";

	if (!execute_query(quer, query_str)) {
		emit error();
		return {};
	}

	model->setQuery(quer);
	while (model->canFetchMore()) {
		model->fetchMore();
	}

	res.reserve(model->rowCount());
	std::string last_country;
	int last_mcc{ 0 };

	for (int i = 0; i < model->rowCount(); ++i) {
		const QSqlRecord rec = model->record(i);

		const int mcc = rec.value("mcc").toInt();
		const int mnc = rec.value("mnc").toInt();
		const std::string op_name = rec.value("op_name").toString().toStdString();
		const std::string c_name = rec.value("c_name").toString().toStdString();
		const std::string c_code = rec.value("c_code").toString().toStdString();

		if (last_country != c_name) {
			last_mcc = mcc;
			last_country = c_name;
			country rec(last_country, c_code, mcc);
			
			res.push_back(std::move(rec));
		}

		if (last_mcc != mcc) { //it seems like some countries has several mcc's for operators
			res.back().add_mcc(mcc);
		}

		mob_operator oper{ mcc, mnc, op_name };
		res.back().add_operator(std::move(oper));
	}

	res.shrink_to_fit();

	return res;
}
