#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>
#include <QString>
#include <QTimer>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    //////////////////////////////////
    // component
    QTcpServer *server;
    QTcpSocket *client;
    QTimer *timer;
    // global variable
    bool serverstart = false;
    bool isconnected = false;
    QString log[9];
    QString displaylog;
    int logindex = 0;
    // variable in ui
    QString IP = "127.0.0.1";
    int ip1 = 127;
    int ip2 = 0;
    int ip3 = 0;
    int ip4 = 1;
    int port = 1667;
    int uicounter = 0;
    // system function
    void logcombine();
    //////////////////////////////////
    ~MainWindow();
public slots:
    // network slots
    void newConnection();
    void readyRead();
    void ClientDisconnected();
    // button slots
    void on_connect_reconnect_clicked();
    void on_send_clicked();
    void on_deletelog_clicked();
    void on_exit_clicked();
    // system slots
    void testresetting();
    void refresh_log();
    void checknetwork();
    void addlog(QString str);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
