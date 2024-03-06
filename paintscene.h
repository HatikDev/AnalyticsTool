#ifndef PAINTSCENE_H
#define PAINTSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <memory>
#include "rectangle.h"

class PaintScene : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit PaintScene(QObject *parent = nullptr);
     ~PaintScene();

signals:
    void currentRectChanged(); // TODO: maybe it should be deleted
    void rectAdded(std::shared_ptr<Rectangle> figure);

private:
    std::shared_ptr<Rectangle> m_current;

    static size_t counter;

    void mousePressEvent(QGraphicsSceneMouseEvent* event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);

private slots:
    void rectSelectionChanged();
};

#endif // PAINTSCENE_H
