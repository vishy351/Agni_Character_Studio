#include "CharacterPage.h"

#include <QFormLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QScrollArea>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QWidget>

namespace
{
QLabel *createHelpLabel(const QString &text)
{
    auto *label = new QLabel(text);
    label->setWordWrap(true);
    label->setStyleSheet(
        "color: #888888;"
        "font-size: 11px;"
    );
    return label;
}
}

CharacterPage::CharacterPage(QWidget *parent)
    : QWidget(parent),
      nameEdit(nullptr),
      descriptionEdit(nullptr),
      personalityEdit(nullptr),
      scenarioEdit(nullptr),
      firstMessageEdit(nullptr),
      alternateGreetingsLayout(nullptr),
      exampleDialogueEdit(nullptr),
      creatorNotesEdit(nullptr)
{
    auto *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);

    auto *content = new QWidget;
    auto *layout = new QVBoxLayout(content);

    auto *basicGroup = new QGroupBox("Character");
    auto *basicLayout = new QFormLayout(basicGroup);

    nameEdit = new QLineEdit;
    basicLayout->addRow("Name:", nameEdit);
    basicLayout->addRow(
        createHelpLabel(
            "The character's display name. This identifies the character "
            "to the frontend."
        )
    );

    descriptionEdit = new QTextEdit;
    descriptionEdit->setMinimumHeight(140);
    basicLayout->addRow("Description:", descriptionEdit);
    basicLayout->addRow(
        createHelpLabel(
            "Describe who the character is, their appearance, background, "
            "or other information you want the AI to know. This can affect "
            "the character's behavior."
        )
    );

    personalityEdit = new QTextEdit;
    personalityEdit->setMinimumHeight(100);
    basicLayout->addRow("Personality:", personalityEdit);
    basicLayout->addRow(
        createHelpLabel(
            "Describe personality traits, attitudes, habits, and behavioral "
            "tendencies. This can affect the character's behavior."
        )
    );

    scenarioEdit = new QTextEdit;
    scenarioEdit->setMinimumHeight(100);
    basicLayout->addRow("Scenario:", scenarioEdit);
    basicLayout->addRow(
        createHelpLabel(
            "Describe the situation or setting in which the conversation "
            "takes place. This can affect the character's behavior."
        )
    );

    layout->addWidget(basicGroup);

    auto *greetingGroup = new QGroupBox("Greetings");
    auto *greetingLayout = new QVBoxLayout(greetingGroup);

    greetingLayout->addWidget(new QLabel("First Message"));

    firstMessageEdit = new QTextEdit;
    firstMessageEdit->setMinimumHeight(180);
    greetingLayout->addWidget(firstMessageEdit);

    greetingLayout->addWidget(
        createHelpLabel(
            "The character's opening message. This is the first message "
            "shown when starting a conversation and can strongly influence "
            "the initial interaction."
        )
    );

    auto *alternateLabel = new QLabel("Alternate Greetings");
    alternateLabel->setStyleSheet("font-weight: bold;");
    greetingLayout->addWidget(alternateLabel);

    alternateGreetingsLayout = new QVBoxLayout;
    greetingLayout->addLayout(alternateGreetingsLayout);

    auto *addGreetingButton =
        new QPushButton("+ Add Alternate Greeting");

    greetingLayout->addWidget(addGreetingButton);

    greetingLayout->addWidget(
        createHelpLabel(
            "Alternate opening messages that can be selected when starting "
            "a new conversation. They can influence how the conversation "
            "begins."
        )
    );

    layout->addWidget(greetingGroup);

    auto *dialogueGroup = new QGroupBox("Example Dialogue");
    auto *dialogueLayout = new QVBoxLayout(dialogueGroup);

    exampleDialogueEdit = new QTextEdit;
    exampleDialogueEdit->setMinimumHeight(180);
    dialogueLayout->addWidget(exampleDialogueEdit);

    dialogueLayout->addWidget(
        createHelpLabel(
            "Example conversations showing how the character speaks and "
            "responds. This can influence the character's behavior and "
            "speaking style."
        )
    );

    layout->addWidget(dialogueGroup);

    auto *notesGroup = new QGroupBox("Creator Notes");
    auto *notesLayout = new QVBoxLayout(notesGroup);

    creatorNotesEdit = new QTextEdit;
    creatorNotesEdit->setMinimumHeight(100);
    notesLayout->addWidget(creatorNotesEdit);

    notesLayout->addWidget(
        createHelpLabel(
            "Notes from the card creator. Whether these affect the AI "
            "depends on the frontend and how it handles this field; they "
            "are not inherently part of the character's prompt."
        )
    );

    layout->addWidget(notesGroup);
    layout->addStretch();

    scrollArea->setWidget(content);

    auto *pageLayout = new QVBoxLayout(this);
    pageLayout->setContentsMargins(0, 0, 0, 0);
    pageLayout->addWidget(scrollArea);

    connect(
        addGreetingButton,
        &QPushButton::clicked,
        this,
        [this]()
        {
            auto *container = new QWidget;
            auto *containerLayout = new QVBoxLayout(container);
            containerLayout->setContentsMargins(0, 0, 0, 8);

            auto *edit = new QTextEdit;
            edit->setMinimumHeight(150);

            auto *removeButton =
                new QPushButton("Remove Greeting");

            containerLayout->addWidget(edit);
            containerLayout->addWidget(removeButton);

            alternateGreetingsLayout->addWidget(container);

            connect(
                removeButton,
                &QPushButton::clicked,
                container,
                [container]()
                {
                    container->deleteLater();
                }
            );
        }
    );
}

