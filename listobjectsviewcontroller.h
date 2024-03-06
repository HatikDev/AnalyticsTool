#pragma once

#include "figure.h"

#include <memory>
#include <QListWidgetItem>

class MainWindow;

class ListObjectsViewController : public QObject {
    Q_OBJECT

public:
    ListObjectsViewController(QObject* parent = nullptr);
    ~ListObjectsViewController();

private:

signals:

public slots:
    void itemAdded(std::shared_ptr<Figure> figure);
    //void itemEdited(QListWidgetItem* item);
    //void itemDoubleClicked(QListWidgetItem* item);
    void itemDeleted(std::shared_ptr<Figure> figure);

    void itemClicked(QListWidgetItem* item);
};