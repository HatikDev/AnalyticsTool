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

void Controller::addRect(std::shared_ptr<Rectangle> rect)
{
    Model::instanse().addRect(rect);
}
