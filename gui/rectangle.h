#ifndef FIGURE_H
#define FIGURE_H

#include <QObject>
#include <QGraphicsItem>

#include <string>

class RectangleGraphicSettings
{
public:
    RectangleGraphicSettings(QColor color, size_t depthColor);
    ~RectangleGraphicSettings();

    QColor color() const { return m_color; }

    size_t depthColor() const { return m_depthColor; }

    void setColor(QColor color) { m_color = color; }

    void setDepthColor(size_t depthColor) { m_depthColor = depthColor; }

private:
    QColor m_color;
    size_t m_depthColor;
    // TODO: add style
};

class Rectangle : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)

public:
    explicit Rectangle(std::string name, QPointF point,
                    RectangleGraphicSettings graphicSettings, QObject* parent = 0);
    ~Rectangle();

    std::string name() const;

    void setName(const std::string& name);

    QPointF topLeft() const;

    QPointF topRight() const;

    QPointF bottomLeft() const;

    QPointF bottomRight() const;

    void setStartPoint(QPointF point);

    void setEndPoint(QPointF point);

    RectangleGraphicSettings graphicSettings() const;

    void setGraphicSettings(RectangleGraphicSettings settings);

    void select();

    void deselect();

public slots:
    void updateRomb();

signals:
    void pointChanged();
    void rectSelected();
    void rectDeselected();

protected:
    std::string m_name;
    RectangleGraphicSettings m_graphicSettings;

    QPointF m_startPoint;
    QPointF m_endPoint;

    bool m_isSelected;

    QRectF boundingRect() const;

private:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};

#endif // FIGURE_H
