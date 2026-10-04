#include "CharacterBookPage.h"

#include <QCheckBox>
#include <QComboBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QTextEdit>
#include <QVBoxLayout>

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

QStringList splitKeys(const QString &text)
{
    QStringList result;

    const QStringList parts =
        text.split(',', Qt::SkipEmptyParts);

    for (const QString &part : parts) {
        const QString key = part.trimmed();

        if (!key.isEmpty()) {
            result.append(key);
        }
    }

    return result;
}
}

CharacterBookPage::CharacterBookPage(QWidget *parent)
    : QWidget(parent),
      nameEdit(nullptr),
      descriptionEdit(nullptr),
      scanDepthSpin(nullptr),
      tokenBudgetSpin(nullptr),
      recursiveScanningCheck(nullptr),
      entriesLayout(nullptr)
{
    auto *layout = new QVBoxLayout(this);

    //
    // Book information
    //
    auto *infoGroup = new QGroupBox("Character Book");
    auto *infoLayout = new QFormLayout(infoGroup);

    nameEdit = new QLineEdit;
    infoLayout->addRow("Name:", nameEdit);

    descriptionEdit = new QTextEdit;
    descriptionEdit->setMinimumHeight(100);
    infoLayout->addRow("Description:", descriptionEdit);

    scanDepthSpin = new QSpinBox;
    scanDepthSpin->setRange(0, 100000);
    infoLayout->addRow("Scan Depth:", scanDepthSpin);

    tokenBudgetSpin = new QSpinBox;
    tokenBudgetSpin->setRange(0, 1000000);
    infoLayout->addRow("Token Budget:", tokenBudgetSpin);

    recursiveScanningCheck =
        new QCheckBox("Enable Recursive Scanning");

    infoLayout->addRow(recursiveScanningCheck);

    infoLayout->addRow(
        createHelpLabel(
            "The Character Book contains entries that can be "
            "activated when their keys match the conversation."
        )
    );

    layout->addWidget(infoGroup);

    //
    // Entries
    //
    auto *entriesGroup = new QGroupBox("Entries");
    auto *entriesGroupLayout = new QVBoxLayout(entriesGroup);

    entriesGroupLayout->addWidget(
        createHelpLabel(
            "Entries contain trigger keys and content. "
            "Advanced controls are available inside each entry."
        )
    );

    entriesLayout = new QVBoxLayout;
    entriesGroupLayout->addLayout(entriesLayout);

    auto *addButton = new QPushButton("+ Add Entry");
    entriesGroupLayout->addWidget(addButton);

    layout->addWidget(entriesGroup);
    layout->addStretch();

    connect(
        addButton,
        &QPushButton::clicked,
        this,
        [this]()
        {
            auto *entryGroup = new QGroupBox("Entry");
            auto *entryLayout = new QFormLayout(entryGroup);

            auto *keysEdit = new QLineEdit;
            keysEdit->setPlaceholderText(
                "key1, key2, key3"
            );
            entryLayout->addRow("Keys:", keysEdit);

            auto *secondaryKeysEdit = new QLineEdit;
            secondaryKeysEdit->setPlaceholderText(
                "secondary1, secondary2"
            );
            entryLayout->addRow(
                "Secondary Keys:",
                secondaryKeysEdit
            );

            auto *contentEdit = new QTextEdit;
            contentEdit->setMinimumHeight(140);
            entryLayout->addRow("Content:", contentEdit);

            auto *enabledCheck =
                new QCheckBox("Enabled");
            enabledCheck->setChecked(true);
            entryLayout->addRow(enabledCheck);

            auto *caseSensitiveCheck =
                new QCheckBox("Case Sensitive");
            entryLayout->addRow(caseSensitiveCheck);

            auto *selectiveCheck =
                new QCheckBox("Selective");
            entryLayout->addRow(selectiveCheck);

            auto *constantCheck =
                new QCheckBox("Constant");
            entryLayout->addRow(constantCheck);

            auto *nameEdit = new QLineEdit;
            entryLayout->addRow("Name:", nameEdit);

            auto *prioritySpin = new QSpinBox;
            prioritySpin->setRange(-100000, 100000);
            entryLayout->addRow(
                "Priority:",
                prioritySpin
            );

            auto *idSpin = new QSpinBox;
            idSpin->setRange(-2147483647, 2147483647);
            entryLayout->addRow(
                "ID:",
                idSpin
            );

            auto *commentEdit = new QLineEdit;
            entryLayout->addRow(
                "Comment:",
                commentEdit
            );

            auto *insertionOrderSpin = new QSpinBox;
            insertionOrderSpin->setRange(
                -100000,
                100000
            );
            insertionOrderSpin->setValue(100);

            entryLayout->addRow(
                "Insertion Order:",
                insertionOrderSpin
            );

            auto *positionCombo = new QComboBox;

            positionCombo->addItem(
                "Before Character",
                "before_char"
            );

            positionCombo->addItem(
                "After Character",
                "after_char"
            );

            positionCombo->addItem(
                "Before Example Dialogue",
                "before_example"
            );

            positionCombo->addItem(
                "After Example Dialogue",
                "after_example"
            );

            positionCombo->addItem(
                "At Depth",
                "at_depth"
            );

            entryLayout->addRow(
                "Position:",
                positionCombo
            );

            auto *removeButton =
                new QPushButton("Remove Entry");

            entryLayout->addRow(removeButton);

            entriesLayout->addWidget(entryGroup);

            entryWidgets.append({
                entryGroup,
                keysEdit,
                secondaryKeysEdit,
                contentEdit,
                enabledCheck,
                caseSensitiveCheck,
                selectiveCheck,
                constantCheck,
                nameEdit,
                prioritySpin,
                idSpin,
                commentEdit,
                insertionOrderSpin,
                positionCombo
            });

            connect(
                removeButton,
                &QPushButton::clicked,
                entryGroup,
                [this, entryGroup]()
                {
                    for (int i = 0;
                         i < entryWidgets.size();
                         ++i) {
                        if (entryWidgets.at(i).group ==
                            entryGroup) {
                            entryWidgets.removeAt(i);
                            break;
                        }
                    }

                    entryGroup->deleteLater();
                }
            );
        }
    );
}

