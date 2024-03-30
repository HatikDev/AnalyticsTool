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

#include "controller/controller.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void loadDataset(const std::string& path);

private:
    Ui::MainWindow *ui;

    Controller m_controller;

    PaintScene* m_paintScene;

    void updateLabels();

private slots:
    void on_selectColorButton_clicked();

    void on_rectsListWidget_itemClicked(QListWidgetItem *item);

    void on_rectAdded(std::shared_ptr<Rectangle> rect);
    void on_rectSelected(std::shared_ptr<Rectangle> rect);
    void on_rectDeselected(std::shared_ptr<Rectangle> rect);
    void on_rectRemoved(std::shared_ptr<Rectangle> rect);

    void provideContextMenu(const QPoint& pos);

    void on_prevButton_clicked();
    void on_nextButton_clicked();
};
#endif // MAINWINDOW_H
