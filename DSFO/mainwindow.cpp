#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "graphview.h"
#include <QStackedWidget>
#include <QComboBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
   ui->setupUi(this);
   ui->stackedWidget->setCurrentIndex(0);

   connect(ui->nextButton, &QPushButton::clicked, this, &MainWindow::onNextClicked);

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

void MainWindow::onNextClicked() {
    qDebug() << "in here";
    ui->stackedWidget->setCurrentIndex(1);

}
