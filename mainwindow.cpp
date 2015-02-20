#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QString>
#include <QStringList>
#include <QByteArray>
#include <QTime>

Verbs readVerbs(QString filename)
{

    Verbs ret;
    QFile verbsFile(filename);

    if (!verbsFile.open(QFile::ReadOnly)) {
        qDebug("can't open file.");
        return ret;
    }

    QString line;

    do {
        line = verbsFile.readLine();
        QStringList splitLine = line.split('\t');

        if (splitLine.length() == 4) {
            Verb newVerb;
            newVerb.infinitive = splitLine.at(0);
            newVerb.pastTense = splitLine.at(1);
            newVerb.pastParticiple = splitLine.at(2);
            newVerb.translatedString = splitLine.at(3);
            ret.append(newVerb);
        }
    } while(verbsFile.canReadLine());

    return ret;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    currentIndex(0),
    kittenCounter(0),
    uiState(UiExpectsReply)
{
    qsrand(QTime::currentTime().msec());

    verbs = readVerbs(":/files/czaso.csv");
    ui->setupUi(this);
    ui->webView->load(QUrl("http://thecatapi.com/api/images/get?format=html&results_per_page=1&size=med"));
    presentQuestion();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::presentQuestion()
{
    static const int EditsCnt = 3;
    QLineEdit *edits[EditsCnt] = {
                           ui->inInfinitive,
                           ui->inPastTense,
                           ui->inPastParticiple };

    for (int i = 0; i < EditsCnt; ++i) {
        edits[i]->setStyleSheet("");
        edits[i]->setText("");
    }
    currentIndex = qrand() % verbs.length();
    ui->outVerb->setText(verbs[currentIndex].translatedString);
    ui->inInfinitive->setFocus();
    uiState = UiExpectsReply;
}

bool MainWindow::verifyPartialAnswer(const QString &expected, QLineEdit *input)
{
    if (QString::compare(expected, input->text(), Qt::CaseInsensitive) != 0) {
        input->setText(expected);
        input->setStyleSheet("font-weight: bold");
        return false;
    } else
        return true;
}

void MainWindow::expectsKitten()
{
    if (uiState == UiExpectsReply) {
        Verb *curVerb = &verbs[currentIndex];
        bool allGood = verifyPartialAnswer(curVerb->infinitive, ui->inInfinitive) *
                       verifyPartialAnswer(curVerb->pastTense, ui->inPastTense) *
                       verifyPartialAnswer(curVerb->pastParticiple, ui->inPastParticiple);

        if (allGood) {
            kittenCounter++;
            ui->kittenCounter->setText(QString().sprintf("%d", kittenCounter));
            ui->webView->reload();
        }
        uiState = UiShowsResults;
    } else {
        presentQuestion();
    }

}
