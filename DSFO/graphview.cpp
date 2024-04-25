#include "graphview.h"
#include <QGraphicsItem>
#include <QResizeEvent>
#include <QPushButton>
#include <QGraphicsProxyWidget>
#include <QComboBox>
#include <queue>
#include <QTimer>
#include <QMargins>
#include <ctime>

GraphView::GraphView(QWidget *parent) : QGraphicsView(parent) {

    // ESSENTIAL SETTINGS!
    setMinimumSize(500, 400);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    srand(time(NULL));

    QRectF sceneBox(0, 0, width(), height());
    QImage westCoastMap = QImage(":/images/swbackground.png").scaled(size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    graphScene = new QGraphicsScene(sceneBox, this);
    westCoast = graphScene->addPixmap(QPixmap::fromImage(westCoastMap));
    priorityQueue = new std::priority_queue<Node*, QVector<Node*>, Comparison>();

    //Generate edge costs
    generateCosts();

    //Add nodes to the scene
    QSet<Edge*> edges;

    for (Node *airport : vertices) {
        graphScene->addItem(airport);
        airport->setZValue(1);
    }

    //! GROUP: Here I place everything that's going in the center
    qreal horizontalCenter = sceneBox.center().x();

    animate = new QPushButton("Animate");
    animationButton = graphScene->addWidget(animate);
    animationButton->setPos(horizontalCenter - animate->width() / 2, 0);

    //! GROUP: Here I place everything going on the left
    newGraph = new QPushButton("Randomize graph");
    newGraphButton = graphScene->addWidget(newGraph);
    newGraphButton->setPos(0, 0);

    selector = new QComboBox();
    selector->setMaxVisibleItems(3);
    selector->setFixedHeight(newGraph->height());
    for (const QString& airport : vertices.keys())
        selector->addItem(airport);
    airportSelector = graphScene->addWidget(selector);
    airportSelector->setPos((animate->x() + newGraph->rect().right() - selector->width()) / 2, 0);
    airportSelector->setZValue(1);

    //! GROUP: Here I place everything going on the right
    slider = new QSlider(Qt::Horizontal);
    slider->setStyleSheet("background-color: rgba(0,0,0,0);");
    animationSlider = graphScene->addWidget(slider);
    animationSlider->setPos((sceneBox.right() + animate->x() + animate->width() - slider->width()) / 2, 0);
    slider->setMinimum(10);
    slider->setMaximum(200);
    slider->setValue(60);

    QLabel* sliderLabel = new QLabel("Faster                                         Slower");
    sliderLabel->setStyleSheet("background-color: rgba(0,0,0,0);");
    sliderLabel->setFixedSize(slider->size());
    animationSliderLabel = graphScene->addWidget(sliderLabel);
    animationSliderLabel->setPos(slider->x(), slider->height());

    //! GROUP: Everything else - nodes, edges, tips - is placed after this point
    label = new QLabel();
    label->setFixedSize(200, 100);
    label->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    label->setWordWrap(true);
    label->setVisible(false);
    label->setStyleSheet("background-color: rgba(0,0,0,0);");
    animationLabel = graphScene->addWidget(label);
    animationLabel->setPos(0, 335);

    tips = new QPushButton("Display tips");
    tipsButton = graphScene->addWidget(tips);
    tipsButton->setPos(0, label->y() - tips->height());

    connect(animate, &QPushButton::clicked, this, &GraphView::startAnimation);
    connect(tips, &QPushButton::clicked, this, &GraphView::toggleTips);
    connect(newGraph, &QPushButton::clicked, this, &GraphView::randomizeGraph);

    //Calculate the cheapest costs for cities.
    calculateCheapestCosts();
    setScene(graphScene);
}

GraphView::~GraphView() {
    delete graphScene;
    delete westCoast;
    delete airportSelector;
    delete animationButton;
    delete animationSlider;
    delete animationLabel;
    delete tipsButton;
    delete animate;
    delete sliderLabel;
    delete label;
    delete slider;
    delete selector;
    delete tips;
    delete newGraph;
    delete priorityQueue;
}

void GraphView::addEdge(QString port1, QString port2, int cost) {
    // If a vertex doesn't already exists in the graph,
    // create a new object and add it to the graph
    if(!vertices.contains(port1))
        vertices[port1] = new Node();
    if(!vertices.contains(port2))
        vertices[port2] = new Node();
    // Adds a new edge from vertex1 to vertex2
    vertices[port1]->addEdge(vertices[port2], cost);
}

void GraphView::resizeEvent(QResizeEvent *event)
{
    QWidget *parent = parentWidget();
    if (!parent) return;

    int newWidth = event->size().width();
    int newHeight = event->size().height();
    // Ensures that the width and height don't violate the aspect ratio.
    if (newWidth < newHeight * aspectRatio)
        resize(newWidth, newWidth / aspectRatio);
    else resize(newHeight * aspectRatio, newHeight);
    // Fairly straightforward: scales the QGraphicsScene
    fitInView(graphScene->sceneRect(), Qt::KeepAspectRatio);

    // Centers StackView inside its parent widget
    int x = (parent->width() - width()) / 2;
    int y = (parent->height() - height()) / 2;
    move(x,y);
}

void GraphView::startAnimation() {
    //Take care of UI and event stuff
    emit animationButtonPushed();
    newGraph->setEnabled(false);
    //Select start node from combobox
    Node* node = vertices[selector->currentText()];
    for (Node* node : vertices.values())
        node->reset();
    node->total = 0;
    //Pq will already be empty when animation starts
    priorityQueue->push(node);
    animationStep(priorityQueue);
}

void GraphView::animationStep(std::priority_queue<Node*, QVector<Node*>, Comparison>* priorityQueue) {

    animationSpeed = slider->value()*20;

    //Get start node
    Node* node = priorityQueue->top();
    priorityQueue->pop();

    //Update the node that we are visiting
    node->setBrush(QBrush(Qt::yellow));

    //Stagger updating ui elements with cursed single shot timing
    int staggerTiming = 0;
    for (Edge* edge : node->neighbors)
    {
        //Find neighbor based on edge
        Node* neighbor;
        if (edge->first == node)
            neighbor = edge->second;
        else
            neighbor = edge->first;

        if (neighbor->visited)
            continue;

        //Flash and unflash edges we are looking at
        QTimer::singleShot(staggerTiming*animationSpeed, this, [edge]{edge->setPen(QPen(Qt::yellow, 5)); edge->costText->setBrush(Qt::yellow);});
        QTimer::singleShot((staggerTiming+1)*animationSpeed, this, [edge]{edge->setPen(QPen(Qt::black, 5)); edge->costText->setBrush(Qt::black);});

        QTimer::singleShot(staggerTiming*animationSpeed, this, [neighbor]{neighbor->setBrush(QBrush(Qt::lightGray));});
        QTimer::singleShot((staggerTiming+1)*animationSpeed, this, [neighbor]{neighbor->setBrush(QBrush(Qt::gray));});

        //Get old cost for updating label
        int oldCost = neighbor->total;
        QTimer::singleShot(staggerTiming*animationSpeed, this, [this, node, edge, neighbor, oldCost]{updateAnimationLabel(node, edge, neighbor, oldCost);});

        //Check if we need to update node total
        if (node->total + edge->cost < neighbor->total && !neighbor->visited)
        {
            neighbor->total = node->total + edge->cost;
            if (!neighbor->addedToQueue)
            {
                priorityQueue->push(neighbor);
                neighbor->addedToQueue = true;
            }
        }
        staggerTiming++;
    }

    //Stagger updating node so text does not turn white until after visiting
    QTimer::singleShot((staggerTiming)*animationSpeed, this, [node]{node->setBrush(QBrush(Qt::black)); node->visited = true;});

    //Check if pq is empty to know if we need to end
    if (!priorityQueue->empty())
    {
        QTimer::singleShot(staggerTiming*animationSpeed, this, [this, priorityQueue]{label->setText("The cheapest known non-visited node is " + vertices.key(priorityQueue->top()) + ", so go there next.");});
        QTimer::singleShot((staggerTiming+1)*animationSpeed, this, [this, priorityQueue]{animationStep(priorityQueue);});
    }
    else
    {
        QTimer::singleShot(staggerTiming*animationSpeed, this, [this, priorityQueue]{label->setText("Every node has been visited, so we are done!");});
        newGraph->setEnabled(true);
    }
}

void GraphView::updateAnimationLabel(Node* node, Edge* edge, Node* neighbor, int oldValue) {
    QString text;
    int calculatedCost = edge->cost + node->total;
    text += "The cost from " + vertices.key(node) + " to " + vertices.key(neighbor) + " is " + QString::number(edge->cost) + " + " + QString::number(node->total) + " = " + QString::number(calculatedCost) + ".";
    if (oldValue == INT_MAX)
        text += " This is the first path we have found to this node, so update the cost.";
    else if (oldValue > calculatedCost)
        text += " This is cheaper than the old cost of " + QString::number(oldValue) + ", so update the cost.";
    else if (oldValue < calculatedCost)
        text += " This is more expensive than the old cost of " + QString::number(oldValue) + ", so don't update the cost.";
    else
        text += " This is the same as the old cost, so nothing really changes.";
    label->setText(text);
}

void GraphView::toggleTips() {
    if (tips->text() == "Display tips")
    {
        tips->setText("Hide tips");
        label->setVisible(true);
    }
    else
    {
        tips->setText("Display tips");
        label->setVisible(false);
    }
}

void GraphView::generateCosts() {

    auto randomOffsetize = [](int input) {
        //Randomize by +- max 1/4 of input
        int randomValue = (rand() % input)/2;
        int randomOffset = randomValue - input/(2*2);
        return input + randomOffset;
    };

    addEdge("Albuquerque", "Denver", randomOffsetize(70));
    addEdge("Albuquerque", "Phoenix", randomOffsetize(65));
    addEdge("Denver", "Salt Lake City", randomOffsetize(75));

    bool slcToWest = false; //Failsafe to make sure slc is at least connected to lv or sf

    //80% chance to have from LV to SLC
    if (rand() % 10 > 0)
    {
        addEdge("Las Vegas", "Salt Lake City", randomOffsetize(72));
        slcToWest = true;
    }
    //20% chance to have edge from SLC to SF
    if (rand()% 10 > 7)
    {
        addEdge("Salt Lake City", "San Francisco", randomOffsetize(160));
        slcToWest = true;
    }
    if (!slcToWest)
        addEdge("Salt Lake City", "San Francisco", randomOffsetize(160));

    int lvConnectionsRand = rand() % 10;

    //60% chance to have edge from LV to SF and LA to LV, 20% to have only one, 20% to have the other only one
    if (lvConnectionsRand > 3)
    {
        addEdge("Las Vegas", "San Francisco", randomOffsetize(125));
        addEdge("Las Vegas", "Los Angeles", randomOffsetize(55));
    }
    else if (lvConnectionsRand > 1)
        addEdge("Las Vegas", "San Francisco", randomOffsetize(125));
    else
        addEdge("Las Vegas", "Los Angeles", randomOffsetize(55));

    //Randomize connections from phoenix to lv and la
    int southConnectionsRand = rand() % 10;

    //40% chance one connection, 40% chance the other, 20% chance both
    if (southConnectionsRand <= 3)
        addEdge("Phoenix", "Los Angeles", randomOffsetize(70));
    else if (southConnectionsRand <= 7)
        addEdge("Phoenix", "Las Vegas", randomOffsetize(80));
    else
    {
        addEdge("Phoenix", "Los Angeles", randomOffsetize(70));
        addEdge("Phoenix", "Las Vegas", randomOffsetize(80));
    }

    //Randomize connections from denver to phoenix/slc to albuquerque
    int rockiesConnections = rand() % 10;

    //20% chance one connection, 20% chance the other, 60% chance neither and have phx->slc instead
    if (rockiesConnections <= 1)
        addEdge("Salt Lake City", "Albuquerque", randomOffsetize(150));
    else if (rockiesConnections <= 3)
        addEdge("Phoenix", "Denver", randomOffsetize(150));
    else
        addEdge("Phoenix", "Salt Lake City", randomOffsetize(100));

    addEdge("Los Angeles", "San Francisco", randomOffsetize(70));

    //Add node bounding boxes
    vertices["Albuquerque"]->setRect(360, 250, 30, 30);
    vertices["Denver"]->setRect(390, 140, 30, 30);
    vertices["Phoenix"]->setRect(225, 290, 30, 30);
    vertices["Salt Lake City"]->setRect(260, 90, 30, 30);
    vertices["Los Angeles"]->setRect(95, 260, 30, 30);
    vertices["Las Vegas"]->setRect(165, 200, 30, 30);
    vertices["San Francisco"]->setRect(30, 130, 30, 30);

    //Add edges to scene
    QSet<Edge*> edges;

    for (Node *airport : vertices)
        for (Edge *flightPath : airport->neighbors)
            edges << flightPath;

    for (Edge *flightPath : edges) {
        flightPath->updateLine();
        graphScene->addItem(flightPath);
        graphScene->addItem(flightPath->costText);
    }

}

void GraphView::startDijkstra(Node* node) {
    //Run dijsktra without running ui to populate cheapestCosts
    node->total = 0;
    std::priority_queue<Node*, QVector<Node*>, Comparison>* priorityQueue = new std::priority_queue<Node*, QVector<Node*>, Comparison>();
    priorityQueue->push(node);
    dijkstraStep(priorityQueue);
}

void GraphView::dijkstraStep(std::priority_queue<Node*, QVector<Node*>, Comparison>* priorityQueue) {

    //Same as before but no single shots
    Node* node = priorityQueue->top();
    priorityQueue->pop();

    node->visited = true;

    for (Edge* edge : node->neighbors)
    {
        //Find neighbor based on edge
        Node* neighbor;
        if (edge->first == node)
            neighbor = edge->second;
        else
            neighbor = edge->first;

        if (neighbor->visited)
            continue;

        if (node->total + edge->cost < neighbor->total && !neighbor->visited)
        {
            neighbor->total = node->total + edge->cost;
            if (!neighbor->addedToQueue)
            {
                priorityQueue->push(neighbor);
                neighbor->addedToQueue = true;
            }
        }
    }
    if (!priorityQueue->empty())
        dijkstraStep(priorityQueue);
}

QHash<QString, QVector<std::pair<QString, int>>> GraphView::getCheapestCosts() {
    return cheapestCosts;
}

void GraphView::randomizeGraph() {
    //Loop through edges, remove old edges from scene, clear neighbors, generate costs, calculate cheapest costs
    for (Node* node : vertices)
    {
        for (Edge* edge : node->neighbors)
        {
            if (graphScene->items().contains(edge->costText))
                graphScene->removeItem(edge->costText);
            if (graphScene->items().contains(edge))
                graphScene->removeItem(edge);
        }
        node->neighbors.clear();
    }
    generateCosts();
    calculateCheapestCosts();
    emit newGraphPushed();
}

void GraphView::calculateCheapestCosts() {
    QVector<std::pair<QString, int>> costsForNode;
    //Run dijkstra on each node to populate cheapest costs array
    for (Node* node : vertices)
    {
        //Run dijkstra for node we are looking at
        startDijkstra(node);
        costsForNode.clear();
        //Now for every node in the graph, the cheapest cost is stored in the total member so just populate the cheapestCosts with that
        for (Node* destination : vertices)
        {
            costsForNode.push_back(std::pair(vertices.key(destination), destination->total));
            destination->reset();
        }
        cheapestCosts.insert(vertices.key(node), costsForNode);
    }
}

Node::Node(QGraphicsItem *parent) : QGraphicsEllipseItem(parent) {
    setBrush(Qt::gray);
    setPen(QPen(Qt::black, 2));
}

void Node::reset() {
    visited = false;
    addedToQueue = false;
    total = INT_MAX;
    setBrush(Qt::gray);
}

void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    QGraphicsEllipseItem::paint(painter, option, widget);
    painter->setFont(QFont("Helvetica [Cronyx]", 8));
    if (visited)
        painter->setPen(QPen(Qt::white));
    painter->drawText(boundingRect(), Qt::AlignCenter, total == INT_MAX ? "∞" : "$" + QString::number(total));
}

