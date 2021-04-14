#pragma once
#include <QDialog>
#include <string>
#include <vector>
#include <QMessageBox>
#include <QPlainTextEdit>
#include <QDateTimeEdit>
#include <QPushButton>
#include <QLabel>
#include <QGridLayout>
#include <QLineEdit>
#include "IOManager.h"
#include "MainWindow.h"

class NewTaskDialog : public QDialog
{
	Q_OBJECT

public:
	explicit NewTaskDialog(QWidget *parent = nullptr);
	void setData(std::string,std::string ,std::string ,std::string);
	inline void setPath(std::string p) { path = p; }
	inline void setOrigin(MainWindow *o) { origin = o; }
	inline void setDelete(bool x) { delete_->setEnabled(x); }
	std::string oldDueDate;
	std::string oldTitle;
	std::string oldPercent;
	std::string oldDescription;
	~NewTaskDialog();

private slots:
	void onPercentValueChanged(int value);
	void onDescriptionTextChanged();
	void onTitleTextChanged(const QString &arg);
	void onDatePickChanged(const QDate &arg);
	void onSaveButtonClicked();
	void onCancelButtonClicked();
	void onDeleteButtonClicked();

private:
	void checkFields();
	bool newTask;
	std::string path;
	MainWindow *origin;
	QPushButton *cancel, *delete_, *save;
	QLabel *dueDateLabel, *titleLabel, *percentLabel, *descriptionLabel;
	QPlainTextEdit *description;
	QSlider *percent;
	QDateEdit *dueDate;
	QLineEdit *title;
	QGridLayout *layout;
	void init();
};