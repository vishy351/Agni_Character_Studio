#ifndef CHARACTERBOOKPAGE_H
#define CHARACTERBOOKPAGE_H

#include "../card/CharacterCard.h"

#include <QWidget>
#include <QVector>

class QCheckBox;
class QComboBox;
class QGroupBox;
class QLineEdit;
class QSpinBox;
class QTextEdit;
class QVBoxLayout;

class CharacterBookPage : public QWidget
{
    Q_OBJECT

public:
    explicit CharacterBookPage(QWidget *parent = nullptr);

    void loadBook(const CharacterBook &book);
    void updateBook(CharacterBook &book) const;

private:
    struct EntryWidgets
    {
        QGroupBox *group;

        QLineEdit *keysEdit;
        QLineEdit *secondaryKeysEdit;
        QTextEdit *contentEdit;

        QCheckBox *enabledCheck;
        QCheckBox *caseSensitiveCheck;
        QCheckBox *selectiveCheck;
        QCheckBox *constantCheck;

        QLineEdit *nameEdit;
        QSpinBox *prioritySpin;
        QSpinBox *idSpin;
        QLineEdit *commentEdit;

        QSpinBox *insertionOrderSpin;
        QComboBox *positionCombo;
    };

    CharacterBook book;

    QLineEdit *nameEdit;
    QTextEdit *descriptionEdit;
    QSpinBox *scanDepthSpin;
    QSpinBox *tokenBudgetSpin;
    QCheckBox *recursiveScanningCheck;

    QVBoxLayout *entriesLayout;
    QVector<EntryWidgets> entryWidgets;
};

#endif // CHARACTERBOOKPAGE_H

