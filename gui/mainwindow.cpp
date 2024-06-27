#include "analyticsexception.h"
#include "constants.h"
#include "mainwindow.h"
#include "model/model.h"
#include "./ui_mainwindow.h"
#include "paintscene.h"
#include "utils.h"

#include "C:/devEnvironment/AutomatedMicroscopy/ModelLauncher/include/modellauncher.h"

#include <QPushButton>
#include <QColorDialog>
#include <QMessageBox>
#include <QFileDialog>
#include <QInputDialog>

namespace fs = std::filesystem;

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
	, m_paintScene{ new PaintScene() }
	, m_rectModel{ new RectModel() }
{
	ui->setupUi(this);

	ui->rectsListView->setModel(m_rectModel);

	connect(m_paintScene, &PaintScene::rectAdded, this, &MainWindow::on_rectAdded);

	connect(m_paintScene, &PaintScene::rectSelected, this, &MainWindow::on_paintSceneRectSelected);

	auto* rectSelectionModel = ui->rectsListView->selectionModel();
	connect(rectSelectionModel, &QItemSelectionModel::selectionChanged, this, &MainWindow::on_rectsListSelectionChanged);

	connect(ui->rectsListView, &QListView::doubleClicked, this, &MainWindow::on_rectsList_doubleClicked);

	connect(m_paintScene, &PaintScene::rectSelected, this, &MainWindow::on_paintSceneRectSelected);

	connect(m_paintScene, &PaintScene::rectCategorySelected, this, &MainWindow::on_rectDoubleClicked);

	connect(m_paintScene, &PaintScene::edited, this, &MainWindow::on_paintSceneEdited);

	connect(m_paintScene, &PaintScene::rectRemove, this, &MainWindow::on_rectRemoved);

	connect(ui->datasetObjectsList, &QListWidget::itemClicked, this, &MainWindow::on_dataObjectList_clicked);

	m_paintScene->setSceneRect(QRect(QPoint(0, 0), ui->mainGraphicsView->viewport()->rect().size()));

	ui->mainGraphicsView->setScene(m_paintScene);

	//int width = 360;
	//int height = 360;
	//ui->mainGraphicsView->setFixedSize(width, height);
	//ui->mainGraphicsView->setSceneRect(0, 0, width, height);
	//ui->mainGraphicsView->fitInView(0, 0, width, height, Qt::KeepAspectRatio);

	//ui->mainGraphicsView->setAlignment(Qt::AlignTop | Qt::AlignLeft);

	//ui->rectsListWidget->setContextMenuPolicy(Qt::CustomContextMenu);
	//connect(ui->rectsListView, &QListView::select, this, &MainWindow::listWidgetItemChanged);
	//connect(ui->rectsListWidget, &QListWidget::customContextMenuRequested,
	//    this, &MainWindow::provideContextMenu);

	//// connect for rects loading from dataset
	//connect(&Model::instanse().picture(), &Picture::rectAdded, m_paintScene, &PaintScene::on_rectAdded);
	//connect(&Model::instanse().picture(), &Picture::rectAdded, this, &MainWindow::on_rectAdded);

	//// connects for rects drawn by mouse
	//connect(m_paintScene, &PaintScene::rectAdded, this, &MainWindow::on_rectAdded);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::loadDataset(const fs::path& path)
{
	if (m_dataset)
		m_dataset->saveCurrent(m_paintScene->getData());

	m_dataset.reset(new BloodDataset(path));

	updateLabels();

	m_paintScene->loadData(*m_dataset->data());

	ui->mainGraphicsView->fitInView(m_paintScene->sceneRect(), Qt::KeepAspectRatio);

	updateDatasetObjectsList();

	updatePreviousNextButton();

	/*m_paintScene->reset();

	try {
		m_controller.loadDataset(path);
	}
	catch (const AnalyticsException& e) {
		QMessageBox::critical(
			this,
			tr("Analytics Tool"),
			tr("Dataset has invalid format"));

		return;
	}

	loadImage(Model::instanse().dataset().current());

	updateLabels();*/
}

void MainWindow::closeEvent(QCloseEvent* event)
{
	if (m_dataset)
		m_dataset->saveCurrent(m_paintScene->getData());
}

void MainWindow::updateLabels()
{
	size_t currentIndex = m_dataset->currentIndex();
	size_t objectsCount = m_dataset->size();
	ui->counterLabel->setText(QString("%1/%2").arg(currentIndex + 1).arg(objectsCount));

	auto objName = m_dataset->data()->data().imgPath.filename().string();
	ui->pictureName->setText(objName.c_str());
}

void MainWindow::loadImage(Picture picture)
{
	/*picture.scale(ui->mainGraphicsView->size());
	Model::instanse().setPicture(std::move(picture));*/
}

void MainWindow::keyPressEvent(QKeyEvent* keyEvent)
{
	if (m_paintScene->mode() != PaintMode::draw)
		return;

	switch (keyEvent->key()) {
	case Qt::Key_Delete:
	case Qt::Key_Backspace:
		auto selected = ui->rectsListView->selectionModel()->selection().indexes();
		if (selected.size() == 0)
			return;

		int id = m_rectModel->data(selected.at(0), Qt::UserRole).toInt();

		for (auto* item : m_paintScene->items()) {
			auto* rect = dynamic_cast<Rectangle*>(item);
			if (rect && rect->number() == id) {
				m_rectModel->deleteRect(rect);
				m_paintScene->removeItem(item);
				break;
			}
		}
		break;
	}
}

std::pair<size_t, std::string> MainWindow::showSelectTypeDialog()
{
	QInputDialog qDialog;

	QStringList items;
	for (auto& cl : m_dataset->classes())
		items << cl.second.c_str();

	qDialog.setOptions(QInputDialog::UseListViewForComboBoxItems);
	qDialog.setComboBoxItems(items);
	qDialog.setWindowTitle("Choose cell type");

	if (!qDialog.exec())
		throw AnalyticsException("Failed to show dialog");

	auto value = qDialog.textValue().toStdString();
	auto classes = m_dataset->classes();
	auto it = std::find_if(classes.begin(), classes.end(), [&value](const auto& pair) { return pair.second == value; });
	if (it == classes.end())
		throw AnalyticsException("Action has been cancelled by user");

	return *it;
}

void MainWindow::updatePreviousNextButton()
{
	if (!m_dataset)
	{
		ui->prevButton->setEnabled(false);
		ui->nextButton->setEnabled(false);
		return;
	}

	ui->prevButton->setEnabled(m_dataset->hasPrevious());
	ui->nextButton->setEnabled(m_dataset->hasNext());
}

void MainWindow::updateDatasetObjectsList()
{
	if (!m_dataset)
		return;

	auto batch = m_dataset->batch();

	QStringList objNames;
	for (auto& obj : batch)
		objNames << obj->data().imgPath.filename().string().c_str();


	ui->datasetObjectsList->clear();
	ui->datasetObjectsList->addItems(objNames);

	auto selectedRow = m_dataset->currentIndex() % m_dataset->batchSize();
	ui->datasetObjectsList->setCurrentRow(selectedRow);
}

void MainWindow::on_actionLoadDataset_triggered()
{
	QString datasetPath = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
		".",
		QFileDialog::ShowDirsOnly
		| QFileDialog::DontResolveSymlinks);

	if (datasetPath.isEmpty())
		return;

	try {
		auto bytes = datasetPath.toUtf8();
		std::u8string u8Path(bytes.begin(), bytes.end());
		loadDataset(u8Path);
	}
	catch (const AnalyticsException& exception)
	{
		QMessageBox::critical(this, tr("Analytics tool v2"),
			tr("Dataset has invalid format"),
			QMessageBox::Ok,
			QMessageBox::Ok);
	}
}

