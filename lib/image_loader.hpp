#ifndef IMAGE_LOADER_HPP
#define IMAGE_LOADER_HPP

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <boost/static_assert.hpp>
#include <boost/iterator/iterator_adaptor.hpp>
#include <boost/pointee.hpp>
#include <boost/type_traits.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/logical.hpp>

#include <string>

template <class Iterator>
class ImageLoaderIterator
    : public boost::iterator_adaptor<
        ImageLoaderIterator<Iterator> // Derived
      , Iterator                    // Base
      , cv::Mat                       // Value
      , boost::bidirectional_traversal_tag // CategoryOrTraversal
      , cv::Mat&                      // Reference
    >
{
    friend class boost::iterator_core_access;

    struct enabler { };

public:
    ImageLoaderIterator()
        : ImageLoaderIterator::iterator_adaptor_()
        , cachedImage()
    { }

    ImageLoaderIterator(Iterator const& iterator)
        : ImageLoaderIterator::iterator_adaptor_(iterator)
        , cachedImage()
    { }

    template <class OtherValue>
    ImageLoaderIterator(OtherValue const& other,
                        typename boost::enable_if<
                            boost::is_convertible<OtherValue, typename ImageLoaderIterator::base_type>,
                            enabler>::type = enabler())
        : ImageLoaderIterator::iterator_adaptor_(other)
        , cachedImage(other.cachedMat)
    { }

private:
    cv::Mat& dereference() const
    {
        if (this->cachedImage.empty())
            this->cachedImage = cv::imread(*this->base_reference());

        return this->cachedImage;
    }

    void increment()
    {
        this->advance(1);
    }

    void decrement()
    {
        this->advance(-1);
    }

    void advance(typename ImageLoaderIterator::difference_type n)
    {
        if (!this->cachedImage.empty())
        {
            cv::imwrite(*this->base_reference(), this->cachedImage);
            this->cachedImage = cv::Mat();
        }

        std::advance(this->base_reference(), n);
    }

    mutable cv::Mat cachedImage;
};

template <class Iterator>
typename boost::enable_if
<
    typename boost::is_convertible<typename boost::pointee<Iterator>::type, std::string>,
    ImageLoaderIterator<Iterator>
>::type makeImageLoaderIterator(Iterator const& x)
{
    return ImageLoaderIterator<Iterator>(x);
}

template <class Iterator>
class ConstImageLoaderIterator
    : public boost::iterator_adaptor<
        ConstImageLoaderIterator<Iterator> // Derived
      , Iterator                           // Base
      , cv::Mat const                      // Value
      , boost::bidirectional_traversal_tag       // CategoryOrTraversal
      , cv::Mat const&                     // Reference
    >
{
    friend class boost::iterator_core_access;

    struct enabler { };

public:
    ConstImageLoaderIterator()
        : ConstImageLoaderIterator::iterator_adaptor_()
        , cachedImage()
    { }

    ConstImageLoaderIterator(Iterator const& iterator)
        : ConstImageLoaderIterator::iterator_adaptor_(iterator)
        , cachedImage()
    { }

    template <class OtherValue>
    ConstImageLoaderIterator(OtherValue const& other,
                        typename boost::enable_if<
                            boost::is_convertible<OtherValue, typename ConstImageLoaderIterator::base_type>,
                            enabler>::type = enabler())
        : ConstImageLoaderIterator::iterator_adaptor_(other)
        , cachedImage(other.cachedMat)
    { }

private:
    cv::Mat const& dereference() const
    {
        if (this->cachedImage.empty())
            this->cachedImage = cv::imread(*this->base_reference());

        return this->cachedImage;
    }

    void increment()
    {
        this->advance(1);
    }

    void decrement()
    {
        this->advance(-1);
    }

    void advance(typename ConstImageLoaderIterator::difference_type n)
    {
        this->cachedImage = cv::Mat();
        std::advance(this->base_reference(), n);
    }

    mutable cv::Mat cachedImage;
};

template <class Iterator>
typename boost::enable_if
<
    typename boost::is_convertible<typename boost::pointee<Iterator>::type, std::string>,
    ConstImageLoaderIterator<Iterator>
>::type makeConstImageLoaderIterator(Iterator const& x)
{
    return ConstImageLoaderIterator<Iterator>(x);
}

class ImageLoaderRange
{
private:
    std::vector<std::string> const pathRange;

    struct enabler { };

public:
    typedef ConstImageLoaderIterator<std::vector<std::string>::const_iterator> const_iterator;
    typedef ImageLoaderIterator<std::vector<std::string>::const_iterator> iterator;

    template <class StringRange>
    ImageLoaderRange(
            StringRange const& pathRange,
            typename boost::enable_if<
                typename boost::mpl::and_
                <
                    boost::has_range_const_iterator<StringRange const>,
                    boost::is_convertible<typename boost::range_value<StringRange>::type, std::string>
                >::type,
                enabler>::type = enabler())
        : pathRange(boost::const_begin(pathRange), boost::const_end(pathRange))
    {}

    const_iterator begin() const
    {
        return const_iterator(this->pathRange.begin());
    }

    const_iterator end() const
    {
        return const_iterator(this->pathRange.end());
    }

    iterator begin()
    {
        return iterator(this->pathRange.begin());
    }

    iterator end()
    {
        return iterator(this->pathRange.end());
    }
};

struct LoadImageFromDisk
{
    LoadImageFromDisk()
    {}
};

struct ConstLoadImageFromDisk
{
    ConstLoadImageFromDisk()
    {}
};

template <class BidirectionalStringRange>
typename boost::enable_if
<
    typename boost::mpl::and_
    <
        boost::has_range_const_iterator<BidirectionalStringRange const>,
        boost::is_convertible<typename boost::range_value<BidirectionalStringRange>::type, std::string>
    >::type,
    std::pair
    <
        ImageLoaderIterator<typename boost::range_const_iterator<BidirectionalStringRange const>::type>,
        ImageLoaderIterator<typename boost::range_const_iterator<BidirectionalStringRange const>::type>
    >
>::type
operator|(BidirectionalStringRange const& r, LoadImageFromDisk)
{
    return std::make_pair(
                makeImageLoaderIterator(boost::const_begin(r)),
                makeImageLoaderIterator(boost::const_end(r)));
}

template <class BidirectionalStringRange>
typename boost::enable_if
<
    typename boost::mpl::and_
    <
        boost::has_range_const_iterator<BidirectionalStringRange const>,
        boost::is_convertible<typename boost::range_value<BidirectionalStringRange>::type, std::string>
    >::type,
    std::pair
    <
        ConstImageLoaderIterator<typename boost::range_const_iterator<BidirectionalStringRange const>::type>,
        ConstImageLoaderIterator<typename boost::range_const_iterator<BidirectionalStringRange const>::type>
    >
>::type
operator|(BidirectionalStringRange const& r, ConstLoadImageFromDisk)
{
    return std::make_pair(
                makeConstImageLoaderIterator(boost::const_begin(r)),
                makeConstImageLoaderIterator(boost::const_end(r)));
}

#endif // IMAGE_LOADER_HPP
