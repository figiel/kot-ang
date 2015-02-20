#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>

namespace Ui {
class MainWindow;
}

struct Verb {
    QString infinitive;
    QString pastTense;
    QString pastParticiple;
    QString translatedString;
};

enum UiState {
    UiExpectsReply,
    UiShowsResults
};

typedef QList<Verb> Verbs;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void presentQuestion();
    bool verifyPartialAnswer(const QString &expected, QLineEdit *input);
    Ui::MainWindow *ui;
    int currentIndex;
    int kittenCounter;
    Verbs verbs;
    UiState uiState;
public slots:
    void expectsKitten();
};

#endif // MAINWINDOW_H