void Node::addEdge(Node* neighbor, int cost) {
    Edge *edge = new Edge(this, neighbor, cost);
    neighbors.push_back(edge);
    neighbor->neighbors.push_back(edge);
}

Edge::Edge(Node *n1, Node *n2, int cost, QGraphicsItem *parent) : QGraphicsLineItem(parent), first(n1), second(n2), cost(cost)
{
    setPen(QPen(Qt::black, 5));
}

Edge::~Edge()
{
    delete costText;
}

void Edge::updateLine() {
    QGraphicsLineItem::setLine(QLineF(first->rect().center(), second->rect().center()));
    QFont font("Helvetica [Cronyx]", 10);
    font.setBold(true);

    costText = new QGraphicsSimpleTextItem(QString::number(cost));
    costText->setFont(font);

    // I prepare to compute the offset by deriving a perpendicular angle vector
    // from the line.
    qreal angleInRadians(line().angle() * M_PI / 180);
    QVector2D angleVector(sin(angleInRadians), cos(angleInRadians));
    // Your first want to offset by the width of the pen.
    QVector2D offsetFromLine(pen().width(), pen().width());
    // Then add the dimensions of the text (divided by 2) to the offset.
    offsetFromLine += QVector2D(costText->boundingRect().width() / 2, costText->boundingRect().height() / 2);
    // Multiply by the angle vector for a perpendicular and angle-dependent offset.
    offsetFromLine *= angleVector;
    // Finally, an element is centered on the point offset point.
    offsetFromLine += QVector2D(costText->boundingRect().width() / 2, costText->boundingRect().height() / 2);
    // With the offset's help, it doesn't matter if the text's font is changed;
    // it will still appear a reasonable distance from the line.
    costText->setPos(boundingRect().center() - offsetFromLine.toPointF());
}
