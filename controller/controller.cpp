#include "controller/controller.h"
#include "model/model.h"

Controller::Controller()
{}

Controller::~Controller()
{}

void Controller::loadDataset(const std::string& path)
{
    Model::instanse().loadDataset(path);

    Model::instanse().setPicture(Model::instanse().dataset().current());
}

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

void Controller::nextPicture()
{
    auto picture = Model::instanse().dataset().next();
    Model::instanse().setPicture(std::move(picture));
}

void Controller::previousPicture()
{
    auto picture = Model::instanse().dataset().previous();
    Model::instanse().setPicture(std::move(picture));
}

void Controller::changeCurrentDepthColor(int currentDepthColor)
{
    auto settings = Model::instanse().currentSettings();
    settings.setDepthColor(currentDepthColor);
    Model::instanse().setCurrentSettings(settings);
}
