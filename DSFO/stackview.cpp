#include "stackview.h"
#include <QObject>
#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QDebug>
#include <QLayout>
#include <QResizeEvent>
#include <QSequentialAnimationGroup>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QPushButton>
#include <QGraphicsProxyWidget>
#include <QTimer>
#include <QGraphicsLinearLayout>

StackView::StackView(QWidget *parent) : QGraphicsView(parent)
{
    // These are helpful for relative positioning
    QRectF sceneBox(0, 0, 500, 400); //! This is linked to the aspect ratio
    qreal horizontalCenter = sceneBox.center().x();

    qreal tunnelWidth = 80;
    qreal tunnelHeight = 40;
    qreal conveyorWidth = 64;
    qreal conveyorHeight = 160;
    qreal luggageAdderWidth = 54;
    qreal luggageAdderHeight = 40;
    qreal planeWidth = sceneBox.width() / 2;
    qreal planeHeight = 200;

    // All of the values below are defined using positions relative to the
    // sceneBox, which helps with reusability and, possibly, readability.
    // Absolute positions can also be used; for example:
    // rightTunnelY = sceneBox.bottom() - tunnelHeight = 400 - 50 = 350
    qreal leftTunnelX = (horizontalCenter - tunnelWidth) / 2;
    qreal leftTunnelY = sceneBox.top();
    qreal leftConveyorX = (horizontalCenter - conveyorWidth) / 2;
    qreal leftConveyorY = leftTunnelY + tunnelHeight;

    qreal rightTunnelX = horizontalCenter + leftTunnelX;
    qreal rightTunnelY = sceneBox.bottom() - tunnelHeight;
    qreal rightConveyorX = horizontalCenter + leftConveyorX;
    qreal rightConveyorY = rightTunnelY - conveyorHeight;

    qreal luggageAdderX = (horizontalCenter - luggageAdderWidth) / 2;
    qreal luggageAdderY = tunnelHeight + conveyorHeight - luggageAdderHeight - (conveyorWidth - luggageAdderWidth) / 2;
    QLineF dividingLine(horizontalCenter, sceneBox.top(), horizontalCenter, sceneBox.bottom());

    // Below is where the scene is actually created and items added to it.
    stackScene = new QGraphicsScene(sceneBox, this);
    plane = stackScene->addRect(horizontalCenter, sceneBox.top(), planeWidth, planeHeight);


    //Add plane
    planeImg = new QPixmap(":/images/birdsEyePlane.png");
    planeImg->scaled(planeWidth, planeHeight, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
    planePixmap = new QGraphicsPixmapItem(*planeImg);
    planePixmap->setPos(horizontalCenter - planeImg->width()/2, sceneBox.top());
    stackScene->addItem(planePixmap);


    divider = stackScene->addLine(dividingLine, QPen(Qt::black, 3));

    receivingConveyor = stackScene->addRect(leftConveyorX, leftConveyorY, conveyorWidth, conveyorHeight, QPen(Qt::black, 3), Qt::gray);
    sendingConveyor = stackScene->addRect(rightConveyorX, rightConveyorY, conveyorWidth, conveyorHeight, QPen(Qt::black, 3), Qt::gray);
    receivingTunnel = stackScene->addRect(leftTunnelX, leftTunnelY, tunnelWidth, tunnelHeight, QPen(Qt::black, 3), Qt::gray);
    sendingTunnel = stackScene->addRect(rightTunnelX, rightTunnelY, tunnelWidth, tunnelHeight, QPen(Qt::black, 3), Qt::gray);

    receivingTunnel->setZValue(1);
    sendingTunnel->setZValue(1);

    QPushButton *addLuggage = new QPushButton("+");
    addLuggage->setFixedSize(luggageAdderWidth, luggageAdderHeight);
    addLuggage->setStyleSheet("QPushButton { background-color: white; }");
    addLuggage->move(luggageAdderX, luggageAdderY);
    connect(addLuggage, &QPushButton::clicked,
            this, &StackView::addLuggage);
    luggageAdder = stackScene->addWidget(addLuggage);

    // QGraphicsWidget *centralButtons = new QGraphicsWidget;
    // stackScene->addItem(new QGraphicsWidget);
    // QGraphicsLinearLayout *buttonsContainer = new QGraphicsLinearLayout(Qt::Vertical, centralButtons);

    QPushButton *animationBtn = new QPushButton("Animate");
    connect(animationBtn, &QPushButton::clicked,
            this, &StackView::animate);
    animationBtn->setStyleSheet("QPushButton { background-color: white; }"
                                "QPushButton::default { background-color: red; }");
    animationBtn->setEnabled(false);

    animationButton = stackScene->addWidget(animationBtn);
    animationButton->setPos(horizontalCenter - animationButton->minimumWidth() / 2, sceneBox.center().y()  - animationButton->minimumHeight() / 2);

    setScene(stackScene);
}

StackView::~StackView() {
    delete stackScene;
    delete receivingTunnel;
    delete receivingConveyor;
    delete sendingTunnel;
    delete sendingConveyor;
    delete divider;
    delete luggageAdder;
}

void StackView::resizeEvent(QResizeEvent *event)
{
    QWidget *parent = parentWidget();
    if (!parent) return;

    int newWidth = event->size().width();
    int newHeight = event->size().height();
    // Ensures that the width and height don't violate the aspect ratio.
    if (newWidth < newHeight * aspectRatio)
        resize(newWidth, newWidth / aspectRatio);
    else resize(newHeight * aspectRatio, newHeight);

    // Fairly straightforward: scales the QGraphicsScene
    fitInView(stackScene->sceneRect(), Qt::KeepAspectRatio);

    // Centers StackView inside its parent widget
    int x = (parent->width() - width()) / 2;
    int y = (parent->height() - height()) / 2;
    move(x,y);
}

void StackView::addLuggage() {
    if (!luggageAdder->isVisible())
        return;

    luggageAdder->setVisible(false);

    QGraphicsRectItem *bag = stackScene->addRect(luggageAdder->rect(), QPen(Qt::black, 2), Qt::white);
    QGraphicsTextItem *number = new QGraphicsTextItem("A", bag);
    number->setPos(QPointF(bag->x() + (bag->boundingRect().width() / 2), bag->y() + (bag->boundingRect().height() / 2)));

    GraphicsAnimator *luggageAnimator = new GraphicsAnimator(bag, "pos");
    connect(luggageAnimator->animation(), &QPropertyAnimation::finished,
            this,
            [this, luggageAnimator] {
                luggageAnimator->setPos(QPointF(sceneRect().center().x() + luggageAdder->x(), sceneRect().bottom() - luggageAdder->rect().height()));
                luggageAnimator->animation()->setStartValue(luggageAnimator->pos());
                luggageAnimator->animation()->setEndValue(QPointF(luggageAnimator->pos().x(), sceneRect().bottom() - luggageAdder->y() - luggageAdder->rect().height()));
                luggageAnimator->animation()->setDuration(3000);
            });

    luggageAnimator->animation()->setStartValue(luggageAdder->pos());
    luggageAnimator->animation()->setEndValue(QPointF(luggageAdder->x(), sceneRect().top()));
    luggageAnimator->animation()->setDuration(3000);
    luggageAnimator->animation()->start();

    luggage.push_back(luggageAnimator);
    animationButton->setEnabled(luggage.count() > 2);

    QTimer::singleShot(800, this, [this] {luggageAdder->setVisible(true);});
}

void StackView::animate() {
    animationButton->setEnabled(false);

    if (luggage.last()->animation()->state() == QAbstractAnimation::Running) {
        QTimer::singleShot(luggage.last()->animation()->duration() - luggage.last()->animation()->currentTime(), this, &StackView::animate);
        return;
    }

qDebug() << "Animate called";

    tryNextAnimation();
}

void StackView::tryNextAnimation() {

    if (luggage.empty())
        return;

    GraphicsAnimator *animator = luggage.takeAt(luggage.size() - 1);
    QPropertyAnimation *animation = animator->animation();
    qreal targetWidth = animator->target->boundingRect().width();
    qreal endY = animation->endValue().toPointF().y();

    animation->start();

    QTimer::singleShot(animation->duration() * (targetWidth / (sceneRect().bottom() - endY)) * 1.1, this, &StackView::tryNextAnimation);
    qDebug() << "Lambda called";
}


