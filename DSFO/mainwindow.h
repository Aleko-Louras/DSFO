#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QPushButton>
#include <QDialog>
#include <QLabel>

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

public slots:
    void showMoreInfo();



private:
    Ui::MainWindow *ui;
    QVector<QString> descriptions;
    QVector<QString> additionalDescriptions;
    QVector<QString> questionDescriptions;
    QPushButton* readMoreButton;
    QDialog* info;
    QLabel* infoText;
};
#endif // MAINWINDOW_H
