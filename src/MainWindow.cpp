#include "MainWindow.h"

#include "card/CharacterCardImporter.h"
#include "card/CharacterCardExporter.h"

#include "editor/CharacterPage.h"
#include "editor/CharacterBookPage.h"

#include <QFileDialog>
#include <QHBoxLayout>
#include <QListWidget>
#include <QPushButton>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Agni Character Studio");
    resize(1200, 800);

    auto *centralWidget = new QWidget(this);
    auto *mainLayout = new QHBoxLayout(centralWidget);

    auto *topBar = new QHBoxLayout;

    importButton = new QPushButton("Import", centralWidget);
    exportButton = new QPushButton("Export", centralWidget);

    topBar->addWidget(importButton);
    topBar->addWidget(exportButton);
    topBar->addStretch();

    sidebar = new QListWidget(centralWidget);
    sidebar->setFixedWidth(220);

    sidebar->addItem("Character");
    sidebar->addItem("Prompts");
    sidebar->addItem("Metadata");
    sidebar->addItem("Character Book");
    sidebar->addItem("Advanced");

    contentArea = new QStackedWidget(centralWidget);

    characterPage = new CharacterPage(contentArea);
    contentArea->addWidget(characterPage);

    contentArea->addWidget(new QWidget(contentArea)); // Prompts
    contentArea->addWidget(new QWidget(contentArea)); // Metadata

    characterBookPage = new CharacterBookPage(contentArea);
    contentArea->addWidget(characterBookPage); // Character Book

    contentArea->addWidget(new QWidget(contentArea)); // Advanced

    auto *contentLayout = new QVBoxLayout;

    contentLayout->addLayout(topBar);
    contentLayout->addWidget(contentArea, 1);

    mainLayout->addWidget(sidebar);
    mainLayout->addLayout(contentLayout, 1);

    setCentralWidget(centralWidget);

    connect(sidebar, &QListWidget::currentRowChanged,
            contentArea, &QStackedWidget::setCurrentIndex);

    connect(importButton, &QPushButton::clicked,
            this,
            [this]()
            {
                const QString filePath =
                    QFileDialog::getOpenFileName(
                        this,
                        "Import Character Card",
                        QString(),
                        "Character Cards (*.json);;All Files (*)"
                    );

                if (filePath.isEmpty()) {
                    return;
                }

                QString errorMessage;

                CharacterCard importedCard;

                if (!CharacterCardImporter::importFromFile(
                        filePath,
                        importedCard,
                        errorMessage)) {
                    return;
                }

                currentCard = importedCard;

                characterPage->loadCard(currentCard);

                if (currentCard.hasCharacterBook) {
                    characterBookPage->loadBook(currentCard.characterBook);
                } else {
                    CharacterBook emptyBook;
                    characterBookPage->loadBook(emptyBook);
                }
            });

    connect(exportButton, &QPushButton::clicked,
            this,
            [this]()
            {
                characterPage->updateCard(currentCard);

                characterBookPage->updateBook(currentCard.characterBook);

                currentCard.hasCharacterBook =
                    !currentCard.characterBook.name.isEmpty() ||
                    !currentCard.characterBook.description.isEmpty() ||
                    currentCard.characterBook.scanDepth != 0 ||
                    currentCard.characterBook.tokenBudget != 0 ||
                    currentCard.characterBook.recursiveScanning ||
                    !currentCard.characterBook.entries.isEmpty();

                const QString filePath =
                    QFileDialog::getSaveFileName(
                        this,
                        "Export Character Card",
                        QString(),
                        "Character Cards (*.json);;Text Files (*.txt);;All Files (*)"
                    );

                if (filePath.isEmpty()) {
                    return;
                }

                QString errorMessage;

                CharacterCardExporter::exportToFile(
                    filePath,
                    currentCard,
                    errorMessage
                );
            });

    sidebar->setCurrentRow(0);
}

