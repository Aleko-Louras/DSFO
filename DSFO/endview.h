#ifndef ENDVIEW_H
#define ENDVIEW_H

#include <QGraphicsView>
#include <Box2D/Box2D.h>

class EndView : public QGraphicsView
{
    Q_OBJECT
public:
    EndView(QWidget *parent = nullptr);
    ~EndView();
    void triggerAnimation(int userScore);
\
private slots:
    void movePlane();
private:
    void resizeEvent(QResizeEvent *event) override;
    int userScore = 0;

    qreal aspectRatio = 1.25;

    // Maximum and minimum y positions for the plane
    float maxYPos = -50.0f;
    float minYPos = -100.0f;
    float pixelsPerMeter = 30;
    // Physics settings
    float timeStep = 1.0f / 60.0f;
    int32 velocityIterations = 6;
    int32 positionIterations = 2;

    QGraphicsScene *titleScene;
    QGraphicsPixmapItem *plane;
    QGraphicsTextItem *title;

    b2World world;
    b2Body* body;
    QTimer* timer;
    std::vector<b2Vec2> angles;
    std::vector<QPixmap> images;
    int angleIndex;
};

#endif // TITLEVIEW_H
