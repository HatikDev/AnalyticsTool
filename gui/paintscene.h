#ifndef PAINTSCENE_H
#define PAINTSCENE_H

#include "model/picture.h"
#include "gui/rectangle.h"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <memory>

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

public slots:
    // Calls when rect was added not from PaintScene.
    // For instance, loaded from dataset
    void on_rectAdded(std::shared_ptr<Rectangle> rect);

private slots:
    void rectSelectionChanged();
};

#endif // PAINTSCENE_H
