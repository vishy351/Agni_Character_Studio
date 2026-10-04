#ifndef CHARACTERCARD_H
#define CHARACTERCARD_H

#include <QJsonObject>
#include <QString>
#include <QStringList>
#include <QVector>

struct CharacterBookEntry
{
    QStringList keys;
    QString content;

    QJsonObject extensions;

    bool enabled = true;
    int insertionOrder = 100;

    bool caseSensitive = false;

    QString name;
    int priority = 0;
    int id = 0;
    QString comment;

    bool selective = false;
    QStringList secondaryKeys;

    bool constant = false;
    QString position = "before_char";
};

struct CharacterBook
{
    QString name;
    QString description;

    int scanDepth = 0;
    int tokenBudget = 0;

    bool recursiveScanning = false;

    QJsonObject extensions;
    QVector<CharacterBookEntry> entries;
};

struct CharacterCard
{
    QString name;
    QString description;
    QString personality;
    QString scenario;

    QString firstMessage;
    QString exampleDialogue;

    QString creatorNotes;

    QString systemPrompt;
    QString postHistoryInstructions;

    QStringList alternateGreetings;

    bool hasCharacterBook = false;
    CharacterBook characterBook;

    QStringList tags;
    QString creator;
    QString characterVersion;

    QJsonObject extensions;

    // Original imported V2 JSON.
    // Used to preserve fields that Agni does not currently edit.
    QJsonObject originalCard;
};

#endif // CHARACTERCARD_H

