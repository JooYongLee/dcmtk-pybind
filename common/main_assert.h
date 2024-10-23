#pragma once

#include <cstdlib>
#include <cerrno>
#include <ctime>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <typeinfo>

#include <limits>
#include <algorithm>
#include <complex>
#include <deque>
#include <queue>
#include <cassert>
#include <list>
#if __cplusplus >= 201103L
#include <random>
#ifdef EIGEN_USE_THREADS
#include <future>
#endif
#endif

#include <Eigen/QR> // required for createRandomPIMatrixOfRank

/**
 * https://raw.githubusercontent.com/libigl/eigen/master/test/main.h
 */


#define EI_PP_MAKE_STRING2(S) #S
#define EI_PP_MAKE_STRING(S) EI_PP_MAKE_STRING2(S)


namespace Eigen
{
    static std::vector<std::string> g_test_stack;
    // level == 0 <=> abort if test fail
    // level >= 1 <=> warning message to std::cerr if test fail
    static int g_test_level = 0;
    static int g_repeat;
    static unsigned int g_seed;
    static bool g_has_set_repeat, g_has_set_seed;
}

namespace Eigen {


template<typename T> inline typename NumTraits<T>::Real test_precision() { return NumTraits<T>::dummy_precision(); }
template<> inline float test_precision<float>() { return 1e-3f; }
template<> inline double test_precision<double>() { return 1e-6; }
template<> inline long double test_precision<long double>() { return 1e-6l; }
template<> inline float test_precision<std::complex<float> >() { return test_precision<float>(); }
template<> inline double test_precision<std::complex<double> >() { return test_precision<double>(); }
template<> inline long double test_precision<std::complex<long double> >() { return test_precision<long double>(); }



template<typename Type1, typename Type2>
inline bool test_isApprox(const Type1& a, const Type2& b, typename Type1::Scalar* = 0) // Enabled for Eigen's type only
{
    return a.isApprox(b, test_precision<typename Type1::Scalar>());
}

// get_test_precision is a small wrapper to test_precision allowing to return the scalar precision for either scalars or expressions
template<typename T>
typename NumTraits<typename T::Scalar>::Real get_test_precision(const T&, const typename T::Scalar* = 0)
{
    return test_precision<typename NumTraits<typename T::Scalar>::Real>();
}

template<typename T>
typename NumTraits<T>::Real get_test_precision(const T&, typename internal::enable_if<internal::is_arithmetic<typename NumTraits<T>::Real>::value, T>::type* = 0)
{
    return test_precision<typename NumTraits<T>::Real>();
}



// test_relative_error returns the relative difference between a and b as a real scalar as used in isApprox.
template<typename T1, typename T2>
typename NumTraits<typename T1::RealScalar>::NonInteger test_relative_error(const EigenBase<T1>& a, const EigenBase<T2>& b)
{
    using std::sqrt;
    typedef typename NumTraits<typename T1::RealScalar>::NonInteger RealScalar;
    typename internal::nested_eval<T1, 2>::type ea(a.derived());
    typename internal::nested_eval<T2, 2>::type eb(b.derived());
    return sqrt(RealScalar((ea - eb).cwiseAbs2().sum()) / RealScalar((std::min)(eb.cwiseAbs2().sum(), ea.cwiseAbs2().sum())));
}

template<typename T1, typename T2>
typename T1::RealScalar test_relative_error(const T1& a, const T2& b, const typename T1::Coefficients* = 0)
{
    return test_relative_error(a.coeffs(), b.coeffs());
}

template<typename T1, typename T2>
typename T1::Scalar test_relative_error(const T1& a, const T2& b, const typename T1::MatrixType* = 0)
{
    return test_relative_error(a.matrix(), b.matrix());
}

template<typename S, int D>
S test_relative_error(const Translation<S, D>& a, const Translation<S, D>& b)
{
    return test_relative_error(a.vector(), b.vector());
}

template <typename S, int D, int O>
S test_relative_error(const ParametrizedLine<S, D, O>& a, const ParametrizedLine<S, D, O>& b)
{
    return (std::max)(test_relative_error(a.origin(), b.origin()), test_relative_error(a.origin(), b.origin()));
}

template <typename S, int D>
S test_relative_error(const AlignedBox<S, D>& a, const AlignedBox<S, D>& b)
{
    return (std::max)(test_relative_error((a.min)(), (b.min)()), test_relative_error((a.max)(), (b.max)()));
}

template<typename Derived> class SparseMatrixBase;
template<typename T1, typename T2>
typename T1::RealScalar test_relative_error(const MatrixBase<T1>& a, const SparseMatrixBase<T2>& b)
{
    return test_relative_error(a, b.toDense());
}

template<typename Derived> class SparseMatrixBase;
template<typename T1, typename T2>
typename T1::RealScalar test_relative_error(const SparseMatrixBase<T1>& a, const MatrixBase<T2>& b)
{
    return test_relative_error(a.toDense(), b);
}

template<typename Derived> class SparseMatrixBase;
template<typename T1, typename T2>
typename T1::RealScalar test_relative_error(const SparseMatrixBase<T1>& a, const SparseMatrixBase<T2>& b)
{
    return test_relative_error(a.toDense(), b.toDense());
}

template<typename T1, typename T2>
typename NumTraits<typename NumTraits<T1>::Real>::NonInteger test_relative_error(const T1& a, const T2& b, typename internal::enable_if<internal::is_arithmetic<typename NumTraits<T1>::Real>::value, T1>::type* = 0)
{
    typedef typename NumTraits<typename NumTraits<T1>::Real>::NonInteger RealScalar;
    return numext::sqrt(RealScalar(numext::abs2(a - b)) / RealScalar((numext::mini)(numext::abs2(a), numext::abs2(b))));
}

template<typename T>
T test_relative_error(const Rotation2D<T>& a, const Rotation2D<T>& b)
{
    return test_relative_error(a.angle(), b.angle());
}

template<typename T>
T test_relative_error(const AngleAxis<T>& a, const AngleAxis<T>& b)
{
    return (std::max)(test_relative_error(a.angle(), b.angle()), test_relative_error(a.axis(), b.axis()));
}

}

inline void verify_impl(bool condition, const char* testname, const char* file, int line, const char* condition_as_string)
{
    if (!condition)
    {
        if (Eigen::g_test_level > 0)
            std::cerr << "WARNING: ";
        std::cerr << "Test " << testname << " failed in " << file << " (" << line << ")"
            << std::endl << "    " << condition_as_string << std::endl;
        std::cerr << "Stack:\n";
        const int test_stack_size = static_cast<int>(Eigen::g_test_stack.size());
        for (int i = test_stack_size - 1; i >= 0; --i)
            std::cerr << "  - " << Eigen::g_test_stack[i] << "\n";
        std::cerr << "\n";
        if (Eigen::g_test_level == 0)
            abort();
    }
}


// verifyIsApprox is a wrapper to test_isApprox that outputs the relative difference magnitude if the test fails.
template<typename Type1, typename Type2>
inline bool verifyIsApprox(const Type1& a, const Type2& b)
{
    bool ret = test_isApprox(a, b);
    if (!ret)
    {
        std::cerr << "Difference too large wrt tolerance " << get_test_precision(a) << ", relative error is: " << test_relative_error(a, b) << std::endl;
    }
    return ret;
}

#define VERIFY(a) ::verify_impl(a, g_test_stack.back().c_str(), __FILE__, __LINE__, EI_PP_MAKE_STRING(a))


#define VERIFY_IS_APPROX(a, b) VERIFY(verifyIsApprox(a, b))
