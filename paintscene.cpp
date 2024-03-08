#include "paintscene.h"
#include "model.h"

#include "picture.h"

#include <stdexcept>

#include <QPixmap>

size_t PaintScene::counter = 0;

PaintScene::PaintScene(QObject* parent)
    : QGraphicsScene{ parent }, m_current{ nullptr },
      m_picture{"C:/Users/Sergey/Documents/cpp/AnalyticsTool/build/Debug/data/images/{0cd9a857-ad75-4771-ad9b-1a927a9bf856}_2.jpg", "",
              { 480, 360 } } // TODO: change size
{
    setSceneRect(0, 0, 480, 360); // TODO: think about size

    loadImage("", "");

    m_picture.setPos(0, 0);

    addItem(&m_picture);
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

void PaintScene::loadImage(const std::string& path, const std::string labelName)
{
    m_picture = {"C:/Users/Sergey/Documents/cpp/AnalyticsTool/build/Debug/data/images/{0cd9a857-ad75-4771-ad9b-1a927a9bf856}_2.jpg",
                "", { 640, 640}/*sceneRect().size().toSize()*/};
}

void PaintScene::rectSelectionChanged()
{
    update();
}

