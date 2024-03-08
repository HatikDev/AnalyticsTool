#include "mainwindow.h"
#include "model.h"
#include "./ui_mainwindow.h"
#include "paintscene.h"

#include <QPushButton>
#include <QColorDialog>
#include <QMessageBox>
#include <QDebug>

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlTableModel>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_paintScene{ new PaintScene() }
{
    ui->setupUi(this);

    ui->mainGraphicsView->setScene(m_paintScene);
    ui->mainGraphicsView->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    ui->rectsListWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->rectsListWidget, &QListWidget::customContextMenuRequested,
            this, &MainWindow::provideContextMenu);

    connect(m_paintScene, &PaintScene::rectAdded, this, &MainWindow::on_rectAdded);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_selectColorButton_clicked()
{
    QColor color = QColorDialog::getColor(Qt::yellow, this);

    m_controller.setDefaultColor(color);
}

void MainWindow::on_rectsListWidget_itemClicked(QListWidgetItem *item)
{
    auto elementName = item->text();

    try {
        auto& rects = Model::instanse().picture().rects();
        for (auto& obj : rects)
            obj->deselect();

        auto rect = Model::instanse().picture().rectByName(elementName.toStdString());
        rect->select();

        ui->mainGraphicsView->update();
    } catch (...) {
        QMessageBox::critical(this, tr("Analytics tool v2"),
                                       tr("Failed to find rectangle"),
                                       QMessageBox::Ok,
                                       QMessageBox::Ok);
    }
}

void MainWindow::on_rectsListWidget_itemDoubleClicked(QListWidgetItem *item)
{

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

void MainWindow::on_updateButton_clicked()
{
    // TODO: clear data
    ui->rectsListWidget->clear();

    auto& list = Model::instanse().picture().rects();
    for (auto& obj : list)
    {
        QPixmap pixmap(10, 10);
        pixmap.fill(obj->graphicSettings().color());

        QListWidgetItem* item = new QListWidgetItem(QIcon(pixmap), obj->name().c_str());
        ui->rectsListWidget->addItem(item);
    }
    ui->mainGraphicsView->update();
}

void MainWindow::on_spinBox_valueChanged(int value)
{
    m_controller.changeCurrentDepthColor(value);
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
