#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QTime>
#include <QFileDialog>
#include <QMovie>
#include <QVector>
#include <QPixmap>
#include <QBuffer>


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

struct Message {
    QTime time;
    QString text;
    QByteArray imageData;
};
Q_DECLARE_METATYPE(Message)

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_2_clicked();
    void slotReadyRead();

    void on_pushButton_clicked();

    void on_pushButton_pressed();

private:
    QVector<QPixmap> images;
    Ui::MainWindow *ui;
    QTcpSocket *socket;
    QByteArray Data;
    quint16 nextBlockSize;

    void sendToServer(QString str);
};
#endif // MAINWINDOW_H
