#include "figure.h"

Figure::Figure(std::string name, QPointF point,
               FigureGraphicSettings graphicSettings, QObject *parent)
    : QObject{parent}, QGraphicsItem()
    , m_name{name}
    , m_graphicSettings{ graphicSettings }
    , m_isSelected{ false }
{
    setStartPoint(mapFromScene(point));
    setEndPoint(mapFromScene(point));
    /* Подключаем сигнал изменения координат к слоту запуска обновления содержимого объекта
     * Сигнал и слот присутствуют в базовом классе
     * */
    connect(this, &Figure::pointChanged, this, &Figure::updateRomb);
}

Figure::~Figure()
{

}

std::string Figure::name() const
{
    return m_name;
}

void Figure::setName(std::string name)
{
    m_name = name;
}

QPointF Figure::startPoint() const
{
    return m_startPoint;
}

QPointF Figure::endPoint() const
{
    return m_endPoint;
}

void Figure::setStartPoint(QPointF point)
{
    m_startPoint = mapFromScene( point ) ;
    emit pointChanged() ;
}

void Figure::setEndPoint(QPointF point)
{
    m_endPoint = mapFromScene( point ) ;
    emit pointChanged();
}

FigureGraphicSettings Figure::graphicSettings() const
{
    return m_graphicSettings;
}

void Figure::setGraphicSettings(FigureGraphicSettings settings)
{
    m_graphicSettings = settings;
}

void Figure::select()
{
    m_isSelected = true;

    emit figureSelected();
}

void Figure::deselect()
{
    m_isSelected = false;

    emit figureDeselected();
}

void Figure::updateRomb()
{
    update((endPoint().x() > startPoint().x() ? startPoint().x() : endPoint().x()) - 5,
                 (endPoint().y() > startPoint().y() ? startPoint().y() : endPoint().y()) - 5,
                 qAbs(endPoint().x() - startPoint().x()) + 10,
                 qAbs(endPoint().y() - startPoint().y()) + 10);
}

QRectF Figure::boundingRect() const
{
    return QRectF((endPoint().x() > startPoint().x() ? startPoint().x() : endPoint().x()) - 5,
                  (endPoint().y() > startPoint().y() ? startPoint().y() : endPoint().y()) - 5,
                  qAbs(endPoint().x() - startPoint().x()) + 10,
                  qAbs(endPoint().y() - startPoint().y()) + 10);
}

FigureGraphicSettings::FigureGraphicSettings(QColor color, size_t depthColor)
    : m_color{color}
    , m_depthColor{depthColor}
{}

FigureGraphicSettings::~FigureGraphicSettings()
{}
