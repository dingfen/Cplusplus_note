
namespace MY {

template<class InputIterator, class ForwardIterator>
ForwardIterator uninitialized_copy(InputIterator first, InputIterator last,
    ForwardIterator result) {
        return __uninitialize_copy(first, last, value_type(result));
    }

} // namespace MY