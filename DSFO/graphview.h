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
        void visit () {visited = true;}
    };
    class Edge {
    public:
        Node node;
        bool visited;
        int cost;
        Edge(Node node, bool visited, int cost) : node(node), visited(visited), cost(cost) {}
        void visit () {visited = true;}
    };

    QMap<QToolButton*, QVector<Edge>> graph;
    QVector<QToolButton*> buttons;
    QVector<Node> nodes;

    explicit GraphView(QWidget *parent = nullptr);
    ~GraphView();
    void changeNode(QToolButton* node);
    void startDijkstraAnimation(QToolButton* node);
    void advanceDijkstraStep(QToolButton* node);
    void findNextStep();
    void flashNode(QToolButton* node, QString value);
    void unflashNode(QToolButton* node);
    void createConnections();

private:
    Ui::GraphView *ui;
};

#endif // GRAPHVIEW_H