void MainWindow::on_rectsListWidget_itemClicked(QListWidgetItem* item)
{
	/*auto elementName = item->text();

	try {
		auto& rects = Model::instanse().picture().rects();
		for (auto& obj : rects)
			obj->deselect();

		auto rect = Model::instanse().picture().rectByNumber(ui->rectsListWidget->row(item));
		rect->select();

		m_paintScene->update();
	}
	catch (...) {
		QMessageBox::critical(this, tr("Analytics tool v2"),
			tr("Failed to find rectangle"),
			QMessageBox::Ok,
			QMessageBox::Ok);
	}*/
}

void MainWindow::on_rectAdded(Rectangle* rect, bool knownType)
{
	m_rectModel->addRect(rect);

	if (knownType || !m_dataset || m_paintScene->mode() != PaintMode::draw)
		return;

	try {
		auto [cellType, name] = showSelectTypeDialog();
		rect->setCellType(cellType);
		rect->setName(name);
	}
	catch (AnalyticsException& e) {
		on_rectRemoved(rect);
	}

	on_selectModeButton_clicked();
}

void MainWindow::on_paintSceneRectSelected(Rectangle* rect)
{
	int row = m_rectModel->rowByRect(rect);
	QItemSelection selected(m_rectModel->index(row), m_rectModel->index(row));

	auto* model = ui->rectsListView->selectionModel();
	model->select(selected, QItemSelectionModel::ClearAndSelect);
}

