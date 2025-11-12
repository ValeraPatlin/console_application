#include "database_manager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDir>
#include <QCoreApplication>

DatabaseManager::DatabaseManager(const QString& dbPath)
    : dbPath{dbPath} {}

DatabaseManager::~DatabaseManager()
{
    if (db.isOpen())
    {
        db.close();
    }
}

bool DatabaseManager::connect()
{
    QString absoluteDbPath = QDir(QCoreApplication::applicationDirPath()).filePath(dbPath);
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(absoluteDbPath);
    if (!db.open())
    {
        qDebug() << "Ошибка подключения к базе данных:" << db.lastError().text();
        return false;
    }

    qDebug() << "Успешное подключение к базе данных";
    return true;
}

void DatabaseManager::createTable()
{
    QSqlQuery query(db);
    QString sql = "CREATE TABLE IF NOT EXISTS employees ("
                  "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                  "full_name TEXT NOT NULL, "
                  "birth_date DATE NOT NULL, "
                  "gender TEXT NOT NULL"
                  ");";

    if (!query.exec(sql))
    {
        qDebug() << "Ошибка создания таблицы:" << query.lastError().text();
    }
}

bool DatabaseManager::insertEmployee(const QString& fullName,
                                     const QDate& birthDate,
                                     const QString& gender)
{
    Employee employee(fullName, birthDate, gender);
    return insertEmployee(employee);
}

bool DatabaseManager::insertEmployee(const Employee& employee)
{
    return employee.saveToDatabase(db);
}

bool DatabaseManager::batchInsertEmployees(const std::vector<Employee> &employees)
{
    if (employees.empty())
        return true;

    db.transaction();
    QSqlQuery query(db);
    query.prepare("INSERT INTO employees (full_name, birth_date, gender)"
                  " VALUES (:full_name, :birth_date, :gender)");

    for (const auto& employee : employees)
    {
        query.bindValue(":full_name", employee.getFullName());
        query.bindValue(":birth_date", employee.getBirthDate());
        query.bindValue(":gender", employee.getGender());

        if (!query.exec())
        {
            qDebug() << "Ошибка при пакетной вставке сотрудника в БД:" << query.lastError().text();
            db.rollback();
            return false;
        }
    }

    if (!db.commit())
    {
        qDebug() << "Ошибка при подтверждении транзакции:" << db.lastError().text();
        db.rollback();
        return false;
    }
    return true;
}

QSqlQuery DatabaseManager::executeQuery(const QString &queryStr)
{
    QSqlQuery query(db);

    if (!query.exec(queryStr))
    {
        qDebug() << "Ошибка выполнения запроса:" << query.lastError().text();
    }
    return query;
}

QSqlDatabase DatabaseManager::getDatabase() const
{
    return db;
}
