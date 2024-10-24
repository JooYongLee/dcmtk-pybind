#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>
#include <pybind11/stl.h>
#include <vector>
#include <iostream>

namespace py = pybind11;

// NumPy 배열을 생성하는 함수
py::array_t<int16_t> create_numpy_array(int size) {
    // 1D NumPy 배열 생성
    py::array_t<int16_t> array(size);

    // 배열에 접근하여 데이터 채우기
    auto buf = array.mutable_unchecked<1>();  // 1D 배열을 의미합니다.
    for (int i = 0; i < size; i++) {
        buf(i) = static_cast<int16_t>(i);  // 각 요소에 값을 채워넣습니다.
    }

    return array;  // NumPy 배열 반환
}

// NumPy 배열을 생성하는 함수
py::array_t<int16_t> create_numpy_array2(int size) {
    // std::vector<int16_t> vec(size);
    std::vector<int16_t> vec(size, 0);
    // 1D NumPy 배열 생성
    std::cout << "ok------";
    // py::array_t<int16_t> array(vec.data(), vec.size());

    return py::array_t<int16_t>(vec.size(), vec.data());
    // return array;  // NumPy 배열 반환
}


PYBIND11_MODULE(example, m) {
    m.def("create_numpy_array", &create_numpy_array, "Create a NumPy array with size");
    m.def("create_numpy_array2", &create_numpy_array2, "Create a NumPy array with size");
}