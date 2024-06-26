#ifndef FIGURE_H
#define FIGURE_H

#include <QObject>
#include <QGraphicsItem>

#include <string>

class Rectangle : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)

public:
    explicit Rectangle(std::string name, QPointF point, uint8_t cellType, QObject* parent = 0);
    ~Rectangle();

    std::string name() const;

    void setName(const std::string& name);

    QPointF topLeft() const;

    QPointF topRight() const;

    QPointF bottomLeft() const;

    QPointF bottomRight() const;

    void setStartPoint(QPointF point);

    void setEndPoint(QPointF point);

    void select();

    void deselect();

    size_t cellType() const;

    void setCellType(size_t cellType);

public slots:
    void updateRomb();

signals:
    void pointChanged();

protected:
    std::string m_name;

    QPointF m_startPoint;
    QPointF m_endPoint;

    bool m_isSelected;

    size_t m_cellType;

    QRectF boundingRect() const;

private:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};

#endif // FIGURE_H
