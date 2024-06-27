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
    explicit Rectangle(std::string name, size_t number, QPointF point, uint8_t cellType, size_t lineWidth, QObject* parent = 0);
    Rectangle(const Rectangle&) = delete;
    Rectangle(Rectangle&&) = delete;
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

    bool isSelected() const;

    size_t cellType() const;

    void setCellType(size_t cellType);

    size_t number() const;

public slots:
    void updateRomb();

signals:
    void pointChanged();

protected:
    std::string m_name;

    bool m_isSelected = false;

    size_t m_number;

    QPointF m_startPoint;

    QPointF m_endPoint;

    size_t m_cellType;

    const size_t kLineWidth;

    QRectF boundingRect() const;

private:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};

#endif // FIGURE_H
