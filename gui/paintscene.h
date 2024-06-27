#ifndef PAINTSCENE_H
#define PAINTSCENE_H

#include "model/picture.h"
#include "model/dataset.h"
#include "gui/rectangle.h"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>

#include <memory>

enum class MouseState {
    released = 0,
    creating,
    editing,
    selecting,
};

enum class PaintMode {
    select = 0,
    draw,
};

class PaintScene : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit PaintScene(QObject *parent = nullptr);
     ~PaintScene();

    void reset();

    void loadData(IDataObject<BloodCellObj>& dataObject);

    BloodCellObj getData() const;

    void setMode(PaintMode mode);

    PaintMode mode() const;

signals:
    void rectAdded(Rectangle* rect, bool knownType);
    void rectSelected(Rectangle* rect);
    void rectDeselected(Rectangle* rect);
    void rectCategorySelected(Rectangle* rect);
    void edited();
    void rectRemove(Rectangle* rect); // TODO: add deleting rect

private:
    Rectangle* m_currentRect = nullptr;

    static size_t counter;

    MouseState m_mouseState = MouseState::released;

    PaintMode m_paintMode = PaintMode::select;

    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;

    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

    void keyPressEvent(QKeyEvent* keyEvent) override;

    bool tryEditRect(const QGraphicsSceneMouseEvent& event);

    bool tryCreateLocalRect(const QGraphicsSceneMouseEvent& event);

    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) override;

public slots:
    // Calls when rect was added not from PaintScene.
    // For instance, loaded from dataset

    void on_rectSelected(size_t rectId);

    void on_rectDeselected(size_t rectId);

private slots:
    void itemClicked();
};

#endif // PAINTSCENE_H
