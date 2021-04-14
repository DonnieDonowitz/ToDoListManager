#pragma once
#include <qdialog.h>
#include <string>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include "IOManager.h"
#include "MainWindow.h"

class CreateDataFileDialog :
	public QDialog
{
	Q_OBJECT

public:
	explicit CreateDataFileDialog(QWidget* parent = nullptr);
	inline void setOrigin(MainWindow *o) { origin = o; }
	~CreateDataFileDialog();

private slots:
	void onNameTextChanged(const QString &arg);
	void onCancelButtonClicked();
	void onFolderTextChanged(const QString &arg);
	void onCreateButtonClicked();
	void onSearchButtonClicked();

private:
	void checkFields();
	void updateOutputLabel();
	MainWindow* origin;
	QPushButton *create, *search, *cancel;
	QLineEdit *name, *folder;
	QLabel *folderLabel, *nameLabel, *outputLabel;
	QGridLayout* layout;
	void init();
};

