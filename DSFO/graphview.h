#ifndef GRAPHVIEW_H
#define GRAPHVIEW_H

#include <QWidget>
#include <QToolButton>
#include <QDebug>
#include <QMap>

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
        bool visited;
        int total;
        Node(QToolButton* button, bool visited, int total) : button(button), visited(visited), total(total) {}
    };
    class Edge {
    public:
        Node* node;
        int cost;
        Edge(Node* node, int cost) : node(node), cost(cost) {}
    };

    QMap<Node*, QVector<Edge>> graph;
    QVector<QToolButton*> buttons;
    QVector<Node*> nodes;

    Node* albuquerqueNode;
    Node* denverNode;
    Node* phoenixNode;
    Node* lasVegasNode;
    Node* losAngelesNode;
    Node* saltLakeCityNode;
    Node* sanFranciscoNode;

    explicit GraphView(QWidget *parent = nullptr);
    ~GraphView();
    void changeNode(QToolButton* node);
    void startDijkstraAnimation();
    void advanceDijkstraStep(Node* node);
    void findNextStep();
    void flashNode(QToolButton* node, QString value);
    void updateCost(QToolButton* node, QString value);
    void unflashNode(QToolButton* node);
    void createConnections();

private:
    Ui::GraphView *ui;
};

#endif // GRAPHVIEW_H
