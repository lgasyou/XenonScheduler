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
#include <tuple>

#include "Scheduler.h"

static void AddPagesHelper(CreateTaskWizard* wizard);
static std::tuple<QDateTime, Trigger::IntervalType, int> GetExecuteTimeHelper(CreateTaskWizard* wizard);

CreateTaskWizard::CreateTaskWizard(TaskManager* taskManager, Scheduler* scheduler)
    : QWizard(scheduler),
      taskManager(taskManager),
      scheduler(scheduler)
{
    setWindowTitle("Creating Task");
    resize(800, 600);
    ::AddPagesHelper(this);
}

void CreateTaskWizard::accept() {
    QString name = field("name").toString();
    QString brief = field("brief").toString();
    QString opertion = field("task").toString();
    QStringList arguments = QStringList() << field("arguments").toString();

    std::tuple<QDateTime, Trigger::IntervalType, int> tuple = ::GetExecuteTimeHelper(this);
    QDateTime startTime = std::get<0>(tuple);
    Trigger::IntervalType intervalType = std::get<1>(tuple);
    int interval = static_cast<Trigger::IntervalType>(std::get<2>(tuple));

    Task* task = TaskManager::create(name, brief, startTime, intervalType, interval, opertion, arguments);
    scheduler->insertRow(task);
    restart(); // In order to start at the beginning next time.
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
        dailyBtn = new QRadioButton("Daily");
        weeklyBtn = new QRadioButton("Weekly");
        monthlyBtn = new QRadioButton("Monthly");
        onceBtn = new QRadioButton("Once");
        startingUpBtn = new QRadioButton("Starting Up");
        loginBtn = new QRadioButton("Login");
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

private:
    int nextId() const override {
        if (dailyBtn->isChecked()) {
            return CreateTaskWizard::DailyTriggerPage;
        }
        if (weeklyBtn->isChecked()) {
            return CreateTaskWizard::WeeklyTriggerPage;
        }
        if (monthlyBtn->isChecked()) {
            return CreateTaskWizard::MonthlyTriggerPage;
        }
        if (onceBtn->isChecked()) {
            return CreateTaskWizard::OnceTriggerPage;
        }
        if (startingUpBtn->isChecked()) {
            return CreateTaskWizard::StartingUpTriggerPage;
        }
        if (loginBtn->isChecked()) {
            return CreateTaskWizard::LoginTriggerPage;
        }
        throw std::exception("Unreachable");
    }

private:
    QRadioButton* dailyBtn;
    QRadioButton* weeklyBtn;
    QRadioButton* monthlyBtn;
    QRadioButton* onceBtn;
    QRadioButton* startingUpBtn;
    QRadioButton* loginBtn;

};


class DailyTriggerPage : public QWizardPage {
public:
    DailyTriggerPage()
        : QWizardPage()
    {
        setTitle("Daily");
        QLabel* startLabel = new QLabel("Start At:");
        QDateTimeEdit* startEdit = new QDateTimeEdit(QDateTime::currentDateTime());
        QLabel* everyLabel = new QLabel("Every:");
        QSpinBox* everyNEdit = new QSpinBox();
        everyNEdit->setValue(1);
        QLabel* everyN = new QLabel("Day(s)");

        QGridLayout* layout = new QGridLayout();
        setLayout(layout);
        layout->addWidget(startLabel, 0, 0);
        layout->addWidget(startEdit, 0, 1);
        layout->addWidget(everyLabel, 1, 0);
        layout->addWidget(everyNEdit, 1, 1);
        layout->addWidget(everyN, 1, 2);

        registerField("dailyStartTime", startEdit);
        registerField("everyNDays", everyNEdit);
    }

private:
    int nextId() const override {
        return CreateTaskWizard::OperationPage;
    }

};


