#include "controller.h"
#include "model.h"

Controller::Controller()
{}

Controller::~Controller()
{}

void Controller::setDefaultColor(QColor color)
{
    auto settings = Model::instanse().currentSettings();
    settings.setColor(color);
    Model::instanse().setCurrentSettings(settings);
}

void Controller::addRect(std::shared_ptr<Rectangle> rect)
{
    Model::instanse().addRect(rect);
}

void Controller::changeCurrentDepthColor(int currentDepthColor)
{
    auto settings = Model::instanse().currentSettings();
    settings.setDepthColor(currentDepthColor);
    Model::instanse().setCurrentSettings(settings);
}
