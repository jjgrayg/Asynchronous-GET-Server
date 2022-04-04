#ifndef SERVERCONTROLLER_H
#define SERVERCONTROLLER_H

#include <QMainWindow>
#include "ui_servercontroller.h"
#include "server.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ServerController; }
QT_END_NAMESPACE

class ServerController : public QMainWindow {
    Q_OBJECT

public:
    ServerController(QWidget *parent = nullptr);
    ~ServerController();

private:

    unsigned int get_port_num();
    bool get_debugging() { return debugging_input_->isChecked(); }
    bool get_logging() { return logging_input_->isChecked(); }

    Ui::ServerController *ui;
    Server *srv_;
    QDebug *debug_;

    QPushButton *server_control_, *open_readme_;
    QCheckBox *debugging_input_, *logging_input_;
    QLineEdit *port_num_input_;
    QTextEdit *output_console_;

private slots:

    void start_server();
    void stop_server();
    void open_readme();
    void write_to_console(QString to_insert);
};
#endif // SERVERCONTROLLER_H
