#ifndef DATA_ISTREAM_HPP
#define DATA_ISTREAM_HPP

#include "exception.hpp"

#include <opencv2/core/core.hpp>

#include <boost/range/functions.hpp>
#include <boost/range/metafunctions.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/type_traits.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/logical.hpp>

class DataIStream : protected boost::noncopyable
{
private:
    class DataIStreamAdapter
    {
    public:
        virtual ~DataIStreamAdapter()
        {}

        virtual cv::Mat const get() const = 0;
    };

    template <class SinglePassRange, class Container>
    class DataIStreamRangeAdapter : public DataIStreamAdapter
    {
    private:
        typedef typename boost::range_const_iterator<SinglePassRange>::type Iterator;

        boost::shared_ptr<Container> container;
        mutable Iterator current;
        Iterator const end;

    public:
        DataIStreamRangeAdapter(SinglePassRange const& range, boost::shared_ptr<Container> const& container)
            : DataIStreamAdapter()
            , container(container)
            , current(boost::const_begin(range))
            , end(boost::const_end(range))
        {}

        virtual ~DataIStreamRangeAdapter()
        {}

        virtual cv::Mat const get() const
        {
            if (this->current == this->end)
                throw EndOfFile();

            return *(this->current++);
        }
    };

    template <class SinglePassRange, class Container>
    static typename boost::enable_if
    <
        typename boost::mpl::and_
        <
            boost::has_range_const_iterator<SinglePassRange const>,
            boost::is_convertible<typename boost::range_value<SinglePassRange>::type, cv::Mat>
        >::type,
        DataIStreamRangeAdapter<SinglePassRange, Container>*
    >::type
    adaptRange(SinglePassRange const& range, boost::shared_ptr<Container> const& container)
    {
        return new DataIStreamRangeAdapter<SinglePassRange, Container>(range, container);
    }

    DataIStreamAdapter* const adapter;

public:
    template <class SinglePassRange, class Container>
    DataIStream(SinglePassRange const& range, boost::shared_ptr<Container> const& container)
        : adapter(DataIStream::adaptRange(range, container))
    {}

    template <class SinglePassRange>
    DataIStream(boost::shared_ptr<SinglePassRange> const& container)
        : adapter(DataIStream::adaptRange(*container, container))
    {}

    ~DataIStream()
    {
        delete this->adapter;
    }

    DataIStream const& operator >>(cv::Mat& out) const
    {
        cv::Mat const current = this->adapter->get();
        if(out.cols != current.cols || out.rows != current.rows || out.type() != current.type())
            out = cv::Mat(current.rows, current.cols, current.type());

        current.copyTo(out);
        return *this;
    }
};

#endif // DATA_ISTREAM_HPP