void CharacterBookPage::loadBook(
    const CharacterBook &newBook)
{
    book = newBook;

    nameEdit->setText(book.name);
    descriptionEdit->setPlainText(book.description);
    scanDepthSpin->setValue(book.scanDepth);
    tokenBudgetSpin->setValue(book.tokenBudget);
    recursiveScanningCheck->setChecked(
        book.recursiveScanning
    );

    for (const EntryWidgets &widgets : entryWidgets) {
        if (widgets.group) {
            widgets.group->deleteLater();
        }
    }

    entryWidgets.clear();

    for (const CharacterBookEntry &entry : book.entries) {
        auto *entryGroup =
            new QGroupBox("Entry");

        auto *entryLayout =
            new QFormLayout(entryGroup);

        auto *keysEdit = new QLineEdit;
        keysEdit->setText(
            entry.keys.join(", ")
        );
        entryLayout->addRow(
            "Keys:",
            keysEdit
        );

        auto *secondaryKeysEdit =
            new QLineEdit;

        secondaryKeysEdit->setText(
            entry.secondaryKeys.join(", ")
        );

        entryLayout->addRow(
            "Secondary Keys:",
            secondaryKeysEdit
        );

        auto *contentEdit = new QTextEdit;
        contentEdit->setMinimumHeight(140);
        contentEdit->setPlainText(
            entry.content
        );

        entryLayout->addRow(
            "Content:",
            contentEdit
        );

        auto *enabledCheck =
            new QCheckBox("Enabled");

        enabledCheck->setChecked(
            entry.enabled
        );

        entryLayout->addRow(
            enabledCheck
        );

        auto *caseSensitiveCheck =
            new QCheckBox("Case Sensitive");

        caseSensitiveCheck->setChecked(
            entry.caseSensitive
        );

        entryLayout->addRow(
            caseSensitiveCheck
        );

        auto *selectiveCheck =
            new QCheckBox("Selective");

        selectiveCheck->setChecked(
            entry.selective
        );

        entryLayout->addRow(
            selectiveCheck
        );

        auto *constantCheck =
            new QCheckBox("Constant");

        constantCheck->setChecked(
            entry.constant
        );

        entryLayout->addRow(
            constantCheck
        );

        auto *nameEdit = new QLineEdit;
        nameEdit->setText(entry.name);

        entryLayout->addRow(
            "Name:",
            nameEdit
        );

        auto *prioritySpin = new QSpinBox;
        prioritySpin->setRange(
            -100000,
            100000
        );
        prioritySpin->setValue(
            entry.priority
        );

        entryLayout->addRow(
            "Priority:",
            prioritySpin
        );

        auto *idSpin = new QSpinBox;
        idSpin->setRange(
            -2147483647,
            2147483647
        );
        idSpin->setValue(entry.id);

        entryLayout->addRow(
            "ID:",
            idSpin
        );

        auto *commentEdit = new QLineEdit;
        commentEdit->setText(
            entry.comment
        );

        entryLayout->addRow(
            "Comment:",
            commentEdit
        );

        auto *insertionOrderSpin =
            new QSpinBox;

        insertionOrderSpin->setRange(
            -100000,
            100000
        );

        insertionOrderSpin->setValue(
            entry.insertionOrder
        );

        entryLayout->addRow(
            "Insertion Order:",
            insertionOrderSpin
        );

        auto *positionCombo =
            new QComboBox;

        positionCombo->addItem(
            "Before Character",
            "before_char"
        );

        positionCombo->addItem(
            "After Character",
            "after_char"
        );

        positionCombo->addItem(
            "Before Example Dialogue",
            "before_example"
        );

        positionCombo->addItem(
            "After Example Dialogue",
            "after_example"
        );

        positionCombo->addItem(
            "At Depth",
            "at_depth"
        );

        const int positionIndex =
            positionCombo->findData(
                entry.position
            );

        if (positionIndex >= 0) {
            positionCombo->setCurrentIndex(
                positionIndex
            );
        }

        entryLayout->addRow(
            "Position:",
            positionCombo
        );

        auto *removeButton =
            new QPushButton("Remove Entry");

        entryLayout->addRow(
            removeButton
        );

        entriesLayout->addWidget(
            entryGroup
        );

        entryWidgets.append({
            entryGroup,
            keysEdit,
            secondaryKeysEdit,
            contentEdit,
            enabledCheck,
            caseSensitiveCheck,
            selectiveCheck,
            constantCheck,
            nameEdit,
            prioritySpin,
            idSpin,
            commentEdit,
            insertionOrderSpin,
            positionCombo
        });

        connect(
            removeButton,
            &QPushButton::clicked,
            entryGroup,
            [this, entryGroup]()
            {
                for (int i = 0;
                     i < entryWidgets.size();
                     ++i) {
                    if (entryWidgets.at(i).group ==
                        entryGroup) {
                        entryWidgets.removeAt(i);
                        break;
                    }
                }

                entryGroup->deleteLater();
            }
        );
    }
}

