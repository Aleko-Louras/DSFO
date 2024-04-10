#include "graphview.h"
#include "ui_graphview.h"
#include <QVector>
#include <QDebug>
#include <QPicture>
#include <QPainter>
#include <QTimer>

GraphView::GraphView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GraphView)
{
    ui->setupUi(this);

    //Right now each node is a button, linking all of them is clunky and annoying so put em in a vector instead
    QVector<QToolButton*> nodes;
    nodes.push_back(ui->albuquerqueNode);
    nodes.push_back(ui->denverNode);
    nodes.push_back(ui->saltLakeCityNode);
    nodes.push_back(ui->lasVegasNode);
    nodes.push_back(ui->losAngelesNode);
    nodes.push_back(ui->phoenixNode);
    nodes.push_back(ui->sanFranciscoNode);

    //Is this genius or madness? Time will tell...
    for (QToolButton* node : nodes)
    connect(node, &QToolButton::clicked,
                this, [this, node]{changeNode(node);});

    connect(ui->animationButton, &QToolButton::clicked, this, &GraphView::startDijkstraAnimation);
}

GraphView::~GraphView() {
    delete ui;
}

void GraphView::changeNode(QToolButton* node) {
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

void GraphView::startDijkstraAnimation() {
    QVector<QToolButton*> nodes;
    nodes.push_back(ui->sanFranciscoNode);
    nodes.push_back(ui->losAngelesNode);
    nodes.push_back(ui->lasVegasNode);
    nodes.push_back(ui->saltLakeCityNode);
    nodes.push_back(ui->denverNode);
    for (int i = 0; i < 5; i ++)
        QTimer::singleShot(i*1000, this, [this, nodes, i]{flashNode(nodes.at(i));});
}

void GraphView::flashNode(QToolButton* node) {
    node->setIcon(QIcon(":/flashednode.png"));
}
