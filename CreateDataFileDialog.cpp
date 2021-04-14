#include "CreateDataFileDialog.h"

CreateDataFileDialog::CreateDataFileDialog(QWidget *parent) :
	QDialog(parent),
	name(new QLineEdit),
	folder(new QLineEdit),
	create(new QPushButton("Create")),
	search(new QPushButton("Search")),
	cancel(new QPushButton("Cancel")),
	nameLabel(new QLabel("File Name")),
	folderLabel(new QLabel("Folder")),
	outputLabel(new QLabel("")),
	layout(new QGridLayout)
{
	init();
}

CreateDataFileDialog::~CreateDataFileDialog()
{
	delete create;
	delete search;
	delete cancel;
	delete name;
	delete folder;
	delete nameLabel;
	delete folderLabel;
	delete outputLabel;
	delete layout;
}

void CreateDataFileDialog::onCancelButtonClicked()
{
	close();
}

void CreateDataFileDialog::onNameTextChanged(const QString &arg1)
{
	updateOutputLabel();
	checkFields();
}

void CreateDataFileDialog::onFolderTextChanged(const QString &arg1)
{
	updateOutputLabel();
	checkFields();
}

void CreateDataFileDialog::updateOutputLabel() {
	std::string folderName(folder->text().toUtf8().constData());
	std::string titleName(name->text().toUtf8().constData());
	QString output = QString::fromStdString(folderName + QString(QDir::separator()).toUtf8().constData() + titleName + ".txt");
	outputLabel->setText(output);
}

void CreateDataFileDialog::checkFields() {
	std::string folderName(folder->text().toUtf8().constData());
	std::string titleName(name->text().toUtf8().constData());

	if (folderName.length() > 0 && titleName.length() > 0) {
		create->setEnabled(true);
	}
	else {
		create->setEnabled(false);
	}
}

void CreateDataFileDialog::onCreateButtonClicked()
{
	std::string folderName(folder->text().toUtf8().constData());
	std::string titleName(name->text().toUtf8().constData());
	origin->setPath(folderName + QString(QDir::separator()).toUtf8().constData() + titleName + ".txt");
	origin->filter();
	close();
}

void CreateDataFileDialog::onSearchButtonClicked()
{
	QString dir = QFileDialog::getExistingDirectory(this, tr("Select Directory"), NULL, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	folder->setText(dir);
}

void CreateDataFileDialog::init()
{
	layout->addWidget(folderLabel, 0, 0, 1, 4, Qt::AlignLeft);
	layout->addWidget(folder, 1, 0, 1, 3, Qt::AlignLeft);
	layout->addWidget(search, 1, 3, 1, 1);
	layout->addWidget(nameLabel, 2, 0, 1, 2, Qt::AlignLeft);
	layout->addWidget(name, 3, 0, 1, 2);
	layout->addWidget(outputLabel, 3, 2, 1, 2);
	layout->addWidget(cancel, 4, 2, 1, 1);
	layout->addWidget(create, 4, 3, 1, 1);


	QObject::connect(name, &QLineEdit::textChanged, this, &CreateDataFileDialog::onNameTextChanged);
	QObject::connect(folder, &QLineEdit::textChanged, this, &CreateDataFileDialog::onFolderTextChanged);
	QObject::connect(create, &QPushButton::clicked, this, &CreateDataFileDialog::onCreateButtonClicked);
	QObject::connect(search, &QPushButton::clicked, this, &CreateDataFileDialog::onSearchButtonClicked);
	QObject::connect(cancel, &QPushButton::clicked, this, &CreateDataFileDialog::onCancelButtonClicked);

	setWindowTitle("Data File Dialog");
	setWindowIcon(QIcon("data.png"));


	setLayout(layout);
}