class WeeklyTriggerPage : public QWizardPage {
public:
    WeeklyTriggerPage()
        : QWizardPage()
    {
        setTitle("Weekly");
        QLabel* startLabel = new QLabel("Start At:");
        QDateTimeEdit* startEdit = new QDateTimeEdit(QDateTime::currentDateTime());
        QLabel* everyLabel = new QLabel("Every:");
        QSpinBox* everyNEdit = new QSpinBox();
        everyNEdit->setValue(1);
        QLabel* everyN = new QLabel("Week(s)");

        QGridLayout* layout = new QGridLayout();
        setLayout(layout);
        layout->addWidget(startLabel, 0, 0);
        layout->addWidget(startEdit, 0, 1);
        layout->addWidget(everyLabel, 1, 0);
        layout->addWidget(everyNEdit, 1, 1);
        layout->addWidget(everyN, 1, 2);

        registerField("weeklyStartTime", startEdit);
        registerField("everyNWeeks", everyNEdit);
    }

private:
    int nextId() const override {
        return CreateTaskWizard::OperationPage;
    }

};


class MonthlyTriggerPage : public QWizardPage {
public:
    MonthlyTriggerPage()
        : QWizardPage()
    {
        setTitle("Monthly");
        QLabel* startLabel = new QLabel("Start At:");
        QDateTimeEdit* startEdit = new QDateTimeEdit(QDateTime::currentDateTime());
        QLabel* everyLabel = new QLabel("Every:");
        QSpinBox* everyNEdit = new QSpinBox();
        everyNEdit->setValue(1);
        QLabel* everyN = new QLabel("Month(s)");

        QGridLayout* layout = new QGridLayout();
        setLayout(layout);
        layout->addWidget(startLabel, 0, 0);
        layout->addWidget(startEdit, 0, 1);
        layout->addWidget(everyLabel, 1, 0);
        layout->addWidget(everyNEdit, 1, 1);
        layout->addWidget(everyN, 1, 2);

        registerField("monthlyStartTime", startEdit);
        registerField("everyNMonths", everyNEdit);
    }

private:
    int nextId() const override {
        return CreateTaskWizard::OperationPage;
    }

};


class OnceTriggerPage : public QWizardPage {
public:
    OnceTriggerPage()
        : QWizardPage()
    {
        setTitle("Once");
        QLabel* startLabel = new QLabel("Start At:");
        QDateTimeEdit* startEdit = new QDateTimeEdit(QDateTime::currentDateTime());

        QGridLayout* layout = new QGridLayout();
        setLayout(layout);
        layout->addWidget(startLabel, 0, 0);
        layout->addWidget(startEdit, 0, 1);

        registerField("onceStartTime", startEdit);
    }

private:
    int nextId() const override {
        return CreateTaskWizard::OperationPage;
    }

};


class StartingUpTriggerPage : public QWizardPage {
public:
    StartingUpTriggerPage()
        : QWizardPage()
    {
        setTitle("Starting Up");
//        QLabel* startLabel = new QLabel("Start At:");
//        QDateTimeEdit* startEdit = new QDateTimeEdit(QDateTime::currentDateTime());
//        QLabel* everyLabel = new QLabel("Every:");
//        QSpinBox* everyNEdit = new QSpinBox();
//        everyNEdit->setValue(1);
//        QLabel* everyN = new QLabel("Month(s)");

//        QGridLayout* layout = new QGridLayout();
//        setLayout(layout);
//        layout->addWidget(startLabel, 0, 0);
//        layout->addWidget(startEdit, 0, 1);
//        layout->addWidget(everyLabel, 1, 0);
//        layout->addWidget(everyNEdit, 1, 1);
//        layout->addWidget(everyN, 1, 2);

//        registerField("startTime", startEdit);
//        registerField("everyN", everyNEdit);
    }

private:
    int nextId() const override {
        return CreateTaskWizard::OperationPage;
    }

};


class LoginTriggerPage : public QWizardPage {
public:
    LoginTriggerPage()
        : QWizardPage()
    {
        setTitle("Login");
//        QLabel* startLabel = new QLabel("Start At:");
//        QDateTimeEdit* startEdit = new QDateTimeEdit(QDateTime::currentDateTime());
//        QLabel* everyLabel = new QLabel("Every:");
//        QSpinBox* everyNEdit = new QSpinBox();
//        everyNEdit->setValue(1);
//        QLabel* everyN = new QLabel("Month(s)");

//        QGridLayout* layout = new QGridLayout();
//        setLayout(layout);
//        layout->addWidget(startLabel, 0, 0);
//        layout->addWidget(startEdit, 0, 1);
//        layout->addWidget(everyLabel, 1, 0);
//        layout->addWidget(everyNEdit, 1, 1);
//        layout->addWidget(everyN, 1, 2);

//        registerField("startTime", startEdit);
//        registerField("everyN", everyNEdit);
    }

private:
    int nextId() const override {
        return CreateTaskWizard::OperationPage;
    }

};


