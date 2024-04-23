#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QPushButton>
#include <QDialog>
#include <QLabel>
#include <QRadioButton>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onNextClicked();
    void onBackClicked();
    void onPageChanged();
    void toggleAnswerButton();

public slots:
    void showMoreInfo();
    void checkAnswer();



private:
    Ui::MainWindow *ui;
    QVector<QString> descriptions;
    QVector<QString> additionalDescriptions;
    QVector<QString> questionDescriptions;
    QPushButton* readMoreButton;
    QDialog* info;
    QLabel* infoText;
    QVector<QRadioButton*> answerButtons;

    QHash<QString, QList<std::pair<QString, int>>> cheapestCosts;
    int randomCost;
    QString randomSource;
    QString randomDestination;
    int correctIndex;

    int userScore = 0;

    void setQuestion(int currentPage);
    void generateRandomPath();
};
#endif // MAINWINDOW_H
