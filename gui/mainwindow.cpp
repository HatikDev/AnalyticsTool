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

    connect(m_paintScene, &PaintScene::rectSelected, this, &MainWindow::on_paintSceneRectSelected);

    connect(m_paintScene, &PaintScene::rectRemove, this, &MainWindow::on_rectRemoved);

    ui->mainGraphicsView->setScene(m_paintScene);

    //ui->mainGraphicsView->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    //ui->rectsListWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    //connect(ui->rectsListWidget, &QListWidget::itemChanged, this, &MainWindow::listWidgetItemChanged);
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
    m_dataset.reset(new Dataset(path));

    m_paintScene->loadData(*m_dataset->data());
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

void MainWindow::updateLabels()
{
    /*size_t currentIndex = Model::instanse().dataset().currentIndex();
    size_t objectsCount = Model::instanse().dataset().count();
    ui->counterLabel->setText(QString("%1/%2").arg(currentIndex + 1).arg(objectsCount));

    ui->pictureName->setText(Model::instanse().dataset().currentName().c_str());*/
}

void MainWindow::loadImage(Picture picture)
{
    /*picture.scale(ui->mainGraphicsView->size());
    Model::instanse().setPicture(std::move(picture));*/
}

void MainWindow::keyPressEvent(QKeyEvent* keyEvent)
{
    switch (keyEvent->key()) {
    case Qt::Key_Delete:
    case Qt::Key_Backspace:
        auto selected = ui->rectsListView->selectionModel()->selection().indexes();
        if (selected.size() == 0)
            return;

        int id = m_rectModel->data(selected.at(0), Qt::UserRole).toInt();

        for (auto* item : m_paintScene->items()) {
            auto* rect = static_cast<Rectangle*>(item);
            if (rect && rect->number() == id) {
                m_rectModel->deleteRect(rect);
                m_paintScene->removeItem(item);
                break;
            }
        }
        break;
    }
}

void MainWindow::on_actionLoadDataset_triggered()
{
    QString datasetPath = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
        ".",
        QFileDialog::ShowDirsOnly
        | QFileDialog::DontResolveSymlinks);

    if (datasetPath.isEmpty())
        return;

    loadDataset(datasetPath.toStdString());
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

void MainWindow::on_rectAdded(Rectangle* rect)
{
    m_rectModel->addRect(rect);
}

void MainWindow::on_paintSceneRectSelected(Rectangle* rect)
{
    int row = m_rectModel->rowByRect(rect);
    QItemSelection selected(m_rectModel->index(row), m_rectModel->index(row));

    auto* model = ui->rectsListView->selectionModel();
    model->select(selected, QItemSelectionModel::ClearAndSelect);
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
    /*ui->rectsListWidget->clear();

    m_paintScene->reset();

    loadImage(Model::instanse().dataset().previous());

    ui->nextButton->setEnabled(true);

    updateLabels();*/
}

void MainWindow::on_nextButton_clicked()
{
    /*ui->rectsListWidget->clear();

    m_paintScene->reset();

    loadImage(Model::instanse().dataset().next());

    ui->prevButton->setEnabled(true);

    updateLabels();*/
}

void MainWindow::on_actionBrowseModel_triggered()
{
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
}

void MainWindow::listWidgetItemChanged(QListWidgetItem* item)
{
    /*int index = ui->rectsListWidget->row(item);

    if (item->checkState() == Qt::Checked)
        Model::instanse().picture().rectByNumber(index)->show();
    else
        Model::instanse().picture().rectByNumber(index)->hide();*/
}
