#pragma once

#include "picture.h"
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

    const Picture& picture() const;

    // TODO: add setting picture

    void addRect(std::shared_ptr<Rectangle> rect);

    void removeRect(std::shared_ptr<Rectangle> rect);

    void setCurrentSettings(const RectangleGraphicSettings& settings);

    RectangleGraphicSettings currentSettings() const;

private:
    static Model* m_instanse;

    Picture m_picture;

    RectangleGraphicSettings m_currentSettings;

    Model(QObject* parent = nullptr);
};
