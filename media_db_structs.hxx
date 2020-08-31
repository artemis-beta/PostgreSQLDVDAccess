#ifndef MEDIA_DB_STRUCTS_HXX
#define MEDIA_DB_STRUCTS_HXX

#include <QString>

struct DVD
{
    int ID;
    QString Title;
    QString Genre;
    int Length;
    QString Location;
    QString Notes;
    std::tm LastUpdated;

};

#endif // MEDIA_DB_STRUCTS_HXX
