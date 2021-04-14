#include "MainWindow.h"
#include "IOManager.h"
#include "NewTaskDialog.h"
#include "CreateDataFileDialog.h"
#include <QHeaderView>

MainWindow::MainWindow(QWidget * parent) : 
	QMainWindow(parent),
	all(new QRadioButton),
	overdue(new QRadioButton),
	today(new QRadioButton),
	thisweek(new QRadioButton),
	completed(new QCheckBox),
	menuBar(new QMenuBar)
{
	init();
}

MainWindow::~MainWindow()
{
	delete all;
	delete overdue;
	delete today;
	delete thisweek;
	delete completed;
	delete table;
	delete addTask;
	delete changeDB;
	delete exitProgram;
	delete fileMenu;

}

void MainWindow::createMenus()
{
	addTask = new QAction(tr("&Add"), this);
	if (IOManager::exists(path) == false)
	{
		addTask->setEnabled(false);
	}
	addTask->setShortcuts(QKeySequence::New);
	QObject::connect(addTask, &QAction::triggered, this, &MainWindow::onAddTask);

	newDataFile = new QAction(tr("&Create New Data File"), this);
	QObject::connect(newDataFile, &QAction::triggered, this, &MainWindow::onNewDataFile);

	changeDB = new QAction(tr("&Change Data File"), this);
	QObject::connect(changeDB, &QAction::triggered, this, &MainWindow::onChangeDB);

	exitProgram = new QAction(tr("&Exit"), this);
	QObject::connect(exitProgram, &QAction::triggered, this, &MainWindow::onExitProgram);

	fileMenu = menuBar->addMenu(tr("&File"));
	fileMenu->addAction(addTask);
	fileMenu->addAction(newDataFile);
	fileMenu->addAction(changeDB);
	fileMenu->addAction(exitProgram);
}

void MainWindow::createFilters(QHBoxLayout *layout)
{
	all->setText(tr("&All"));
	all->setChecked(true);
	overdue->setText(tr("&Overdue"));
	today->setText(tr("&Today"));
	thisweek->setText(tr("&This Week"));
	completed->setText(tr("&Not Completed"));

	QObject::connect(all, SIGNAL(clicked()), this, SLOT(filter()));
	QObject::connect(overdue, SIGNAL(clicked()), this, SLOT(filter()));
	QObject::connect(today, SIGNAL(clicked()), this, SLOT(filter()));
	QObject::connect(thisweek, SIGNAL(clicked()), this, SLOT(filter()));
	QObject::connect(completed, SIGNAL(clicked()), this, SLOT(filter()));

	QHBoxLayout *cbLayout = new QHBoxLayout;
	cbLayout->setAlignment(Qt::AlignRight);
	cbLayout->addWidget(completed);

	layout->addWidget(all);
	layout->addWidget(overdue);
	layout->addWidget(today);
	layout->addWidget(thisweek);
	layout->addLayout(cbLayout);

	layout->setStretch(0, 0);
	layout->setStretch(1, 0);
	layout->setStretch(2, 0);
	layout->setStretch(3, 0);
	layout->setStretch(4, 1);
}

void MainWindow::initializeTable()
{
	table->setColumnCount(5);
	tableHeader << "Finished" << "Due Date" << "Title" << "% Done" << "Description";
	table->setHorizontalHeaderLabels(tableHeader);
	table->horizontalHeader()->setStretchLastSection(true);
	table->verticalHeader()->setVisible(false);
	table->setEditTriggers(QAbstractItemView::NoEditTriggers);
	table->setSelectionBehavior(QAbstractItemView::SelectRows);
	table->setSelectionMode(QAbstractItemView::SingleSelection);
	table->setStyleSheet("QTableView {selection-background-color: #E0F7FA; selection-color: #000000;}");

	updateTable(IOManager::readFile(path));
}

void MainWindow::onAddTask()
{
	NewTaskDialog dlg;
	dlg.setModal(true);
	dlg.setPath(path);
	dlg.setOrigin(this);
	dlg.exec();
}

void MainWindow::onNewDataFile()
{
	CreateDataFileDialog dlg;
	dlg.setModal(true);
	dlg.setOrigin(this);
	dlg.exec();
}

void MainWindow::onChangeDB()
{
	QFileDialog dlg(this);
	dlg.setFileMode(QFileDialog::ExistingFile);
	dlg.setNameFilter(tr("Text (*.txt)"));
	dlg.setViewMode(QFileDialog::List);

	QStringList fileNames;
	if (dlg.exec()) {
		fileNames = dlg.selectedFiles();
		path = ((QString)fileNames.at(0)).toUtf8().constData();
		filter();
	}

	addTask->setEnabled(true);
}

void MainWindow::onExitProgram()
{
	exit(0);
}

