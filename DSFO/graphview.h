#ifndef GRAPHVIEW_H
#define GRAPHVIEW_H

#include <QWidget>
#include <QToolButton>

namespace Ui {
class GraphView;
}

class GraphView : public QWidget
{
    Q_OBJECT

public:
    explicit GraphView(QWidget *parent = nullptr);
    ~GraphView();
    void changeNode(QToolButton* node);
    void startDijkstraAnimation();
    void flashNode(QToolButton* node);

private:
    Ui::GraphView *ui;
};

#endif // GRAPHVIEW_H
