#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStackedWidget>
#include <QComboBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
   ui->setupUi(this);
   ui->stackedPages->setCurrentIndex(0);

   connect(ui->nextButton, &QPushButton::clicked,
           this, &MainWindow::onNextClicked);
   connect(ui->backButton, &QPushButton::clicked,
           this, &MainWindow::onBackClicked);
   connect(ui->stackedPages, &QStackedWidget::currentChanged,
           this, &MainWindow::onPageChanged);

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

    ui->stackedPages->setCurrentIndex(std::min(previousPage, 0));

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

    ui->stackedPages->setCurrentIndex(std::max(nextPage, ui->stackedPages->count() - 1));
    ui->backButton->setEnabled(true);
}

void MainWindow::onPageChanged(int index) {
    ui->pageTracker->setText(QString("Page " + QString::number(ui->stackedPages->currentIndex() + 1) +
                                     " of " + QString::number(ui->stackedPages->count())));
}
