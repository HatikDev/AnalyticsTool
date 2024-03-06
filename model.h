#pragma once

#include "figure.h"

#include <memory>
#include <vector>

using FigureTypes = Figure::FigureTypes;

class Model : public QObject {
    Q_OBJECT

public:
    Model(const Model&) = delete;
    Model(Model&&) = delete;
    Model& operator=(const Model&) = delete;
    Model& operator=(Model&&) = delete;
    ~Model() {}

    static Model& instanse();

    const std::vector<std::shared_ptr<Figure>>& figures() const;

    void addFigure(std::shared_ptr<Figure> figure);

    //void editFigure(std::shared_ptr<Figure> figure);

    //void deleteFigure();

    void setCurrentSettings(const FigureGraphicSettings& settings);

    FigureGraphicSettings currentSettings() const;

    void setCurrentFigureType(Figure::FigureTypes figureType);

    Figure::FigureTypes currentFigureType() const;

    std::shared_ptr<Figure> figureByName(const std::string& name) const;

private:
    static Model* m_instanse;

    std::vector<std::shared_ptr<Figure>> m_figures;

    FigureGraphicSettings m_currentSettings;

    FigureTypes m_currentFigureType;

    Model(QObject* parent = nullptr);

//signals:
//    void figureAdded(std::shared_ptr<Figure> figure);
//    void figureEdited(std::shared_ptr<Figure> figure);
//    void figureDeleted(std::shared_ptr<Figure> figure);
};
