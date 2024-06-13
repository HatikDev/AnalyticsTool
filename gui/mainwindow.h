#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "rectangle.h"
#include "paintscene.h"
#include "model/model.h"
#include "model/dataset.h"

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

    void loadDataset(const std::filesystem::path& path);

private:
    Ui::MainWindow *ui;

    Controller m_controller;

    PaintScene* m_paintScene;

    RectModel* m_rectModel;

    std::shared_ptr<Dataset> m_dataset;

    void updateLabels();

    void loadImage(Picture picture);

    void keyPressEvent(QKeyEvent* keyEvent) override;

private slots:
    void on_actionLoadDataset_triggered();

    void on_rectsListWidget_itemClicked(QListWidgetItem *item);

    void on_rectAdded(Rectangle* rect);
    void on_paintSceneRectSelected(Rectangle* rect);
    void on_rectsListSelectionChanged(const QItemSelection& selected, const QItemSelection& delesected);
    void on_rectRemoved(Rectangle* rect);

    void provideContextMenu(const QPoint& pos);

    void on_prevButton_clicked();
    void on_nextButton_clicked();
    void on_actionBrowseModel_triggered();

    void listWidgetItemChanged(QListWidgetItem* item);
};
#endif // MAINWINDOW_H
