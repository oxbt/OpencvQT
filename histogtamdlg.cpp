#include "histogtamdlg.h"
#include "ui_histogtamdlg.h"
#include <QMessageBox>
#include <QPainter>

using namespace cv;

HistogtamDlg::HistogtamDlg(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HistogtamDlg)
{
    ui->setupUi(this);    

    left_Threshold=1;
    right_Threshold=255;

    QObject::connect(ui->left_spinBox,SIGNAL(valueChanged(int)),
                     ui->left_horizontalSlider,SLOT(setValue(int)));//spinBox值与滑条对应

    QObject::connect(ui->left_horizontalSlider,SIGNAL(valueChanged(int)),
                     ui->left_spinBox,SLOT(setValue(int)));//滑条与spinBox值对应


    QObject::connect(ui->right_spinBox,SIGNAL(valueChanged(int)),
                     ui->right_horizontalSlider,SLOT(setValue(int)));//spinBox值与滑条对应

    QObject::connect(ui->right_horizontalSlider,SIGNAL(valueChanged(int)),
                     ui->right_spinBox,SLOT(setValue(int)));//滑条与spinBox值对应

    connect(ui->left_horizontalSlider,&QSlider::sliderReleased,this,&HistogtamDlg::setLeftValue);
    connect(ui->right_horizontalSlider,&QSlider::sliderReleased,this,&HistogtamDlg::setRightValue);
    connect(ui->left_spinBox,&QSpinBox::valueChanged,this,&HistogtamDlg::setHistogtamRe);
    connect(ui->right_spinBox,&QSpinBox::valueChanged,this,&HistogtamDlg::setHistogtamRe);

}


HistogtamDlg::~HistogtamDlg()
{
    delete ui;
}

void HistogtamDlg::receiveDataForHistogtamdlg(QList<QString> m_listVatiate,QList<Mat> m_listMat)
{   
    m_listVatiate_glob=m_listVatiate; //lisr全局存储,对应的变量名字
    m_listMat_glob=m_listMat;//list全局，对应的mat组

    for(int i=0;i<m_listVatiate.size();++i)
    {
        QString stri=m_listVatiate.at(i);//QString::number(i,10,0);
        ui->comboBox->addItem(stri);
        ui->comboBox->setCurrentIndex(0);
    }

    //读取combobox对应的图
    for(int i=0;i<m_listVatiate.size();++i)//遍历变量名
    {
        //变量名如果和对话框中的值相等输出对象的Mat
        if(m_listVatiate.at(i)==ui->comboBox->currentText())
        {
            globalMat=m_listMat_glob.at(i).clone();
            break;
        }
    }

    createHistogramGray();//创建直方图
    setHistogtam();//添加直方图
}

void HistogtamDlg::closeEvent(QCloseEvent *event)
{
    QMessageBox mesg3;
    mesg3.about(NULL,"信息","关闭窗口了");

    this->setAttribute(Qt::WA_DeleteOnClose);
}


 void  HistogtamDlg::refreshSenddata()
 {
     emit sendToShowhistogtam(resultMat,
                              ui->left_horizontalSlider->value(),
                              ui->right_horizontalSlider->value());

 }


 ////////////////////2017.10.22////////修复实验

 QImage HistogtamDlg::MatToQImage(const cv::Mat& mat)
 {
     // 8-bits unsigned, NO. OF CHANNELS = 1
     if (mat.type() == CV_8UC1)
     {
         QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);
         // Set the color table (used to translate colour indexes to qRgb values)
         image.setColorCount(256);
         for (int i = 0; i < 256; i++)
         {
             image.setColor(i, qRgb(i, i, i));
         }
         // Copy input Mat
         uchar *pSrc = mat.data;
         for (int row = 0; row < mat.rows; row++)
         {
             uchar *pDest = image.scanLine(row);
             memcpy(pDest, pSrc, mat.cols);
             pSrc += mat.step;
         }
         return image;
     }
     // 8-bits unsigned, NO. OF CHANNELS = 3
     else if (mat.type() == CV_8UC3)
     {
         // Copy input Mat
         const uchar *pSrc = (const uchar*)mat.data;
         // Create QImage with same dimensions as input Mat
         QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
         return image.rgbSwapped();
     }
     else if (mat.type() == CV_8UC4)
     {
         // Copy input Mat
         const uchar *pSrc = (const uchar*)mat.data;
         // Create QImage with same dimensions as input Mat
         QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
         return image.copy();
     }
     else
     {
         return QImage();
     }
 }


 void HistogtamDlg::createHistogramGray()//创建直方图
 {
     Mat intoMat=globalMat.clone();

     Mat src = intoMat.clone();
     Mat gray_plane;
     cvtColor(src,gray_plane,COLOR_BGR2GRAY);

     const int histSize = 256; //直方图每一个维度划分的柱条的数目
     float range[] = {0,255};  //灰度级的范围
     const float* ranges[]={range};
     Mat gray_hist;
     calcHist(&gray_plane, 1, 0, Mat(), gray_hist, 1, &histSize, ranges, true, false);//计算直方图
     normalize(gray_hist,gray_hist,0,255,NORM_MINMAX);

     int hist_width = 512;    //直方图尺寸
     int hist_height = 256;
     int bin_w = cvRound( (double) hist_width/histSize );
     Mat histImage( hist_height, hist_width, CV_8UC3, Scalar( 0,0,100) );
     for( int i = 1; i < histSize; i++ )
     {
         line( histImage, Point( bin_w*(i-1), hist_height - cvRound(gray_hist.at<float>(i-1)) ),
               Point( bin_w*(i), hist_height - cvRound(gray_hist.at<float>(i)) ),
               Scalar( 255, 0, 0), 2, 8, 0  );         
     }

     globalHistMat=histImage;
 }

 void HistogtamDlg::setHistogtam()//滑条函数，刷新直方图
 {
     Mat img2=globalMat.clone();
     Mat dst;
     int value_m=this->ui->left_horizontalSlider->value();
     int value_m2=this->ui->right_horizontalSlider->value();

     cv::cvtColor(img2,dst,COLOR_RGB2GRAY);//进行，灰度处理
     //阈值分割
     threshold( dst,                 //输入图像,原始数组 (单通道 , 8-bit of 32-bit 浮点数).
                dst,                          //输出图像,输出数组，必须与 src 的类型一致，或者为 8-bit.
                value_m,                 //分割值
                0,                         // 使用 CV_THRESH_BINARY 和 CV_THRESH_BINARY_INV 的最大值.
                cv::THRESH_TOZERO); //阈值类型

     resultMat=dst.clone();//处理结果
     refreshSenddata();

     QImage imgshow=MatToQImage(globalHistMat);
     ui->histogtam_label->clear();
     imgshow=imgshow.scaled(this->ui->histogtam_label->width(),this->ui->histogtam_label->height(),Qt::KeepAspectRatio);
     this->ui->histogtam_label->setPixmap(QPixmap::fromImage(imgshow));
 }

 void HistogtamDlg::setHistogtamRe(int value)//刷新直方图
 {
//     createHistogramGray();
     setHistogtam();
 }


 void HistogtamDlg::setLeftValue()
 {

     if(ui->left_horizontalSlider->value()>=ui->right_spinBox->value())
     {
         ui->right_spinBox->setValue(ui->left_horizontalSlider->value()+1);
     }


 }
 void HistogtamDlg::setRightValue()
 {

     if(ui->right_horizontalSlider->value()<=ui->left_spinBox->value())
     {
         ui->left_spinBox->setValue(ui->right_horizontalSlider->value()-1);
     }

 }

