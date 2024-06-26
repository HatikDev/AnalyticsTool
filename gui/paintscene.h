#ifndef PAINTSCENE_H
#define PAINTSCENE_H

#include "model/picture.h"
#include "gui/rectangle.h"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

#include <memory>

enum class MouseState {
    released = 0,
    creating,
    editing,
    selecting,
};

class PaintScene : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit PaintScene(QObject *parent = nullptr);
     ~PaintScene();

    void reset();

signals:
    void rectAdded(std::shared_ptr<Rectangle> rect);
    void rectSelected(std::shared_ptr<Rectangle> rect);
    void rectDeselected(std::shared_ptr<Rectangle> rect);
    void rectRemove(std::shared_ptr<Rectangle> rect); // TODO: add deleting rect

private:
    std::shared_ptr<Rectangle> m_currentRect = nullptr;

    static size_t counter;

    MouseState m_mouseState = MouseState::released;

    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;

    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

    bool tryEditRect(const QGraphicsSceneMouseEvent& event);

    bool trySelectRect(const QGraphicsSceneMouseEvent& event);

    bool tryCreateLocalRect(const QGraphicsSceneMouseEvent& event);

public slots:
    // Calls when rect was added not from PaintScene.
    // For instance, loaded from dataset
    void on_rectAdded(std::shared_ptr<Rectangle> rect);

private slots:
    void rectSelectionChanged();
};

#endif // PAINTSCENE_H
