#include "graphview.h"
#include "ui_graphview.h"
#include <QVector>
#include <QDebug>
#include <QPicture>
#include <QPainter>

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
                this, [this, node]{changeNode(node);});
}

GraphView::~GraphView() {
    delete ui;
}

void GraphView::changeNode(QPushButton* node) {
    if (node->iconSize() == QSize(30, 30))
    {
        node->setIcon(QIcon(":/fillednode.png"));
        node->setIconSize(QSize(31, 31));
    }
    else
    {
        node->setIcon(QIcon(":/emptynode.png"));
        node->setIconSize(QSize(30, 30));
    }
}
