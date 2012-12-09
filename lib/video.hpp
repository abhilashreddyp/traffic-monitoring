#ifndef VIDEO_HPP
#define VIDEO_HPP

#include <vector>
#include <string>

#include <boost/range.hpp>
#include <boost/iterator.hpp>

#include <opencv2/core/core.hpp>

class Video
{
public:
    class const_iterator
            : public boost::iterator_facade<const_iterator, cv::Mat const, boost::single_pass_traversal_tag>
    {
    friend class boost::iterator_core_access;
    public:
        const_iterator();
        const_iterator(Video const* const video);

    private:
        void increment();
        bool equal(const_iterator const& other) const;
        cv::Mat const& dereference() const;

        Video const* video;
        cv::Mat currentImage, currentRaw;
    };

    Video(std::string const& path);

    ~Video();

    void open();
};

#endif // VIDEO_HPP
