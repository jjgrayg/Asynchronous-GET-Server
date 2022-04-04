#ifndef IOSERVICETHREAD_H
#define IOSERVICETHREAD_H

#include <QThread>
#include <QTextEdit>
#include <QMutex>
#include <boost/asio.hpp>
#include <string>

class IOServiceThread : public QThread {
private:
    Q_OBJECT
    boost::asio::io_service *io_service_;

public:
    IOServiceThread(boost::asio::io_service* service) : QThread(), io_service_(service) {}
    void run() override;
};
#endif // IOSERVICETHREAD_H
