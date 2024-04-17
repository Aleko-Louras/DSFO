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

    //Create a collection of nodes and their connections
    createConnections();

    connect(ui->animationButton, &QToolButton::clicked, this, &GraphView::startDijkstraAnimation);

    ui->animationChooseBox->addItem("Albuquerque");
    ui->animationChooseBox->addItem("Denver");
    ui->animationChooseBox->addItem("Phoenix");
    ui->animationChooseBox->addItem("Las Vegas");
    ui->animationChooseBox->addItem("Los Angeles");
    ui->animationChooseBox->addItem("Salt Lake City");
    ui->animationChooseBox->addItem("San Francisco");
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

void GraphView::startDijkstraAnimation() {
    //Get start from combo box
    Node* node = nodes.at(ui->animationChooseBox->currentIndex());
    //Get speed from slider and disable slider
    animationSpeed = 100000/ui->animationSpeedSlider->value();
    ui->animationSpeedSlider->setEnabled(false);
    //Reset the view and values from previous animation
    for (Node* node : nodes)
    {
        node->label->setText("∞");
        node->total = 5000;
        node->visited = false;
        unflashNode(node->button, node->label);
    }
    node->total = 0;
    advanceDijkstraStep(node);
    ui->animationButton->setEnabled(false);
}

void GraphView::advanceDijkstraStep(Node* node) {
    //Illuminate the current node
    flashNode(node->button, node->label, QString::number(node->total));
    node->visited = true;
    int i = 1;
    for (Edge edge : graph.value(node))
    {
        //Flash edges while "looking" at a node/edge
        QTimer::singleShot((i-1)*animationSpeed, this, [this, edge, node]{flashEdge(edge, node);});

        int currentTotal = edge.node->total;
        QTimer::singleShot((i-1)*animationSpeed, this, [this, edge, node, currentTotal]{updateStepLabel(edge, node, currentTotal);});

        //Calculate the value of the path to target node by adding the total cost of this current node to the cost of the current edge.
        //If this value is lower than the target node's current total cost, a better path has been found, update the cost
        if (node->total + edge.cost < edge.node->total && !edge.node->visited)
            edge.node->total = node->total + edge.cost;
        //Update the cost in label after staggered times
        QTimer::singleShot(i*animationSpeed, this, [this, edge]{updateCost(edge.node->label, QString::number(edge.node->total));});
        i ++;
    }
    //Reset the background and dim the node to mark it as visited, then find the next step
    QTimer::singleShot((i-1)*animationSpeed, this, [this]{ui->backgroundlabel->setStyleSheet("border-image: url(:/images/swbackground.png) 0 0 0 0 stretch stretch;");});
    QTimer::singleShot((i-1)*animationSpeed, this, [this, node]{dimNode(node->button, node->label);});
    QTimer::singleShot((i-1)*animationSpeed, this, [this]{findNextStep();});
}

void GraphView::findNextStep() {
    //Just set some high cost for the initial min, bc the cost will never be this high
    int minCost = 5000;
    Node* minNode;
    bool allVisited = true;
    for (Node* node : nodes)
    {
        //Simple minimize function, also check to see if we have visited every node to end Dijkstra
        if (node->total <= minCost && !node->visited)
        {
            minCost = node->total;
            minNode = node;
        }
        if (!node->visited)
            allVisited = false;
    }
    if (!allVisited)
    {
        ui->stepLabel->setText("The cheapest non visited city is " + minNode->name + ", so visit there next.");
        QTimer::singleShot(animationSpeed, this, [this, minNode]{advanceDijkstraStep(minNode);});
    }
    else
    {
        //If we've visited every node, the animation is over so enable the button again, enable the slider and update the label
        ui->animationButton->setEnabled(true);
        ui->animationSpeedSlider->setEnabled(false);
        ui->stepLabel->setText("Every city is visited, so we are done!");

    }
}

void GraphView::updateStepLabel(Edge edge, Node* node, int oldTotal) {
    QString string;
    string += "The cost of travelling to " + edge.node->name + " from " + node->name + " is ";
    string += "$" + std::to_string(node->total) + " + $" + std::to_string(edge.cost) + " = $" + std::to_string(node->total + edge.cost) + ".";
    if (edge.node->visited)
        string = edge.node->name + " has already been visited, so don't update the cost.";
    else if (!(node->total + edge.cost == edge.node->total))
        string += " This is more expensive than the known cost of " + std::to_string(edge.node->total) + ", so don't update the cost.";
    else if (oldTotal == 5000)
        string += " This is the first path we have found to the node, so update the cost.";
    else
        string += " This is cheaper than the known cost of " + std::to_string(oldTotal) + ", so update the cost.";
    ui->stepLabel->setText(string);
}

