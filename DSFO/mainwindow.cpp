#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStackedWidget>
#include <QComboBox>
#include <QDebug>
#include <iostream>
#include <QFile>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    qDebug() << ui->stackedPages->rect();
    ui->stackedPages->setCurrentIndex(0);

    connect(ui->nextButton, &QPushButton::clicked,
           this, &MainWindow::onNextClicked);
    connect(ui->backButton, &QPushButton::clicked,
           this, &MainWindow::onBackClicked);
    connect(ui->stackedPages, &QStackedWidget::currentChanged,
           this, &MainWindow::onPageChanged);
    connect(ui->checkAnswerButton, &QPushButton::clicked, this, &MainWindow::checkAnswer);

    QFile texts(":/texts/descriptions.txt");
    texts.open(QFile::ReadOnly);
    QTextStream stream(&texts);
    while (!stream.atEnd())
    {
        descriptions.append(stream.readLine());
    }

    QFile questions(":/texts/questions.txt");
    questions.open(QFile::ReadOnly);
    QTextStream questionStream(&questions);
    while (!questionStream.atEnd())
    {
        questionDescriptions.append(questionStream.readLine());
    }

    ui->summary->setText(descriptions.at(0));
    ui->questionLabel->setText(questionDescriptions.at(0));
    ui->answerA->setText(questionDescriptions.at(1));
    ui->answerB->setText(questionDescriptions.at(2));
    ui->answerC->setText(questionDescriptions.at(3));
    ui->answerD->setText(questionDescriptions.at(4));

    QFile additionalTexts(":/texts/additionalDescriptions.txt");
    additionalTexts.open(QFile::ReadOnly);
    QTextStream stream2(&additionalTexts);
    while (!stream2.atEnd())
    {
        additionalDescriptions.append(stream2.readLine());
    }
   // QStackedWidget *stackedWidget = new QStackedWidget;

   // QWidget *firstPageWidget = new QWidget;
   // stackedWidget->addWidget(firstPageWidget);

   // QVBoxLayout *layout = new QVBoxLayout;
   // layout->addWidget(stackedWidget);
   // setLayout(layout);

//    QWidget *firstPageWidget = new QWidget;
//    QWidget *secondPageWidget = new QWidget;
//    QWidget *thirdPageWidget = new QWidget;

//    QStackedWidget *stackedWidget = new QStackedWidget;
//    stackedWidget->addWidget(firstPageWidget);
//    stackedWidget->addWidget(secondPageWidget);
//    stackedWidget->addWidget(thirdPageWidget);

//    QVBoxLayout *layout = new QVBoxLayout;
//    layout->addWidget(stackedWidget);
//    setLayout(layout);

//    QComboBox *pageComboBox = new QComboBox;
//    pageComboBox->addItem(tr("Page 1"));
//    pageComboBox->addItem(tr("Page 2"));
//    pageComboBox->addItem(tr("Page 3"));
//    connect(pageComboBox, &QComboBox::activated,
//            stackedWidget, &QStackedWidget::setCurrentIndex);

    //Initialize read more button
    readMoreButton = new QPushButton(this);
    readMoreButton->setText("Read More");
    readMoreButton->setGeometry(0, 0, 100, 50);

    //Initialize info dialog
    info = new QDialog(this);
    info->setWindowTitle("More Information");
    info->resize(300,200);
    infoText = new QLabel(info);
    infoText->setWordWrap(true);
    infoText->setGeometry(QRect(0,0,300,150));
    QPushButton *closeButton = new QPushButton("Close", info);
    closeButton->setGeometry(QRect(100,150,100,30));
    connect(closeButton, &QPushButton::clicked, info, &QDialog::close);
    connect(readMoreButton, &QPushButton::clicked, this, &MainWindow::showMoreInfo);

    ui->summaryLayout->setVisible(false);
}

