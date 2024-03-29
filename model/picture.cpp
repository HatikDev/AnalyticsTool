#include "analyticsexception.h"
#include "picture.h"
#include "model.h"

#include <fstream>
#include <sstream>

#include <QString>

Picture::Picture()
    : QGraphicsPixmapItem() {}

Picture::Picture(const std::string& path, const std::string& labelName, QSize size)
    : QGraphicsPixmapItem(), m_path{ path }, m_name{ labelName }, m_size{ size }
{
    loadImage();
}

Picture::Picture(Picture&& picture)
    : QGraphicsPixmapItem(), m_path{ std::move(picture.m_path) }
    , m_name{ std::move(picture.m_name) }, m_size{ picture.m_size }
    , m_rects{ std::move(picture.m_rects) }
{
    loadImage();
}

Picture& Picture::operator=(Picture&& picture)
{
    m_path = std::move(picture.m_path);
    m_name = std::move(picture.m_name);
    m_size = std::move(picture.m_size);
    m_rects = std::move(picture.m_rects);

    loadImage();

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

    throw AnalyticsException("Failed to find rectangle by name");
}

void Picture::loadImage()
{
    std::string imagePath = m_path + "/images/" + m_name + ".jpg";
    QImage image(imagePath.c_str());
    auto imageScaled = image.scaled(m_size, Qt::KeepAspectRatio);
    setPixmap(QPixmap::fromImage(imageScaled));
}

void Picture::loadLabels(const std::string& path, const std::string& labelName)
{
    std::ifstream file(path + "/images_labels/" + labelName + ".txt");
    if (!file.is_open())
        throw AnalyticsException(std::string("Failed to open file" + labelName).c_str());

    m_rects.clear();

    std::string line;
    size_t type;
    QPoint startPoint;
    QPoint endPoint;
    double x0, y0;
    double x1, y1;

    while (getline(file, line)) {
        std::stringstream ss;
        ss << line;
        ss >> type >> x0 >> y0 >> x1 >> y1;

        // TODO: we need to multiply coordinates on image size
        x0 *= 360;
        y0 *= 360;
        x1 *= 360;
        y1 *= 360;

        // TODO: change type by predefined class
        m_rects.push_back(std::make_shared<Rectangle>(std::to_string(type), QPointF(x0, y0), Model::instanse().currentSettings()));
        m_rects.back()->setEndPoint(QPointF(x1, y1));

        //connect(m_rects.get(), &Rectangle::rectSelected, this, &PaintScene::rectSelectionChanged);
        //connect(m_rects.get(), &Rectangle::rectDeselected, this, &PaintScene::rectSelectionChanged);
        // TODO: add disconnection
        emit rectAdded(m_rects.back());
    }
}