void GraphView::flashEdge(Edge edge, Node* node) {
    //Hacky switch statement to change backgrounds bc thats prolly the easiest thing cause drawing lines sucks 🤠
    switch(edge.cost)
    {
        //Basically just look at the cost of the edge to know which one to target
        case 50:
            ui->backgroundlabel->setStyleSheet("border-image: url(:/images/la-lv.png) 0 0 0 0 stretch stretch;");
            break;
        case 90:
            ui->backgroundlabel->setStyleSheet("border-image: url(:/images/slc-lv.png) 0 0 0 0 stretch stretch;");
            break;
        case 100:
            ui->backgroundlabel->setStyleSheet("border-image: url(:/images/la-sf.png) 0 0 0 0 stretch stretch;");
            break;
        case 120:
            ui->backgroundlabel->setStyleSheet("border-image: url(:/images/slc-dv.png) 0 0 0 0 stretch stretch;");
            break;
        case 130:
            ui->backgroundlabel->setStyleSheet("border-image: url(:/images/px-la.png) 0 0 0 0 stretch stretch;");
            break;
        case 180:
            ui->backgroundlabel->setStyleSheet("border-image: url(:/images/lv-sf.png) 0 0 0 0 stretch stretch;");
            break;
        case 200:
            ui->backgroundlabel->setStyleSheet("border-image: url(:/images/slc-px.png) 0 0 0 0 stretch stretch;");
            break;
        //But two nodes have cost 80, luckily they both go to/come from Albuquerque, so check the neighboring nodes to the edge for correct one
        case 80:
            if (node->button->objectName() == "phoenixNode" || edge.node->button->objectName() == "phoenixNode")
                ui->backgroundlabel->setStyleSheet("border-image: url(:/images/aq-px.png) 0 0 0 0 stretch stretch;");
            else
                ui->backgroundlabel->setStyleSheet("border-image: url(:/images/dv-aq.png) 0 0 0 0 stretch stretch;");
            break;
    }
}


void GraphView::flashNode(QToolButton* node, QLabel* label, QString value) {
    node->setIcon(QIcon(":/images/flashednode.png"));
    label->setText("$" + value);
}

void GraphView::updateCost(QLabel* label, QString value) {
    label->setText("$" + value);
}

void GraphView::unflashNode(QToolButton* node, QLabel* label) {
    node->setIcon(QIcon(":/images/emptynode.png"));
    label->setStyleSheet("color: black");
}

void GraphView::dimNode(QToolButton* node, QLabel* label) {
    node->setIcon(QIcon(":/images/fillednode.png"));
    label->setStyleSheet("color: white");
}

void GraphView::createConnections() {

    //Have to use new keyword bc have to use pointers for map and dont know how to initialize otherwise.
    //The only bad thing about this is then we have to delete these but we can't delete them at the end of this method,
    //So I just made them member variables for now so we have access to delete them in the constructor. If someone knows a better way they
    //can suggest? I don't think I learned pointers enough tbh.
    albuquerqueNode = new Node(ui->albuquerqueNode, ui->albuquerqueLabel, "Albuquerque", false, 2000);
    denverNode = new Node(ui->denverNode, ui->denverLabel, "Denver", false, 2000);
    phoenixNode = new Node(ui->phoenixNode, ui->phoenixLabel, "Phoenix", false, 2000);
    lasVegasNode = new Node(ui->lasVegasNode, ui->lasVegasLabel, "Las Vegas", false, 2000);
    losAngelesNode = new Node(ui->losAngelesNode, ui->losAngelesLabel, "Los Angeles", false, 2000);
    saltLakeCityNode = new Node(ui->saltLakeCityNode, ui->saltLakeCityLabel, "Salt Lake City", false, 2000);
    sanFranciscoNode = new Node(ui->sanFranciscoNode, ui->sanFranciscoLabel, "San Francisco", false, 2000);

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
