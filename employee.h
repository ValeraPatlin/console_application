#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <QString>
#include <QDate>
#include <QSqlDatabase>


class Employee
{
public:
    explicit Employee(const QString& fullName, const QDate& birthDate, const QString& gender);

    QString getFullName() const;
    QDate getBirthDate() const;
    QString getGender() const;
    int getAge() const;
    QString toSqlValues() const;
    bool saveToDatabase(QSqlDatabase& db) const;

private:
    QString fullName;
    QDate birthDate;
    QString gender;
};

#endif // EMPLOYEE_H
