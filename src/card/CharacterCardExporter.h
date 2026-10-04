#ifndef CHARACTERCARDEXPORTER_H
#define CHARACTERCARDEXPORTER_H

#include "CharacterCard.h"

#include <QString>

class CharacterCardExporter
{
public:
    static bool exportToFile(const QString &filePath,
                             const CharacterCard &card,
                             QString &errorMessage);
};

#endif // CHARACTERCARDEXPORTER_H

