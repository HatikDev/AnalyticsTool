#pragma once

#include "rectangle.h"

#include <memory>
#include <vector>

class Model : public QObject {
    Q_OBJECT

public:
    Model(const Model&) = delete;
    Model(Model&&) = delete;
    Model& operator=(const Model&) = delete;
    Model& operator=(Model&&) = delete;
    ~Model() {}

    static Model& instanse();

    const std::vector<std::shared_ptr<Rectangle>>& rects() const;

    void addRect(std::shared_ptr<Rectangle> rect);

    //void editFigure(std::shared_ptr<Figure> figure);

    //void deleteFigure();

    void setCurrentSettings(const RectangleGraphicSettings& settings);

    RectangleGraphicSettings currentSettings() const;

    std::shared_ptr<Rectangle> rectByName(const std::string& name) const;

private:
    static Model* m_instanse;

    std::vector<std::shared_ptr<Rectangle>> m_rects;

    RectangleGraphicSettings m_currentSettings;

    Model(QObject* parent = nullptr);

//signals:
//    void figureAdded(std::shared_ptr<Figure> figure);
//    void figureEdited(std::shared_ptr<Figure> figure);
//    void figureDeleted(std::shared_ptr<Figure> figure);
};
