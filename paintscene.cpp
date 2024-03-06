#include "paintscene.h"
#include "circle.h"
#include "model.h"
#include "rectangle.h"

#include <stdexcept>

using FigureTypes = Figure::FigureTypes;

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
    switch (Model::instanse().currentFigureType()) {
    case FigureTypes::rectangle:
        name = "New rect " + std::to_string(counter++);
        m_current = std::make_shared<Rectangle>(name, event->scenePos());
        break;
    case FigureTypes::circle:
        name = "New circle " + std::to_string(counter++);
        m_current = std::make_shared<Circle>(name, event->scenePos());
        break;
        // TODO: add other types
    }
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

    connect(m_current.get(), &Figure::figureSelected, this, &PaintScene::figureSelectionChanged);
    connect(m_current.get(), &Figure::figureDeselected, this, &PaintScene::figureSelectionChanged);
    // TODO: add disconnection
    emit figureAdded(m_current);
    m_current.reset();

    update();
}

void PaintScene::figureSelectionChanged()
{
    update();
}

