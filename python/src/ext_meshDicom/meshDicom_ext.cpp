#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>
#include <pybind11/functional.h>
//#include <Eigen/Core>
#include <string>
#include <vector>

#include "meshDicom.h"

namespace py = pybind11;


int set_callback_func(CVolumeDicomReader &self, const ProgressCallback  &fun)
{
	self.set_callback(fun);
	return 0;
}

py::array_t<int16_t> cpp_to_numpy( CVolumeDicomReader &self) {
    // 입력된 C++ 벡터 데이터를 numpy 배열로 변환
	const std::vector<int16_t> &vec = self.getData();
    return py::array_t<int16_t>(vec.size(), vec.data());
}

#ifdef EXTENSION_ALL

void init_mesh_dicom(py::module_ &m) {
	// m.def("read_dicom", &read_dicom, "abc");
	// m.def("read_dicom", &read_dicom, "abc");
	py::class_<CVolumeDicomReader>(m, "CVolumeDicomReader")
		.def(py::init<>())
		.def("readVolume", &CVolumeDicomReader::readVolume)
		.def("getData", &cpp_to_numpy)
		.def("set_callback", &set_callback_func)
		.def_readwrite("spacing", &CVolumeDicomReader::spacing)
		.def_readwrite("shape", &CVolumeDicomReader::shape)
		.def_readwrite("vtkOrder", &CVolumeDicomReader::vtkOrder)
		.def_readwrite("windowing", &CVolumeDicomReader::windowing, "windowing width & windowing level")
		;
	
}

#else
	
PYBIND11_MODULE(meshDicom, m) {
	m.doc() = "pybind11 example plugin";
	
	// m.def("read_dicom", &read_dicom, "abc");
	py::class_<CVolumeDicomReader>(m, "CVolumeDicomReader")
	.def(py::init<>())
	.def("readVolume", &CVolumeDicomReader::readVolume)
	.def("getData", cpp_to_numpy)
	.def("set_callback", &set_callback_func)
	.def_readwrite("spacing", &CVolumeDicomReader::spacing)
	.def_readwrite("shape", &CVolumeDicomReader::shape)
	.def_readwrite("vtkOrder", &CVolumeDicomReader::vtkOrder)
	.def_readwrite("windowing", &CVolumeDicomReader::windowing, "windowing width & windowing level")
	;
}

#endif // 


