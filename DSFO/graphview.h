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

signals:
    void animationButtonPushed();
    void newGraphPushed();

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
    QComboBox *selector;
    QSlider *slider;
    QLabel *label;
    QPushButton *tips;
};

#endif // GRAPHVIEW_H
