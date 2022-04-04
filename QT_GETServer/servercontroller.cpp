#include "servercontroller.h"
#include "ui_servercontroller.h"

ServerController::ServerController(QWidget *parent) : QMainWindow(parent), ui(new Ui::ServerController) {
    ui->setupUi(this);
    output_console_ = findChild<QTextEdit*>("ConsoleOutputBroswer");
    output_console_->setReadOnly(true);
    write_to_console(QString::fromStdString("<span style='color:grey'>[" + get_current_date_and_time() + "]</span> ~ <span style='color:blue;'> MAIN:: </span> Initializing control panel..."));
    server_control_ = findChild<QPushButton*>("ServerControl");
    open_readme_ = findChild<QPushButton*>("OpenReadme");
    debugging_input_ = findChild<QCheckBox*>("DebuggingInput");
    logging_input_ = findChild<QCheckBox*>("LoggingInput");
    port_num_input_ = findChild<QLineEdit*>("PortNumInput");

    logging_input_->setCheckState(Qt::Checked);
    connect(server_control_, SIGNAL(clicked()), this, SLOT(start_server()));
    write_to_console(QString::fromStdString("<span style='color:grey'>[" + get_current_date_and_time() + "]</span> ~ <span style='color:blue;'> MAIN:: </span> Finished initializing"));
    write_to_console(QString::fromStdString("<span style='color:grey'>[" + get_current_date_and_time() + "]</span> ~ <span style='color:blue;'> MAIN:: </span> Welcome to the very basic GET server!"));
    write_to_console(QString::fromStdString("<span style='color:grey'>[" + get_current_date_and_time() + "]</span> ~ <span style='color:blue;'> MAIN:: </span> This server works on the back of two libraries"));
    write_to_console(QString::fromStdString("<span style='color:grey'>[" + get_current_date_and_time() + "]</span> ~ <span style='color:blue;'> MAIN:: </span> &emsp; - Boost, for the server and its ASIO libraries"));
    write_to_console(QString::fromStdString("<span style='color:grey'>[" + get_current_date_and_time() + "]</span> ~ <span style='color:blue;'> MAIN:: </span> &emsp; - QT, for the GUI and threading"));
    write_to_console(QString::fromStdString("<span style='color:grey'>[" + get_current_date_and_time() + "]</span> ~ <span style='color:blue;'> MAIN:: </span> Use it for anything that's not illegal"));
    write_to_console(QString::fromStdString("<span style='color:grey'>[" + get_current_date_and_time() + "]</span> ~ <span style='color:blue;'> MAIN:: </span> Find the source on Github at the link below:"));
    write_to_console(QString::fromStdString("<span style='color:grey'>[" + get_current_date_and_time() + "]</span> ~ <span style='color:blue;'> MAIN:: </span> https://github.com/jjgrayg/Boost-ASIO-Asynchronous-GET-Server<br>"));
}

ServerController::~ServerController() {
    delete ui;
}

void ServerController::start_server() {
    unsigned int port_num = get_port_num();
    bool debugging = get_debugging();
    bool logging = get_logging();

    qDebug() << "Starting server\n";
    server_control_->disconnect();
    connect(server_control_, SIGNAL(clicked()), this, SLOT(stop_server()));
    server_control_->setText("Stop Server");
    debugging_input_->setEnabled(false);
    logging_input_->setEnabled(false);
    port_num_input_->setEnabled(false);
    srv_ = new Server(output_console_, port_num, logging, debugging);
    connect(srv_, SIGNAL(ready_write_console(QString)), this, SLOT(write_to_console(QString)));
    srv_->start();
}

void ServerController::stop_server() {
    qDebug() << "Stopping server\n";
    srv_->stop();
    server_control_->disconnect();
    connect(server_control_, SIGNAL(clicked()), this, SLOT(start_server()));
    server_control_->setText("Start Server");
    debugging_input_->setEnabled(true);
    logging_input_->setEnabled(true);
    port_num_input_->setEnabled(true);
}

void ServerController::open_readme() {

}

unsigned int ServerController::get_port_num() {
    return std::stoul(port_num_input_->text().toStdString());
}

void ServerController::write_to_console(QString to_insert) {
    output_console_->append(to_insert);
}

