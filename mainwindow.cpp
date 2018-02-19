#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    server = new QTcpServer(this);
    client = new QTcpSocket();
    timer = new QTimer;
    timer->setInterval(200);
    connect(timer, SIGNAL(timeout()),this, SLOT(checknetwork()));
    connect(timer, SIGNAL(timeout()), this, SLOT(testresetting()));
    timer->start();
    on_connect_reconnect_clicked();
}

MainWindow::~MainWindow()
{
    delete ui;
}
//////////////////////////////////////////////////////
//network connection
void MainWindow::newConnection(){
    if(!isconnected){
        qDebug() << "new connection";
        isconnected = true;
        refresh_log();

        client = server->nextPendingConnection();
        QString ip = client->peerAddress().toString();
        qDebug() << ip;
        ip = ip.remove("::ffff:");
        ui->ipadressoutput->setText(ip);
        ui->sendipadress->setText(ip);
        int clientport = client->peerPort();
        ui->portinput->setValue(clientport);
        ui->processingoutput->setText("connecting with client");
        uicounter = 0;
        connect(client, SIGNAL(readyRead()),this, SLOT(readyRead()));
        connect(client, SIGNAL(disconnected()),this,SLOT(ClientDisconnected()));
    }
}
void MainWindow::readyRead(){
    ui->processingoutput->setText("receiving from client");
    uicounter = 0;
    QByteArray byte = client->readAll();
    qDebug() << byte;
    addlog(byte);
}
void MainWindow::ClientDisconnected()
{
    ui->processingoutput->setText("client disconnected");
    uicounter = 0;
    isconnected = false;
    ui->ipadressoutput->setText("0.0.0.0");
    ui->sendipadress->setText("0.0.0.0");
    ui->portinput->setValue(0);
    disconnect(client, SIGNAL(readyRead()),this, SLOT(readyRead()));
    disconnect(client, SIGNAL(disconnected()),this,SLOT(ClientDisconnected()));

}
//////////////////////////////////////////////////////
// button function
void MainWindow::on_connect_reconnect_clicked(){
    port = ui->serverport->value();
    if(serverstart){
        refresh_log();
        serverstart = false;
        client->disconnectFromHost();
        isconnected = false;
        server->close();
        disconnect(server,SIGNAL(newConnection()), this, SLOT(newConnection()));
        ui->processingoutput->setText("server restarting");
        uicounter = 0;
    }
    else{
        ui->processingoutput->setText("server starting");
        uicounter = 0;
    }
    if(!server->listen(QHostAddress::Any,port)){
        ui->processingoutput->setText("server start fail");
        uicounter = 0;
        serverstart = false;
        server->close();
    }
    else{
       serverstart = true;
       connect(server,SIGNAL(newConnection()), this, SLOT(newConnection()));
    }
}
void MainWindow::on_send_clicked(){
    if(isconnected){
        QString temp;
        temp = ui->sendtext->text();
        temp = temp + "\n";
        client->write( temp.toUtf8() );
    }
}
void MainWindow::on_deletelog_clicked(){
    refresh_log();
}
void MainWindow::on_exit_clicked(){
     exit(1);
}
//////////////////////////////////////////////////////
// system function
void MainWindow::testresetting(){
    uicounter++;
    if(uicounter == 15){
        uicounter = 0;
        ui->processingoutput->setText("resting");
    }
}
void MainWindow::refresh_log(){
    displaylog = "";
    for(int i = 0; i < 9; i++){
        log[i] = "";
    }
    ui->logtextbrower->setText(displaylog);
    logindex = 0;
    ui->processingoutput->setText("log refreshing");
    uicounter = 0;
}
void MainWindow::checknetwork(){
    if(serverstart){
        ui->serverstatueoutput->setText("on");
    }
    else{
        ui->serverstatueoutput->setText("off");
    }
    if(isconnected){
        ui->connectionoutput->setText("connected");
    }
    else{
        ui->connectionoutput->setText("disconnected");
    }
}
void MainWindow::addlog(QString str){
    QString temp = str;
    temp = temp.remove("\n");
    temp = temp.remove("\r");
    if(logindex == 9){
        for(int i = 0; i < 8; i++){
            log[i] = log[i+1];
        }
        log[8] = temp;
    }
    else{
        log[logindex] = temp;
        logindex++;
    }
    displaylog = log[0];
    for(int i = 0; i < logindex-1; i++){
        displaylog = displaylog + "\n" + log[i+1];
    }
    ui->logtextbrower->setText(displaylog);
}
