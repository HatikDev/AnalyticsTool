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

const std::vector<std::shared_ptr<Rectangle>>& Model::rects() const
{
    return m_rects;
}

void Model::addRect(std::shared_ptr<Rectangle> rect)
{
    m_rects.push_back(rect);

    //    emit figureAdded(figure);
}

void Model::setCurrentSettings(const RectangleGraphicSettings &settings)
{
    m_currentSettings = settings;
}

RectangleGraphicSettings Model::currentSettings() const
{
    return m_currentSettings;
}

std::shared_ptr<Rectangle> Model::rectByName(const std::string &name) const
{
    for (auto& obj : m_rects)
        if (name == obj->name())
            return obj;

    throw std::logic_error("456"); // TODO: change error type
}

Model::Model(QObject* parent)
    : QObject(parent)
    , m_rects{}
    , m_currentSettings{Qt::black, 5}
{
    m_instanse = this;
}
