#include "model.h"
#include "rectangle.h"

#include <QPainter>

Rectangle::Rectangle(std::string name, QPointF point, QObject *parent)
    : Figure{name, point, Model::instanse().currentSettings(), parent}
{}

Rectangle::~Rectangle()
{}

void Rectangle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPen pen;
    if (m_isSelected) {
        pen = QPen(m_graphicSettings.color(), m_graphicSettings.depthColor(),
              Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    }
    else {
        pen = QPen(m_graphicSettings.color(), m_graphicSettings.depthColor(),
              Qt::DashDotLine, Qt::RoundCap, Qt::RoundJoin);
    }
    painter->setPen(pen);

    painter->drawRect({ m_startPoint, m_endPoint });

    Q_UNUSED(option)
    Q_UNUSED(widget)
}

