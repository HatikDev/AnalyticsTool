#ifndef PAINTSCENE_H
#define PAINTSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <memory>
#include "figure.h"

class PaintScene : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit PaintScene(QObject *parent = nullptr);
     ~PaintScene();

signals:
    void currentFigureChanged(); // TODO: maybe it should be deleted
    void figureAdded(std::shared_ptr<Figure> figure);

private:
    std::shared_ptr<Figure> m_current;

    static size_t counter;

    void mousePressEvent(QGraphicsSceneMouseEvent* event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);

private slots:
    void figureSelectionChanged();
};

#endif // PAINTSCENE_H
