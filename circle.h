#ifndef CIRCLE_H
#define CIRCLE_H

#include <QObject>
#include <QGraphicsItem>
#include "figure.h"

class Circle : public Figure
{
    Q_OBJECT

public:
    explicit Circle(std::string name, QPointF point, QObject* parent = 0);
    ~Circle();

private:
    QPointF m_center;
    float m_radius;

    // Для Прямоугольника реализуем только саму отрисовку
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
};

#endif // CIRCLE_H
