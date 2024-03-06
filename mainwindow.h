#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "figure.h"
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
    void on_rectangleButton_clicked();
    void on_circleButton_clicked();
    void on_selectColorButton_clicked();
    void on_figuresListWidget_itemClicked(QListWidgetItem *item);
    void on_figuresListWidget_itemDoubleClicked(QListWidgetItem *item);

    void on_figureAdded(std::shared_ptr<Figure> figure);
    void on_updateButton_clicked();
    void on_spinBox_valueChanged(int value);

    void provideContextMenu(const QPoint& pos);
};
#endif // MAINWINDOW_H
