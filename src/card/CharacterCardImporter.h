#ifndef CHARACTERCARDIMPORTER_H
#define CHARACTERCARDIMPORTER_H

#include "CharacterCard.h"

#include <QString>

class CharacterCardImporter
{
public:
    static bool importFromFile(const QString &filePath,
                               CharacterCard &card,
                               QString &errorMessage);
};

#endif // CHARACTERCARDIMPORTER_H

