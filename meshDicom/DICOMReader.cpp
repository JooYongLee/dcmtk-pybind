#include "DICOMReader.h"
#include <iostream>

//#include <Eigen/Core>
#include <plog/Log.h>

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcdeftag.h>
#include <dcmtk/dcmdata/dctk.h>
#include <dcmtk/dcmimgle/diutils.h>
#include <dcmtk/dcmimgle/dcmimage.h>
#include <dcmtk/dcmjpeg/djdecode.h>			// JPEG decoders
#include <dcmtk/dcmjpls/djdecode.h>			// JPEG-LS decoders
#include <dcmtk/dcmdata/dcrledrg.h>			// RLE decoder
#include <dcmtk/dcmjpeg/djencode.h>			// JPEG encoders
#include <dcmtk/dcmjpls/djencode.h>			// JPEG-LS encoders
#include <dcmtk/dcmdata/dcrleerg.h>	
#include <dcmtk/dcmdata/dctk.h>	
#include <dcmtk/dcmdata/dcpxitem.h>	

//#include "mfccommon.h"

//#include "common_def.h"
#include "commons.h"
#include "dicominlines.h"


//#include "../meshMiscellUtils/include/meshMiscellUtils.h"


//#define DEBUG_OPENCV
#ifdef DEBUG_OPENCV
#include "opencv2/opencv.hpp"
#endif // 