void MainWindow::onCellSelected(int nRow, int nCol)
{
	NewTaskDialog dlg;
	dlg.setModal(true);
	dlg.setData(table->item(nRow, 1)->text().toUtf8().constData(), table->item(nRow, 2)->text().toUtf8().constData(), table->item(nRow, 3)->text().toUtf8().constData(), table->item(nRow, 4)->text().toUtf8().constData());
	dlg.setPath(path);
	dlg.setOrigin(this);
	dlg.setDelete(true);
	dlg.exec();
}

void MainWindow::updateTable(std::vector<std::string*> data) {
	table->clearContents();
	table->setRowCount(data.size());

	for (int i = 0; i < data.size(); i++) {
		QCheckBox *qcb = new QCheckBox;
		qcb->setStyleSheet(QStringLiteral("QCheckBox::indicator {subcontrol-position: center;}"));
		qcb->setAttribute(Qt::WA_TransparentForMouseEvents, true);
		qcb->setFocusPolicy(Qt::NoFocus);
		qcb->setChecked(stoi(data[i][2]) == 100);

		table->setAlternatingRowColors(true);
		table->setCellWidget(i, 0, qcb);
		

		QTableWidgetItem *qdd = new QTableWidgetItem(QString::fromStdString(data[i][0]));
		qdd->setTextAlignment(Qt::AlignCenter);
		table->setItem(i, 1, qdd);
		table->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(data[i][1])));

		QTableWidgetItem *qtwi = new QTableWidgetItem(QString::fromStdString(data[i][2]));
		qtwi->setTextAlignment(Qt::AlignCenter);
		table->setItem(i, 3, qtwi);
		table->setItem(i, 4, new QTableWidgetItem(QString::fromStdString(data[i][3])));
	}
}

void MainWindow::filter()
{
	std::vector<std::string*> data = IOManager::readFile(path);
	bool isCompleted = completed->isChecked();

	time_t now = time(0);
	tm cur = *gmtime(&now);

	for (int i = 0; i < data.size(); i++) {
		int year = stoi(IOManager::split(data[i][0], '/')[0]);
		int month = stoi(IOManager::split(data[i][0], '/')[1]);
		int day = stoi(IOManager::split(data[i][0], '/')[2]);

		if (isCompleted) {
			if (stoi(data[i][2]) == 100) {
				data.erase(data.begin() + i);
				i--;
				continue;
			}
		}

		if (today->isChecked()) {
			if (cur.tm_year + 1900 != year || cur.tm_mon + 1 != month || cur.tm_mday != day) {
				data.erase(data.begin() + i);
				i--;
			}
		}
		else if (thisweek->isChecked()) {
			time_t auxd = time(0);
			tm *auxdate = gmtime(&auxd);
			auxdate->tm_mday = day;
			auxdate->tm_mon = month - 1;
			auxdate->tm_year = year - 1900;
			auxd = mktime(auxdate);
			auxdate = gmtime(&auxd);
			tm cp = *auxdate;

			if (getWeekNumber(cur) != getWeekNumber(cp) || cur.tm_year != cp.tm_year) {
				data.erase(data.begin() + i);
				i--;
			}
		}
		else if (overdue->isChecked()) {
			if (cur.tm_year + 1900 < year || (cur.tm_year + 1900 == year && cur.tm_mon + 1 < month) || (cur.tm_year + 1900 == year && cur.tm_mon + 1 == month && cur.tm_mday <= day)) {
				data.erase(data.begin() + i);
				i--;
			}
		}
	}

	updateTable(data);
}

int MainWindow::getWeekNumber(tm t)
{
	time_t nnow = time(0);
	tm *mauxdate = gmtime(&nnow);
	mauxdate->tm_mday = 0;
	mauxdate->tm_mon = 0;
	mauxdate->tm_year = mauxdate->tm_year;
	nnow = mktime(mauxdate);
	mauxdate = gmtime(&nnow);

	int julian = t.tm_yday;  
	int dow = t.tm_wday;     
	int dowJan1 = mauxdate->tm_wday;  
	int weekNum = ((julian + 6) / 7); 
	if (dow < dowJan1) {               
		++weekNum;
	}
	return weekNum;
}

void MainWindow::init()
{
	setPath("./db.txt");

	setMenuBar(menuBar);

	QWidget *w = new QWidget;
	setCentralWidget(w);

	createMenus();

	QHBoxLayout *filters = new QHBoxLayout;
	createFilters(filters);

	table = new QTableWidget(this);
	initializeTable();

	QVBoxLayout *layout = new QVBoxLayout;
	layout->setMargin(8);
	layout->addLayout(filters);
	layout->addWidget(table);
	layout->setStretch(0, 0);
	layout->setStretch(1, 0);
	layout->setStretch(2, 1);

	w->setLayout(layout);

	setWindowTitle("CAM To-Do List");
	setMinimumSize(480, 320);
	resize(580, 480);
	setWindowIcon(QIcon("icon.png"));
	QObject::connect(table, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(onCellSelected(int, int)));
}