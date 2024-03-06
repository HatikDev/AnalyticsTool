#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "rectangle.h"

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

    void setDefaultColor(QColor color);

    void addRect(std::shared_ptr<Rectangle> rect);

    void changeCurrentDepthColor(int currentDepthColor);

private:

};

#endif // CONTROLLER_H