void HistogtamDlg::on_pushButton_clicked()
{

    /*
    直方图的均值化。
    图像中像素个数多的灰度级进行展宽，
    而对图像中像素个数少的灰度进行压缩，
    从而扩展像原取值的动态范围，
    提高了对比度和灰度色调的变化，
    使图像更加清晰。
    */

}

void HistogtamDlg::on_show_pushButton_clicked()
{

    //读取combobox对应的图
    for(int i=0;i< m_listVatiate_glob.size();++i)//遍历变量名
    {
        //变量名如果和对话框中的值相等输出对象的Mat
        if( m_listVatiate_glob.at(i)==ui->comboBox->currentText())
        {
            globalMat=m_listMat_glob.at(i).clone();
            break;
        }
    }

     setHistogtam();//刷新视图
}


//确定按钮
void HistogtamDlg::on_out_pushButton_clicked()
{


    Mat dstImage;
    if( m_listMat_glob.isEmpty()){
        QMessageBox emptybox;
        emptybox.about(NULL,"信息","没有加载图片");
    }
    else
    {
        QString inImgDlgVar=ui->comboBox->currentText();//输入变量名字
        QString outImgDlgVar=ui->out_lineEdit->text();//输出变量名字

        ////////////////////////////////
        ///*******获取变量**********//
        ///////////////////////
        QString left_var1=QString::number(ui->left_spinBox->value(),10,0);//获取变量1
        QString right_var2=QString::number(ui->right_spinBox->value(),10,0);//获取变量2


        for(int i=0;i< m_listVatiate_glob.size();++i)//遍历变量名，如果输出重复设为默认
        {
            if(outImgDlgVar==m_listVatiate_glob.at(i))//如果没有输入变量
            {

                QMessageBox mesg;
                mesg.about(NULL,"提示","无输出对象或对象重复,设置为默认");

                QString strVar= QString::number(m_listVatiate_glob.size(),10,0);
                QString strVariable=QString("image%1").arg(strVar);
                outImgDlgVar=strVariable;
                break;
            }
        }

        if(outImgDlgVar.isEmpty())//如果没有输入变量，设为默认
        {
            QMessageBox mesg;
            mesg.about(NULL,"提示","无输出对象或对象重复,设置为默认");

            QString strVar= QString::number(m_listVatiate_glob.size(),10,0);
            QString strVariable=QString("image%1").arg(strVar);
            outImgDlgVar=strVariable;
        }

        m_listVatiate_glob.append(outImgDlgVar);//添加变量名list
        //            listNameMainWid.append(strg);//添加操作名list

        if(m_listMat_glob.size()>9)//mat list 长度超过9后要开辟空间
        {
            Mat matImageMainWid = Mat(Size(2200, 2200), CV_8UC3);//QPixmap(2200,2200);
            m_listMat_glob.append(matImageMainWid);
        }
        m_listMat_glob.insert(m_listMat_glob.size()-1,resultMat);//添加图元list


        //code内容
        QString sendstrg=QString("threshold( %1,%1, %2, 0,cv::THRESH_TOZERO); //阈值类型;\r\n"
                                 "threshold( %1,dst,%3, 0,cv:: cv::THRESH_TOZERO_INV ); //阈值类型;//设置核\r\n"
                                 "threshold( dst,%4, 1, 255,cv:: cv::THRESH_BINARY); //阈值类型,\r\n")
                .arg(inImgDlgVar).arg(left_var1).arg(right_var2).arg(outImgDlgVar);

        QMessageBox mesg2;
        mesg2.about(NULL,"信息",sendstrg);

        QString strg="灰度阈值分割";


//        QMessageBox mesg3;
//        mesg3.about(NULL,"信息",outImgDlgVar);

        dstImage=resultMat.clone();

        emit hdSendDataMW(dstImage,strg,outImgDlgVar,0,-1);
        emit hdSendStr(-1,sendstrg);

    }

}




