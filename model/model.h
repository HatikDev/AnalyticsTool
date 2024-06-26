#pragma once

#include "dataset.h"
#include "picture.h"
#include "gui/rectangle.h"

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

    Picture& picture();

    void setPicture(Picture&& picture);

    Dataset& dataset();

    void addRect(std::shared_ptr<Rectangle> rect);

    void removeRect(std::shared_ptr<Rectangle> rect);

    void loadDataset(const std::string& path);

private:
    static Model* m_instanse;

    Picture m_picture;

    Dataset m_dataset;

    Model(QObject* parent = nullptr);
};
