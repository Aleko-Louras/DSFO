#include "graphview.h"
#include <QGraphicsItem>
#include <QResizeEvent>
#include <QPushButton>
#include <QGraphicsProxyWidget>
#include <QComboBox>
#include <queue>
#include <QTimer>
#include <QMargins>

GraphView::GraphView(QWidget *parent) : QGraphicsView(parent) {
    // ESSENTIAL SETTINGS!
    setMinimumSize(500, 400);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QRectF sceneBox(0, 0, width(), height());
    QImage westCoastMap = QImage(":/images/swbackground.png").scaled(size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    graphScene = new QGraphicsScene(sceneBox, this);
    westCoast = graphScene->addPixmap(QPixmap::fromImage(westCoastMap));

    addEdge("Albuquerque", "Denver", 80);
    addEdge("Albuquerque", "Phoenix", 80);
    addEdge("Denver", "Salt Lake City", 120);
    addEdge("Phoenix", "Los Angeles", 130);
    addEdge("Phoenix", "Salt Lake City", 200);
    addEdge("Las Vegas", "Salt Lake City", 90);
    addEdge("Las Vegas", "Los Angeles", 50);
    addEdge("Las Vegas", "San Francisco", 180);
    addEdge("Los Angeles", "San Francisco", 100);

    vertices["Albuquerque"]->setRect(360, 250, 30, 30);
    vertices["Denver"]->setRect(390, 140, 30, 30);
    vertices["Phoenix"]->setRect(225, 290, 30, 30);
    vertices["Salt Lake City"]->setRect(260, 90, 30, 30);
    vertices["Los Angeles"]->setRect(95, 260, 30, 30);
    vertices["Las Vegas"]->setRect(165, 200, 30, 30);
    vertices["San Francisco"]->setRect(30, 130, 30, 30);

    qDebug() << vertices.count();

    QSet<Edge*> edges;

    for (Node *airport : vertices) {
        graphScene->addItem(airport);
        for (Edge *flightPath : airport->neighbors)
            edges << flightPath;
        airport->setZValue(1);
    }

    for (Edge *flightPath : edges) {
        flightPath->updateLine();
        graphScene->addItem(flightPath);
        graphScene->addItem(flightPath->costText);
    }

    selector = new QComboBox();
    for (const QString& airport : vertices.keys())
        selector->addItem(airport);
    airportSelector = graphScene->addWidget(selector);
    airportSelector->setPos(0, 250);
    airportSelector->setZValue(1);

    QPushButton *animate = new QPushButton("Animate");
    animationButton = graphScene->addWidget(animate);
    animationButton->setPos(100, 375);

    slider = new QSlider(Qt::Horizontal);
    animationSlider = graphScene->addWidget(slider);
    animationSlider->setPos(0, 350);
    slider->setMinimum(10);
    slider->setMaximum(200);
    slider->setValue(60);

    QLabel *label = new QLabel("label");
    animationLabel = graphScene->addWidget(label);
    animationLabel->setPos(380, 25);

    connect(animate, &QPushButton::clicked, this, &GraphView::startAnimation);

    setScene(graphScene);
}

GraphView::~GraphView() {
    delete graphScene;
    delete westCoast;
    delete airportSelector;
    delete animationButton;
    delete animationSlider;
    delete animationLabel;
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
    Node* node = vertices[selector->currentText()];
    for (Node* node : vertices.values())
        node->reset();
    node->total = 0;
    std::priority_queue<Node*, QVector<Node*>, Comparison>* priorityQueue = new std::priority_queue<Node*, QVector<Node*>, Comparison>();
    priorityQueue->push(node);
    animationStep(priorityQueue);
}

void GraphView::animationStep(std::priority_queue<Node*, QVector<Node*>, Comparison>* priorityQueue) {

    animationSpeed = slider->value()*20;

    Node* node = priorityQueue->top();
    priorityQueue->pop();

    //Update the node that we are visiting
    node->setBrush(QBrush(Qt::yellow));

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

        QTimer::singleShot(staggerTiming*animationSpeed, this, [neighbor]{neighbor->setBrush(Qt::gray);});
        // QTimer::singleShot((staggerTiming+1)*animationSpeed, this, [neighbor]{neighbor->setBrush(Qt::gray);});

        if (node->total + edge->cost < neighbor->total && !neighbor->visited)
        {
            neighbor->total = node->total + edge->cost;
            //Have to change node brush slightly for some reason to get text to display correctly
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
    // QTimer::singleShot((staggerTiming+1)*animationSpeed, this, [node]{});
    if (!priorityQueue->empty())
        QTimer::singleShot((staggerTiming+1)*animationSpeed, this, [this, priorityQueue]{animationStep(priorityQueue);});
}

Node::Node(QGraphicsItem *parent) : QGraphicsEllipseItem(parent)
{
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
