#ifndef UTILS_H
#define UTILS_H

#include <QString>
#include <QDate>


namespace Utils
{
    QString generateRandomName();
    QDate generateRandomDate(int startYear, int endYear);
    QString generateRandomGender();
}

#endif // UTILS_H
