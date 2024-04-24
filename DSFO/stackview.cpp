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
    QFont font("Helvetica [Cronyx]", 16);
    font.setBold(true);
    addLuggage->setFont(font);
    connect(addLuggage, &QPushButton::clicked,
            this, &StackView::addLuggage);
    luggageAdder = stackScene->addWidget(addLuggage);

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

    QGraphicsRectItem *bag = stackScene->addRect(luggageAdder->rect(), QPen(Qt::black, 2), Qt::white);
    QGraphicsSimpleTextItem *number = new QGraphicsSimpleTextItem(QString::number(luggage.count() + 1), bag);
    GraphicsAnimator *luggageAnimator = new GraphicsAnimator(bag, "pos");

    luggageAdder->setVisible(false);
    luggage.push_back(luggageAnimator);
    animationButton->setEnabled(luggage.count() > 2);

    QFont font("Helvetica [Cronyx]", 14);
    font.setBold(true);
    number->setFont(font);
    number->setPos(QPointF(bag->x() + (bag->boundingRect().width() - number->boundingRect().width()) / 2,
                           bag->y() + (bag->boundingRect().height() - number->boundingRect().height()) / 2));

    // Advances the luggage to the sending conveyor when its animation is finished for the
    // the receiving conveyor
    connect(luggageAnimator->animation(), &QPropertyAnimation::finished,
            this,
            [this, luggageAnimator] {
                luggageAnimator->setPos(QPointF(sceneRect().center().x() + luggageAdder->x(), sceneRect().bottom() - luggageAdder->rect().height()));
                luggageAnimator->animation()->setStartValue(luggageAnimator->pos());
                luggageAnimator->animation()->setEndValue(QPointF(luggageAnimator->pos().x(), luggageAnimator->pos().y() - luggageAdder->y()));
                luggageAnimator->animation()->setDuration(3000);
            });

    // Animates luggage's progress along the receiving conveyor
    luggageAnimator->animation()->setStartValue(luggageAdder->pos());
    luggageAnimator->animation()->setEndValue(QPointF(luggageAdder->x(), sceneRect().top()));
    luggageAnimator->animation()->setDuration(2000);
    luggageAnimator->animation()->start();

    qreal delay = 2000 * (luggageAdder->boundingRect().height() / luggageAdder->y());

    QTimer::singleShot(delay, this, [this] {luggageAdder->setVisible(true);});
}

void StackView::animate() {
    animationButton->setEnabled(false);
    luggageAdder->setEnabled(false);

    if (luggage.last()->animation()->state() == QAbstractAnimation::Running) {
        QTimer::singleShot(luggage.last()->animation()->duration() - luggage.last()->animation()->currentTime(), this, &StackView::tryNextAnimation);
        return;
    }

    tryNextAnimation();
}

void StackView::tryNextAnimation() {

    if (luggage.empty()) {
        luggageAdder->setEnabled(true);
        return;
    }

    GraphicsAnimator *animator = luggage.takeLast();
    QPropertyAnimation *animation = animator->animation();

    qreal targetWidth = animator->target->boundingRect().width();
    qreal travelDistance = sceneRect().bottom() - animation->endValue().toPointF().y();
    qreal delay = animation->duration() * (targetWidth / travelDistance);

    animation->start();
    QTimer::singleShot(delay, this, &StackView::tryNextAnimation);

}


