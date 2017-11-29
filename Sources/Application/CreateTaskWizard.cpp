#include "CreateTaskWizard.h"

#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QRadioButton>
#include <QPlainTextEdit>
#include <QDateTimeEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QFileDialog>

#include "Scheduler.h"

static void AddPages(CreateTaskWizard* wizard);

const QStringList CreateTaskWizard::kInternalChoices = {
    "day", "week", "month", "once", "startingUp", "login"
};

CreateTaskWizard::CreateTaskWizard(TaskManager* taskManager, Scheduler* scheduler)
    : QWizard(scheduler),
      taskManager(taskManager),
      scheduler(scheduler)
{
    setWindowTitle("Creating Task Wizard");
    resize(800, 600);
    ::AddPages(this);
}

void CreateTaskWizard::accept() {
    QString name = field("name").toString();
    QString brief = field("brief").toString();
    QDateTime startTime = field("startTime").toDateTime();
    int everyN = field("everyN").toInt();
    Trigger::Internal internal = static_cast<Trigger::Internal>(everyN);
    QString opertion = field("task").toString();
    QStringList arguments = QStringList() << field("arguments").toString();

    Task* task = taskManager->create(name, brief, startTime, internal, opertion, arguments);
    scheduler->insertRow(task);
    QWizard::accept();
}


class BasicPage : public QWizardPage {
public:
    BasicPage()
        : QWizardPage()
    {
        setTitle("Basic");
        QGridLayout *l = new QGridLayout();
        setLayout(l);

        QLineEdit* nameEdit = new QLineEdit();
        QPlainTextEdit* briefEdit = new QPlainTextEdit();

        l->addWidget(new QLabel("Name:"), 0, 0);
        l->addWidget(nameEdit, 0, 1);
        l->addWidget(new QLabel("Brief:"), 1, 0);
        l->addWidget(briefEdit, 1, 1);

        registerField("name*", nameEdit);
        registerField("brief", briefEdit);
    }
};


class SelectingTriggerPage : public QWizardPage {
public:
    SelectingTriggerPage()
        : QWizardPage()
    {
        setTitle("Trigger");

        QRadioButton* dailyBtn = new QRadioButton("Daily");
        QRadioButton* weeklyBtn = new QRadioButton("Weekly");
        QRadioButton* monthlyBtn = new QRadioButton("Monthly");
        QRadioButton* onceBtn = new QRadioButton("Once");
        QRadioButton* startingUpBtn = new QRadioButton("Starting Up");
        QRadioButton* loginBtn = new QRadioButton("Login");
        dailyBtn->click();

        QGridLayout* layout = new QGridLayout();
        setLayout(layout);
        layout->addWidget(dailyBtn);
        layout->addWidget(weeklyBtn);
        layout->addWidget(monthlyBtn);
        layout->addWidget(onceBtn);
        layout->addWidget(startingUpBtn);
        layout->addWidget(loginBtn);

        registerField("day", dailyBtn);
        registerField("week", weeklyBtn);
        registerField("month", monthlyBtn);
        registerField("once", onceBtn);
        registerField("startingUp", startingUpBtn);
        registerField("login", loginBtn);
    }
};


class TriggerPage : public QWizardPage {
public:
    TriggerPage()
        : QWizardPage()
    {
        setTitle("Daily");
        QLabel* startLabel = new QLabel("Start At:");
        QDateTimeEdit* startEdit = new QDateTimeEdit(QDateTime::currentDateTime());
        QLabel* everyLabel = new QLabel("Every:");
        QSpinBox* everyNEdit = new QSpinBox();
        everyNEdit->setValue(1);
        QLabel* everyN = new QLabel("Days");

        QGridLayout* layout = new QGridLayout();
        setLayout(layout);
        layout->addWidget(startLabel, 0, 0);
        layout->addWidget(startEdit, 0, 1);
        layout->addWidget(everyLabel, 1, 0);
        layout->addWidget(everyNEdit, 1, 1);
        layout->addWidget(everyN, 1, 2);

        registerField("startTime", startEdit);
        registerField("everyN", everyNEdit);
    }
};


