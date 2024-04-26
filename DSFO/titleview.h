/**
header file for the title view, which is where the title is actually displayed in the graphical interface.
University of Utah CS 3505 final project, group LAQE
Writen by Lucas Pearce, Ethan Block, Will Black, Quinn Pritchett, Aleko Louras
**/

#ifndef TITLEVIEW_H
#define TITLEVIEW_H

#include <QGraphicsView>
#include <Box2D/Box2D.h>
#include <QLabel>

/**
 * @brief The TitleView class
 * A class that represents the box 2d animation of the programs title screen, along with the title text, and
 * explanation label.
 */
class TitleView : public QGraphicsView
{
    Q_OBJECT
public:

    /**
     * @brief TitleView - The TitleView constructor.
     * @param parent - The parental QWidget class
     */
    TitleView(QWidget *parent = nullptr);

    /**
     * @brief ~TitleView - The TitleView destructor.
     */
    ~TitleView();

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
     * @brief aspectRatio The aspect ratio for resizing the QGrpahicsScene.
     */
    qreal aspectRatio = 1.25;

    /**
     * @brief maxYPos - The maximum y position to correct the plane force.
     * @brief maxXpos - The maximum x position to correct the plane force.
     */
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

    /**
     * @brief world - The box2D physics world
     * @brief body - The box2D physics body
     * @brief timer - The timer responsible for getting the live world
     */
    b2World world;
    b2Body* body;
};

#endif // TITLEVIEW_H
