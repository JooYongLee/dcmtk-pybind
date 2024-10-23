

#include <pybind11/pybind11.h>
//#include <pybind11/eigen.h>
/*
The pybind11/stl.h header also includes support for 
std::optional<> and std::variant<>. 
These require a C++17 compiler and standard library. 
In C++14 mode, std::experimental::optional<> is supported 
if available.
*/
#include <pybind11/stl.h>

// 중요 callback 을 사용하므로 추가 해줘야한다!!
#include <pybind11/functional.h>
//#include <Eigen/Core>
#include <string>

#include "meshDicom.h"

namespace py = pybind11;


int set_callback_func(CVolumeDicomReader &self, const ProgressCallback  &fun)
{
	self.set_callback(fun);
	return 0;
}

#ifdef EXTENSION_ALL

void init_mesh_dicom(py::module_ &m) {
	// m.def("read_dicom", &read_dicom, "abc");
	// m.def("read_dicom", &read_dicom, "abc");
	py::class_<CVolumeDicomReader>(m, "CVolumeDicomReader")
		.def(py::init<>())
		.def("readVolume", &CVolumeDicomReader::readVolume)
		.def("getData", &CVolumeDicomReader::getData)
		.def("set_callback", &set_callback_func)
		.def_readwrite("spacing", &CVolumeDicomReader::spacing)
		.def_readwrite("shape", &CVolumeDicomReader::shape)
		.def_readwrite("vtkOrder", &CVolumeDicomReader::vtkOrder)
		.def_readwrite("windowing", &CVolumeDicomReader::windowing, "windowing width & windowing level")
		;
	
}

#else
	
PYBIND11_MODULE(meshlibs, m) {
	m.doc() = "pybind11 example plugin";
	
	// m.def("read_dicom", &read_dicom, "abc");
	py::class_<CVolumeDicomReader>(m, "CVolumeDicomReader")
	.def(py::init<>())
	.def("readVolume", &CVolumeDicomReader::readVolume)
	.def("getData", &CVolumeDicomReader::getData)
	.def("set_callback", &set_callback_func)
	.def_readwrite("spacing", &CVolumeDicomReader::spacing)
	.def_readwrite("shape", &CVolumeDicomReader::shape)
	.def_readwrite("vtkOrder", &CVolumeDicomReader::vtkOrder)
	.def_readwrite("windowing", &CVolumeDicomReader::windowing, "windowing width & windowing level")
	;
}

#endif // 


