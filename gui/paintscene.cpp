#include "analyticsexception.h"
#include "constants.h"
#include "model/model.h"
#include "model/picture.h"
#include "paintscene.h"

#include <stdexcept>

#include <QPixmap>

namespace
{
	bool isInside(QPointF point, const Rectangle& rect) {
		auto startPoint = rect.topLeft();
		auto endPoint = rect.bottomRight();

		return point.x() >= startPoint.x() && point.x() <= endPoint.x()
			&& point.y() >= startPoint.y() && point.y() <= endPoint.y();
	}

	size_t distance(QPointF p1, QPointF p2) {
		return sqrt(pow(p1.x() - p2.x(), 2) + pow(p1.y() - p2.y(), 2));
	}
}

size_t PaintScene::counter = 0;

namespace fs = std::filesystem;

PaintScene::PaintScene(QObject* parent)
	: QGraphicsScene{ parent }
{
	setSceneRect(0, 0, 512, 512); // TODO: think about size

	connect(this, &QGraphicsScene::selectionChanged, this, &PaintScene::itemClicked);

	//auto& picture = Model::instanse().picture();
	//picture.setPos(0, 0); // TODO: think about position

	//addItem(&Model::instanse().picture());
}

PaintScene::~PaintScene()
{}

void PaintScene::reset()
{
	auto rects = items();
	for (auto* item : rects) {
		removeItem(item);

		auto* rect = dynamic_cast<Rectangle*>(item);
		if (!rect)
			continue;

		emit rectRemove(rect);
	}

	counter = 0;
	m_mouseState = MouseState::released;
}

void PaintScene::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	QGraphicsScene::mousePressEvent(event);

	if (m_paintMode == PaintMode::draw && tryEditRect(*event)) {
		m_mouseState = MouseState::editing;
		return;
	}

	if (selectedItems().size() != 0 || m_paintMode != PaintMode::draw)
		return;

	tryCreateLocalRect(*event);
	m_mouseState = MouseState::creating;
}

void PaintScene::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
	QGraphicsScene::mouseMoveEvent(event);

	if (!m_currentRect)
		return;

	m_currentRect->setEndPoint(event->scenePos());

	update();
}

void PaintScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
	QGraphicsScene::mouseReleaseEvent(event);

	if (!m_currentRect)
		return;

	if (m_mouseState == MouseState::creating)
	{
		emit rectAdded(m_currentRect, false);
		m_currentRect = nullptr;
	}

	if (m_mouseState == MouseState::editing)
		m_currentRect = nullptr;

	m_mouseState = MouseState::released;

	update();
}

void PaintScene::keyPressEvent(QKeyEvent* keyEvent)
{
	if (m_paintMode != PaintMode::draw)
		return;

	switch (keyEvent->key()) {
	case Qt::Key_Delete:
	case Qt::Key_Backspace:
		auto items = selectedItems();
		if (items.size() == 0)
			return;

		emit rectRemove(static_cast<Rectangle*>(items.at(0)));
		//removeItem(items.at(0));
	}
}

bool PaintScene::tryEditRect(const QGraphicsSceneMouseEvent& event)
{
	for (auto* rect : selectedItems()) {
		auto boundingRect = rect->boundingRect();
		if (distance(boundingRect.topLeft(), event.scenePos()) <= kSelectionCircleRadius) {
			m_currentRect = static_cast<Rectangle*>(rect);
			//m_currentRect->setFlag(QGraphicsItem::ItemIsMovable, false);
			m_currentRect->setStartPoint(boundingRect.bottomRight());
			m_currentRect->setEndPoint(boundingRect.topLeft());
			return true;
		}

		if (distance(boundingRect.topRight(), event.scenePos()) <= kSelectionCircleRadius) {
			m_currentRect = static_cast<Rectangle*>(rect);
			//m_currentRect->setFlag(QGraphicsItem::ItemIsMovable, false);
			m_currentRect->setStartPoint(boundingRect.bottomLeft());
			m_currentRect->setEndPoint(boundingRect.topRight());
			return true;
		}

		if (distance(boundingRect.bottomLeft(), event.scenePos()) <= kSelectionCircleRadius) {
			m_currentRect = static_cast<Rectangle*>(rect);
			//m_currentRect->setFlag(QGraphicsItem::ItemIsMovable, false);
			m_currentRect->setStartPoint(boundingRect.topRight());
			m_currentRect->setEndPoint(boundingRect.bottomLeft());
			return true;
		}

		if (distance(boundingRect.bottomRight(), event.scenePos()) <= kSelectionCircleRadius) {
			m_currentRect = static_cast<Rectangle*>(rect);
			//m_currentRect->setFlag(QGraphicsItem::ItemIsMovable, false);
			m_currentRect->setStartPoint(boundingRect.topLeft());
			m_currentRect->setEndPoint(boundingRect.bottomRight());
			return true;
		}
	}

	return false;
}

