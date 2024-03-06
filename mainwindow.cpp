#include "listobjectsviewcontroller.h"
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

    ui->figuresListWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->figuresListWidget, &QListWidget::customContextMenuRequested,
            this, &MainWindow::provideContextMenu);

    connect(m_paintScene, &PaintScene::figureAdded, this, &MainWindow::on_figureAdded);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_rectangleButton_clicked()
{
    m_controller.setDefaultFigure(Figure::FigureTypes::rectangle);
}

void MainWindow::on_circleButton_clicked()
{
    m_controller.setDefaultFigure(Figure::FigureTypes::circle);
}

void MainWindow::on_selectColorButton_clicked()
{
    QColor color = QColorDialog::getColor(Qt::yellow, this);

    m_controller.setDefaultColor(color);
}

void MainWindow::on_figuresListWidget_itemClicked(QListWidgetItem *item)
{
    auto elementName = item->text();

    try {
        auto& figures = Model::instanse().figures();
        for (auto& obj : figures)
            obj->deselect();

        auto figure = Model::instanse().figureByName(elementName.toStdString());
        figure->select();

        ui->mainGraphicsView->update();
    } catch (...) {
        QMessageBox::critical(this, tr("Analytics tool v2"),
                                       tr("Failed to find figure"),
                                       QMessageBox::Ok,
                                       QMessageBox::Ok);
    }
}

void MainWindow::on_figuresListWidget_itemDoubleClicked(QListWidgetItem *item)
{

}

void MainWindow::on_figureAdded(std::shared_ptr<Figure> figure)
{
    m_controller.addFigure(figure);

    QPixmap pixmap(10, 10);
    pixmap.fill(figure->graphicSettings().color());

    QListWidgetItem* item = new QListWidgetItem(QIcon(pixmap), figure->name().c_str());
    item->setFlags(item->flags() | Qt::ItemIsEditable);
    ui->figuresListWidget->addItem(item);
}

void MainWindow::on_updateButton_clicked()
{
    // TODO: clear data
    ui->figuresListWidget->clear();

    auto& list = Model::instanse().figures();
    for (auto& obj : list)
    {
        QPixmap pixmap(10, 10);
        pixmap.fill(obj->graphicSettings().color());

        QListWidgetItem* item = new QListWidgetItem(QIcon(pixmap), obj->name().c_str());
        ui->figuresListWidget->addItem(item);
    }
    ui->mainGraphicsView->update();
}

void MainWindow::on_spinBox_valueChanged(int value)
{
    m_controller.changeCurrentDepthColor(value);
}

void MainWindow::provideContextMenu(const QPoint& pos)
{
    QPoint item = ui->figuresListWidget->mapToGlobal(pos);
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
