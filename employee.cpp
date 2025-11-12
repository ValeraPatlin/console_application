#include "employee.h"
#include <QDate>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

Employee::Employee(const QString& fullName, const QDate& birthDate, const QString& gender)
    : fullName{fullName}, birthDate{birthDate}, gender{gender}
{}

QString Employee::getFullName() const
{
    return fullName;
}

QDate Employee::getBirthDate() const
{
    return birthDate;
}

QString Employee::getGender() const
{
    return gender;
}

int Employee::getAge() const
{
    return birthDate.isValid() ? QDate::currentDate().year() - birthDate.year() : -1;
}

QString Employee::toSqlValues() const
{
    return QString("('%1', '%2')")
            .arg(birthDate.toString("yyyy-MM-dd"))
            .arg(gender);
}

bool Employee::saveToDatabase(QSqlDatabase& db) const
{
    QSqlQuery query(db);
    query.prepare("INSERT INTO employees (full_name, birth_date, gender)"
                  " VALUES (:full_name, :birth_date, :gender)");
    query.bindValue(":full_name", fullName);
    query.bindValue(":birth_date", birthDate);
    query.bindValue(":gender", gender);

    if (!query.exec())
    {
        qDebug() << "Ошибка при добавлении сотрудника в БД:" << query.lastError().text();
        return false;
    }
    return true;
}