void CharacterPage::loadCard(const CharacterCard &card)
{
    nameEdit->setText(card.name);
    descriptionEdit->setPlainText(card.description);
    personalityEdit->setPlainText(card.personality);
    scenarioEdit->setPlainText(card.scenario);

    firstMessageEdit->setPlainText(card.firstMessage);
    exampleDialogueEdit->setPlainText(card.exampleDialogue);
    creatorNotesEdit->setPlainText(card.creatorNotes);

    while (alternateGreetingsLayout->count() > 0) {
        QLayoutItem *item = alternateGreetingsLayout->takeAt(0);

        if (item->widget()) {
            item->widget()->deleteLater();
        }

        delete item;
    }

    for (const QString &greeting : card.alternateGreetings) {
        auto *container = new QWidget;
        auto *containerLayout = new QVBoxLayout(container);
        containerLayout->setContentsMargins(0, 0, 0, 8);

        auto *edit = new QTextEdit;
        edit->setMinimumHeight(150);
        edit->setPlainText(greeting);

        auto *removeButton =
            new QPushButton("Remove Greeting");

        containerLayout->addWidget(edit);
        containerLayout->addWidget(removeButton);

        alternateGreetingsLayout->addWidget(container);

        connect(
            removeButton,
            &QPushButton::clicked,
            container,
            [container]()
            {
                container->deleteLater();
            }
        );
    }
}

void CharacterPage::updateCard(CharacterCard &card) const
{
    card.name = nameEdit->text();
    card.description = descriptionEdit->toPlainText();
    card.personality = personalityEdit->toPlainText();
    card.scenario = scenarioEdit->toPlainText();

    card.firstMessage = firstMessageEdit->toPlainText();
    card.exampleDialogue = exampleDialogueEdit->toPlainText();
    card.creatorNotes = creatorNotesEdit->toPlainText();

    card.alternateGreetings.clear();

    for (int i = 0; i < alternateGreetingsLayout->count(); ++i) {
        QLayoutItem *item = alternateGreetingsLayout->itemAt(i);

        if (!item || !item->widget()) {
            continue;
        }

        auto *container = item->widget();
        auto *edit = container->findChild<QTextEdit *>();

        if (edit) {
            card.alternateGreetings.append(edit->toPlainText());
        }
    }
}

