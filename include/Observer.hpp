#ifndef RTK_OBSERVER_HPP_
#define RTK_OBSERVER_HPP_

namespace rtk {
template <typename T>
class Observer {
public:
    virtual ~Observer() = default;

    virtual void next(T&) = 0;
    virtual void error() = 0;
    virtual void complete() = 0;
};
}

#endif /* !OBSERVER_HPP_ */
