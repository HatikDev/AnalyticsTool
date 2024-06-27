#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "rectangle.h"
#include "paintscene.h"
#include "model/model.h"
#include "model/dataset.h"

#include <QMainWindow>
#include <QListWidgetItem>
#include <QShortcut>
#include <memory>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

#include "controller/controller.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget* parent = nullptr);
	~MainWindow();

	void loadDataset(const std::filesystem::path& path);

protected:
	void closeEvent(QCloseEvent* event);

private:
	Ui::MainWindow* ui;

	Controller m_controller;

	PaintScene* m_paintScene;

	RectModel* m_rectModel;

	std::shared_ptr<BloodDataset> m_dataset;

	QShortcut* m_keyA, * m_keyD, * m_keyO;

	void updateLabels();

	void loadImage(Picture picture);

	void keyPressEvent(QKeyEvent* keyEvent) override;

	std::pair<size_t, std::string> showSelectTypeDialog();

	void updatePreviousNextButton();

	void updateDatasetObjectsList();

	void loadIcons();

private slots:
	void on_actionLoadDataset_triggered();

	void on_rectsListWidget_itemClicked(QListWidgetItem* item);

	void on_rectAdded(Rectangle* rect, bool knownType);
	void on_paintSceneRectSelected(Rectangle* rect);
	void on_paintSceneEdited();
	void on_rectsListSelectionChanged(const QItemSelection& selected, const QItemSelection& delesected);
	void on_rectDoubleClicked(Rectangle* rect);
	void on_rectsList_doubleClicked(const QModelIndex& index);
	void on_rectRemoved(Rectangle* rect);

	void provideContextMenu(const QPoint& pos);

	void on_prevButton_clicked();
	void on_nextButton_clicked();
	void on_selectModeButton_clicked();
	void on_drawModeButton_clicked();
	void on_saveButton_clicked();
	void on_actionselect_triggered();
	void on_actiondraw_triggered();
	void on_dataObjectList_clicked(QListWidgetItem* item);

	void listWidgetItemChanged(QListWidgetItem* item);
	void on_zoomInButton_clicked();
	void on_zoomOutButton_clicked();
	void on_loadDatasetButton_clicked();
	void on_drawMarkerButton_clicked();
	void on_nextPushButton_clicked();
	void on_prevPushButton_clicked();
	void on_saveMarkerButton_clicked();
};
#endif // MAINWINDOW_H
