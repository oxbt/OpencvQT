#ifndef SHOWHISTOGTAMRESULT_H
#define SHOWHISTOGTAMRESULT_H

#include "math.h"
#include <QOpenGLWidget>
#include "histogtamdlg.h"

#include "opencv2/opencv.hpp"
#include <opencv2/core/types_c.h>

namespace Ui {
class ShowHistogtamResult;
}

class ShowHistogtamResult : public QOpenGLWidget
{
    Q_OBJECT

public:
    explicit ShowHistogtamResult(QWidget *parent = 0);
    ~ShowHistogtamResult();

    void setPicture(cv::Mat);
    void showHistogtam_Mat(cv::Mat);
    void reFresh();
    cv::Mat source_gray_Mat;
    cv::Mat resultMat;


private:
    Ui::ShowHistogtamResult *ui;
    void paintEvent(QPaintEvent *);


public slots:
    void receiveHistogtam_Mat(cv::Mat);

};
#endif // SHOWHISTOGTAMRESULT_H
