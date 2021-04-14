#pragma once
#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QRadioButton>
#include <QCheckBox>
#include <QTableWidget>
#include <string>
#include <vector>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <QFileDialog>
#include <QAction>
#include <QMenuBar>
#include <QMenu>

class MainWindow :
	public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget* parent = nullptr);
	~MainWindow();
	inline void setPath(std::string p) { path = p; }

private slots:
	void onAddTask();
	void onChangeDB();
	void onExitProgram();
	void onNewDataFile();
	void onCellSelected(int, int);

public slots:
	void filter();

private:
	std::string path;
	QRadioButton *all, *overdue, *today, *thisweek;
	QCheckBox *completed;
	QTableWidget *table;
	QStringList tableHeader;
	QMenu *fileMenu;
	QAction *newDataFile, *addTask, *changeDB, *exitProgram;
	QMenuBar *menuBar;
	void init();
	void createMenus();
	void createFilters(QHBoxLayout *layout);
	void initializeTable();
	int getWeekNumber(tm t);
	void updateTable(std::vector<std::string*> data);
};

