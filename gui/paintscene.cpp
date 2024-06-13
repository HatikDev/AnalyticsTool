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

namespace fs = std::filesystem;

PaintScene::PaintScene(QObject* parent)
    : QGraphicsScene{ parent }
{
    setSceneRect(0, 0, 480, 360); // TODO: think about size

    //auto& picture = Model::instanse().picture();
    //picture.setPos(0, 0); // TODO: think about position

    //addItem(&Model::instanse().picture());
}

PaintScene::~PaintScene()
{}

void PaintScene::reset()
{
    /*QList<QGraphicsItem*> rects = items();
    for (auto* item : rects)
        removeItem(item);

    m_currentRect.reset();
    counter = 0;
    m_mouseState = MouseState::released;

    addItem(&Model::instanse().picture());*/
}

void PaintScene::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsScene::mousePressEvent(event);

    if (tryEditRect(*event)) {
        m_mouseState = MouseState::editing;
        return;
    }

    if (selectedItems().size() != 0)
        return;

    //if (trySelectRect(*event)) {
    //    m_mouseState = MouseState::selecting;
    //    return;
    //}

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
        connect(this, &QGraphicsScene::selectionChanged, this, &PaintScene::itemClicked);
        m_currentRect = nullptr;
    }

    if (m_mouseState == MouseState::editing) {
        //m_currentRect->setFlag(QGraphicsItem::ItemIsMovable, true);
        m_currentRect = nullptr;
    }

    m_mouseState = MouseState::released;

    update();
}

void PaintScene::keyPressEvent(QKeyEvent* keyEvent)
{
    switch (keyEvent->key()) {
    case Qt::Key_Delete:
    case Qt::Key_Backspace:
        auto items = selectedItems();
        if (items.size() == 0)
            return;

        emit rectRemove(static_cast<Rectangle*>(items.at(0)));
        //removeItem(items.at(0));
    }
}

bool PaintScene::tryEditRect(const QGraphicsSceneMouseEvent& event)
{
    for (auto* rect : selectedItems()) {
        auto boundingRect = rect->boundingRect();
        if (distance(boundingRect.topLeft(), event.scenePos()) <= kSelectionCircleRadius) {
            m_currentRect = static_cast<Rectangle*>(rect);
            m_currentRect->setFlag(QGraphicsItem::ItemIsMovable, false);
            m_currentRect->setStartPoint(boundingRect.bottomRight());
            m_currentRect->setEndPoint(boundingRect.topLeft());
            return true;
        }

        if (distance(boundingRect.topRight(), event.scenePos()) <= kSelectionCircleRadius) {
            m_currentRect = static_cast<Rectangle*>(rect);
            m_currentRect->setFlag(QGraphicsItem::ItemIsMovable, false);
            m_currentRect->setStartPoint(boundingRect.bottomLeft());
            m_currentRect->setEndPoint(boundingRect.topRight());
            return true;
        }

        if (distance(boundingRect.bottomLeft(), event.scenePos()) <= kSelectionCircleRadius) {
            m_currentRect = static_cast<Rectangle*>(rect);
            m_currentRect->setFlag(QGraphicsItem::ItemIsMovable, false);
            m_currentRect->setStartPoint(boundingRect.topRight());
            m_currentRect->setEndPoint(boundingRect.bottomLeft());
            return true;
        }

        if (distance(boundingRect.bottomRight(), event.scenePos()) <= kSelectionCircleRadius) {
            m_currentRect = static_cast<Rectangle*>(rect);
            m_currentRect->setFlag(QGraphicsItem::ItemIsMovable, false);
            m_currentRect->setStartPoint(boundingRect.topLeft());
            m_currentRect->setEndPoint(boundingRect.bottomRight());
            return true;
        }
    }

    return false;
}

bool PaintScene::tryCreateLocalRect(const QGraphicsSceneMouseEvent& event)
{
    std::string name = "New rect " + std::to_string(counter++);
    m_currentRect = new Rectangle(name, items().size(), event.scenePos(), 0); // TODO: change type
    m_currentRect->setFlag(QGraphicsItem::ItemIsSelectable, true);
    //m_currentRect->setFlag(QGraphicsItem::ItemIsMovable, true);

    addItem(m_currentRect);

    m_mouseState = MouseState::creating;

    update();

    emit rectAdded(m_currentRect);

    return true;
}

void PaintScene::loadData(const IDataObject& dataObject)
{
    // here we should call dataObject.data() to get data from dataObject
    // It's temporary realization

    fs::path path = dataObject.name();
    QImage image(path.string().c_str());
    auto imageScaled = image.scaled(QSize(2048, 2048), Qt::KeepAspectRatio);

    QGraphicsPixmapItem* pixmap = new QGraphicsPixmapItem();
    pixmap->setPixmap(QPixmap::fromImage(imageScaled));

    addItem(pixmap);

    update();
}

void PaintScene::on_rectSelected(size_t rectId)
{
    for (auto* item : items()) {
        auto* rect = static_cast<Rectangle*>(item);

        if (rect->number() == rectId) {
            rect->select();
            update();
            break;
        }
    }
}

void PaintScene::on_rectDeselected(size_t rectId)
{
    for (auto* item : items()) {
        auto* rect = static_cast<Rectangle*>(item);

        if (rect->number() == rectId) {
            rect->deselect();
            update();
            break;
        }
    }
}

void PaintScene::itemClicked()
{
    for (auto* item : items())
        static_cast<Rectangle*>(item)->deselect();

    for (auto* item : selectedItems()) {
        auto* rect = static_cast<Rectangle*>(item);
        rect->select();
        emit rectSelected(rect);
    }

    update();
}

