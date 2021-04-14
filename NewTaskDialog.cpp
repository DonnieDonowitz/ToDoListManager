#include "NewTaskDialog.h"

NewTaskDialog::NewTaskDialog(QWidget* parent) :
	QDialog(parent),
	cancel(new QPushButton("Cancel")),
	delete_(new QPushButton("Delete Task")),
	save(new QPushButton("Add Task")),
	dueDate(new QDateEdit),
	description(new QPlainTextEdit),
	percent(new QSlider),
	dueDateLabel(new QLabel("Due Date")),
	titleLabel(new QLabel("Task Name")),
	percentLabel(new QLabel("0% Done")),
	descriptionLabel(new QLabel("Description")),
	title(new QLineEdit),
	layout(new QGridLayout)
{
	init();
}

NewTaskDialog::~NewTaskDialog()
{
	delete dueDate;
	delete dueDateLabel;
	delete title;
	delete titleLabel;
	delete percent;
	delete percentLabel;
	delete description;
	delete descriptionLabel;
	delete delete_;
	delete save;
	delete cancel;
	delete layout;
}

void NewTaskDialog::setData(std::string dueDateString, std::string titleString, std::string percentString, std::string descriptionString)
{
	oldDueDate = dueDateString;
	oldTitle = titleString;
	oldPercent = percentString;
	oldDescription = descriptionString;

	std::string *date = IOManager::split(dueDateString, '/');
	QDate *d = new QDate(stoi(date[0]), stoi(date[1]), stoi(date[2]));
	dueDate->setDate(*d);
	title->setText(QString::fromStdString(titleString));
	percent->setValue(stoi(percentString));
	description->setPlainText(QString::fromStdString(descriptionString));
	newTask = false;
	save->setText("Modify");
	delete_->setVisible(true);
}

void NewTaskDialog::onPercentValueChanged(int value)
{
	percentLabel->setText(QString::fromStdString(std::to_string(value) + "% Done"));
	checkFields();
}

void NewTaskDialog::checkFields()
{
	std::string date(dueDate->text().toUtf8().constData());
	std::string titleString(title->text().toUtf8().constData());
	std::string percentage(percentLabel->text().toUtf8().constData());
	percentage = percentage.substr(0, percentage.find('%'));
	std::string descriptionString(description->toPlainText().toUtf8().constData());

	if (date.length() != 10 || titleString.length() == 0 || percentage.length() == 0 || descriptionString.length() == 0)
	{
		save->setEnabled(false);
	}
	else
	{
		save->setEnabled(true);
	}
}

void NewTaskDialog::onDescriptionTextChanged()
{
	checkFields();
}

void NewTaskDialog::onTitleTextChanged(const QString &arg)
{
	checkFields();
}

void NewTaskDialog::onDatePickChanged(const QDate &arg)
{
	checkFields();
}

void NewTaskDialog::onSaveButtonClicked()
{
	std::vector<std::string*> lines = IOManager::readFile(path);

	std::string date(dueDate->text().toUtf8().constData());
	std::string titleString(title->text().toUtf8().constData());
	std::string percentage(percentLabel->text().toUtf8().constData());
	percentage = percentage.substr(0, percentage.find('%'));
	std::string descriptionString(description->toPlainText().toUtf8().constData());
	std::string s = "";

	if (newTask)
	{
		for (int i = 0; i < lines.size(); i++)
		{
			s += lines[i][0] + ";" + lines[i][1] + ";" + lines[i][2] + ";" + lines[i][3] + "\n";
		}
		s += date + ";" + titleString + ";" + percentage + ";" + descriptionString;
	}
	else
	{
		for (int i = 0; i < lines.size(); i++)
		{
			if (lines[i][0].compare(oldDueDate) == 0 && lines[i][1].compare(oldTitle) == 0 && lines[i][2].compare(oldPercent) == 0 && lines[i][3].compare(oldDescription) == 0)
			{
				s += date + ";" + titleString + ";" + percentage + ";" + descriptionString + "\n";
			}
			else
			{
				s += lines[i][0] + ";" + lines[i][1] + ";" + lines[i][2] + ";" + lines[i][3] + "\n";
			}
		}
	}

	IOManager::writeFile(path, s);
	lines.clear();
	origin->filter();
	close();
}

void NewTaskDialog::onCancelButtonClicked()
{
	close();
}

void NewTaskDialog::onDeleteButtonClicked()
{
	std::vector<std::string*> lines = IOManager::readFile(path);

	QMessageBox::StandardButton reply;
	reply = QMessageBox::question(this, "Delete Task", "Are you sure you want to delete this task?", QMessageBox::Yes | QMessageBox::No);
	if (reply == QMessageBox::No)
	{
		return;
	}
	std::string s = "";
	
	for (int i = 0; i < lines.size(); i++)
	{
		if (lines[i][0].compare(oldDueDate) != 0 || lines[i][1].compare(oldTitle) != 0 || lines[i][2].compare(oldPercent) != 0 || lines[i][3].compare(oldDescription) != 0)
		{
			s += lines[i][0] + ";" + lines[i][1] + ";" + lines[i][2] + ";" + lines[i][3] + "\n";
		}
	}

	IOManager::writeFile(path, s);
	lines.clear();
	origin->filter();
	close();
}

void NewTaskDialog::init()
{
	percent->setOrientation(Qt::Horizontal);
	layout->addWidget(dueDateLabel, 0, 0, Qt::AlignLeft);
	layout->addWidget(dueDate, 0, 1, 1, 4);
	layout->addWidget(titleLabel, 1, 0, Qt::AlignLeft);
	layout->addWidget(title, 1, 1, 1, 4);
	layout->addWidget(percentLabel, 2, 0, Qt::AlignLeft);
	layout->addWidget(percent, 2, 1, 1, 4);
	layout->addWidget(descriptionLabel, 3, 0, Qt::AlignLeft);
	layout->addWidget(description, 3, 1, 10, 4);
	layout->addWidget(delete_, 14, 0);
	layout->addWidget(cancel, 14, 3);
	layout->addWidget(save, 14, 4);

	setLayout(layout);

	setWindowTitle("Add Task");
	setWindowIcon(QIcon("task.png"));

	dueDate->setDisplayFormat("dd/MM/yyyy");
	QDate date = QDate::currentDate();
	dueDate->setDate(date);

	percent->setRange(0, 100);
	percent->setSingleStep(5);

	save->setEnabled(false);
	delete_->setEnabled(false);
	newTask = true;

	QObject::connect(dueDate, &QDateEdit::dateChanged, this, &NewTaskDialog::onDatePickChanged);
	QObject::connect(title, &QLineEdit::textChanged, this, &NewTaskDialog::onTitleTextChanged);
	QObject::connect(percent, &QSlider::valueChanged, this, &NewTaskDialog::onPercentValueChanged);
	QObject::connect(description, &QPlainTextEdit::textChanged, this, &NewTaskDialog::onDescriptionTextChanged);
	QObject::connect(delete_, &QPushButton::clicked, this, &NewTaskDialog::onDeleteButtonClicked);
	QObject::connect(save, &QPushButton::clicked, this, &NewTaskDialog::onSaveButtonClicked);
	QObject::connect(cancel, &QPushButton::clicked, this, &NewTaskDialog::onCancelButtonClicked);
}