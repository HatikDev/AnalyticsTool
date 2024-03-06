#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "figure.h"

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

    void setDefaultFigure(Figure::FigureTypes figureType);

    void setDefaultColor(QColor color);

    void addFigure(std::shared_ptr<Figure> figure);

    void changeCurrentDepthColor(int currentDepthColor);

private:

};

#endif // CONTROLLER_H
