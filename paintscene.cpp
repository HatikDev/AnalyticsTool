#include "paintscene.h"
#include "model.h"

#include <stdexcept>

size_t PaintScene::counter = 0;

PaintScene::PaintScene(QObject* parent)
    : QGraphicsScene{ parent }
{
    setSceneRect(0, 0, 480, 360); // TODO: think about size
}

PaintScene::~PaintScene()
{}

void PaintScene::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsScene::mousePressEvent(event);

    std::string name;

    name = "New rect " + std::to_string(counter++);
    m_current = std::make_shared<Rectangle>(name, event->scenePos(), Model::instanse().currentSettings());

    addItem(m_current.get());

    update();
}

void PaintScene::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    m_current->setEndPoint(event->scenePos());

    update();
}

void PaintScene::mouseReleaseEvent(QGraphicsSceneMouseEvent*)
{
    if (!m_current)
        throw std::logic_error("123"); // TODO: change exception type

    connect(m_current.get(), &Rectangle::rectSelected, this, &PaintScene::rectSelectionChanged);
    connect(m_current.get(), &Rectangle::rectDeselected, this, &PaintScene::rectSelectionChanged);
    // TODO: add disconnection
    emit rectAdded(m_current);
    m_current.reset();

    update();
}

void PaintScene::rectSelectionChanged()
{
    update();
}

