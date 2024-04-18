#ifndef GRAPHVIEW_H
#define GRAPHVIEW_H

#include <QWidget>
#include <QToolButton>
#include <QDebug>
#include <QMap>
#include <QLabel>
#include <QGraphicsView>
#include <QGraphicsEllipseItem>

class Edge;
class Node : public QGraphicsEllipseItem {
public:
    QList<Edge*> neighbors;

    bool visited = false;
    int total = INT_MAX;
    // Node(qreal x, qreal y, qreal width, qreal height, QGraphicsItem *parent = nullptr);
    Node(QGraphicsItem *parent = nullptr);
    void addEdge(Node* neighbor, int cost);
};

class Edge : public QGraphicsLineItem {
public:
    std::pair<Node*, Node*> neighbors;
    int cost;
    Edge(Node* n1, Node* n2, int cost, QGraphicsItem *parent = nullptr);
};

class GraphView : public QGraphicsView
{
    Q_OBJECT

public:
    // class Node {
    // public:
    //     QToolButton* button;
    //     QLabel* label;
    //     QString name;
    //     bool visited;
    //     int total;
    //     Node(QToolButton* button, QLabel* label, QString name, bool visited, int total) : button(button), label(label), name(name), visited(visited), total(total) {}
    // };
    // class Edge {
    // public:
    //     Node* node;
    //     int cost;
    //     Edge(Node* node, int cost) : node(node), cost(cost) {}
    // };

    // QMap<Node*, QVector<Edge>> graph;
    // QVector<Node*> nodes;

    // int animationSpeed = 0;

    // Node* albuquerqueNode;
    // Node* denverNode;
    // Node* phoenixNode;
    // Node* lasVegasNode;
    // Node* losAngelesNode;
    // Node* saltLakeCityNode;
    // Node* sanFranciscoNode;

    GraphView(QWidget *parent = nullptr);
    ~GraphView();

    // void startDijkstraAnimation();
    // void advanceDijkstraStep(Node* node);
    // void findNextStep();
    // void updateStepLabel(Edge edge, Node* node, int oldTotal);
    // void flashNode(QToolButton* node, QLabel* label, QString value);
    // void updateCost(QLabel* label, QString value);
    // void unflashNode(QToolButton* node, QLabel* label);
    // void dimNode(QToolButton* node, QLabel* label);
    // void flashEdge(Edge edge, Node* node);
    // void createConnections();

private:


    void addEdge(QString port1, QString port2, int cost);

    /// Resizes 'this' StackView alongside its parent widget.
    void resizeEvent(QResizeEvent *event) override;


    QHash<QString, Node*> vertices;

    // Ratio of width to height (currently 5:4)
    qreal aspectRatio = 1.25;

    QGraphicsScene *graphScene;
    QGraphicsEllipseItem *node;
    QGraphicsPixmapItem *westCoast;
    QGraphicsProxyWidget *airportSelector;
    QGraphicsProxyWidget *animationButton;
    // Ratio of width to height (currently 5:4)
    // qreal aspectRatio = 1.25;

    // QGraphicsScene *stackScene;
    // // Items drawn in the QGraphicsScene
    // QGraphicsRectItem *receivingTunnel;
    // QGraphicsRectItem *receivingConveyor;
    // QGraphicsRectItem *sendingTunnel;
    // QGraphicsRectItem *sendingConveyor;
    // QGraphicsLineItem *divider;
    // // Luggage-related items
    // QGraphicsRectItem *luggage;
    // QGraphicsProxyWidget *luggageAdder;
};

#endif // GRAPHVIEW_H