void CharacterBookPage::updateBook(
    CharacterBook &newBook) const
{
    newBook.name = nameEdit->text();
    newBook.description =
        descriptionEdit->toPlainText();

    newBook.scanDepth =
        scanDepthSpin->value();

    newBook.tokenBudget =
        tokenBudgetSpin->value();

    newBook.recursiveScanning =
        recursiveScanningCheck->isChecked();

    newBook.entries.clear();

    for (const EntryWidgets &widgets :
         entryWidgets) {
        CharacterBookEntry entry;

        entry.keys =
            splitKeys(
                widgets.keysEdit->text()
            );

        entry.secondaryKeys =
            splitKeys(
                widgets.secondaryKeysEdit->text()
            );

        entry.content =
            widgets.contentEdit->toPlainText();

        entry.enabled =
            widgets.enabledCheck->isChecked();

        entry.caseSensitive =
            widgets.caseSensitiveCheck->isChecked();

        entry.selective =
            widgets.selectiveCheck->isChecked();

        entry.constant =
            widgets.constantCheck->isChecked();

        entry.name =
            widgets.nameEdit->text();

        entry.priority =
            widgets.prioritySpin->value();

        entry.id =
            widgets.idSpin->value();

        entry.comment =
            widgets.commentEdit->text();

        entry.insertionOrder =
            widgets.insertionOrderSpin->value();

        entry.position =
            widgets.positionCombo
                ->currentData()
                .toString();

        newBook.entries.append(entry);
    }

    book = newBook;
}

