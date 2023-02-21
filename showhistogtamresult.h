#ifndef SHOWHISTOGTAMRESULT_H
#define SHOWHISTOGTAMRESULT_H

#include <QWidget>
#include "histogtamdlg.h"

#include "math.h"

#include "opencv2/opencv.hpp"
#include <opencv2/core/types_c.h>
using namespace cv;

namespace Ui {
class ShowHistogtamResult;
}

class ShowHistogtamResult : public QWidget
{
    Q_OBJECT

public:
    explicit ShowHistogtamResult(QWidget *parent = 0);
    ~ShowHistogtamResult();

    void setPicture(Mat);
    void showHistogtam_Mat(Mat);
    void reFresh();
    Mat source_gray_Mat;
    Mat resultMat;


private:
    Ui::ShowHistogtamResult *ui;
    void paintEvent(QPaintEvent *);


private slots:
    void receiveHistogtam_Mat(Mat);

};
#endif // SHOWHISTOGTAMRESULT_H