class OperationPage : public QWizardPage {
public:
    OperationPage()
        : QWizardPage()
    {
        setTitle("Operation");
        QLabel* taskLabel = new QLabel("Program or Script:");
        QLineEdit* taskEdit = new QLineEdit();

        QPushButton* taskBrowser = new QPushButton("Browser");
        QFileDialog* browerDialog = new QFileDialog(this);
        connect(taskBrowser, &QPushButton::clicked,
                browerDialog, &QFileDialog::show);
        connect(browerDialog, &QFileDialog::fileSelected,
                taskEdit, &QLineEdit::setText);

        QLabel* addArgLabel = new QLabel("Add Arguments(Optional):");
        QLineEdit* addArgEdit = new QLineEdit();

        QGridLayout* layout = new QGridLayout();
        setLayout(layout);
        layout->addWidget(taskLabel, 0, 0);
        layout->addWidget(taskEdit, 1, 0);
        layout->addWidget(taskBrowser, 1, 1);
        layout->addWidget(addArgLabel, 2, 0);
        layout->addWidget(addArgEdit, 2, 1);

        registerField("task*", taskEdit);
        registerField("arguments", addArgEdit);
    }
};


class FinishingPage : public QWizardPage {
public:
    FinishingPage()
        : QWizardPage()
    {
        setTitle("Finishing");

        QLabel* nameLabel = new QLabel("Name:");
        this->nameEdit = new QLineEdit();
        nameEdit->setDisabled(true);

        QLabel* briefLabel = new QLabel("Brief:");
        this->briefEdit = new QTextEdit();
        briefEdit->setDisabled(true);

        QLabel* triggerLabel = new QLabel("Trigger:");
        this->triggerEdit = new QLineEdit();
        triggerEdit->setDisabled(true);

        QLabel* operationLabel = new QLabel("Operation:");
        this->operationEdit = new QLineEdit();
        operationEdit->setDisabled(true);

        QGridLayout* layout = new QGridLayout();
        setLayout(layout);
        layout->addWidget(nameLabel, 0, 0);
        layout->addWidget(nameEdit, 0, 1);
        layout->addWidget(briefLabel, 1, 0);
        layout->addWidget(briefEdit, 1, 1);
        layout->addWidget(triggerLabel, 2, 0);
        layout->addWidget(triggerEdit, 2, 1);
        layout->addWidget(operationLabel, 3, 0);
        layout->addWidget(operationEdit, 3, 1);
    }

    void showEvent(QShowEvent* e) override {
        QString name = field("name").toString();
        QString brief = field("brief").toString();
        QDateTime startTime = field("startTime").toDateTime();
        int everyN = field("everyN").toInt();
        QString operation = field("task").toString();
        QString arguments = field("arguments").toString();

        nameEdit->setText(name);
        briefEdit->setText(brief);
        const QStringList& choices = CreateTaskWizard::kInternalChoices;
        for (const QString& c : choices) {
            if (field(c).toBool()) {
                QString triggerText = QString("Every %1 %2(s); Start at %3").arg(everyN).arg(c, startTime.toString());
                triggerEdit->setText(triggerText);
                break;
            }
        }
        operationEdit->setText(operation + ';' + arguments);
        QWizardPage::showEvent(e);
    }

private:
    QLineEdit* nameEdit;
    QTextEdit* briefEdit;
    QLineEdit* triggerEdit;
    QLineEdit* operationEdit;
};


static inline void AddPages(CreateTaskWizard* wizard) {
    wizard->addPage(new BasicPage());
    wizard->addPage(new SelectingTriggerPage());
    wizard->addPage(new TriggerPage());
    wizard->addPage(new OperationPage());
    wizard->addPage(new FinishingPage());
}
