#include "picture.h"

#include <fstream>

#include <QString>

Picture::Picture()
    : QGraphicsPixmapItem() {}

Picture::Picture(const std::string& path, const std::string& labelName, QSize size)
    : QGraphicsPixmapItem()
{
    QImage image(path.c_str());
    auto imageScaled = image.scaled(size, Qt::KeepAspectRatio);
    setPixmap(QPixmap::fromImage(imageScaled));

    loadLabels(labelName);
}

Picture::Picture(const Picture& image)
{
    // TODO: add code
}

Picture::Picture(Picture&& image)
{
    // TODO: add code
}

Picture& Picture::operator=(const Picture& image) {
    // TODO: add code
    return *this;
}

Picture& Picture::operator=(Picture&& image) {
    // TODO: add code
    return *this;
}

Picture::~Picture() {}

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
