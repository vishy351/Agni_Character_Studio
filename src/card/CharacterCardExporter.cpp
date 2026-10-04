#include "CharacterCardExporter.h"
#include "CharacterBookSerializer.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

bool CharacterCardExporter::exportToFile(const QString &filePath,
                                         const CharacterCard &card,
                                         QString &errorMessage)
{
    QJsonObject data = card.originalCard;

    data["name"] = card.name;
    data["description"] = card.description;
    data["personality"] = card.personality;
    data["scenario"] = card.scenario;

    data["first_mes"] = card.firstMessage;
    data["mes_example"] = card.exampleDialogue;

    data["creator_notes"] = card.creatorNotes;

    data["system_prompt"] = card.systemPrompt;
    data["post_history_instructions"] = card.postHistoryInstructions;

    QJsonArray alternateGreetings;
    for (const QString &greeting : card.alternateGreetings) {
        alternateGreetings.append(greeting);
    }
    data["alternate_greetings"] = alternateGreetings;

    QJsonArray tags;
    for (const QString &tag : card.tags) {
        tags.append(tag);
    }
    data["tags"] = tags;

    data["creator"] = card.creator;
    data["character_version"] = card.characterVersion;

    data["extensions"] = card.extensions;

    if (card.hasCharacterBook) {
        data["character_book"] =
            CharacterBookSerializer::toJson(card.characterBook);
    } else {
        data.remove("character_book");
    }

    QJsonObject root;
    root["spec"] = "chara_card_v2";
    root["spec_version"] = "2.0";
    root["data"] = data;

    QFile file(filePath);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        errorMessage = file.errorString();
        return false;
    }

    const QJsonDocument document(root);
    file.write(document.toJson(QJsonDocument::Indented));

    return true;
}

