#include "mainwindow.h"
#include "model/model.h"
#include "./ui_mainwindow.h"
#include "paintscene.h"

#include <QPushButton>
#include <QColorDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_paintScene{ new PaintScene() }
{
    ui->setupUi(this);

    connect(m_paintScene, &PaintScene::rectSelected, this, &MainWindow::on_rectSelected);
    connect(m_paintScene, &PaintScene::rectDeselected, this, &MainWindow::on_rectDeselected);

    ui->mainGraphicsView->setScene(m_paintScene);
    ui->mainGraphicsView->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    ui->rectsListWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->rectsListWidget, &QListWidget::customContextMenuRequested,
        this, &MainWindow::provideContextMenu);

    // connect for rects loading from dataset
    connect(&Model::instanse().picture(), &Picture::rectAdded, m_paintScene, &PaintScene::on_rectAdded);
    connect(&Model::instanse().picture(), &Picture::rectAdded, this, &MainWindow::on_rectAdded);

    // connects for rects drawn by mouse
    connect(m_paintScene, &PaintScene::rectAdded, this, &MainWindow::on_rectAdded);

    loadDataset("C:/Users/Sergey/Documents/cpp/AnalyticsTool/build/Debug/data");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadDataset(const std::string& path)
{
    m_controller.loadDataset(path);

    updateLabels();
}

void MainWindow::updateLabels()
{
    size_t currentIndex = Model::instanse().dataset().currentIndex();
    size_t objectsCount = Model::instanse().dataset().count();
    ui->counterLabel->setText(QString("%1/%2").arg(currentIndex + 1).arg(objectsCount));

    ui->pictureName->setText(Model::instanse().dataset().currentName().c_str());
}

void MainWindow::on_selectColorButton_clicked()
{
    QColor color = QColorDialog::getColor(Qt::yellow, this);

    m_controller.setDefaultColor(color);
}

void MainWindow::on_rectsListWidget_itemClicked(QListWidgetItem* item)
{
    auto elementName = item->text();

    try {
        auto& rects = Model::instanse().picture().rects();
        for (auto& obj : rects)
            obj->deselect();

        auto rect = Model::instanse().picture().rectByName(elementName.toStdString());
        rect->select();

        m_paintScene->update();
    }
    catch (...) {
        QMessageBox::critical(this, tr("Analytics tool v2"),
            tr("Failed to find rectangle"),
            QMessageBox::Ok,
            QMessageBox::Ok);
    }
}

void MainWindow::on_rectAdded(std::shared_ptr<Rectangle> rect)
{
    m_controller.addRect(rect);

    QPixmap pixmap(10, 10);
    pixmap.fill(rect->graphicSettings().color());

    QListWidgetItem* item = new QListWidgetItem(QIcon(pixmap), rect->name().c_str());
    item->setFlags(item->flags() | Qt::ItemIsEditable);
    ui->rectsListWidget->addItem(item);
}

void MainWindow::on_rectSelected(std::shared_ptr<Rectangle> rect)
{
    ui->rectsListWidget->items();
    // TODO: add handler
}

void MainWindow::on_rectDeselected(std::shared_ptr<Rectangle> rect)
{
    // TODO: add handler
}

void MainWindow::on_rectRemoved(std::shared_ptr<Rectangle> rect)
{
    // TODO: add handles of removing rects
}

void MainWindow::provideContextMenu(const QPoint& pos)
{
    QPoint item = ui->rectsListWidget->mapToGlobal(pos);
    QMenu submenu;
    submenu.addAction("edit");
    submenu.addAction("delete");
    QAction* rightClickItem = submenu.exec(item); // TODO: should we free memory?
    if (rightClickItem) {
        if (rightClickItem->text().contains("edit")) {

        }
        else if (rightClickItem->text().contains("delete")) {

        }
    }
}

void MainWindow::on_prevButton_clicked()
{
    ui->rectsListWidget->clear();

    try {
        m_controller.previousPicture();
    }
    catch (...) {
        ui->prevButton->setEnabled(false);
    }

    ui->nextButton->setEnabled(true);

    updateLabels();
}

void MainWindow::on_nextButton_clicked()
{
    ui->rectsListWidget->clear();

    try {
        m_controller.nextPicture();
    }
    catch (...) {
        ui->nextButton->setEnabled(false);
    }

    ui->prevButton->setEnabled(true);

    updateLabels();
}
