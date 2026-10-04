#ifndef CHARACTERBOOKSERIALIZER_H
#define CHARACTERBOOKSERIALIZER_H

#include "CharacterCard.h"

#include <QJsonObject>

class CharacterBookSerializer
{
public:
    static CharacterBook fromJson(const QJsonObject &object);
    static QJsonObject toJson(const CharacterBook &book);
};

#endif // CHARACTERBOOKSERIALIZER_H

