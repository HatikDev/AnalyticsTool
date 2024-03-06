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

const std::vector<std::shared_ptr<Figure>>& Model::figures() const
{
    return m_figures;
}

void Model::addFigure(std::shared_ptr<Figure> figure)
{
    m_figures.push_back(figure);

    //    emit figureAdded(figure);
}

void Model::setCurrentSettings(const FigureGraphicSettings &settings)
{
    m_currentSettings = settings;
}

FigureGraphicSettings Model::currentSettings() const
{
    return m_currentSettings;
}

Figure::FigureTypes Model::currentFigureType() const
{
    return m_currentFigureType;
}

std::shared_ptr<Figure> Model::figureByName(const std::string &name) const
{
    for (auto& obj : m_figures)
        if (name == obj->name())
            return obj;

    throw std::logic_error("456"); // TODO: change error type
}

void Model::setCurrentFigureType(Figure::FigureTypes figureType)
{
    m_currentFigureType = figureType;
}

Model::Model(QObject* parent)
    : QObject(parent)
    , m_figures{}
    , m_currentSettings{Qt::black, 5}
    , m_currentFigureType{ Figure::FigureTypes::rectangle }
{
    m_instanse = this;
}
