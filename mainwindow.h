#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "rectangle.h"
#include "paintscene.h"

#include <QMainWindow>
#include <QListWidgetItem>
#include <memory>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

#include "controller.h"

//class ListObjectsView;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    Controller m_controller;

    PaintScene* m_paintScene;

public slots:

private slots:
    void on_selectColorButton_clicked();
    void on_rectsListWidget_itemClicked(QListWidgetItem *item);
    void on_rectsListWidget_itemDoubleClicked(QListWidgetItem *item);

    void on_rectAdded(std::shared_ptr<Rectangle> rect);
    void on_updateButton_clicked();
    void on_spinBox_valueChanged(int value);

    void provideContextMenu(const QPoint& pos);
};
#endif // MAINWINDOW_H
