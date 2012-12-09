#include "video_processor.hpp"

#include "frame_processor.hpp"
#include "utility.hpp"

#include <iostream>

VideoProcessor::VideoProcessor(QObject* parent)
    : QThread(parent)
    , capture()
    , processor(NULL)
    , delay(-1)
    , numberOfFrames(0)
    , stop(false)
    , frameToStop(-1)
    , callIt(false)
    , mutex()
{}

void VideoProcessor::setFrameProcessor(FrameProcessor* ptr)
{
    this->processor = ptr;
}

void VideoProcessor::setInput(QString const& filename)
{
    this->numberOfFrames = 0;
    if(!this->capture.open(filename.toStdString())) {
#warning TODO: lançar exceção
    }
}

void VideoProcessor::run()
{
    cv::Mat frame, output;

    if(!this->isOpened()) return;

    this->stop = false;

    while(!this->isStopped()) {

        if(!this->readNextFrame(frame))
            break;

        emit(frameLoaded(utility::toQImage(frame)));

        if(this->callIt) {
            this->processor->process(frame, output);
            this->numberOfFrames++;
        }
        else
            output = frame;

        emit(frameProcessed(utility::toQImage(output)));

        if(this->delay > 0)
            msleep(this->delay);

        if(this->frameToStop >= 0 && this->getFrameNumber() == this->frameToStop)
            this->stopIt();
    }
}

void VideoProcessor::stopIt()
{
    QMutexLocker locker(&this->mutex);
    this->stop = true;
}

bool VideoProcessor::isStopped()
{
    QMutexLocker locker(&this->mutex);
    return this->stop;
}

bool VideoProcessor::isOpened() const
{
    return this->capture.isOpened();
}

void VideoProcessor::setDelay(int delay)
{
    this->delay = delay;
}

void VideoProcessor::stopAtFrame(long frame)
{
    this->frameToStop = frame;
}

long VideoProcessor::getFrameNumber()
{
    return static_cast<long>(this->capture.get(CV_CAP_PROP_POS_FRAMES));
}

bool VideoProcessor::readNextFrame(cv::Mat &frame)
{
    return this->capture.read(frame);
}

int VideoProcessor::getFrameRate()
{
    return static_cast<int>(this->capture.get(CV_CAP_PROP_FPS));
}