bool PaintScene::tryCreateLocalRect(const QGraphicsSceneMouseEvent& event)
{
	std::string name = "New rect " + std::to_string(counter++);
	m_currentRect = new Rectangle(name, items().size(), event.scenePos(), 0); // TODO: change type
	m_currentRect->setFlag(QGraphicsItem::ItemIsSelectable, true);
	//m_currentRect->setFlag(QGraphicsItem::ItemIsMovable, true);

	addItem(m_currentRect);

	m_mouseState = MouseState::creating;

	update();

	return true;
}

void PaintScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event)
{
	auto itemsList = items(event->scenePos());
	if (itemsList.isEmpty())
		return;

	for (auto* item : itemsList) {
		auto* rect = dynamic_cast<Rectangle*>(item);
		if (!rect)
			continue;

		emit rectCategorySelected(rect);
	}
}

void PaintScene::loadData(IDataObject<BloodCellObj>& dataObject)
{
	// here we should call dataObject.data() to get data from dataObject
	// It's temporary realization

	// load image
	auto& obj = dataObject.data();

	fs::path path = obj.imgPath;
	QImage image(path.string().c_str());
	auto imageScaled = image.scaled({ 480, 480 }, Qt::KeepAspectRatio);

	QGraphicsPixmapItem* pixmap = new QGraphicsPixmapItem;
	pixmap->setPixmap(QPixmap::fromImage(imageScaled));
	pixmap->setPos(0, 0);
	pixmap->setFlag(QGraphicsPixmapItem::ItemIsSelectable, false);

	addItem(pixmap);
	setSceneRect(QRect(QPoint(0, 0), imageScaled.size()));

	// load rects
	auto pos = pixmap->pos();

	auto sceneWidth = imageScaled.size().width();
	auto sceneHeight = imageScaled.size().height();
	for (auto& rect : obj.rects) {
		QPointF p1 = { (rect.centerX - rect.w / 2) * sceneWidth, (rect.centerY - rect.h / 2) * sceneHeight };
		QPointF p2 = { (rect.centerX + rect.w / 2) * sceneWidth, (rect.centerY + rect.h / 2) * sceneHeight };

		auto* rectObj = new Rectangle(rect.name, counter++, p1, rect.type);
		rectObj->setEndPoint(p2);
		rectObj->setFlag(QGraphicsItem::ItemIsSelectable, true);
		addItem(rectObj);

		emit rectAdded(rectObj, true);
	}

	update();
}

BloodCellObj PaintScene::getData() const
{
	BloodCellObj obj;
	obj.rects.reserve(items().length());

	auto sceneWidth = 480; // TODO: replace constant
	auto sceneHeight = 480; // TODO: replace constant

	for (auto* item : items())
	{
		auto* rect = dynamic_cast<Rectangle*>(item);
		if (!rect) // now we just skip image path
			continue;

		float centerX = (rect->topLeft().x() + rect->topRight().x()) / 2. / sceneWidth;
		float centerY = (rect->topLeft().y() + rect->bottomLeft().y()) / 2. / sceneHeight;
		float width = (rect->topRight().x() - rect->topLeft().x()) * 1. / sceneWidth;
		float height = (rect->bottomLeft().y() - rect->topLeft().y()) * 1. / sceneHeight;
		obj.rects.emplace_back(static_cast<size_t>(rect->cellType()), rect->name(), centerX, centerY, width, height);
	}

	return obj;
}

void PaintScene::setMode(PaintMode mode)
{
	m_paintMode = mode;
}

PaintMode PaintScene::mode() const
{
	return m_paintMode;
}

void PaintScene::on_rectSelected(size_t rectId)
{
	for (auto* item : items()) {
		auto* rect = dynamic_cast<Rectangle*>(item);
		if (!rect)
			continue;

		if (rect->number() == rectId) {
			rect->select();
			update();
			break;
		}
	}
}

void PaintScene::on_rectDeselected(size_t rectId)
{
	for (auto* item : items()) {
		auto* rect = dynamic_cast<Rectangle*>(item);

		if (rect && rect->number() == rectId) {
			rect->deselect();
			update();
			break;
		}
	}
}

void PaintScene::itemClicked()
{
	for (auto* item : items()) {
		auto* obj = dynamic_cast<Rectangle*>(item);
		if (!obj)
			continue;

		obj->deselect();
	}

	for (auto* item : selectedItems()) {
		auto* rect = dynamic_cast<Rectangle*>(item);
		if (!rect)
			continue;

		rect->select();
		emit rectSelected(rect);
	}

	update();
}

