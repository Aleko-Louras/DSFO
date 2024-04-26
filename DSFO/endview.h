#ifndef ENDVIEW_H
#define ENDVIEW_H

#include <QGraphicsView>
#include <QWidget>
#include <Box2D/Box2D.h>
#include <QTimer>
#include <QPainter>
#include <QTransform>
#include <QGraphicsView>
#include <QResizeEvent>
#include <QGraphicsPixmapItem>


/**
 * @brief The EndView class
 * A class that inherets from QGraphicsView to be used with a QGraphicsScene for animation of the end screen,
 * implemented with the Box2D physics engine.
 */
class EndView : public QGraphicsView
{
    Q_OBJECT
public:

    /**
     * @brief EndView - The EndView constructor.
     * @param parent - The parental QWidget class
     */
    EndView(QWidget *parent = nullptr);

    /**
     * @brief ~EndView - The Endview destructor.
     */
    ~EndView();

    /**
     * @brief triggerAnimation A method used by the main window class to activate the end screen physics
     * when the program reaches the end screen.
     * @param userScore - The main windows player score to be displayed.
     */
    void triggerAnimation(int userScore);
\

private slots:

    /**
     * @brief movePlane - A slot for the physics of the Box2D engine to move the plane component in the
     * QGraphicsScene.
     */
    void movePlane();


private:
    /**
     * @brief resizeEvent - A private method that resizes the QGraphicsScene to the current window size.
     * @param event - The event to be resized to the window.
     */
    void resizeEvent(QResizeEvent *event) override;

    /**
     * @brief userScore - The users score that is passed from main window.
     */
    int userScore = 0;

    /**
     * @brief aspectRatio The aspect ratio for resizing the QGrpahicsScene.
     */
    qreal aspectRatio = 1.25;

    // Maximum and minimum y positions for the plane
    float maxYPos = -50.0f;
    float minYPos = -100.0f;

    /**
     * @brief pixelsPerMeter - The measurement to scale the box2D location to a location
     * in the QGraphicsScene.
     */
    float pixelsPerMeter = 30;

    /**
     * @brief timeStep, velocityIterations, positionIterations - Box2D necessities for getting a live world
     * physics update per a timer.
     */
    float timeStep = 1.0f / 60.0f;
    int32 velocityIterations = 6;
    int32 positionIterations = 2;

    /**
     * @brief titleScene - The QGraphicsScene for the end text.
     * @brief plane - The pixmap plane icon that is moved with box2d.
     * @brief title
     */
    QGraphicsScene *titleScene;
    QGraphicsPixmapItem *plane;
    QGraphicsTextItem *title;

    /**
     * @brief world - The box2D physics world
     * @brief body - The box2D physics body
     * @brief timer - The timer responsible for getting the live world
     */
    b2World world;
    b2Body* body;
    QTimer* timer;
};

#endif // TITLEVIEW_H
