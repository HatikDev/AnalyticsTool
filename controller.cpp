#include "controller.h"
#include "model.h"

Controller::Controller()
{}

Controller::~Controller()
{}

void Controller::setDefaultFigure(Figure::FigureTypes figureType)
{
    Model::instanse().setCurrentFigureType(figureType);
}

void Controller::setDefaultColor(QColor color)
{
    auto settings = Model::instanse().currentSettings();
    settings.setColor(color);
    Model::instanse().setCurrentSettings(settings);
}

void Controller::addFigure(std::shared_ptr<Figure> figure)
{
    Model::instanse().addFigure(figure);
}

void Controller::changeCurrentDepthColor(int currentDepthColor)
{
    auto settings = Model::instanse().currentSettings();
    settings.setDepthColor(currentDepthColor);
    Model::instanse().setCurrentSettings(settings);
}
