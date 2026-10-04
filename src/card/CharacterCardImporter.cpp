#include "CharacterCardImporter.h"
#include "CharacterBookSerializer.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

bool CharacterCardImporter::importFromFile(const QString &filePath,
                                           CharacterCard &card,
                                           QString &errorMessage)
{
    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly)) {
        errorMessage = file.errorString();
        return false;
    }

    const QJsonDocument document = QJsonDocument::fromJson(
        file.readAll()
    );

    if (document.isNull() || !document.isObject()) {
        errorMessage = "The selected file is not a valid JSON object.";
        return false;
    }

    const QJsonObject root = document.object();

    if (root.value("spec").toString() != "chara_card_v2") {
        errorMessage = "The selected file is not a Character Card V2.";
        return false;
    }

    const QJsonObject data = root.value("data").toObject();

    if (data.isEmpty()) {
        errorMessage = "The Character Card V2 does not contain a data object.";
        return false;
    }

    card.originalCard = data;

    card.name = data.value("name").toString();
    card.description = data.value("description").toString();
    card.personality = data.value("personality").toString();
    card.scenario = data.value("scenario").toString();

    card.firstMessage = data.value("first_mes").toString();
    card.exampleDialogue = data.value("mes_example").toString();

    card.creatorNotes = data.value("creator_notes").toString();

    card.systemPrompt = data.value("system_prompt").toString();
    card.postHistoryInstructions =
        data.value("post_history_instructions").toString();

    const QJsonArray alternateGreetings =
        data.value("alternate_greetings").toArray();

    for (const QJsonValue &greeting : alternateGreetings) {
        card.alternateGreetings.append(greeting.toString());
    }

    const QJsonArray tags = data.value("tags").toArray();

    for (const QJsonValue &tag : tags) {
        card.tags.append(tag.toString());
    }

    card.creator = data.value("creator").toString();
    card.characterVersion = data.value("character_version").toString();

    if (data.contains("extensions")) {
        card.extensions = data.value("extensions").toObject();
    }

    if (data.contains("character_book") &&
        data.value("character_book").isObject()) {
        card.hasCharacterBook = true;
        card.characterBook =
            CharacterBookSerializer::fromJson(
                data.value("character_book").toObject()
            );
    }

    return true;
}

