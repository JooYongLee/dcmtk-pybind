#include "VolumeDicomReader.h"
#include "DICOMReader.h"
#include "dicominlines.h"

#include <iostream>
#include <windows.h>
#include <cassert>
#include <vector>
#include <algorithm>

#include <plog/Log.h>

//#include <ppl.h>
#include <omp.h>
#include "progressbar.hpp"
#include "SplitPath.h"


#define MINIMUM_SLICE_FOR_VOLUME			5

static char* ConvertWCtoC(const wchar_t* str);
static wchar_t* ConverCtoWC(const char* str);
class Glob {
public:
	Glob(const std::string& pattern);
	~Glob();



	std::string GetFileName() const {
		assert(ok_);
#ifdef UNICODE
		return ConvertWCtoC(find_data_.cFileName);
#else
		return find_data_.cFileName;
#endif
	}
	operator bool() const {
		return ok_;
	}

	bool Next();

	std::string getFulllPathName() const;

	static std::string pathAppend(const std::string& p1, const std::string& p2);

private:
	Glob(const Glob&);
	void operator=(const Glob&);

private:
	bool ok_;
	HANDLE find_handle_;
	WIN32_FIND_DATA find_data_;
};


std::string pathAppend(const std::string& p1, const std::string& p2) {

	char sep = '/';
	std::string tmp = p1;

	char check_sep1 = '/';
	char check_sep2 = '\\';
#ifdef _WIN32
	sep = '\\';
#endif

	//if (p1[p1.length()] != check_sep1 && \
	//	p1[p1.length()] != check_sep2) { // Need to add a
	//	tmp += sep;                // path separator
	//	return(tmp + p2);
	//}
	if (p1[p1.length() - 1] != check_sep1 && \
		p1[p1.length() - 1] != check_sep2) { // Need to add a
		tmp += sep;                // path separator
		return(tmp + p2);
	}
	else
		return(p1 + p2);
}



Glob::Glob(const std::string& pattern)
	: ok_(false),
	find_handle_(INVALID_HANDLE_VALUE)
{
#ifdef UNICODE

	find_handle_ = FindFirstFile(ConverCtoWC(pattern.c_str()), &find_data_);
#else
	find_handle_ = FindFirstFile(pattern.c_str(), &find_data_);
#endif

	ok_ = find_handle_ != INVALID_HANDLE_VALUE;
}

Glob::~Glob() {
	if (find_handle_ != INVALID_HANDLE_VALUE) {
		FindClose(find_handle_);
	}
}

bool Glob::Next() {
	ok_ = FindNextFile(find_handle_, &find_data_) != 0;
	return ok_;
}

std::string Glob::getFulllPathName() const
{
	//std::string  ->GetFileName();
	return std::string();
}



std::string Glob::pathAppend(const std::string& p1, const std::string& p2)
{
	return pathAppend(p1, p2);
}

std::vector<std::string> glob_files(const std::string& pathName)
{
	//std::string datapath = "../../../Shared_files/data/Abutment/Digital Abutment/molar";
	std::string glob_patterns = pathAppend(pathName, "*.dcm");// "../../../Shared_files/data/Abutment/Digital Abutment/*.stl";
	Glob gdata(glob_patterns);
	std::vector<std::string> pathList;
	pathList.reserve(500);
	while (gdata)
	{
		pathList.push_back(pathAppend(pathName, gdata.GetFileName()));
		gdata.Next();
	}


	if (pathList.size() == 0)
	{
		Glob gdata2(pathAppend(pathName, "*.DCM"));
		while (gdata2)
		{
			pathList.push_back(pathAppend(pathName, gdata2.GetFileName()));
			gdata2.Next();
		}
	}
	return pathList;
}


class DicomIndex
{
public:
	DicomIndex(int index, int instanceNumber, const std::string& fileName, double pose)
		: fileIndex(index)
		, instanceIndex(instanceNumber)
		, strFileName(fileName)
		, slicePose(pose)
	{}
	~DicomIndex() = default;
	int fileIndex;
	int instanceIndex;
	std::string strFileName;
	double slicePose;
};

