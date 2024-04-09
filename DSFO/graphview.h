#ifndef GRAPHVIEW_H
#define GRAPHVIEW_H

#include <QWidget>
#include <QPushButton>

namespace Ui {
class GraphView;
}

class GraphView : public QWidget
{
    Q_OBJECT

public:
    explicit GraphView(QWidget *parent = nullptr);
    ~GraphView();
    void changeNode(QPushButton* node);

private:
    Ui::GraphView *ui;
};

#endif // GRAPHVIEW_H
