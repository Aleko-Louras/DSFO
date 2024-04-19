#include "graphview.h"
#include <QGraphicsItem>
#include <QResizeEvent>
#include <QPushButton>
#include <QGraphicsProxyWidget>
#include <QComboBox>

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
    addEdge("Salt Lake City", "Las Vegas", 90);
    addEdge("Las Vegas", "Los Angeles", 50);
    addEdge("Las Vegas", "San Francisco", 180);
    addEdge("Los Angeles", "San Francisco", 100);

    vertices["Albuquerque"]->setRect(360, 250, 25, 25);
    vertices["Denver"]->setRect(390, 140, 25, 25);
    vertices["Phoenix"]->setRect(225, 290, 25, 25);
    vertices["Salt Lake City"]->setRect(260, 90, 25, 25);
    vertices["Los Angeles"]->setRect(95, 260, 25, 25);
    vertices["Las Vegas"]->setRect(165, 200, 25, 25);
    vertices["San Francisco"]->setRect(30, 130, 25, 25);

    qDebug() << vertices.count();

    QSet<Edge*> edges;

    for (Node *airport : vertices) {
        graphScene->addItem(airport);
        for (Edge *flightPath : airport->neighbors)
            edges << flightPath;
        airport->setZValue(1);
    }

    for (Edge *flightPath : edges) {
        auto& flight = flightPath->neighbors;
        QPointF pointA(flight.first->rect().center());
        QPointF pointB(flight.second->rect().center());
        flightPath->setLine(QLineF(pointA.y() < pointB.y() ? pointA, pointB : pointB, pointA));
        graphScene->addItem(flightPath);
    }

    QComboBox *selector = new QComboBox();
    for (const QString& airport : vertices.keys())
        selector->addItem(airport);
    airportSelector = graphScene->addWidget(selector);

    QPushButton *animate = new QPushButton("Animate");
    animationButton = graphScene->addWidget(animate);
    animationButton->setPos(50, 50);

    setScene(graphScene);
}

GraphView::~GraphView() {
    delete graphScene;
    delete westCoast;
    delete node;
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

Node::Node(QGraphicsItem *parent) : QGraphicsEllipseItem(parent)
{
    setBrush(Qt::gray);
}

Node::~Node()
{
    delete totalText;
}

void Node::reset() {
    visited = false;
    total = INT_MAX;
    setBrush(Qt::gray);
}

void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    QGraphicsEllipseItem::paint(painter, option, widget);
    painter->setFont(QFont("Helvetica [Cronyx]", 10));
    painter->drawText(boundingRect(), Qt::AlignCenter, total == INT_MAX ? "∞" : QString::number(total));
}

void Node::addEdge(Node* neighbor, int cost) {
    Edge *edge = new Edge(this, neighbor, cost);
    neighbors.push_back(edge);
    neighbor->neighbors.push_back(edge);
}

Edge::Edge(Node *n1, Node *n2, int cost, QGraphicsItem *parent) : QGraphicsLineItem(parent), neighbors(n1, n2), cost(cost)
{
    setPen(QPen(Qt::black, 5));
    // n1->setPos(100, 50);
    // setLine(QLineF(n1->pos(), n2->pos()));
}

void Edge::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    QGraphicsLineItem::paint(painter, option, widget);

    qreal angleInRadians(line().angle() * M_PI / 180);
    QVector2D angleVector(sin(angleInRadians), cos(angleInRadians));
    QRectF textBox(boundingRect().topLeft() - angleVector.toPointF() * 20, boundingRect().size());

    QFont font("Helvetica [Cronyx]", 10);
    font.setBold(true);
    painter->setFont(font);
    painter->setPen(Qt::black);
    painter->drawText(textBox, Qt::AlignCenter, cost == INT_MAX ? "∞" : QString::number(cost));

    // qDebug() << line().angle();
    // qDebug() << angleInRadians;
    // qDebug() << angleVector;
}