bool CVolumeDicomReader::readVolume(const std::string& pathName)
{	
	using namespace meshlibs;
	std::vector<std::string> pathList = glob_files(pathName);

	LOGI.printf("founded dicomes #%d", pathList.size());

	if (pathList.size() == 0)
	{
		LOGE << "empty dicom files:#" << pathList.size();
		return false;
	}
	std::vector<DicomIndex> sortedDicoms;
	sortedDicoms.reserve(pathList.size());
	int fileIndex = 0;

	this->shape.resize(3);
	this->spacing.resize(3);
	this->windowing.resize(2);

//#pragma omp parallel for
	for (int i = 0; i < pathList.size(); i++)
	{
		const std::string& strFilepath = pathList[i];
		CDICOMReader reader;

		if (reader.readDicomTag(strFilepath))
		{
			// 최초 한번만 저장
			if (this->imageInfo.empty())
			{
				this->imageInfo = reader.getTagInfo();
				//reader.getFloatValues(, &this->spacing[0], 1);				
				

				
				this->shape[0] = pathList.size();
				this->shape[1] = reader.imgHeight;
				this->shape[2] = reader.imgWidth;

				this->imgHeight = reader.imgHeight;
				this->imgWidth = reader.imgWidth;
				this->imgDepth = pathList.size();

				std::string sliceThick = reader.getStringValue(DCM_SliceThickness);
				
				this->spacing[1] = reader.getTagInfo().fResY;
				this->spacing[2] = reader.getTagInfo().fResX;

				this->spacing[0] = sliceThick.empty() ? this->spacing[0] : atof(sliceThick.c_str());

				this->windowing[0] = this->imageInfo.dcmWndParam.nWndWidth;
				this->windowing[1] = this->imageInfo.dcmWndParam.nWndLevel;
			}
			//unsigned int realSliceNumber = 0;
			////if (!reader.getUintValues(DCM_InstanceNumber, &realSliceNumber, 1))
			////{
			////	LOGE << "failed load instance number";
			////}
			//LOGI.printf("[%d]:%d", i, realSliceNumber);
			fileIndex = i;
			DicomIndex dcmIndex(i, 
				reader.getTagInfo().nInstanceNumber,
				strFilepath,
				reader.getTagInfo().afImgPos[2]);
			sortedDicoms.push_back(dcmIndex);

			//imgInfo = reader.getTagInfo();
			//this->imgWidth = reader.imgWidth;
			//this->imgWidth = reader.imgH;
			//this->imgWidth = reader.imgWidth;
		}
		else
		{
			LOGF << "failed read dicomtag:" << strFilepath;
			return false;
		}
	}
	auto slice_increase_order = [](DicomIndex& pItem1, DicomIndex& pItem2)
	{	
		//return (pItem1.instanceIndex <= pItem2.instanceIndex);
		return pItem1.slicePose >= pItem2.slicePose;
	};
	auto slice_decrease_order = [](DicomIndex& pItem1, DicomIndex& pItem2)
	{
		//return (pItem1.instanceIndex >= pItem2.instanceIndex);		
		return pItem1.slicePose <= pItem2.slicePose;
	};


	if (this->vtkOrder)
	{
		// do sort, first.
		std::sort(sortedDicoms.begin(), sortedDicoms.end(), slice_increase_order);
	}
	else
	{
		std::sort(sortedDicoms.begin(), sortedDicoms.end(), slice_decrease_order);
	}

	
	size_t total_elem = 1;
	for (auto& v : this->shape)
		total_elem *= v;
	
	this->_volumeBuffer.resize(total_elem);

	
	int i = 0;
	
	auto invert_image_yaxis = [&](const std::vector<short>& in_buffer, std::vector<short>& out_buffer)
	{
		out_buffer.resize(in_buffer.size());
		for (int iy = 0; iy < this->imgHeight; iy++)
		{
			for (int ix = 0; ix < this->imgWidth; ix++)
			{
				int ry = this->imgHeight - 1 - iy;
				int i = iy * this->imgWidth + ix;
				int j = ry * this->imgWidth + ix;
				out_buffer[j] = in_buffer[i];
			}
		}

	};

	
	int call_interval = static_cast<int>(sortedDicoms.size()* this->_progress_call);

	int sum_progress = 0;
	progressbar bar(sortedDicoms.size(), false);
	bar.set_callback(this->_callback);
	//#pragma omp parallel for reduction(+:sum_progress )
	#pragma omp parallel for //# reduction(+ : sum_progress)
	for (int i = 0; i < sortedDicoms.size(); i++)
	{
		const auto& sortedDcm = sortedDicoms[i];
		meshlibs::CSplitPath split(sortedDcm.strFileName);
		
		LOGI.printf("[%d]:filename:%s // instance:%d // pose %f", i, split.fname, sortedDcm.instanceIndex, sortedDcm.slicePose);
		//std::string msg = meshlibs::string_format("[%d]:filename:%s // instance:%d", i, split.fname, sortedDcm.instanceIndex);
		//std::cout << msg;

		CDICOMReader reader;
		if (reader.readImage(sortedDcm.strFileName))
		{
			const auto& sliceBuffer = reader.getData();
			size_t sizeSliceBuffer = sliceBuffer.size();
			//std::cout << "[" << i << "]" << sortedDcm.strFileName << ":" << reader.imgWidth << "X" << reader.imgHeight
			//	<< ":" << sliceBuffer.size() << "==" << reader.imgWidth * reader.imgHeight << "\n";

			short* ptrBuffer = this->_volumeBuffer.data() + i * sizeSliceBuffer;

			if (this->vtkOrder)
			{
				std::vector<short> invertBuffer;
				invert_image_yaxis(sliceBuffer, invertBuffer);
				
				memcpy(ptrBuffer, invertBuffer.data(), sizeof(short)* sizeSliceBuffer);
			}
			else
			{				
				memcpy(ptrBuffer, sliceBuffer.data(), sizeof(short)* sizeSliceBuffer);
			}
			
			sum_progress += 1;

			#pragma omp critical
				bar.update();
			//if ((sum_progress % call_interval) == 0 && this->_callback)
			//{
			//	this->_callback(static_cast<float>(sum_progress) / sortedDicoms.size());
			//}
		}
		else
		{
			LOGE << "failed loading";
		}

		//auto &buffer = reader.getData();
		
	}



	return true;
}

const std::vector<int16_t>& CVolumeDicomReader::getData()
{	
	return this->_volumeBuffer;
}
void CVolumeDicomReader::set_callback(const ProgressCallback& callback)
{
	this->_callback = callback;
}
//
//std::vector<int> CVolumeDicomReader::getShape()
//{
//	return std::vector<int>();
//}
//
//