void MainWindow::showMoreInfo() {
    int currentPageIndex = ui->stackedPages->currentIndex();
    if(currentPageIndex < descriptions.size()) {
        ui->summary->setText(descriptions.at(currentPageIndex));
    } else {
        infoText->setText("No additional information available");
    }
    info->exec();
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onBackClicked() {
    int previousPage = ui->stackedPages->currentIndex() - 1;

    ui->backButton->setEnabled(previousPage > 0);

    // if (previousPage <= 0)
    //     ui->backButton->setEnabled(false);
    // else
    //     ui->backButton->setEnabled(true);

    ui->stackedPages->setCurrentIndex(std::max(0, previousPage));

    ui->nextButton->setEnabled(true);
}

void MainWindow::onNextClicked() {
    int nextPage = ui->stackedPages->currentIndex() + 1;

    // Disables nextButton if the last page has been reached (LESS READABLE, MORE COMPACT)
    ui->nextButton->setEnabled(nextPage < ui->stackedPages->count() - 1);

    // Disables nextButton if the last page has been reached (MORE READABLE, LESS COMPACT)
    // if (nextPage >= ui->stackedPages->count() - 1)
    //     ui->nextButton->setEnabled(false);
    // else
    //     ui->nextButton->setEnabled(true);

    ui->stackedPages->setCurrentIndex(std::min(ui->stackedPages->count() - 1, nextPage));
    ui->backButton->setEnabled(true);

}

void MainWindow::onPageChanged() {
    ui->pageTracker->setText(QString("Page " + QString::number(ui->stackedPages->currentIndex() + 1) +
                                     " of " + QString::number(ui->stackedPages->count())));
    ui->summary->setText(descriptions.at(ui->stackedPages->currentIndex()));
    int currentPage = ui->stackedPages->currentIndex();
    if(currentPage < additionalDescriptions.size()) {
        infoText->setText(additionalDescriptions.at(currentPage));
        setQuestion(currentPage);
    } else {
        infoText->setText("No additional information available");
    }

    ui->summaryLayout->setVisible(currentPage != 0);
}

void MainWindow::setQuestion(int currentPage) {
    qDebug() << currentPage;

    ui->answerA->setEnabled(true);
    ui->answerB->setEnabled(true);
    ui->answerC->setEnabled(true);
    ui->answerD->setEnabled(true);

    ui->answerA->setChecked(false);
    ui->answerB->setChecked(false);
    ui->answerC->setChecked(false);
    ui->answerD->setChecked(false);

    ui->answerA->setStyleSheet("");
    ui->answerB->setStyleSheet("");
    ui->answerC->setStyleSheet("");
    ui->answerD->setStyleSheet("");

    if (currentPage == 1) {
        ui->questionLabel->setText(questionDescriptions.at(0));
        ui->questionLabel->setWordWrap(true);
        ui->answerA->setText(questionDescriptions.at(1));
        ui->answerB->setText(questionDescriptions.at(2));
        ui->answerC->setText(questionDescriptions.at(3));
        ui->answerD->setText(questionDescriptions.at(4));
    }
    if (currentPage == 2) {
        QString labelText = "Which of the following statements best describes the essence of the stack data structure?<br><br>"
                            "A) A data structure that follows the Last In, First Out (LIFO) principle, where the last element added is the first one to be removed.<br><br>"
                            "B) A data structure that follows the First In, First Out (FIFO) principle, where the first element added is the first one to be removed.<br><br>"
                            "C) A data structure that allows elements to be accessed randomly, without any specific order.<br><br>"
                            "D) A data structure that organizes elements in a hierarchical manner, resembling the structure of a tree.";
        ui->questionLabel->setText(labelText);

        ui->answerA->setText("A");
        ui->answerB->setText("B");
        ui->answerC->setText("C");
        ui->answerD->setText("D");
    }
}

void MainWindow::checkAnswer() {
    ui->answerA->setEnabled(false);
    ui->answerB->setEnabled(false);
    ui->answerC->setEnabled(false);
    ui->answerD->setEnabled(false);

    if (ui->answerA->isChecked()) {
        ui->answerA->setStyleSheet("color: green");
        userScore++;
    }
    if (ui->answerB->isChecked()) {
        ui->answerB->setStyleSheet("color: red");
    }
    if (ui->answerC->isChecked()) {
        ui->answerC->setStyleSheet("color: red");
    }
    if (ui->answerD->isChecked()) {
        ui->answerD->setStyleSheet("color: red");
    }
}
