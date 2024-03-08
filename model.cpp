#include "model.h"

#include <stdexcept>
#include <QColor>

Model* Model::m_instanse = nullptr;

Model& Model::instanse()
{
    if (!m_instanse)
        m_instanse = new Model();

    return *m_instanse;
}

const Picture& Model::picture() const
{
    return m_picture;
}

void Model::addRect(std::shared_ptr<Rectangle> rect)
{
    m_picture.addRect(rect);

    //    emit figureAdded(figure);
}

void Model::removeRect(std::shared_ptr<Rectangle> rect)
{
    // TODO: add remove objects
}

void Model::setCurrentSettings(const RectangleGraphicSettings &settings)
{
    m_currentSettings = settings;
}

RectangleGraphicSettings Model::currentSettings() const
{
    return m_currentSettings;
}

Model::Model(QObject* parent)
    : QObject(parent)
    , m_picture{}
    , m_currentSettings{Qt::black, 5}
{
    m_instanse = this;
}
