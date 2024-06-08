#include "analyticsexception.h"
#include "constants.h"
#include "model/model.h"
#include "model/picture.h"
#include "paintscene.h"

#include <stdexcept>

#include <QPixmap>

namespace
{
    bool isInside(QPointF point, const Rectangle& rect) {
        auto startPoint = rect.topLeft();
        auto endPoint = rect.bottomRight();

        return point.x() >= startPoint.x() && point.x() <= endPoint.x()
            && point.y() >= startPoint.y() && point.y() <= endPoint.y();
    }

    size_t distance(QPointF p1, QPointF p2) {
        return sqrt(pow(p1.x() - p2.x(), 2) + pow(p1.y() - p2.y(), 2));
    }
}

size_t PaintScene::counter = 0;

PaintScene::PaintScene(QObject* parent)
    : QGraphicsScene{ parent }
{
    setSceneRect(0, 0, 480, 360); // TODO: think about size

    auto& picture = Model::instanse().picture();
    picture.setPos(0, 0); // TODO: think about position

    addItem(&Model::instanse().picture());
}

PaintScene::~PaintScene()
{}

void PaintScene::reset()
{
    QList<QGraphicsItem*> rects = items();
    for (auto* item : rects)
        removeItem(item);

    m_currentRect.reset();
    counter = 0;
    m_mouseState = MouseState::released;

    addItem(&Model::instanse().picture());
}

void PaintScene::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsScene::mousePressEvent(event);

    if (tryEditRect(*event)) {
        m_mouseState = MouseState::editing;
        return;
    }

    if (trySelectRect(*event)) {
        m_mouseState = MouseState::selecting;
        return;
    }

    tryCreateLocalRect(*event);
    m_mouseState = MouseState::creating;
}

void PaintScene::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsScene::mouseMoveEvent(event);

    if (!m_currentRect)
        return;

    m_currentRect->setEndPoint(event->scenePos());

    update();
}

void PaintScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsScene::mouseReleaseEvent(event);

    if (!m_currentRect)
        return;

    if (m_mouseState == MouseState::creating) {
        emit rectAdded(m_currentRect);
        m_currentRect.reset();
    }

    m_mouseState = MouseState::released;

    update();
}

bool PaintScene::tryEditRect(const QGraphicsSceneMouseEvent& event)
{
    for (auto& rect : Model::instanse().picture().rects()) {
        if (distance(rect->topLeft(), event.scenePos()) <= kSelectionCircleRadius) {
            m_currentRect = rect;
            m_currentRect->setStartPoint(rect->bottomRight());
            m_currentRect->setEndPoint(rect->topLeft());
            return true;
        }

        if (distance(rect->topRight(), event.scenePos()) <= kSelectionCircleRadius) {
            m_currentRect = rect;
            m_currentRect->setStartPoint(rect->bottomLeft());
            m_currentRect->setEndPoint(rect->topRight());
            return true;
        }

        if (distance(rect->bottomLeft(), event.scenePos()) <= kSelectionCircleRadius) {
            m_currentRect = rect;
            m_currentRect->setStartPoint(rect->topRight());
            m_currentRect->setEndPoint(rect->bottomLeft());
            return true;
        }

        if (distance(rect->bottomRight(), event.scenePos()) <= kSelectionCircleRadius) {
            m_currentRect = rect;
            m_currentRect->setStartPoint(rect->topLeft());
            m_currentRect->setEndPoint(rect->bottomRight());
            return true;
        }
    }

    return false;
}

bool PaintScene::trySelectRect(const QGraphicsSceneMouseEvent& event)
{
    bool isSelected = false;
    auto& rects = Model::instanse().picture().rects();
    for (auto& rect : rects) {
        if (isSelected = isInside(event.scenePos(), *rect)) {
            for (auto& otherRect : rects) {
                if (otherRect == rect) continue;

                otherRect->deselect();
                emit rectDeselected(otherRect);
            }

            rect->select();

            update();
            emit rectSelected(rect);

            return true;
        }
    }

    return isSelected;
}

bool PaintScene::tryCreateLocalRect(const QGraphicsSceneMouseEvent& event)
{
    std::string name = "New rect " + std::to_string(counter++);
    m_currentRect = std::make_shared<Rectangle>(name, items().size(), event.scenePos(), 0); // TODO: change type

    addItem(m_currentRect.get());

    m_mouseState = MouseState::creating;

    update();

    return true;
}

void PaintScene::on_rectAdded(std::shared_ptr<Rectangle> rect)
{
    addItem(rect.get());

    update();
}

void PaintScene::rectSelectionChanged()
{
    update();
}
