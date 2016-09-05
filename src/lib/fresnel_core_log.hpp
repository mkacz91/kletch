#ifndef KLETCH_LIB_FRESNEL_CORE_LOG_HPP
#define KLETCH_LIB_FRESNEL_CORE_LOG_HPP

namespace kletch {

// Log that records comprehensive information about `FresnelCore` evaluation
template <class T>
struct FullFresnelCoreLog
{
    struct eval_smk1_core_log
    {
        T k0; T k1; T s; int n; T th;
        std::vector<vec2<T>> intermediate_results;
        std::vector<int> nested_iterations;
        void intermediate_result(vec2<T> const& value);
        void nested_iteration() { ++nested_iterations.back(); }
    };

    static std::vector<eval_smk1_core_log> eval_smk1_core_logs;
    static eval_smk1_core_log* eval_smk1_core(T k0, T k1, T s, int n, T th);
};

template <class T> std::vector<typename FullFresnelCoreLog<T>::eval_smk1_core_log>
FullFresnelCoreLog<T>::eval_smk1_core_logs;

template <class T> inline typename FullFresnelCoreLog<T>::eval_smk1_core_log*
FullFresnelCoreLog<T>::eval_smk1_core(T k0, T k1, T s, int n, T th)
{
    eval_smk1_core_logs.push_back({k0, k1, s, n, th});
    return &eval_smk1_core_logs.back();
}

template <class T> inline
void FullFresnelCoreLog<T>::eval_smk1_core_log::intermediate_result(vec2<T> const& value)
{
    intermediate_results.push_back(value);
    nested_iterations.push_back(0);
}

} // namespace kletch

#endif // KLETCH_LIB_FRESNEL_CORE_LOG_HPP
