#ifndef RTK_OPERATOR_HPP_
#define RTK_OPERATOR_HPP_

#include "Observable.hpp"
#include <functional>

namespace rtk {

template <typename T>
class Observable;

template <typename T, typename U>
using Operator = std::function<Observable<U>(Observable<T>)>;
}

#endif /* !RTK_OPERATOR_HPP_ */
