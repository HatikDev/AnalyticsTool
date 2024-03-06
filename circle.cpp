#include "circle.h"
#include "model.h"

#include <QPainter>

Circle::Circle(std::string name, QPointF point, QObject *parent)
    : Figure{name, point, Model::instanse().currentSettings(), parent}
{}

Circle::~Circle()
{}

void Circle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
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

    QRectF rect(endPoint().x() > startPoint().x() ? startPoint().x() : endPoint().x(),
             endPoint().y() > startPoint().y() ? startPoint().y() : endPoint().y(),
             qAbs(endPoint().x() - startPoint().x()),
             qAbs(endPoint().y() - startPoint().y()));

    painter->drawEllipse(rect);

    Q_UNUSED(option)
    Q_UNUSED(widget)
}
