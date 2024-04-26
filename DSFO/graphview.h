/**
header file for the graphview, i.e. the Dijkstra view and animation.
University of Utah CS 3505 final project, group LAQE
Writen by Lucas Pearce, Ethan Block, Will Black, Quinn Pritchett, Aleko Louras
**/

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
    //Total cost of travelling to the node
    int total = INT_MAX;

    ///
    /// \brief Node constructor. Creates a node with no initialized values, but is drawn in gray with a black outline.
    /// \param parent: the parent qgraphicsitem
    ///
    Node(QGraphicsItem *parent = nullptr);

    ///
    /// \brief addEdge: adds an edge to the node.
    /// \param neighbor: the neighboring node on the other side of the edge.
    /// \param cost: the cost of the edge
    ///
    void addEdge(Node* neighbor, int cost);

    ///
    /// \brief reset: resets the node (i.e. when doing a new animation) by setting its cost to inf, visited to false, etc...
    ///
    void reset();

private:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
};

class Edge : public QGraphicsLineItem {
public:
    //Displaying the cost in the view
    QGraphicsSimpleTextItem *costText;
    Node *first;
    Node *second;
    int cost;

    ///
    /// \brief Edge constructor. Given two nodes, a cost, and qgraphics parent item
    /// \param n1: the first node this edge is attached to
    /// \param n2: the second node this edge is attached to
    /// \param cost: the cost of this node
    /// \param parent: the parent qgraphicsitem
    ///
    Edge(Node* n1, Node* n2, int cost, QGraphicsItem *parent = nullptr);

    ///
    /// \brief edge destructor
    ///
    ~Edge();

    ///
    /// \brief updateLine: updates the line in the view by essentially recalculating how to draw it and the text, doing lots of fancy trig.
    ///
    void updateLine();
};

///Struct needed for comparing nodes in priority queue.
struct Comparison {
    bool operator()(const Node* first, const Node* second) {
        return first->total > second->total;
    }
};

class GraphView : public QGraphicsView
{
    Q_OBJECT

public:

    ///
    /// \brief GraphView constructor.
    /// \param parent: the qwidget parent.
    ///
    GraphView(QWidget *parent = nullptr);

    ///
    /// \brief the GraphView destructor
    ///
    ~GraphView();

    ///
    /// \brief getCheapestCosts: simply returns cheapestCosts so mainWindow has the data.
    /// \return the cheapest costs to get to each city from each city.
    ///
    QHash<QString, QVector<std::pair<QString, int>>> getCheapestCosts();

signals:
    ///
    /// \brief animationButtonPushed: simple signal for when the animation button is pushed in the graphView
    ///
    void animationButtonPushed();

    ///
    /// \brief newGraphPushed: simple signal for when the new graph button is pushed in the graphView
    ///
    void newGraphPushed();

private:


    ///
    /// \brief addEdge: adds an edge to the vertices.
    /// \param port1: the string key to the first airport of the edge.
    /// \param port2: the string key to the second airport of the edge.
    /// \param cost: the cost of the edge
    ///
    void addEdge(QString port1, QString port2, int cost);

    ///
    /// \brief startAnimation: simply starts the dijkstra animation
    ///
    void startAnimation();

    ///
    /// \brief animationStep: recursive call to advance to the next step of the dijkstra animation
    /// \param priorityQueue: the priority queue dijkstra is working with
    ///
    void animationStep(std::priority_queue<Node*, QVector<Node*>, Comparison>* priorityQueue);

    ///
    /// \brief updateAnimationLabel: updates the tips label during the animation.
    /// \param node: the current node being looked at, passed in
    /// \param edge: the current edge being looked at
    /// \param neighbor: the current neighbor being looked at
    /// \param oldValue: the old value of the neighbor, because the current cost is overwritten
    ///
    void updateAnimationLabel(Node* node, Edge* edge, Node* neighbor, int oldValue);

    ///
    /// \brief toggleTips: simply toggles the tip button and label
    ///
    void toggleTips();

    ///
    /// \brief generateCosts: generates random costs/edges
    ///
    void generateCosts();

    ///
    /// \brief startDijkstra: starts dijkstra on a node but does not animate, used to calculate cheapest costs.
    /// \param node
    ///
    void startDijkstra(Node* node);

    ///
    /// \brief dijkstraStep: adjances dijkstra on a node with no animation, also used to calculate cheapest costs.
    /// \param priorityQueue: the current priority queue dijkstra is working with.
    ///
    void dijkstraStep(std::priority_queue<Node*, QVector<Node*>, Comparison>* priorityQueue);

    ///
    /// \brief randomizeGraph: randomizes the graph to a certain extent, by changing weights and some edges, and taking care of regenerating the graph/view
    ///
    void randomizeGraph();

    ///
    /// \brief calculateCheapestCosts: calculates the cheapest costs given the current graph.
    ///
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
};

#endif // GRAPHVIEW_H
