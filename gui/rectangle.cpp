#include "constants.h"
#include "rectangle.h"
#include "utils.h"

#include <QPen>
#include <QPainter>
#include <qdrawutil.h>
#include <QPalette>

Rectangle::Rectangle(std::string name, QPointF point,
    uint8_t rectType, QObject* parent)
    : QObject{ parent }, QGraphicsItem()
    , m_name{ name }
    , m_graphicSettings{ utils::colorByClass(rectType), kLineWidth }
    , m_isSelected{ false }
{
    setStartPoint(mapFromScene(point));
    setEndPoint(mapFromScene(point));
    /* Подключаем сигнал изменения координат к слоту запуска обновления содержимого объекта
     * Сигнал и слот присутствуют в базовом классе
     * */
    connect(this, &Rectangle::pointChanged, this, &Rectangle::updateRomb);
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

RectangleGraphicSettings Rectangle::graphicSettings() const
{
    return m_graphicSettings;
}

void Rectangle::setGraphicSettings(RectangleGraphicSettings settings)
{
    m_graphicSettings = settings;
}

void Rectangle::select()
{
    m_isSelected = true;
}

void Rectangle::deselect()
{
    m_isSelected = false;
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
        pen = QPen(m_graphicSettings.color(), m_graphicSettings.depthColor(),
            Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    }
    else {
        pen = QPen(m_graphicSettings.color(), m_graphicSettings.depthColor(),
            Qt::DashDotLine, Qt::RoundCap, Qt::RoundJoin);
    }
    painter->setPen(pen);

    painter->drawRect(QRect{ m_startPoint.toPoint(), m_endPoint.toPoint() });

    if (m_isSelected) {
        painter->fillRect({ m_startPoint, m_endPoint },
                          { m_graphicSettings.color(), Qt::BDiagPattern });

        QPoint p1({ m_startPoint.toPoint().x(), m_startPoint.toPoint().y() });
        QPoint p2({ m_startPoint.toPoint().x(), m_endPoint.toPoint().y() });
        QPoint p3({ m_endPoint.toPoint().x(), m_startPoint.toPoint().y() });
        QPoint p4({ m_endPoint.toPoint().x(), m_endPoint.toPoint().y() });

        painter->setBrush(m_graphicSettings.color());
        painter->drawEllipse(p1, kSelectionCircleRadius, kSelectionCircleRadius);
        painter->drawEllipse(p2, kSelectionCircleRadius, kSelectionCircleRadius);
        painter->drawEllipse(p3, kSelectionCircleRadius, kSelectionCircleRadius);
        painter->drawEllipse(p4, kSelectionCircleRadius, kSelectionCircleRadius);
    }

    Q_UNUSED(option)
        Q_UNUSED(widget)
}

RectangleGraphicSettings::RectangleGraphicSettings(QColor color, size_t depthColor)
    : m_color{ color }
    , m_depthColor{ depthColor }
{}

RectangleGraphicSettings::~RectangleGraphicSettings()
{}
