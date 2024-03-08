#include "picture.h"

#include <fstream>

#include <QString>

Picture::Picture()
    : QGraphicsPixmapItem() {}

Picture::Picture(const std::string& path, const std::string& labelName, QSize size)
    : QGraphicsPixmapItem(), m_path{ path }, m_name{ labelName }, m_size{ size }
{
    QImage image(path.c_str());
    auto imageScaled = image.scaled(size, Qt::KeepAspectRatio);
    setPixmap(QPixmap::fromImage(imageScaled));

    loadLabels(labelName);
}

Picture::Picture(const Picture& picture)
    : Picture{ picture.m_path, picture.m_name, picture.m_size }
{}

Picture::Picture(Picture&& picture)
    : Picture{ std::move(picture.m_path), std::move(picture.m_name), picture.m_size }
{}

// TODO: think about refactor
Picture& Picture::operator=(Picture picture) {
    std::swap(m_path, picture.m_path);
    std::swap(m_name, picture.m_name);
    std::swap(m_size, picture.m_size);
    std::swap(m_rects, picture.m_rects);

    QImage image(m_path.c_str());
    auto imageScaled = image.scaled(m_size, Qt::KeepAspectRatio);
    setPixmap(QPixmap::fromImage(imageScaled));

    loadLabels(m_name);

    return *this;
}

Picture::~Picture() {}

const std::string& Picture::name() const
{
    return m_name;
}

const std::vector<std::shared_ptr<Rectangle>>& Picture::rects() const
{
    return m_rects;
}

void Picture::addRect(std::shared_ptr<Rectangle> rect)
{
    m_rects.push_back(rect);
}

std::shared_ptr<Rectangle> Picture::rectByName(const std::string& name) const
{
    for (auto& obj : m_rects)
        if (name == obj->name())
            return obj;

    throw std::logic_error("456"); // TODO: change error type
}

void Picture::loadLabels(const std::string& labelName)
{
    std::ifstream file(labelName);
    std::string line;
    while (getline(file, line)) {
        
    }
}
