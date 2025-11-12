#include <QCoreApplication>
#include <QDebug>
#include <QStringList>
#include <QSqlQuery>
#include <vector>
#include <QDate>
#include <QTime>
#include <QTextStream>
#include "database_manager.h"
#include "employee.h"
#include "utils.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QStringList args = a.arguments();
    QString dbPath = "./my_database.sqlite";
    DatabaseManager dbManager(dbPath);

    if (!dbManager.connect())
    {
        return 1;
    }

    if (args.size() < 2)
    {
        qDebug() << "Использование: myApp [mode] [parameters...]";
        return 1;
    }

    int mode = args[1].toInt();

    switch (mode)
    {
        case 1:
        {
            dbManager.createTable();
            qDebug() << "Таблица /'employees/' создана.";
            break;
        }

        case 2:
        {
            if (args.size() != 5)
            {
                qDebug() << "Использование: myApp 2 \"FullName\" YYYY-MM-DD Gender";
                return 1;
            }

            QString fullName = args[2];
            QDate birthDate = QDate::fromString(args[3], "yyyy-MM-dd");
            QString gender = args[4];

            if (dbManager.insertEmployee(fullName, birthDate, gender))
            {
                qDebug() << "Сотрудник добавлен.";
            }
            else
            {
                qDebug() << "Ошибка при добавлении сотрудника.";
            }
            break;
        }

        case 3:
        {
            QSqlQuery query = dbManager.executeQuery("SELECT full_name, birth_date,"
                                                 " gender FROM employees ORDER BY full_name");

            qDebug() << "ФИО \t\t Дата рождения \t  Пол \t Возраст";

            while (query.next())
            {
                QString fullName = query.value(0).toString();
                QDate birthDate = query.value(1).toDate();
                QString gender = query.value(2).toString();
                Employee employee(fullName, birthDate, gender);

                qDebug() << fullName << "\t\t" << birthDate.toString("yyyy-MM-dd")
                         << "\t" << gender << "\t " << employee.getAge();
            }
            break;
        }

        case 4:
        {
            const int totalRows = 1000000;
            int specialRows = 100;

            std::vector<Employee> employees;
            employees.reserve(totalRows + specialRows);

            for (int i = 0; i < totalRows; ++i)
            {
                employees.push_back(Employee(Utils::generateRandomName(),
                                             Utils::generateRandomDate(1950, 2005),
                                             Utils::generateRandomGender()));
            }

            for (int i = 0; i < specialRows; ++i)
            {
                QString name = "F" + Utils::generateRandomName();
                employees.push_back(Employee(name, Utils::generateRandomDate(1950, 2005),
                                          "Male"));
            }

            QTime startTime = QTime::currentTime();

            if (dbManager.batchInsertEmployees(employees))
            {
                QTime endTime = QTime::currentTime();
                qint64 elapsedTime = startTime.msecsTo(endTime);

                qDebug() << "Данные добавлены. Время выполнения: "
                         << elapsedTime << " мс";
            }
            else
            {
                qDebug() << "Ошибка при пакетной вставке данных.";
            }

            break;
        }

        case 5:
        {
            QTime startTime = QTime::currentTime();
            QSqlQuery query = dbManager.executeQuery("SELECT full_name, birth_date,"
                               " gender FROM employees WHERE gender "
                                              "= 'Male' AND full_name LIKE 'F%'");
            QTime endTime = QTime::currentTime();
            qint64 elapsedTime = startTime.msecsTo(endTime);

            qDebug() << "Результаты выборки (Male, F*):"
                     << "\n ФИО \t\t\t Дата рождения \t\t Пол \t\t Возраст";

            while (query.next())
            {
                QString fullName = query.value(0).toString();
                QDate birthDate = query.value(1).toDate();
                QString gender = query.value(2).toString();
                Employee employee(fullName, birthDate, gender);

                qDebug() << fullName << "\t\t" << birthDate.toString("yyyy-MM-dd")
                         << "\t\t" << gender << "\t " << employee.getAge();
            }

            qDebug() << "Время выполнения запроса: " << elapsedTime << " мс";

            break;
        }

        default:
        {
            qDebug() << "Неизвестный режим работы.";
            break;
        }
    }

    qDebug() << "END";

    return a.exec();
}
