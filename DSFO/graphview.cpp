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

    connect(ui->animationButton, &QToolButton::clicked, this, [this]{startDijkstraAnimation(ui->sanFranciscoNode);});
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

void GraphView::startDijkstraAnimation(QToolButton* node) {
    //Set all node lengths to infinity
    for (QToolButton* button : buttons)
        button->setText("∞");
    //Start indexing and intiialize minimum (as a high value)
    int i = 1;
    QTimer::singleShot(10, this, [this, node]{flashNode(node, QString::number(0));});
    for (Edge& neighbor : graph.value(node))
    {
        QTimer::singleShot(i*1000, this, [this, neighbor]{flashNode(neighbor.node.button, QString::number(neighbor.cost));});
        i ++;
    }
    //Update visited values
    QVector<Edge> edges = graph.value(node);
    for(Edge& edge : edges)
        edge.visit();
    graph[node] = edges;
    QTimer::singleShot(i*1000, this, [this]{findNextStep();});
}

void GraphView::advanceDijkstraStep(QToolButton* node) {
    int i = 1;
    QVector<QString> banned;
    bool infExist = false;
    for (int k = 0; k < nodes.size(); k ++)
    {
        if (nodes.at(k).button->text() == "∞")
        {
            infExist = true;
        }
    }
    for (int k = 0; k < nodes.size(); k ++)
    {
        if (!infExist)
        {
            if (nodes.at(k).visited == true)
            {
                //qDebug() << " instructing to skip " << nodes.at(k).button->objectName() << "...";
               banned.append(nodes.at(k).button->objectName());
            }
        }
        else
        {
            if (nodes.at(k).button->text() != "∞")
            {
                //qDebug() << " instructing to skip " << nodes.at(k).button->objectName() << "...";
                banned.append(nodes.at(k).button->objectName());
            }
        }
    }
    for (Edge neighbor : graph.value(node))
    {
        if (banned.contains(neighbor.node.button->objectName()))
        {
            //qDebug() << " skipping" << neighbor.node.button->objectName() << " bc it is BANNED";
            continue;
        }
        int newCost = neighbor.cost + node->text().removeFirst().toInt();
        if (neighbor.node.button->text().length() > 1)
            if (newCost > neighbor.node.button->text().removeFirst().toInt())
                newCost = neighbor.node.button->text().removeFirst().toInt();
        qDebug() << "new cost " << newCost << " neighbor cost " << neighbor.cost;
        QTimer::singleShot(i*1000, this, [this, neighbor, newCost]{flashNode(neighbor.node.button, QString::number(newCost));});
        i ++;
    }

    QVector<Edge> edges = graph.value(node);
    for(Edge& edge : edges)
        edge.visit();
    graph[node] = edges;
    QTimer::singleShot(i*1000, this, [this]{findNextStep();});
}

void GraphView::findNextStep() {
    int i = 0;
    for (QToolButton* button : buttons)
    {
        QVector<Edge> neighbors = graph[button];
        bool nonVisitedNeighbor = false;
        for (Edge neighbor : neighbors)
            if (neighbor.visited == false)
                nonVisitedNeighbor = true;
        if (!nonVisitedNeighbor)
            nodes[i].visit();
        i ++;
    }
    int min = 5000;
    Node minNode = Node(ui->albuquerqueNode, false, 0);
    int fullyVisited = 0;
    for (Node node : nodes)
    {
        if (node.visited == true)
        {
            //qDebug() << " skipping " << node.button->objectName() << " because it was fully visited...";
            fullyVisited = 0;
            continue;
        }
        if (node.button->text() != "∞")
            node.total = node.button->text().removeFirst().toInt();
        if (node.total <= min)
        {
            minNode = node;
            min = node.total;
        }
    }
    //qDebug() << minNode.button->objectName() << " is global min name with a min value of " << min;
    if (fullyVisited < nodes.size())
        QTimer::singleShot(10, this, [this, minNode]{advanceDijkstraStep(minNode.button);});
}

void GraphView::flashNode(QToolButton* node, QString value) {
    node->setIconSize(QSize(29, 29));
    node->setIcon(QIcon(":/flashednode.png"));
    node->setText("$" + value);
}

void GraphView::unflashNode(QToolButton* node) {
    node->setIconSize(QSize(30, 30));
    node->setIcon(QIcon(":/emptynode.png"));
}

//Create the connections
void GraphView::createConnections() {

    Node albuquerqueNode(ui->albuquerqueNode, false, 2000);
    Node denverNode(ui->denverNode, false, 2000);
    Node phoenixNode(ui->phoenixNode, false, 2000);
    Node lasVegasNode(ui->lasVegasNode, false, 2000);
    Node losAngelesNode(ui->losAngelesNode, false, 2000);
    Node saltLakeCityNode(ui->saltLakeCityNode, false, 2000);
    Node sanFranciscoNode(ui->sanFranciscoNode, false, 2000);

    nodes.append(albuquerqueNode);
    nodes.append(denverNode);
    nodes.append(phoenixNode);
    nodes.append(lasVegasNode);
    nodes.append(losAngelesNode);
    nodes.append(saltLakeCityNode);
    nodes.append(sanFranciscoNode);

    QVector<Edge> albuquerqueConnections;
    albuquerqueConnections.push_back(Edge(denverNode, false, 80));
    albuquerqueConnections.push_back(Edge(phoenixNode, false, 80));
    graph.insert(ui->albuquerqueNode, albuquerqueConnections);

    QVector<Edge> denverConnections;
    denverConnections.push_back(Edge(albuquerqueNode, false, 80));
    denverConnections.push_back(Edge(saltLakeCityNode, false, 120));
    graph.insert(ui->denverNode, denverConnections);

    QVector<Edge> phoenixConnections;
    phoenixConnections.push_back(Edge(albuquerqueNode, false, 80));
    phoenixConnections.push_back(Edge(losAngelesNode, false, 130));
    phoenixConnections.push_back(Edge(saltLakeCityNode, false, 200));
    graph.insert(ui->phoenixNode, phoenixConnections);

    QVector<Edge> saltLakeCityConnections;
    saltLakeCityConnections.push_back(Edge(denverNode, false, 120));
    saltLakeCityConnections.push_back(Edge(lasVegasNode, false, 90));
    saltLakeCityConnections.push_back(Edge(phoenixNode, false, 200));
    graph.insert(ui->saltLakeCityNode, saltLakeCityConnections);

    QVector<Edge> lasVegasConnections;
    lasVegasConnections.push_back(Edge(saltLakeCityNode, false, 90));
    lasVegasConnections.push_back(Edge(losAngelesNode, false, 50));
    lasVegasConnections.push_back(Edge(sanFranciscoNode, false, 180));
    graph.insert(ui->lasVegasNode, lasVegasConnections);

    QVector<Edge> losAngelesConnections;
    losAngelesConnections.push_back(Edge(lasVegasNode, false, 50));
    losAngelesConnections.push_back(Edge(sanFranciscoNode, false, 100));
    losAngelesConnections.push_back(Edge(phoenixNode, false, 130));
    graph.insert(ui->losAngelesNode, losAngelesConnections);


    QVector<Edge> sanFranciscoConnections;
    sanFranciscoConnections.push_back(Edge(losAngelesNode, false, 100));
    sanFranciscoConnections.push_back(Edge(lasVegasNode, false, 180));
    graph.insert(ui->sanFranciscoNode, sanFranciscoConnections);
}
