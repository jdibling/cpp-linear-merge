//
// Created by john on 4/13/15.
//

#ifndef QC_MEMORY_HPP
#define QC_MEMORY_HPP
// ===========================================================

namespace pico {
namespace pstd {

template<typename T, typename... Args> typename std::enable_if<!std::is_array<T>::value, std::unique_ptr < T>>::type make_unique(Args && ...args)
{
    return std::unique_ptr<T>(new
    T(std::forward<Args>(args)
    ...));
}


/***  NOTE
 * make_unique for arrays will value-initialize every element in the array.
 * This matches the semantics of make_unique for scalars.
 *
 * Without this, make_unique would default-initialize.
 *

 * In theory that should be fine, but in certian cases (esp unit tests)
 * we will examine every element of the array even if we haven't assigned
 * every element.  Aside from this being Undefined Behavior, it is also
 * going to return results that are potentially unexpected.
 */
    template<typename T>
    typename std::enable_if<std::is_array<T>::value, std::unique_ptr < T>>

    ::type
    make_unique(size_t n) {
        typedef typename std::remove_extent<T>::type Elem;
        return std::unique_ptr<T>(new Elem[n]());
    }
// ===========================================================
}
}
#endif //QC_MEMORY_HPP
