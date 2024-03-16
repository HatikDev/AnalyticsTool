#include "analyticsexception.h"
#include "paintscene.h"
#include "model/model.h"
#include "model/picture.h"

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
}

size_t PaintScene::counter = 0;

PaintScene::PaintScene(QObject* parent)
    : QGraphicsScene{ parent }, m_current{ nullptr }
{
    setSceneRect(0, 0, 480, 360); // TODO: think about size

    Model::instanse().picture().setPos(0, 0); // think about position

    addItem(&Model::instanse().picture());
}

PaintScene::~PaintScene()
{}

void PaintScene::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsScene::mousePressEvent(event);

    if (selectRect(*event))
        return;

    createLocalRect(*event);
}

void PaintScene::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsScene::mouseMoveEvent(event);

    if (!m_current)
        return;

    m_current->setEndPoint(event->scenePos());

    update();
}

void PaintScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsScene::mouseReleaseEvent(event);

    if (!m_current)
        return;

    connect(m_current.get(), &Rectangle::rectSelected, this, &PaintScene::rectSelectionChanged);
    connect(m_current.get(), &Rectangle::rectDeselected, this, &PaintScene::rectSelectionChanged);
    // TODO: add disconnection
    emit rectAdded(m_current);
    m_current.reset();

    update();
}

bool PaintScene::selectRect(const QGraphicsSceneMouseEvent& event) const
{
    bool isSelected = false;
    auto& rects = Model::instanse().picture().rects();
    for (auto& rect : rects) {
        if (isInside(event.scenePos(), *rect)) {
            rect->select();
            isSelected = true;
        }
        else {
            rect->deselect();
        }
    }

    return isSelected;
}

void PaintScene::createLocalRect(const QGraphicsSceneMouseEvent& event)
{
    std::string name = "New rect " + std::to_string(counter++);
    m_current = std::make_shared<Rectangle>(name, event.scenePos(), Model::instanse().currentSettings());

    addItem(m_current.get());

    update();
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