void MainWindow::on_paintSceneEdited()
{
	if (!m_dataset)
		return;

	ui->saveButton->setEnabled(true);

	setWindowTitle("Automatic Microscopic Bone Morrow*");
}

void MainWindow::on_rectsListSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected)
{
	auto deselectedList = deselected.indexes();
	for (auto& item : deselectedList) {
		size_t number = m_rectModel->data(item, Qt::UserRole).toInt();
		m_paintScene->on_rectDeselected(number);
	}

	auto selectedList = selected.indexes();
	if (selectedList.size() == 0)
		return;

	size_t number = m_rectModel->data(selectedList.at(0), Qt::UserRole).toInt();
	m_paintScene->on_rectSelected(number);
}

void MainWindow::on_rectDoubleClicked(Rectangle* rect)
{
	if (!m_dataset || m_paintScene->mode() != PaintMode::draw)
		return;

	try {
		auto [cellType, name] = showSelectTypeDialog();
		rect->setCellType(cellType);
		rect->setName(name);
	}
	catch (AnalyticsException& e) {}

	on_selectModeButton_clicked();
}

void MainWindow::on_rectsList_doubleClicked(const QModelIndex& index)
{
	if (!m_dataset || m_paintScene->mode() != PaintMode::draw)
		return;

	try {
		auto [cellType, name] = showSelectTypeDialog();

		auto id = m_rectModel->data(index, Qt::UserRole).toInt();
		for (auto* item : m_paintScene->items()) {
			auto* rect = dynamic_cast<Rectangle*>(item);
			if (rect && rect->number() == id) {
				rect->setCellType(cellType);
				rect->setName(name);
				break;
			}
		}
	}
	catch (AnalyticsException& e) {}
}

void MainWindow::on_rectRemoved(Rectangle* rect)
{
	// TODO: add handles of removing rects
	m_rectModel->deleteRect(rect);
	delete rect;
}

void MainWindow::provideContextMenu(const QPoint& pos)
{
	//QPoint item = ui->rectsListWidget->mapToGlobal(pos);
	//QMenu submenu;
	//submenu.addAction("edit");
	//submenu.addAction("delete");
	//QAction* rightClickItem = submenu.exec(item); // TODO: should we free memory?
	//if (rightClickItem) {
	//    if (rightClickItem->text().contains("edit")) {

	//    }
	//    else if (rightClickItem->text().contains("delete")) {

	//    }
	//}
}

void MainWindow::on_prevButton_clicked()
{
	m_dataset->saveCurrent(m_paintScene->getData());

	m_dataset->previous();

	m_paintScene->reset();

	m_paintScene->loadData(*m_dataset->data());

	updateLabels();

	ui->mainGraphicsView->fitInView(m_paintScene->sceneRect(), Qt::KeepAspectRatio);

	updateDatasetObjectsList();

	updatePreviousNextButton();
}

