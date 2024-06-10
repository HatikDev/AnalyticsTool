#include "analyticsexception.h"
#include "picture.h"
#include "model.h"

#include <fstream>
#include <sstream>

#include <QString>

Picture::Picture()
    : QGraphicsPixmapItem() {}

Picture::Picture(const std::string& path, const std::string& labelName, QSize size)
    : QGraphicsPixmapItem(), m_path{ path }, m_name{ labelName }
{
    auto sideSize = std::min(size.width(), size.height());
    m_size = { sideSize, sideSize };
    loadImage(m_size);
}

Picture::Picture(Picture&& picture)
    : QGraphicsPixmapItem(), m_path{ std::move(picture.m_path) }
    , m_name{ std::move(picture.m_name) }
    , m_rects{ std::move(picture.m_rects) }, m_size{ picture.m_size }
{
    loadImage(m_size);
}

Picture& Picture::operator=(Picture&& picture)
{
    m_path = std::move(picture.m_path);
    m_name = std::move(picture.m_name);
    m_size = std::move(picture.m_size);
    m_rects = std::move(picture.m_rects);

    loadImage(m_size);

    // TODO: think about copy construct of this class.
    // We must load labels only one time
    loadLabels(m_path, m_name);
    return *this;
}

Picture::~Picture() {}

const std::string& Picture::name() const
{
    return m_name;
}

//const std::vector<std::shared_ptr<Rectangle>>& Picture::rects() const
//{
//    return m_rects;
//}
//
//void Picture::addRect(std::shared_ptr<Rectangle> rect)
//{
//    m_rects.push_back(rect);
//}

void Picture::scale(QSize size) {
    auto sideSize = std::min(size.width(), size.height());
    m_size = { sideSize, sideSize };
    setPixmap(pixmap().scaled(m_size, Qt::KeepAspectRatio));
}

std::shared_ptr<Rectangle> Picture::rectByNumber(size_t number) const
{
    for (auto& obj : m_rects)
        if (number == obj->number())
            return obj;

    throw AnalyticsException("Failed to find rectangle by name");
}

void Picture::hideRect(size_t rectIndex)
{

}

void Picture::showRect(size_t rectIndex)
{

}

void Picture::loadImage(QSize size)
{
    std::string imagePath = m_path + "/images/" + m_name + ".jpg";
    QImage image(imagePath.c_str());
    auto imageScaled = image.scaled(size, Qt::KeepAspectRatio);
    setPixmap(QPixmap::fromImage(imageScaled));
}

void Picture::loadLabels(const std::string& path, const std::string& labelName)
{
    //std::ifstream file(path + "/images_labels/" + labelName + ".txt");
    //if (!file.is_open())
    //    throw AnalyticsException(std::string("Failed to open file" + labelName).c_str());

    //m_rects.clear();

    //std::string line;
    //uint8_t type;
    //double width, height;
    //double centerX, centerY;

    //auto& classes = Model::instanse().dataset().classes();
    //size_t counter = 0;
    //while (getline(file, line)) {
    //    std::stringstream ss;
    //    ss << line;
    //    ss >> type >> centerX >> centerY >> width >> height;

    //    type -= '0';

    //    // TODO: we need to multiply coordinates on image size
    //    double x0 = (centerX - width / 2) * m_size.width();
    //    double y0 = (centerY - height / 2) * m_size.height();
    //    double x1 = x0 + width * m_size.width();
    //    double y1 = y0 + height * m_size.height();

    //    // TODO: change type by predefined class
    //    auto classNameIt = classes.find(type);
    //    std::string className = classNameIt == classes.end() ? std::to_string(type) : classNameIt->second;
    //    m_rects.push_back(std::make_shared<Rectangle>(className, counter++, QPointF(x0, y0), type));
    //    m_rects.back()->setEndPoint(QPointF(x1, y1));

    //    //connect(m_rects.get(), &Rectangle::rectSelected, this, &PaintScene::rectSelectionChanged);
    //    //connect(m_rects.get(), &Rectangle::rectDeselected, this, &PaintScene::rectSelectionChanged);
    //    // TODO: add disconnection
    //    emit rectAdded(m_rects.back());
    //}
}