namespace meshlibs {;

CDICOMReader::CDICOMReader()
{

}

CDICOMReader::~CDICOMReader()
{
	SAFE_DELETE(_pFileFormat);
}
//friend std::ostream& CImageInfo::operator<< (std::ostream& os, const CImageInfo& rhs)
//{
//	os << "resolution:" << rhs.nImgHeight << "X" << rhs.nImgWidth << "\n"
//		<< "spcacing:" << rhs.fResX << "X" << rhs.fResY << "\n"
//		<< "image slice[z]:#" << rhs.afImgPos[2] << "\n"
//		<< "windowing(WW/WC):" << rhs.dcmWndParam.nWndWidth << "/" << rhs.dcmWndParam.nWndLevel;
//	return os;
//}
std::ostream& operator<< (std::ostream& os, const CImageInfo& rhs)
{
	os << "resolution:" << rhs.nImgHeight << "X" << rhs.nImgWidth << "\n"
		<< "spcacing:" << rhs.fResX << "X" << rhs.fResY << "\n"
		<< "image slice[z]:#" << rhs.afImgPos[2] << "\n"
		<< "windowing(WW/WC):" << rhs.dcmWndParam.nWndWidth << "/" << rhs.dcmWndParam.nWndLevel;
	return os;

}

bool CDICOMReader::readDicomTag(const std::string& filename)
{
	this->_filename = filename;
	bool bReturn = true;
	//bool bReturn = this->_readDicomFile();
	if (!this->_readDicomFile())
	{
		LOGE << "failed reading dicom-file";
		return false;
	}
	ASSERT_MSG(this->_pFileFormat, "");

	//DcmDataset* pDataset = _pDcmFileData->pDcmFF->getDataset();
	DcmDataset* pDataset = _pFileFormat->getDataset();

	DcmXfer xfer = pDataset->getCurrentXfer();
	//LOGI.printf("compressed? : %d", xfer.isEncapsulated());
	// image position
	if (!ReadFloatValues(pDataset, DCM_ImagePositionPatient, _imgInfo.afImgPos, 3))
	{
		LOGE << "failed DCM_ImagePositionPatient";
	}
	// image orientation
	if (!ReadFloatValues(pDataset, DCM_ImageOrientationPatient, _imgInfo.afOrient, 6))
	{
		//
		LOGE << "failed DCM_ImageOrientationPatient";

	}


	// sop class UID
	BOOL bModalityFound = FALSE;

	std::string strModality = ReadStringValue(pDataset, DCM_Modality);
	if (!strModality.empty())
	{
		strcpy(_imgInfo.lpszModality, strModality.c_str());
		bModalityFound = TRUE;
	}

	unsigned int instance_number = 0;
	ReadUint32Values(pDataset, DCM_InstanceNumber, &instance_number, 1);
	std::string instanceNumber = ReadStringValue(pDataset, DCM_InstanceNumber);
	if (!instanceNumber.empty())
	{
		//std::cout << this->_filename << ":" << instanceNumber << ":" << instance_number << "\n";
		this->_imgInfo.nInstanceNumber = std::atoi(instanceNumber.c_str());
	}
	
	//LOGI << "instance number:" << instanceNumber;

	if (!bModalityFound)
	{
		CString strSOPClassUid = ReadStringValue(pDataset, DCM_SOPClassUID);
		if (!strSOPClassUid.empty())
		{
			strcpy(_imgInfo.lpszModality, strSOPClassUid.c_str());
			bModalityFound = true;
		}
	}

	if (!bModalityFound)
	{
		ASSERT(0);
		_imgInfo.lpszModality[0] = L'\0';
	}


	// pixel representation
	/*_bSignedRepresent = false;*/


	DcmElement* pElement = nullptr;
	pElement = SearchDcmElement(DCM_PixelRepresentation, pDataset);
	if (pElement)
	{
		USHORT val = 0;
		pElement->getUint16(val);

		//if (val != 0)
		//	_bSignedRepresent = true;
	}

	// rescale slope
	if (!ReadFloatValues(pDataset, DCM_RescaleSlope, &(_imgInfo.rescale.fRescaleSlope), 1))
		_imgInfo.rescale.fRescaleSlope = 1.0f;

	if (_imgInfo.rescale.fRescaleSlope == 0.0f)
	{
		ASSERT(0);
		_imgInfo.rescale.fRescaleSlope = 1.0f;
	}

	// Rescale intercept
	if (!ReadFloatValues(pDataset, DCM_RescaleIntercept, &(_imgInfo.rescale.fRescaleIntercept), 1))
		_imgInfo.rescale.fRescaleIntercept = 0.0f;



	// image resolution
	pElement = SearchDcmElement(DCM_PixelSpacing, pDataset);
	if (pElement)
	{
		double dVal = 0.0;
		pElement->getFloat64(dVal, 0);
		_imgInfo.fResX = static_cast<float>(dVal);

		dVal = 0.0;
		pElement->getFloat64(dVal, 1);
		_imgInfo.fResY = static_cast<float>(dVal);

		if (_imgInfo.fResY == 0.0f)
			_imgInfo.fResY = _imgInfo.fResX;
	}
	else
	{
		_imgInfo.fResX = _imgInfo.fResY = 0.0f;
	}


	// image bit-depth info.
	uint16_t nBitStored = 0;
	uint16_t nBitAllocated = 0;

	// bits-stored
	pElement = SearchDcmElement(DCM_BitsStored, pDataset);
	if (pElement)
		pElement->getUint16(nBitStored);

	//_nBitStored = nBitStored;

	// bits-allocated
	pElement = SearchDcmElement(DCM_BitsAllocated, pDataset);
	if (pElement)
		pElement->getUint16(nBitAllocated);
	

	// default window width/level
	float fWndLevel = 0.f, fWndWidth = 0.f;
	if (ReadFloatValues(pDataset, DCM_WindowCenter, &fWndLevel, 1))
		_imgInfo.dcmWndParam.nWndLevel = static_cast<int>(fWndLevel + 0.5f);

	if (ReadFloatValues(pDataset, DCM_WindowWidth, &fWndWidth, 1))
		_imgInfo.dcmWndParam.nWndWidth = static_cast<int>(fWndWidth + 0.5f);


	// Presentation LUT shape (Invert state)
	std::string strLUTShape = ReadStringValue(pDataset, DCM_PresentationLUTShape);
	if (strLUTShape.compare("INVERSE") == 0)
	{
		_imgInfo.bInverted = true;
	}
	DicomImage dcmImg(pDataset, EXS_LittleEndianExplicit, CIF_AcrNemaCompatibility);
	this->imgWidth = dcmImg.getWidth();
	this->imgHeight= dcmImg.getHeight();
	this->imgBits = dcmImg.getDepth();
	this->_imgInfo.nImgWidth = this->imgWidth;
	this->_imgInfo.nImgHeight = this->imgHeight;

	if (bReturn)
	{


		// read dicom tag
	}
	return bReturn;
}
bool CDICOMReader::readImage(const std::string& filename)
{
	this->_filename = filename;
	bool bReturn = this->readDicomTag(filename);

	if (bReturn)
	{
		bReturn = this->_readImage();
	}

	return bReturn;
}
bool CDICOMReader::_readDicomFile()
{
	if (!this->_pFileFormat)
	{
		_pFileFormat = new DcmFileFormat();
		//bool isGood = _pFileFormat->loadFile(this->_filename.c_str()).good();
		// autoplannign 코드 참고 
		// https://192.168.98.1/svn/automerge/branches/AutoPlanning_1.0.0.1/DataAccessor/DIOCOMReader.cpp::CDICOMReader::Load
		OFCondition cond = _pFileFormat->loadFile(this->_filename.c_str(),
			EXS_Unknown, EGL_withGL, DCM_MaxReadLength, ERM_autoDetect);
		bool isGood = cond.good();
		if (!isGood)
		{
			SAFE_DELETE(_pFileFormat);
		}
		return isGood;
	}
	return true;
}


bool CDICOMReader::_readImage()
{
	bool bReturn = false;
	OFCondition result;

	if (this->_pFileFormat)
	{

		DcmDataset* pDataSet = _pFileFormat->getDataset();
		if (pDataSet == nullptr)
		{
			LOGE << "cannot get dataset";
			return false;
		}

		DcmXfer xfer = pDataSet->getCurrentXfer();

		DicomImage dcmImg(pDataSet, EXS_LittleEndianExplicit, CIF_AcrNemaCompatibility);

		// https://stackoverflow.com/questions/38258460/dcmtk-c-how-to-get-data-pixels-from-dicom-files
		this->imgBits = dcmImg.getDepth();
		this->imgWidth = dcmImg.getWidth();
		this->imgHeight = dcmImg.getHeight();

		/**
		 * convert bits to byte
		 * example
		 * 11 bits --> 2byte
		 * 16 bits ---> 2byte
		 * \pram val
		 * \return
		 */
		auto bits2Bytes = [](int val)
		{
			return val / 8 + static_cast<int>(val % 8 != 0);
		};

		int imgBytes = bits2Bytes(this->imgBits);

		// non-compressed
		if (!xfer.isEncapsulated())
		{

			DiPixel* pixel = (DiPixel*)dcmImg.getInterData();
			dcmImg.getInterData()->getCount();


			int numElem = this->imgWidth * this->imgHeight;
			_buffer.resize(numElem);
			memcpy(_buffer.data(), pixel->getDataPtr(), numElem * sizeof(short));
			bReturn = true;
		}
		else
		{
			DcmElement* element = nullptr;
			result = pDataSet->findAndGetElement(DCM_PixelData, element);
			if (result.bad() || element == NULL)
			{
				LOGE << "failed get elements";
				return false;
			}


			DcmPixelData* dpix = NULL;
			//DcmElement* element = NULL;
			dpix = OFstatic_cast(DcmPixelData*, element);



			/* Since we have compressed data, we must utilize DcmPixelSequence
			   in order to access it in raw format, e. g. for decompressing it
			   with an external library.
			 */
			DcmPixelSequence* dseq = NULL;
			E_TransferSyntax xferSyntax = EXS_Unknown;
			const DcmRepresentationParameter* rep = NULL;

			// Find the key that is needed to access the right representation of the data within DCMTK
			dpix->getOriginalRepresentationKey(xferSyntax, rep);
			// Access original data representation and get result within pixel sequence
			result = dpix->getEncapsulatedRepresentation(xferSyntax, rep, dseq);

			if (result.good())
			{
				bReturn = true;

				this->_buffer.resize(this->imgWidth * this->imgHeight);

				//buffer = new short[nImgSize];
				//int imgBytes = imgBits / 8;

				Uint32 nStartFragment = 0;
				OFString OFstrDecompressModel;

				unsigned long bufferLength = imgBytes * this->imgWidth * this->imgHeight;

				result = dpix->getUncompressedFrame(
					pDataSet,
					0,
					nStartFragment,
					this->_buffer.data(),
					bufferLength,
					OFstrDecompressModel
				);
				bReturn = result.good();
				if (result.bad())
				{
					LOGE << "failed decompressed";
				}
#ifdef DEBUG_OPENCV
				typedef Eigen::Vector<short, Eigen::Dynamic> VectorXs;
				VectorXs vec = Eigen::Map<VectorXs>((int16_t*)_buffer.data(), imgHeight * imgWidth);
				LOGI << "eigen min max" << vec.minCoeff() << "~" << vec.maxCoeff();

				cv::Mat img = cv::Mat(imgHeight, imgWidth, CV_16SC1, _buffer.data());
				double dmin, dmax;
				cv::minMaxIdx(img, &dmin, &dmax);
				LOGI.printf("pixel value range : %f~%f", dmin, dmax);
				cv::imwrite("test.png", img);

				unsigned short umin = vec.minCoeff();
				unsigned short umax = vec.maxCoeff();
				float scale = 255.f / (umax - umin);

				ImageData uimg = ((vec.cast<float>().array() - umin) * scale).cast<uint8_t>();
				img = cv::Mat(imgHeight, imgWidth, CV_8UC1, uimg.data());
				std::string time_str;
				meshlibs::get_time_str(time_str);
				time_str += ".png";
				cv::imwrite(time_str.c_str(), img);

#endif // 




			}
			else
			{

			}
		}
	}
	return bReturn;
}

const std::vector<int16_t>& CDICOMReader::getData()
{	
	return this->_buffer;
}

CImageInfo CDICOMReader::getTagInfo() 
{
	return this->_imgInfo;
}

bool CDICOMReader::getFloatValues(const DcmTagKey& key, float* pfData, int nNums)
{
	if (this->_pFileFormat)
	{
		DcmDataset* pDataset = _pFileFormat->getDataset();
		if (::ReadFloatValues(pDataset, key, pfData, nNums))
		{
			return true;
		}
	}
	return false;
}

bool CDICOMReader::getUintValues(const DcmTagKey& key, unsigned int* pData, int nNums)
{	
	if (this->_pFileFormat)
	{
		DcmDataset* pDataset = _pFileFormat->getDataset();
		if (::ReadUint32Values(pDataset, key, pData, nNums))
		{
			return true;
		}
	}
	return false;
}


std::string CDICOMReader::getStringValue(const DcmTagKey& key)
{
	std::string strValue;
	if (_pFileFormat)
	{
		DcmDataset* pDataset = _pFileFormat->getDataset();
		strValue = ::ReadStringValue(pDataset, key);
	}

	return strValue;
}


}