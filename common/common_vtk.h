#pragma once



#include <vtkPolyData.h>
#include <vtkActor.h>
#include <vtkSmartPointer.h>
#include <vtkProp.h>
#include <vector>
#include <vtkImageData.h>

namespace meshlibs {\

using vtkPolydataList = std::vector<vtkSmartPointer<vtkPolyData>>;
using vtkPropList = std::vector<vtkSmartPointer<vtkProp>>;
using vtkActorList = std::vector<vtkSmartPointer<vtkActor>>;
using SmartPolyData = vtkSmartPointer<vtkPolyData>;
using SmartActor = vtkSmartPointer<vtkActor>;


}