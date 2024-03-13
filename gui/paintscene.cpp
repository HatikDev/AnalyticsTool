#include "analyticsexception.h"
#include "paintscene.h"
#include "model/model.h"
#include "model/picture.h"

#include <stdexcept>

#include <QPixmap>

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

    std::string name = "New rect " + std::to_string(counter++);
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
        throw AnalyticsException("Current rectangle is not created");

    connect(m_current.get(), &Rectangle::rectSelected, this, &PaintScene::rectSelectionChanged);
    connect(m_current.get(), &Rectangle::rectDeselected, this, &PaintScene::rectSelectionChanged);
    // TODO: add disconnection
    emit rectAdded(m_current);
    m_current.reset();

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
