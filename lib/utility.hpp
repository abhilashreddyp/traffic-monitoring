#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <QImage>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

namespace utility
{

QImage toQImage(cv::Mat const& src)
{
    assert(src.channels() == 1 || src.channels() == 3);
    cv::Mat dst;
    if(src.channels() == 1)
        cv::cvtColor(src, dst, CV_GRAY2RGB);
    else if(src.channels() == 3)
        cv::cvtColor(src, dst, CV_BGR2RGB);

    QImage img(dst.data, dst.size().width, dst.size().height, dst.step, QImage::Format_RGB888);
    return img;
}

} //namespace

#endif // UTILITY_HPP
