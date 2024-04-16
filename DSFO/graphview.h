#ifndef GRAPHVIEW_H
#define GRAPHVIEW_H

#include <QWidget>
#include <QToolButton>
#include <QDebug>
#include <QMap>
#include <QLabel>

namespace Ui {
class GraphView;
}

class GraphView : public QWidget
{
    Q_OBJECT

public:
    class Node {
    public:
        QToolButton* button;
        QLabel* label;
        bool visited;
        int total;
        Node(QToolButton* button, QLabel* label, bool visited, int total) : button(button), label(label), visited(visited), total(total) {}
    };
    class Edge {
    public:
        Node* node;
        int cost;
        Edge(Node* node, int cost) : node(node), cost(cost) {}
    };

    QMap<Node*, QVector<Edge>> graph;
    QVector<Node*> nodes;

    int animationSpeed = 2000;

    Node* albuquerqueNode;
    Node* denverNode;
    Node* phoenixNode;
    Node* lasVegasNode;
    Node* losAngelesNode;
    Node* saltLakeCityNode;
    Node* sanFranciscoNode;

    explicit GraphView(QWidget *parent = nullptr);
    ~GraphView();
    void startDijkstraAnimation();
    void advanceDijkstraStep(Node* node);
    void findNextStep();
    void flashNode(QToolButton* node, QLabel* label, QString value);
    void updateCost(QLabel* label, QString value);
    void unflashNode(QToolButton* node, QLabel* label);
    void dimNode(QToolButton* node, QLabel* label);
    void flashEdge(Edge edge, Node* node);
    void createConnections();

private:
    Ui::GraphView *ui;
};

#endif // GRAPHVIEW_H