void MainWindow::on_nextButton_clicked()
{
	m_dataset->saveCurrent(m_paintScene->getData());

	m_dataset->next();

	m_paintScene->reset();

	m_paintScene->loadData(*m_dataset->data());

	updateLabels();

	ui->mainGraphicsView->fitInView(m_paintScene->sceneRect(), Qt::KeepAspectRatio);

	updateDatasetObjectsList();

	updatePreviousNextButton();
}

void MainWindow::on_selectModeButton_clicked()
{
	ui->actionselect->setChecked(true);
	ui->actiondraw->setChecked(false);

	ui->selectModeButton->setChecked(true);
	ui->drawModeButton->setChecked(false);

	m_paintScene->setMode(PaintMode::select);
}

void MainWindow::on_drawModeButton_clicked()
{
	ui->actionselect->setChecked(false);
	ui->actiondraw->setChecked(true);

	ui->selectModeButton->setChecked(false);
	ui->drawModeButton->setChecked(true);

	m_paintScene->setMode(PaintMode::draw);
}

void MainWindow::on_saveButton_clicked()
{
	if (!m_dataset)
		return;

	m_dataset->saveCurrent(m_paintScene->getData());

	setWindowTitle("Automatic Microscopic Bone Morrow");

	ui->saveButton->setEnabled(false);
}

void MainWindow::on_actionselect_triggered()
{
	ui->actionselect->setChecked(true);
	ui->actiondraw->setChecked(false);

	ui->selectModeButton->setChecked(true);
	ui->drawModeButton->setChecked(false);

	m_paintScene->setMode(PaintMode::select);
}

void MainWindow::on_actiondraw_triggered()
{
	ui->actionselect->setChecked(false);
	ui->actiondraw->setChecked(true);

	ui->selectModeButton->setChecked(false);
	ui->drawModeButton->setChecked(true);

	m_paintScene->setMode(PaintMode::draw);
}

void MainWindow::on_dataObjectList_clicked(QListWidgetItem* item)
{
	m_dataset->saveCurrent(m_paintScene->getData());

	auto index = ui->datasetObjectsList->row(item);
	m_dataset->setIndex(m_dataset->batchStart() + index);

	m_paintScene->reset();

	m_paintScene->loadData(*m_dataset->data());

	updateLabels();

	ui->mainGraphicsView->fitInView(m_paintScene->sceneRect(), Qt::KeepAspectRatio);

	updatePreviousNextButton();
}

//void MainWindow::on_actionLoadDataset_triggered()
//{
	//QString fileName = QFileDialog::getOpenFileName(this,
	//                                         tr("Open onnx model"), ".", tr("All files (*.*)"));
	//if (fileName.isEmpty())
	//    return;

	//std::filesystem::path file = fileName.toStdString();
	//if (file.extension() != ".onnx") {
	//    QMessageBox::critical(
	//        this,
	//        tr("Analytics Tool"),
	//        tr("Selected file is not onnx model"));
	//    return;
	//}

	//if (Model::instanse().dataset().count() == 0) {
	//    QMessageBox::critical(
	//        this,
	//        tr("Analytics Tool"),
	//        tr("Dataset is not loaded"));
	//    return;
	//}

	//std::string datasetPath = Model::instanse().dataset().path();
	//loader::loadModel(fileName.toStdString().c_str(), datasetPath.data());

	//for (size_t i = 0; i < Model::instanse().dataset().count(); ++i) {
	//    QString resultPath = QString("%1/images_results/%2.txt").arg(Model::instanse().dataset().path().c_str())
	//                                    .arg(Model::instanse().dataset().currentName().c_str());
	//    QString imagePath = QString("%1/images/%2.jpg").arg(Model::instanse().dataset().path().c_str())
	//                                    .arg(Model::instanse().dataset().currentName().c_str());
	//    loader::predict(imagePath.toStdString().c_str(), resultPath.toStdString().c_str());

	//    Model::instanse().dataset().next(); // TODO: refactor this
	//}
//}

void MainWindow::listWidgetItemChanged(QListWidgetItem* item)
{
	/*int index = ui->rectsListWidget->row(item);

	if (item->checkState() == Qt::Checked)
		Model::instanse().picture().rectByNumber(index)->show();
	else
		Model::instanse().picture().rectByNumber(index)->hide();*/
}
