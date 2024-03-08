#ifndef IMAGE_H
#define IMAGE_H

#include "rectangle.h"

#include <memory>

#include <QObject>
#include <QImage>
#include <QSize>

class Picture : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

public:
    Picture();
    Picture(const std::string& path, const std::string& labelPath, QSize size);
    Picture(const Picture& image);
    Picture(Picture&& image);
    Picture& operator=(Picture image);
    ~Picture();

    const std::string& name() const;

    const std::vector<std::shared_ptr<Rectangle>>& rects() const;

    void addRect(std::shared_ptr<Rectangle> rect);

    // TODO: add removing rect

    std::shared_ptr<Rectangle> rectByName(const std::string& name) const;

private:
    std::string m_path;

    std::string m_name;

    QSize m_size;

    std::vector<std::shared_ptr<Rectangle>> m_rects;

    void loadLabels(const std::string& labelPaths);
};

#endif // IMAGE_H
