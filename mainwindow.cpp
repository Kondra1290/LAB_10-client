#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    socket = new QTcpSocket(this);
    nextBlockSize = 0;

    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);
    connect(ui->pushButton_3, &QPushButton::clicked, this, [this](){
         QString file  = QFileDialog::getOpenFileName(this, tr("Open Directory"),
                                                      "/home",
                                                        tr("Bitmap Files (*.bmp)\n"
                                                        "JPEG (*.jpg *jpeg)\n"
                                                        "GIF (*.gif)\n"
                                                            "PNG (*.png)\n"));


         QLabel* imageLabel = new QLabel(this);
         QPixmap pixmap(file); // Replace "path_to_your_image.png" with the path to your image
         images.push_back(pixmap);
         imageLabel->setPixmap(pixmap.scaled(50, 50));
         ui->horizontalLayout->addWidget(imageLabel);
         //QAction *searchAction = new QAction(ui->lineEdit);
         //searchAction->setIcon(QIcon(file));
         //ui->lineEdit->addAction(searchAction,QLineEdit::TrailingPosition);
         //QUrl Uri ( QString ( "file://%1" ).arg ( file ) );

         //ui->textBrowser->setHtml( QString("<img src='%1'>").arg(file));
        // ui->textBrowser->document()->setBaseUrl(QUrl( QString ( "file://%1" ).arg ( file ) ));
        //QMovie *movie = new QMovie(":/images/other/images/16x16/loading.gif");
        //QLabel *processLabel = new QLabel(this);
        //processLabel->setMovie(movie);
        //movie->start();

        //ui->lineEdit_1->setText(dir.path());
    });
}

MainWindow::~MainWindow()
{
    //for(auto& item: ui->horizontalLayout)
    delete ui;
}

void MainWindow::on_pushButton_2_clicked()
{
    socket->connectToHost("127.0.0.1", 2323);
}

void MainWindow::sendToServer(QString str){
    //Message message;
    Data.clear();

    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_6);

    //out << qint16(0) << QTime::currentTime() << str;
    out << QTime::currentTime() << str;

    for(auto& image: images){
        QByteArray imageData;
        imageData.clear();
        QBuffer buffer(&imageData);
        buffer.open(QIODevice::WriteOnly);
        image.save(&buffer, "PNG");
        out << imageData;
    }

    //if(!images.isEmpty()){
        //image.save(&buffer, "PNG");

        //message.imageData = imagesData;

        //ui->horizontalLayout;
    //}

    //out.device()->seek(0);
    //out << quint16(Data.size() - sizeof(quint16));
    socket->write(Data);

    ui->lineEdit->clear();

    QLayoutItem* item;
    while ((item = ui->horizontalLayout->takeAt(0)) != nullptr) { // Remove and delete all items until the layout is empty
        QWidget* widget = item->widget();
        if (widget) {
            widget->deleteLater();
        }
        delete item;
    }
    //ui->horizontalLayout.cle
    images.clear();
    /*
    Data.clear();
    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_6);
    if(!images.isEmpty()){
        QByteArray byteArray;
        QDataStream dataStream(&byteArray, QIODevice::WriteOnly);
        for(auto& image: images)
            dataStream << image.toImage();
        images.clear();
    }

    out << qint16(0) << QTime::currentTime() << str;

    out.device()->seek(0);
    out << quint16(Data.size() - sizeof(quint16));
    socket->write(Data);
    ui->lineEdit->clear();
    */
}

void MainWindow::slotReadyRead()
{
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_6_6);
    QString str;
    QTime time;
    QList<QImage> im;

    in >> time >> str;

    while (!in.atEnd()) {
        QByteArray imageData;
        imageData.clear();
        in >> imageData;
        QImage image;
        image.loadFromData(imageData);
        im.append(image);
    }

    ui->textBrowser->append(time.toString() + ": " + str);
    //if(str.isEmpty()) ui->textBrowser->append(time.toString() + ": " str);

    //QString htmlContent = "<html><body>";
    //htmlContent += "<p>" + time.toString() + ": " + str + "</p>";

    if(!im.isEmpty()){
        QString imageData;
        for(auto& image: im){
            QByteArray imageDataBase64;
            imageDataBase64.clear();
            QBuffer buffer(&imageDataBase64);
            buffer.open(QIODevice::WriteOnly);
            image.save(&buffer, "PNG");
            imageData = QString::fromLatin1(imageDataBase64.toBase64().data());

            ui->textBrowser->append("<img src='data:image/png;base64," + imageData + "'>");
            //htmlContent += "<p><img src='data:image/png;base64," + imageData + "'></p>";

        }
    }

    //htmlContent += "</body></html>";
    //ui->textBrowser->setHtml(htmlContent);
/*
    if(in.status()==QDataStream::Ok)
    {
        while(true){
            if(nextBlockSize == 0){
                if(socket->bytesAvailable() < 2){
                    break;
                }
                in >> nextBlockSize;
            }
            if(socket->bytesAvailable() < nextBlockSize){
                break;
            }

            QString str;
            QTime time;
            QByteArray  byteimage;
            QImage image;
            in >> time >> str;

            //ui->textBrowser->loadResource();
            ui->textBrowser->append(time.toString() + ": " + str);
            image.loadFromData(byteimage);
            if(!image.isNull()){
                QByteArray imageDataBase64;
                QBuffer buffer(&imageDataBase64);
                buffer.open(QIODevice::WriteOnly);
                image.save(&buffer, "PNG");
                QString base64String = QString::fromLatin1(imageDataBase64.toBase64().data());
                ui->textBrowser->setHtml(QString("<img src='data:image/png;base64,%1'/>").arg(base64String ));
            }
            nextBlockSize = 0;
            //QLabel *lable;
            //ui->textBrowser->append(lable);
        }
    }
    else
    {
        qDebug() << "";
    }
*/
}


void MainWindow::on_pushButton_clicked()
{
    sendToServer(ui->lineEdit->text());
}
void MainWindow::on_pushButton_pressed()
{
    sendToServer(ui->lineEdit->text());
}

