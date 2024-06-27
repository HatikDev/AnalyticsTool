#include "constants.h"
#include "rectangle.h"
#include "utils.h"

#include <QPen>
#include <QPainter>
#include <qdrawutil.h>
#include <QPalette>

Rectangle::Rectangle(std::string name, size_t number, QPointF point,
    uint8_t cellType, size_t lineWidth, QObject* parent)
    : QObject(parent), QGraphicsItem()
    , m_name{ name }
    , m_number{ number }
    , m_cellType{ cellType }
    , kLineWidth{ lineWidth }
{
    setStartPoint(mapFromScene(point));
    setEndPoint(mapFromScene(point));
    /* Подключаем сигнал изменения координат к слоту запуска обновления содержимого объекта
     * Сигнал и слот присутствуют в базовом классе
     * */
    //connect(this, &Rectangle::pointChanged, this, &Rectangle::updateRomb);
}

Rectangle::~Rectangle()
{}

std::string Rectangle::name() const
{
    return m_name;
}

void Rectangle::setName(const std::string& name)
{
    m_name = name;
}

QPointF Rectangle::topLeft() const
{
    auto x = m_startPoint.x() > m_endPoint.x() ? m_endPoint.x() : m_startPoint.x();
    auto y = m_startPoint.y() > m_endPoint.y() ? m_endPoint.y() : m_startPoint.y();

    return { x, y };
}

QPointF Rectangle::topRight() const
{
    auto x = m_startPoint.x() > m_endPoint.x() ? m_startPoint.x() : m_endPoint.x();
    auto y = m_startPoint.y() > m_endPoint.y() ? m_endPoint.y() : m_startPoint.y();

    return { x, y };
}

QPointF Rectangle::bottomLeft() const
{
    auto x = m_startPoint.x() > m_endPoint.x() ? m_endPoint.x() : m_startPoint.x();
    auto y = m_startPoint.y() > m_endPoint.y() ? m_startPoint.y() : m_endPoint.y();

    return { x, y };
}

QPointF Rectangle::bottomRight() const
{
    auto x = m_startPoint.x() > m_endPoint.x() ? m_startPoint.x() : m_endPoint.x();
    auto y = m_startPoint.y() > m_endPoint.y() ? m_startPoint.y() : m_endPoint.y();

    return { x, y };
}

void Rectangle::setStartPoint(QPointF point)
{
    m_startPoint = mapFromScene(point);
    emit pointChanged();
}

void Rectangle::setEndPoint(QPointF point)
{
    m_endPoint = mapFromScene(point);
    emit pointChanged();
}

void Rectangle::select()
{
    m_isSelected = true;
}

void Rectangle::deselect()
{
    m_isSelected = false;
}

bool Rectangle::isSelected() const
{
    return m_isSelected;
}

size_t Rectangle::cellType() const
{
    return m_cellType;
}

void Rectangle::setCellType(size_t cellType)
{
    m_cellType = cellType;
}

size_t Rectangle::number() const
{
    return m_number;
}

void Rectangle::updateRomb()
{
    update((m_endPoint.x() > m_startPoint.x() ? m_startPoint.x() : m_endPoint.x()) - 5,
        (m_endPoint.y() > m_startPoint.y() ? m_startPoint.y() : m_endPoint.y()) - 5,
        qAbs(m_endPoint.x() - m_startPoint.x()) + 10,
        qAbs(m_endPoint.y() - m_startPoint.y()) + 10);
}

QRectF Rectangle::boundingRect() const
{
    return QRectF((m_endPoint.x() > m_startPoint.x() ? m_startPoint.x() : m_endPoint.x()) - 5,
        (m_endPoint.y() > m_startPoint.y() ? m_startPoint.y() : m_endPoint.y()) - 5,
        qAbs(m_endPoint.x() - m_startPoint.x()) + 10,
        qAbs(m_endPoint.y() - m_startPoint.y()) + 10);
}

void Rectangle::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    QPen pen;
    if (m_isSelected) {
        pen = QPen(utils::colorByClass(m_cellType), kLineWidth,
            Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    }
    else {
        pen = QPen(utils::colorByClass(m_cellType), kLineWidth,
            Qt::DashDotLine, Qt::RoundCap, Qt::RoundJoin);
    }
    painter->setPen(pen);

    painter->drawRect(QRect{ m_startPoint.toPoint(), m_endPoint.toPoint() });

    if (m_isSelected) {
        painter->fillRect({ m_startPoint, m_endPoint },
                          { kDefaultRectColor, Qt::BDiagPattern });

        QPoint p1({ m_startPoint.toPoint().x(), m_startPoint.toPoint().y() });
        QPoint p2({ m_startPoint.toPoint().x(), m_endPoint.toPoint().y() });
        QPoint p3({ m_endPoint.toPoint().x(), m_startPoint.toPoint().y() });
        QPoint p4({ m_endPoint.toPoint().x(), m_endPoint.toPoint().y() });

        painter->setBrush(kDefaultRectColor);
        auto ellipseRadius = 4 * kLineWidth; // TODO: replace by constant
        painter->drawEllipse(p1, ellipseRadius, ellipseRadius);
        painter->drawEllipse(p2, ellipseRadius, ellipseRadius);
        painter->drawEllipse(p3, ellipseRadius, ellipseRadius);
        painter->drawEllipse(p4, ellipseRadius, ellipseRadius);
    }

    Q_UNUSED(option)
    Q_UNUSED(widget)
}
