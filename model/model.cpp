#include "constants.h"
#include "model.h"

#include <stdexcept>
#include <QColor>

Model* Model::m_instanse = nullptr;

Model& Model::instanse()
{
    if (!m_instanse)
        m_instanse = new Model();

    return *m_instanse;
}

Picture& Model::picture()
{
    return m_picture;
}

void Model::setPicture(Picture&& picture)
{
    m_picture = std::move(picture);
}

Dataset& Model::dataset()
{
    return m_dataset;
}

void Model::addRect(std::shared_ptr<Rectangle> rect)
{
    m_picture.addRect(rect);
}

void Model::removeRect(std::shared_ptr<Rectangle> rect)
{
    // TODO: add remove objects
}

void Model::loadDataset(const std::string& path)
{
    m_dataset = Dataset(path);
}

Model::Model(QObject* parent)
    : QObject(parent)
    , m_picture{}
{
    m_instanse = this;
}
