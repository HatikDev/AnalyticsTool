#ifndef FIGURE_H
#define FIGURE_H

#include <QObject>
#include <QGraphicsItem>

#include <string>

class FigureGraphicSettings
{
public:
    FigureGraphicSettings(QColor color, size_t depthColor);
    ~FigureGraphicSettings();

    QColor color() const { return m_color; }

    size_t depthColor() const { return m_depthColor; }

    void setColor(QColor color) { m_color = color; }

    void setDepthColor(size_t depthColor) { m_depthColor = depthColor; }

private:
    QColor m_color;
    size_t m_depthColor;
    // TODO: add style
};

class Figure : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)

    Q_PROPERTY(QPointF startPoint
        READ startPoint WRITE setStartPoint
        NOTIFY pointChanged)
    Q_PROPERTY(QPointF endPoint
        READ endPoint WRITE setEndPoint
        NOTIFY pointChanged)

public:
    enum class FigureTypes {
        rectangle,
        circle,
        romb,
        triangle,
        invalid,
    };

    explicit Figure(std::string name, QPointF point,
                    FigureGraphicSettings graphicSettings, QObject* parent = 0);
    ~Figure();

    std::string name() const;
    void setName(std::string name);

    QPointF startPoint() const;

    QPointF endPoint() const;

    void setStartPoint(QPointF point);

    void setEndPoint(QPointF point);

    FigureGraphicSettings graphicSettings() const;

    void setGraphicSettings(FigureGraphicSettings settings);

    void select();

    void deselect();

public slots:
    void updateRomb();     // Слот обновления области, в которой содержится фигура

signals:
    void pointChanged();
    void figureSelected();
    void figureDeselected();

protected:
    std::string m_name;
    FigureGraphicSettings m_graphicSettings;

    QPointF m_startPoint;
    QPointF m_endPoint;

    bool m_isSelected;

    QRectF boundingRect() const;    // Область, в которой содержится фигура
};

#endif // FIGURE_H
