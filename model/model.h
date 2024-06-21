#pragma once

//#include "dataset.h"
//#include "picture.h"
#include "gui/rectangle.h"

#include <memory>
#include <vector>
#include <QAbstractItemModel>

//class Model : public QObject {
//    Q_OBJECT
//
//public:
//    Model(const Model&) = delete;
//    Model(Model&&) = delete;
//    Model& operator=(const Model&) = delete;
//    Model& operator=(Model&&) = delete;
//    ~Model() {}
//
//    static Model& instanse();
//
//    Picture& picture();
//
//    void setPicture(Picture&& picture);
//
//    Dataset& dataset();
//
//    void addRect(std::shared_ptr<Rectangle> rect);
//
//    void removeRect(std::shared_ptr<Rectangle> rect);
//
//    void loadDataset(const std::string& path);
//
//private:
//    static Model* m_instanse;
//
//    Picture m_picture;
//
//    Dataset m_dataset;
//
//    Model(QObject* parent = nullptr);
//};

class RectModel : public QAbstractListModel {
	Q_OBJECT
public:
    RectModel(QObject* parent = 0);

    int rowCount(const QModelIndex& parent = QModelIndex()) const;

    int columnCount(const QModelIndex& parent = QModelIndex()) const;

    QVariant data(const QModelIndex& index, int role) const;

    bool setData(const QModelIndex& index, const QVariant& value, int role) override;

    void addRect(Rectangle* rect);

    void deleteRect(Rectangle* rect);

    int rowByRect(Rectangle* rect) const;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

private:
    std::unordered_map<size_t, Rectangle*> m_rects;
};
