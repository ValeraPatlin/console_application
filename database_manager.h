#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>
#include <QString>
#include <QDir>
#include <vector>
#include "employee.h"

class DatabaseManager
{
public:
    explicit DatabaseManager(const QString& dbPath);
    ~DatabaseManager();

    bool connect();
    void createTable();

    bool insertEmployee(const QString& fullName, const QDate& birthDate, const QString& gender);
    bool insertEmployee(const Employee& employee);
    bool batchInsertEmployees(const std::vector<Employee>& employees); // Пакетная вставка

    QSqlQuery executeQuery(const QString& queryStr); // Выполнение произвольного запроса
    QSqlDatabase getDatabase() const;

private:
    QSqlDatabase db;
    QString dbPath; // Путь к файлу БД
};

#endif // DATABASEMANAGER_H