class OperationPage : public QWizardPage {
public:
    OperationPage()
        : QWizardPage()
    {
        setTitle("Operation");
        QLabel* taskLabel = new QLabel("Program or Script:");
        QLineEdit* taskEdit = new QLineEdit("python");

        QPushButton* taskBrowser = new QPushButton("Browser");
        QFileDialog* browerDialog = new QFileDialog(this);
        connect(taskBrowser, &QPushButton::clicked,
                browerDialog, &QFileDialog::show);
        connect(browerDialog, &QFileDialog::fileSelected,
                taskEdit, &QLineEdit::setText);

        QLabel* addArgLabel = new QLabel("Add Arguments(Optional):");
        QLineEdit* addArgEdit = new QLineEdit("F:/Scripts/encoding_tester.py");

        QGridLayout* layout = new QGridLayout();
        setLayout(layout);
        layout->addWidget(taskLabel, 0, 0);
        layout->addWidget(taskEdit, 1, 0, 1, 2);
        layout->addWidget(taskBrowser, 1, 2, 1, 1);
        layout->addWidget(addArgLabel, 2, 0, 1, 1);
        layout->addWidget(addArgEdit, 2, 1, 1, 2);

        // TODO: recover to "task*" when release this program.
        registerField("task", taskEdit);
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
        QString operation = field("task").toString();
        QString arguments = field("arguments").toString();

        nameEdit->setText(name);
        briefEdit->setText(brief);
        // TODO: brief
//        const QStringList& choices = CreateTaskWizard::kIntervalChoices;
//        for (const QString& c : choices) {
//            if (field(c).toBool()) {
//                QString triggerText = QString("Every %1 %2(s); Start at %3").arg(everyN).arg(c, startTime.toString());
//                triggerEdit->setText(triggerText);
//                break;
//            }
//        }
        operationEdit->setText(operation + "; " + arguments);
        QWizardPage::showEvent(e);
    }

private:
    QLineEdit* nameEdit;
    QTextEdit* briefEdit;
    QLineEdit* triggerEdit;
    QLineEdit* operationEdit;
};


static inline void AddPagesHelper(CreateTaskWizard* wizard) {
    wizard->addPage(new BasicPage());
    wizard->addPage(new SelectingTriggerPage());
    wizard->addPage(new DailyTriggerPage());
    wizard->addPage(new WeeklyTriggerPage());
    wizard->addPage(new MonthlyTriggerPage());
    wizard->addPage(new OnceTriggerPage());
    wizard->addPage(new StartingUpTriggerPage());
    wizard->addPage(new LoginTriggerPage());
    wizard->addPage(new OperationPage());
    wizard->addPage(new FinishingPage());
}


static std::tuple<QDateTime, Trigger::IntervalType, int> GetExecuteTimeHelper(CreateTaskWizard* wizard) {
    if (wizard->field("day").toBool()) {
        QDateTime startTime = wizard->field("dailyStartTime").toDateTime();
        int interval = wizard->field("everyNDays").toInt();
        return { startTime, Trigger::Day, interval };
    }

    if (wizard->field("week").toBool()) {
        QDateTime startTime = wizard->field("weeklyStartTime").toDateTime();
        int interval = wizard->field("everyNWeeks").toInt();
        return { startTime, Trigger::Week, interval };
    }

    if (wizard->field("month").toBool()) {
        QDateTime startTime = wizard->field("monthlyStartTime").toDateTime();
        int interval = wizard->field("everyNMonths").toInt();
        return { startTime, Trigger::Month, interval };
    }

    if (wizard->field("once").toBool()) {
        QDateTime startTime = wizard->field("onceStartTime").toDateTime();
        return { startTime, Trigger::Once, -1 };
    }

    if (wizard->field("startingUp").toBool()) {
        return { QDateTime(), Trigger::StartingUp, -1 };
    }

    if (wizard->field("login").toBool()) {
        return { QDateTime(), Trigger::Login, -1 };
    }

    throw std::exception("Unreachable");
}
