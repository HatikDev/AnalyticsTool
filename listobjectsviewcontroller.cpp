#include "listobjectsviewcontroller.h"

ListObjectsViewController::ListObjectsViewController(QObject* parent)
    : QObject{ parent } {}

ListObjectsViewController::~ListObjectsViewController() {}

void ListObjectsViewController::itemAdded(std::shared_ptr<Figure> figure)
{

}

void ListObjectsViewController::itemClicked(QListWidgetItem* item)
{

}

void ListObjectsViewController::itemDeleted(std::shared_ptr<Figure> figure)
{
    // TODO: add deleting
}