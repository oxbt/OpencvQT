#ifndef MYIMAGELIST_H
#define MYIMAGELIST_H

#include <QWidget>
#include <math.h>
#include <QWheelEvent>
#include <QListWidget>
#include <QGraphicsView>    //视图类
#include <QGraphicsScene>   //场景类
#include <QGraphicsItem>    //图元类
#include "mypixitem.h"

#include "opencv2/opencv.hpp"

namespace Ui {
class MyImageList;
}

class MyImageList : public QWidget
{
    Q_OBJECT

public:
    explicit MyImageList(QWidget *parent = 0);
    ~MyImageList();

    void showListItem(cv::Mat dstImage,QString strName,QString controlName);
    void intoList(QString listName,QString listV,cv::Mat listMat,int listRow,int marks);
    void updataListShow(QString listNames,cv::Mat listMats,int listRow);
    void updataListShows(int marks);
    void showTableView(cv::Mat,QString,QString,int);

public slots:
    void receiveDataWM(cv::Mat,QString,QString,int,int);//接收mainwindow打开文件数据
    void receiveAction();

private slots:
    void on_listWidget_itemClicked(QListWidgetItem *item);

//    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);    //实验双击
//    void  on_listWidget_itemActivated(QListWidgetItem *item); //实验
signals:
     //图元窗口图片被选中后弹出视图窗口，图元窗口间Mat和变量名返回给主窗口，主窗口再讲变量发给视图窗口
    void  sendDataList(cv::Mat imageData,QString textData,QString controlName);//发送信号

private:
    Ui::MyImageList *ui;

    //声明IplImage指针    
    cv::Mat pImgs;

    QList<QString> listText;//存储图像名称
    QList<QString> listData;//存储图像变量名称
    QList<QPixmap> pixmapList;//存储图像

    QList<cv::Mat> matList;//存储mat图像

};

#endif // MYIMAGELIST_H
