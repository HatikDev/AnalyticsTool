#include "controller/controller.h"
#include "model/model.h"

Controller::Controller()
{}

Controller::~Controller()
{}

void Controller::loadDataset(const std::string& path)
{
    Model::instanse().loadDataset(path);
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
