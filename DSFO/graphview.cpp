#include "graphview.h"
#include "ui_graphview.h"
#include <QVector>

GraphView::GraphView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GraphView)
{
    ui->setupUi(this);

    //Right now each node is a button, linking all of them is clunky and annoying so put em in a vector instead
    QVector<QPushButton*> nodes;
    nodes.push_back(ui->albuquerqueNode);
    nodes.push_back(ui->denverNode);
    nodes.push_back(ui->saltLakeCityNode);
    nodes.push_back(ui->lasVegasNode);
    nodes.push_back(ui->losAngelesNode);
    nodes.push_back(ui->phoenixNode);
    nodes.push_back(ui->sanFranciscoNode);

    //Is this genius or madness? Time will tell...
    for (QPushButton* node : nodes)
    connect(node, &QPushButton::clicked,
            this, [this, node]{node->setIcon(QIcon(":/fillednode.png"));});
}

GraphView::~GraphView()
{
    delete ui;
}
