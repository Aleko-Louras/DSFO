/**
header file for the main window, i.e. the graphical interface and where everything is displayed/interacted with
University of Utah CS 3505 final project, group LAQE
Writen by Lucas Pearce, Ethan Block, Will Black, Quinn Pritchett, Aleko Louras
**/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QLabel>
#include <QTimer>
#include <QRadioButton>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
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
public slots:
    void showMoreInfo();
    void checkAnswer();
private:
    Ui::MainWindow *ui;
    QVector<QString> additionalDescriptions;
    QAction* readMore;
    QDialog* info;
    QLabel* infoText;
    QVector<QRadioButton*> answerButtons;
    QTimer timer;
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
