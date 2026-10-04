#include "CharacterBookSerializer.h"

#include <QJsonArray>
#include <QJsonValue>

CharacterBook CharacterBookSerializer::fromJson(const QJsonObject &object)
{
    CharacterBook book;

    book.name = object.value("name").toString();
    book.description = object.value("description").toString();
    book.scanDepth = object.value("scan_depth").toInt();
    book.tokenBudget = object.value("token_budget").toInt();
    book.recursiveScanning =
        object.value("recursive_scanning").toBool();

    if (object.contains("extensions")) {
        book.extensions = object.value("extensions").toObject();
    }

    const QJsonArray entries = object.value("entries").toArray();

    for (const QJsonValue &value : entries) {
        if (!value.isObject()) {
            continue;
        }

        const QJsonObject entryObject = value.toObject();
        CharacterBookEntry entry;

        const QJsonArray keys = entryObject.value("keys").toArray();
        for (const QJsonValue &key : keys) {
            entry.keys.append(key.toString());
        }

        entry.content = entryObject.value("content").toString();

        if (entryObject.contains("extensions")) {
            entry.extensions =
                entryObject.value("extensions").toObject();
        }

        entry.enabled = entryObject.value("enabled").toBool(true);
        entry.insertionOrder =
            entryObject.value("insertion_order").toInt(100);

        entry.caseSensitive =
            entryObject.value("case_sensitive").toBool(false);

        entry.name = entryObject.value("name").toString();
        entry.priority = entryObject.value("priority").toInt();
        entry.id = entryObject.value("id").toInt();
        entry.comment = entryObject.value("comment").toString();

        entry.selective =
            entryObject.value("selective").toBool(false);

        const QJsonArray secondaryKeys =
            entryObject.value("secondary_keys").toArray();

        for (const QJsonValue &key : secondaryKeys) {
            entry.secondaryKeys.append(key.toString());
        }

        entry.constant =
            entryObject.value("constant").toBool(false);

        entry.position =
            entryObject.value("position").toString("before_char");

        book.entries.append(entry);
    }

    return book;
}

QJsonObject CharacterBookSerializer::toJson(const CharacterBook &book)
{
    QJsonObject object;

    object["name"] = book.name;
    object["description"] = book.description;
    object["scan_depth"] = book.scanDepth;
    object["token_budget"] = book.tokenBudget;
    object["recursive_scanning"] = book.recursiveScanning;
    object["extensions"] = book.extensions;

    QJsonArray entries;

    for (const CharacterBookEntry &entry : book.entries) {
        QJsonObject entryObject;

        QJsonArray keys;
        for (const QString &key : entry.keys) {
            keys.append(key);
        }

        entryObject["keys"] = keys;
        entryObject["content"] = entry.content;
        entryObject["extensions"] = entry.extensions;

        entryObject["enabled"] = entry.enabled;
        entryObject["insertion_order"] = entry.insertionOrder;
        entryObject["case_sensitive"] = entry.caseSensitive;

        entryObject["name"] = entry.name;
        entryObject["priority"] = entry.priority;
        entryObject["id"] = entry.id;
        entryObject["comment"] = entry.comment;

        entryObject["selective"] = entry.selective;

        QJsonArray secondaryKeys;
        for (const QString &key : entry.secondaryKeys) {
            secondaryKeys.append(key);
        }

        entryObject["secondary_keys"] = secondaryKeys;
        entryObject["constant"] = entry.constant;
        entryObject["position"] = entry.position;

        entries.append(entryObject);
    }

    object["entries"] = entries;

    return object;
}

