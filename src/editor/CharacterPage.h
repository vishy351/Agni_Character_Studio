#ifndef CHARACTERPAGE_H
#define CHARACTERPAGE_H

#include "../card/CharacterCard.h"

#include <QWidget>

class QLineEdit;
class QTextEdit;
class QVBoxLayout;

class CharacterPage : public QWidget
{
    Q_OBJECT

public:
    explicit CharacterPage(QWidget *parent = nullptr);

    void loadCard(const CharacterCard &card);
    void updateCard(CharacterCard &card) const;

private:
    QLineEdit *nameEdit;
    QTextEdit *descriptionEdit;
    QTextEdit *personalityEdit;
    QTextEdit *scenarioEdit;

    QTextEdit *firstMessageEdit;

    QVBoxLayout *alternateGreetingsLayout;

    QTextEdit *exampleDialogueEdit;
    QTextEdit *creatorNotesEdit;
};

#endif // CHARACTERPAGE_H

