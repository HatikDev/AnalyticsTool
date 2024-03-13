#ifndef IMAGE_H
#define IMAGE_H

#include "gui/rectangle.h"

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
    Picture(const Picture& picture) = delete;
    Picture(Picture&& picture);
    Picture& operator=(const Picture& picture) = delete;
    Picture& operator=(Picture&& picture);
    ~Picture();

    const std::string& name() const;

    const std::vector<std::shared_ptr<Rectangle>>& rects() const;

    void addRect(std::shared_ptr<Rectangle> rect);

    // TODO: add removing rect

    std::shared_ptr<Rectangle> rectByName(const std::string& name) const;

signals:
    void rectAdded(std::shared_ptr<Rectangle> rect);

private:
    std::string m_path;

    std::string m_name;

    QSize m_size;

    std::vector<std::shared_ptr<Rectangle>> m_rects;

    void loadImage();

    void loadLabels(const std::string& path, const std::string& labelPaths);
};

#endif // IMAGE_H
