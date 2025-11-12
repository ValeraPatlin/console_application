#include "utils.h"
#include <QRandomGenerator>
#include <QStringList>

namespace Utils
{
    QString generateRandomName()
    {
        QStringList firstNames = {"John", "Jane", "David", "Emily",
                                  "Michael", "Sarah", "Robert", "Jessica", "William", "Ashley"};
        QStringList lastNames = {"Smith", "Johnson", "Williams", "Brown",
                                 "Jones", "Garcia", "Miller", "Davis", "Rodriguez", "Wilson"};

        QString firstName = firstNames[QRandomGenerator::global()->bounded(firstNames.size())];
        QString lastName = lastNames[QRandomGenerator::global()->bounded(lastNames.size())];

        return firstName + " " + lastName;
    }

    QDate generateRandomDate(int startYear, int endYear)
    {
        int year = QRandomGenerator::global()->bounded(startYear, endYear + 1);
        int month = QRandomGenerator::global()->bounded(1, 13);
        int day = QRandomGenerator::global()->bounded(1, 29);

        return QDate(year, month, day);
    }

    QString generateRandomGender()
    {
        return QRandomGenerator::global()->bounded(2) == 0 ? "Male" : "Female";
    }

} // namespace Utils
