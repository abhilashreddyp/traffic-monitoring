#ifndef VIDEO_PROCESSOR_HPP
#define VIDEO_PROCESSOR_HPP

#include <QThread>
#include <QMutex>
#include <QImage>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class FrameProcessor;

class VideoProcessor : public QThread
{
    Q_OBJECT

public:
    VideoProcessor(QObject* parent = 0);

    void setFrameProcessor(FrameProcessor* ptr);

    void setInput(QString const& filename);

    void setDelay(int delay);

    int getFrameRate();

public slots:
    void stopIt();

signals:
    void frameProcessed(QImage);
    void frameLoaded(QImage);

protected:
    virtual void run();

private:
    cv::VideoCapture capture;
    FrameProcessor* processor;
    int delay;
    long numberOfFrames;
    bool stop;
    long frameToStop;
    bool callIt;
    QMutex mutex;

    bool readNextFrame(cv::Mat &frame);

    bool isStopped();

    bool isOpened() const;

    void stopAtFrame(long frame);

    long getFrameNumber();

};

#endif // VIDEO_PROCESSOR_HPP

