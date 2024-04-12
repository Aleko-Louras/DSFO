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

//Create a collection of nodes and their connections
QHash<GraphView::Node*, QVector<GraphView::Node*>> graph;
GraphView::Node* albuquerque = new GraphView::Node(0);
GraphView::Node* denver = new GraphView::Node(1);
GraphView::Node* saltLakeCity = new GraphView::Node(2);
GraphView::Node* lasVegas = new GraphView::Node(3);
GraphView::Node* losAngeles = new GraphView::Node(4);
GraphView::Node* phoenix = new GraphView::Node(5);
GraphView::Node* sanFrancisco = new GraphView::Node(6);

//Create the connections
void createConnections() {
    QVector<GraphView::Node*> albuquerqueConnections;
    albuquerqueConnections.push_back(denver);
    albuquerqueConnections.push_back(phoenix);
    graph.insert(albuquerque, albuquerqueConnections);

    QVector<GraphView::Node*> denverConnections;
    denverConnections.push_back(albuquerque);
    denverConnections.push_back(saltLakeCity);
    graph.insert(denver, denverConnections);

    QVector<GraphView::Node*> saltLakeCityConnections;
    saltLakeCityConnections.push_back(denver);
    saltLakeCityConnections.push_back(lasVegas);
    graph.insert(saltLakeCity, saltLakeCityConnections);

    QVector<GraphView::Node*> lasVegasConnections;
    lasVegasConnections.push_back(saltLakeCity);
    lasVegasConnections.push_back(losAngeles);
    graph.insert(lasVegas, lasVegasConnections);

    QVector<GraphView::Node*> losAngelesConnections;
    losAngelesConnections.push_back(lasVegas);
    losAngelesConnections.push_back(sanFrancisco);
    losAngelesConnections.push_back(phoenix);
    graph.insert(losAngeles, losAngelesConnections);

    QVector<GraphView::Node*> phoenixConnections;
    phoenixConnections.push_back(albuquerque);
    phoenixConnections.push_back(losAngeles);
    graph.insert(phoenix, phoenixConnections);

    QVector<GraphView::Node*> sanFranciscoConnections;
    sanFranciscoConnections.push_back(losAngeles);
    graph.insert(sanFrancisco, sanFranciscoConnections);
}


