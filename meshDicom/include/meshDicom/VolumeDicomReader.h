#pragma once
#include <string>
#include <vector>
#include <functional>

#include "DICOMReader.h"
#include "progresscallback.h"

class DcmFileFormat;

class MESHDICOM_API CVolumeDicomReader
{
public:
	CVolumeDicomReader() = default;
	~CVolumeDicomReader() = default;

	// generally x
	int imgWidth = 0;
	// generally y
	int imgHeight = 0;
	// generally z
	int imgDepth = 0;

	bool readVolume(const std::string& pathName);
	

	std::vector<int> shape;
	std::vector<float> spacing;
	// (nWndWidth, nWndLevel) (ww/wl)
	std::vector<int> windowing; 
	meshlibs::CImageInfo imageInfo;

	const std::vector<int16_t> & getData();

	
	void set_callback(const ProgressCallback& callback);
	
	
	/**
	 * 
	 * https://www.slicer.org/wiki/Coordinate_systems	 
	 * Auto-Planning / (in generall?) coordinate
	 * Voxel(CBCT)- LPS -
	 *  (from right towards left) - x
	 *  (from anterior towards posterior) - y
	 *  ( from inferior towards superior) - z
	 * 
	 * VTK Order - LAI(Left,Anterior,Inferior)
	 * (from right towards left) - x
	 * (from posterior towards anterior) - y
	 * (from superior towards inferior) - z
	 */
	bool vtkOrder = true;
protected:
	
	std::string _pathName;
	//std::vector<short>  _volumeBuffer;
	std::vector<int16_t> _volumeBuffer;
	// dcmtk fileformat, to handle reading only once
	DcmFileFormat* _pFileFormat = nullptr;
	ProgressCallback _callback = nullptr;
	float _progress_call = 0.05;
};
