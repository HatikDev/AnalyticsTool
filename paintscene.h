#ifndef PAINTSCENE_H
#define PAINTSCENE_H

#include "picture.h"
#include "rectangle.h"

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

    Picture m_picture;

    void mousePressEvent(QGraphicsSceneMouseEvent* event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);

    void loadImage(const std::string& path, const std::string labelName);

private slots:
    void rectSelectionChanged();
};

#endif // PAINTSCENE_H
