#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "gui/rectangle.h"

#include <memory>

class Controller
{
public:
    Controller();
    Controller(const Controller&) = delete;
    Controller(Controller&&) = delete;
    Controller& operator=(const Controller&) = delete;
    Controller& operator=(Controller&&) = delete;
    ~Controller();

    void loadDataset(const std::string& path);

    void addRect(std::shared_ptr<Rectangle> rect);
};

#endif // CONTROLLER_H
