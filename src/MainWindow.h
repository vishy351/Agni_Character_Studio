#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "card/CharacterCard.h"

class CharacterPage;
class CharacterBookPage;
class QListWidget;
class QStackedWidget;
class QPushButton;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    QListWidget *sidebar;
    QStackedWidget *contentArea;
    CharacterPage *characterPage;
    CharacterBookPage *characterBookPage;
    QPushButton *importButton;
    QPushButton *exportButton;
    CharacterCard currentCard;
};

#endif // MAINWINDOW_H

