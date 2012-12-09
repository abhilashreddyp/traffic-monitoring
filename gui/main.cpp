#include <QtGui/QApplication>
#include <QObject>
#include <QLabel>

#include "frame_processor.hpp"
#include "video_processor.hpp"
#include "main_window.hpp"

#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    VideoProcessor processor;
    MainWindow win;

    QObject::connect(&processor, SIGNAL(frameProcessed(QImage)), &win, SLOT(teste(QImage)));
    QObject::connect(&win, SIGNAL(closed()), &processor, SLOT(stopIt()));
    win.show();

    processor.setInput("/home/arthur/Videos/bhtrans/bias_fortes_bahia.avi");
    processor.setDelay(1000/processor.getFrameRate());
    processor.start();

    a.exec();
    processor.wait();

    return 0;
}

//#include "data_istream.hpp"
//#include "image_loader.hpp"

//#include <vector>
//#include <string>
//#include <iostream>

//#include <boost/shared_ptr.hpp>

//int main(int argc, char *argv[])
//{
//    std::vector<std::string> paths;
//    paths.push_back("/home/arthur/Pictures/faces/arthur1.jpg");
//    paths.push_back("/home/arthur/Pictures/faces/arthur2.jpg");
//    paths.push_back("/home/arthur/Pictures/faces/arthur3.jpg");
//    paths.push_back("/home/arthur/Pictures/faces/arthur4.jpg");
//    paths.push_back("/home/arthur/Pictures/faces/arthur5.jpg");
//    paths.push_back("/home/arthur/Pictures/faces/arthur6.jpg");

//    DataIStream source(boost::shared_ptr<ImageLoaderRange>(new ImageLoaderRange(paths)));

//    cv::namedWindow("teste", CV_WINDOW_NORMAL);
//    while(true) {
//        cv::Mat img;
//        try {
//            source >> img;
//        }
//        catch(IOException const&) {
//            break;
//        }
//        cv::imshow("teste", img);
//        cv::waitKey();
//    }

//    return 0;
//}
