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

    connect(ui->graphicsView, &GraphView::animationButtonPushed, this, [this]{ui->checkAnswerButton->setDisabled(true);});
    connect(ui->graphicsView, &GraphView::newGraphPushed, this, [this]{ui->checkAnswerButton->setEnabled(true); generateRandomPath(); setQuestion(ui->stackedPages->currentIndex());});

    generateRandomPath();

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

    //Push back buttons into a list
    answerButtons.push_back(ui->answerA);
    answerButtons.push_back(ui->answerB);
    answerButtons.push_back(ui->answerC);
    answerButtons.push_back(ui->answerD);


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

    QMenu *menu = menuBar()->addMenu(tr("Menu"));
    readMore = new QAction(tr("&More Information"), this);
    menu->addAction(readMore);
    connect(readMore, &QAction::triggered, this, &MainWindow::showMoreInfo);
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
    ui->summaryLayout->setVisible(false);

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
    if (ui->stackedPages->currentIndex() == 1)
        ui->summary->setText("Imagine you are planning a trip around the American Southwest. You have a tight budget, so you want to save as much as possible on your flight costs. Unfortunately, it's a busy time of year for the airline companies, and flights are inconsistent in their prices and availability. Given a graph (pictured on the left) of given flights and costs for a week, how do you find the cheapest way to travel from a certain city to a certain city? <br><br> A solution to this problem is to use something known as Dijkstra's algorithm. Dijkstra's algorithm entails a few basic steps, that can be explained with this airport analogy here: <br><br> 1. Set the cost for the start airport to $0, and the cost for destination airports to infinity. Select the starting airport to look at first. <br><br> 2. Calculate the cost of travelling to each adjacent airport from the airport we are looking at, and if the new cost is cheaper, update the cost. <br><br> 3. Mark this airport as visited, and find the cheapest airport that we have found a path to (and that hasn't been visited) and look at that one next. <br><br> 4. Repeat step 2 if there is a cheapest non-visited airport that was found. <br><br> 5. If all airports have been visited, we are done.<br><br> You can test your knowledge of Dijkstra's by asking questions and playing an animation for clarity,  and you can try a new random scenario to make the paths different.");
    else
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

    //Lambda/nested function to create random offsets for incorrect answers
    auto randomOffsetize = [](int input) {
        //Randomize by +- max 1/6 of input
        int randomValue = (rand() % input)/3;
        int randomOffset = randomValue - input/(3*2);
        return input + randomOffset;
    };

    for (QRadioButton* answer : answerButtons)
    {
        answer->setEnabled(true);
        answer->setChecked(false);
        answer->setStyleSheet("");
    }

    if (currentPage == 1) {
        ui->questionLabel->setText("What is the cheapest cost when travelling from " + randomSource + " to " + randomDestination + "?");
        ui->questionLabel->setWordWrap(true);
        correctIndex = rand() % 4;
        for (int i = 0; i < 4; i ++)
        {
            if (i == correctIndex)
                answerButtons.at(i)->setText(QString::number(randomCost));
            else
                answerButtons.at(i)->setText(QString::number(randomOffsetize(randomCost)));
        }
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
    if (ui->checkAnswerButton->text() == "New question")
    {
        generateRandomPath();
        setQuestion(ui->stackedPages->currentIndex());
        ui->checkAnswerButton->setText("Check answer");
        return;
    }

    for (QRadioButton* answer : answerButtons)
    {
        answer->setEnabled(false);
        if (answer->isChecked())
        {
            if (answerButtons.indexOf(answer) == correctIndex)
            {
                answer->setStyleSheet("color: green");
                userScore ++;
            }
            else
                answer->setStyleSheet("color: red");
        }
    }
    ui->checkAnswerButton->setText("New question");
}

void MainWindow::generateRandomPath () {

    cheapestCosts = ui->graphicsView->getCheapestCosts();

    auto maxCost = [this](QString input) {
        int max = 0;
        for (std::pair<QString, int> pair : cheapestCosts.value(input))
            if (pair.second > max)
                max = pair.second;
        return max;
    };

    randomCost = 0;
    //Loop through and make sure that the cost question we are asking is not trivial; right now the questions are filtered so we are only asking questions with maximal distance
    do
    {
        int randomSourceInt = rand() % 7;
        int randomDestinationInt = rand() % 7;
        randomSource = cheapestCosts.keys().at(randomSourceInt);
        randomDestination = cheapestCosts.value(randomSource).at(randomDestinationInt).first;
        randomCost = cheapestCosts.value(randomSource).at(randomDestinationInt).second;
    } while (randomCost < maxCost(randomSource));
}
