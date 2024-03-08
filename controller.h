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

    // TODO: add setting picture

    void addRect(std::shared_ptr<Rectangle> rect);

    // TODO: add removing rects

    void changeCurrentDepthColor(int currentDepthColor);
};

#endif // CONTROLLER_H
