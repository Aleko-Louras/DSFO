#ifndef GRAPHVIEW_H
#define GRAPHVIEW_H

#include <QWidget>
#include <QToolButton>
#include <QDebug>
#include <QMap>
#include <QLabel>
#include <QGraphicsView>
#include <QGraphicsEllipseItem>
#include <QComboBox>
#include <queue>
#include <QPushButton>

class Edge;
class Node : public QGraphicsEllipseItem {
public:
    QList<Edge*> neighbors;
    bool visited = false;
    bool addedToQueue = false;
    int total = INT_MAX;

    Node(QGraphicsItem *parent = nullptr);
    void addEdge(Node* neighbor, int cost);
    void reset();

private:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
};

class Edge : public QGraphicsLineItem {
public:
    QGraphicsSimpleTextItem *costText;
    Node *first;
    Node *second;
    int cost;

    Edge(Node* n1, Node* n2, int cost, QGraphicsItem *parent = nullptr);
    ~Edge();
    void updateLine();
};

struct Comparison {
    bool operator()(const Node* first, const Node* second) {
        return first->total > second->total;
    }
};

class GraphView : public QGraphicsView
{
    Q_OBJECT

public:

    GraphView(QWidget *parent = nullptr);
    ~GraphView();
    QHash<QString, QVector<std::pair<QString, int>>> getCheapestCosts();

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

signals:
    animationButtonPushed();
    newGraphPushed();

private:


    void addEdge(QString port1, QString port2, int cost);

    void startAnimation();
    void animationStep(std::priority_queue<Node*, QVector<Node*>, Comparison>* priorityQueue);
    void updateAnimationLabel(Node* node, Edge* edge, Node* neighbor, int oldValue);
    void toggleTips();
    void generateCosts();
    void startDijkstra(Node* node);
    void dijkstraStep(std::priority_queue<Node*, QVector<Node*>, Comparison>* priorityQueue);
    void randomizeGraph();
    void calculateCheapestCosts();

    /// Resizes 'this' StackView alongside its parent widget.
    void resizeEvent(QResizeEvent *event) override;


    QHash<QString, Node*> vertices;

    //To be populated with the cheapest costs to each city from each city, for quizzing purposes.
    //Hash city string name to list of pairs of target city name and int cheapest cost.
    QHash<QString, QVector<std::pair<QString, int>>> cheapestCosts;

    // Ratio of width to height (currently 5:4)
    qreal aspectRatio = 1.25;

    int animationSpeed;

    std::priority_queue<Node*, QVector<Node*>, Comparison>* priorityQueue;
    QGraphicsScene *graphScene;
    QGraphicsEllipseItem *node;
    QGraphicsPixmapItem *westCoast;
    QGraphicsProxyWidget *airportSelector;
    QGraphicsProxyWidget *animationButton;
    QGraphicsProxyWidget *animationLabel;
    QGraphicsProxyWidget *animationSlider;
    QGraphicsProxyWidget *animationSliderLabel;
    QGraphicsProxyWidget *tipsButton;
    QGraphicsProxyWidget *newGraphButton;
    QPushButton *animate;
    QLabel* sliderLabel;
    QComboBox *selector;
    QSlider *slider;
    QLabel *label;
    QPushButton *tips;
    QPushButton *newGraph;
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
