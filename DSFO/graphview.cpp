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
    buttons.push_back(ui->albuquerqueNode);
    buttons.push_back(ui->denverNode);
    buttons.push_back(ui->phoenixNode);
    buttons.push_back(ui->lasVegasNode);
    buttons.push_back(ui->losAngelesNode);
    buttons.push_back(ui->saltLakeCityNode);
    buttons.push_back(ui->sanFranciscoNode);

    //Create a collection of nodes and their connections
    createConnections();

    //Link everything at once
    for (QToolButton* button : buttons)
    connect(button, &QToolButton::clicked,
                this, [this, button]{changeNode(button);});

    connect(ui->animationButton, &QToolButton::clicked, this, [this]{startDijkstraAnimation(sanFranciscoNode);});
}

GraphView::~GraphView() {
    delete albuquerqueNode;
    delete denverNode;
    delete phoenixNode;
    delete lasVegasNode;
    delete losAngelesNode;
    delete saltLakeCityNode;
    delete sanFranciscoNode;
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

void GraphView::startDijkstraAnimation(Node* node) {
    for (Node* node : nodes)
    {
        node->button->setText("∞");
        node->total = 5000;
    }
    node->total = 0;
    advanceDijkstraStep(node);
}

void GraphView::advanceDijkstraStep(Node* node) {
    flashNode(node->button, QString::number(node->total));
    node->visit();
    int i = 1;
    for (Edge edge : graph.value(node))
    {
        //Calculate the value of the path to target node by adding the total cost of this current node to the cost of the current edge.
        //If this value is lower than the target node's current total cost, a better path has been found, update this
        if (node->total + edge.cost < edge.node->total && !edge.node->visited)
            edge.node->total = node->total + edge.cost;
        QTimer::singleShot(i*1000, this, [this, edge]{updateCost(edge.node->button, QString::number(edge.node->total));});
        i ++;
    }
    QTimer::singleShot(i*1000, this, [this]{findNextStep();});
}

void GraphView::findNextStep() {
    int minCost = 5000;
    Node* minNode;
    bool allVisited = true;
    for (Node* node : nodes)
    {
        if (node->total <= minCost && !node->visited)
        {
            minCost = node->total;
            minNode = node;
        }
        if (!node->visited)
            allVisited = false;
    }
    if (!allVisited)
        advanceDijkstraStep(minNode);
}

void GraphView::flashNode(QToolButton* node, QString value) {
    node->setIconSize(QSize(29, 29));
    node->setIcon(QIcon(":/flashednode.png"));
    node->setText("$" + value);
}

void GraphView::updateCost(QToolButton* node, QString value) {
    node->setText("$" + value);
}

void GraphView::unflashNode(QToolButton* node) {
    node->setIconSize(QSize(30, 30));
    node->setIcon(QIcon(":/emptynode.png"));
}

//Create the connections
void GraphView::createConnections() {

    albuquerqueNode = new Node(ui->albuquerqueNode, false, 2000);
    denverNode = new Node(ui->denverNode, false, 2000);
    phoenixNode = new Node(ui->phoenixNode, false, 2000);
    lasVegasNode = new Node(ui->lasVegasNode, false, 2000);
    losAngelesNode = new Node(ui->losAngelesNode, false, 2000);
    saltLakeCityNode = new Node(ui->saltLakeCityNode, false, 2000);
    sanFranciscoNode = new Node(ui->sanFranciscoNode, false, 2000);

    nodes.append(albuquerqueNode);
    nodes.append(denverNode);
    nodes.append(phoenixNode);
    nodes.append(lasVegasNode);
    nodes.append(losAngelesNode);
    nodes.append(saltLakeCityNode);
    nodes.append(sanFranciscoNode);

    QVector<Edge> albuquerqueConnections;
    albuquerqueConnections.push_back(Edge(denverNode, 80));
    albuquerqueConnections.push_back(Edge(phoenixNode, 80));
    graph.insert(albuquerqueNode, albuquerqueConnections);

    QVector<Edge> denverConnections;
    denverConnections.push_back(Edge(albuquerqueNode, 80));
    denverConnections.push_back(Edge(saltLakeCityNode, 120));
    graph.insert(denverNode, denverConnections);

    QVector<Edge> phoenixConnections;
    phoenixConnections.push_back(Edge(albuquerqueNode, 80));
    phoenixConnections.push_back(Edge(losAngelesNode, 130));
    phoenixConnections.push_back(Edge(saltLakeCityNode, 200));
    graph.insert(phoenixNode, phoenixConnections);

    QVector<Edge> saltLakeCityConnections;
    saltLakeCityConnections.push_back(Edge(denverNode, 120));
    saltLakeCityConnections.push_back(Edge(lasVegasNode, 90));
    saltLakeCityConnections.push_back(Edge(phoenixNode, 200));
    graph.insert(saltLakeCityNode, saltLakeCityConnections);

    QVector<Edge> lasVegasConnections;
    lasVegasConnections.push_back(Edge(saltLakeCityNode, 90));
    lasVegasConnections.push_back(Edge(losAngelesNode, 50));
    lasVegasConnections.push_back(Edge(sanFranciscoNode, 180));
    graph.insert(lasVegasNode, lasVegasConnections);

    QVector<Edge> losAngelesConnections;
    losAngelesConnections.push_back(Edge(lasVegasNode, 50));
    losAngelesConnections.push_back(Edge(sanFranciscoNode, 100));
    losAngelesConnections.push_back(Edge(phoenixNode, 130));
    graph.insert(losAngelesNode, losAngelesConnections);


    QVector<Edge> sanFranciscoConnections;
    sanFranciscoConnections.push_back(Edge(losAngelesNode, 100));
    sanFranciscoConnections.push_back(Edge(lasVegasNode, 180));
    graph.insert(sanFranciscoNode, sanFranciscoConnections);

}
