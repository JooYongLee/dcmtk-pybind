#pragma once

#include <string>
#include <vector>

#ifdef MESHDICOM_EXPORTS
#define MESHDICOM_API __declspec(dllexport)
#else
#define MESHDICOM_API __declspec(dllimport)
#endif

class DcmFileFormat;
class DcmTagKey;


namespace meshlibs { ;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// types of image buffer
enum E_ImgBufType
{
	BufType_Undefined = -1,		// Undefined, or uninitialized
	BufType_BYTE = 0,			// BYTE type 8bit grayscale image
	BufType_WORD_12,			// WORD type 12bit grayscale image
	BufType_WORD_16,			// WORD type 16bit grayscale image
	BufType_YUV,				// YUV type 24bit color image
	BufType_RGB,				// RGB type 24bit color image

	_End_Of_BufType_
};



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Windowing parameter
class CWndParam
{
public:

	CWndParam()
		: bInit(false)
		, nWndLevel(0)
		, nWndWidth(0)
	{};

	int nWndLevel;
	int nWndWidth;
	bool bInit;

	// 값 설정.
	inline void Set(int nLevel, int nWidth)
	{
		nWndLevel = nLevel;
		nWndWidth = nWidth;
		bInit = true;
	}

	// copy operator
	CWndParam& operator= (const CWndParam& param)
	{
		nWndLevel = param.nWndLevel;
		nWndWidth = param.nWndWidth;
		bInit = param.bInit;
		return (*this);
	}

};



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Rescale factors
class CRescaleFactors
{
public:	
	~CRescaleFactors() = default;
	float fRescaleSlope;
	float fRescaleIntercept;

	// Convert unrescaled value to rescaled value.
	inline float Rescale(float fValue) const
	{
		return (fValue * fRescaleSlope + fRescaleIntercept);
	}
	// Convert rescaled value to unrescaled value.
	inline float Unrescale(float fValue) const
	{
		return (fValue - fRescaleIntercept) / fRescaleSlope;
	}
	// Convert unrescaled range to rescaled range.
	inline float RescaleRange(float fRange) const
	{
		return fRange * fRescaleSlope;
	}
	// Convert rescaled range to unrescaled range.
	inline float UnrescaleRange(float fRange) const
	{
		return fRange / fRescaleSlope;
	}

	// default constructor
	CRescaleFactors(void)
		: fRescaleSlope(1.f), fRescaleIntercept(0.f)
	{
	}

	// constructor
	CRescaleFactors(float fSlope, float fIntercept)
		: fRescaleSlope(fSlope), fRescaleIntercept(fIntercept)
	{
	}

	// copy cosntructor
	CRescaleFactors(const CRescaleFactors& rhs)
		: fRescaleSlope(rhs.fRescaleSlope), fRescaleIntercept(rhs.fRescaleIntercept)
	{
	}

	// copy operator
	CRescaleFactors& operator= (const CRescaleFactors& param)
	{
		fRescaleIntercept = param.fRescaleIntercept;
		fRescaleSlope = param.fRescaleSlope;
		return (*this);
	}

	// compare operator ==
	bool operator == (const CRescaleFactors& param) const
	{
		if (fRescaleIntercept == param.fRescaleIntercept && fRescaleSlope == param.fRescaleSlope)
			return true;
		else
			return false;
	}

	// compare operator !=
	bool operator != (const CRescaleFactors& param) const
	{
		if (fRescaleIntercept == param.fRescaleIntercept && fRescaleSlope == param.fRescaleSlope)
			return false;
		else
			return true;
	}
};


// struct ImageInfo
// 영상 정보를 담기 위한 구조체.
class MESHDICOM_API CImageInfo
{
public:
	E_ImgBufType	nImgType;			// Buffer type
	int				nImgWidth;			// Image width
	int				nImgHeight;			// Image height
	float			fResX;				// Horizontal Resolution (mm/pel)
	float			fResY;				// Vertical Resolution  	
	int				nNumFrames;			// Number of frames (Multiframe). 아마도 우리는 항상 1일듯. (Singleframe은 1)
	float			afImgPos[3];		// Image position (mm)
	float			afOrient[6];		// Image orientation, two unit-vectors
	CWndParam		dcmWndParam;		// original windowing parameter that stored in DICOM tags.
	CRescaleFactors	rescale;			// rescale-factors
	char			lpszModality[10];	// modality string;
	bool			bInverted;			// Invert 여부 (Presentation LUT Shape. 우리는 쓸일 없음)
	int				nInstanceNumber;	// instance number
	// Default constructor
	CImageInfo(void)
		: nImgType(BufType_Undefined), nImgWidth(0), nImgHeight(0)
		, fResX(-1.f), fResY(-1.f), nNumFrames(0), bInverted(false)
		, nInstanceNumber(0)
	{
		memset(afImgPos, 0, sizeof(float) * 3);
		memset(afOrient, 0, sizeof(float) * 6);

		dcmWndParam.Set(-1, -1);
		rescale.fRescaleIntercept = 0.f;
		rescale.fRescaleSlope = 1.f;

		lpszModality[0] = '\0';
	}

	bool empty()
	{
		return nImgWidth == 0 && nImgHeight == 0 && \
			fResX < 0 && fResY < 0;
	}


	// copy constructor
	CImageInfo(const CImageInfo& info)
		: nImgType(info.nImgType), nImgWidth(info.nImgWidth), nImgHeight(info.nImgHeight)
		, fResX(info.fResX), fResY(info.fResY), nNumFrames(info.nNumFrames), bInverted(info.bInverted)
		, nInstanceNumber(info.nInstanceNumber)
	{
		memcpy(afImgPos, info.afImgPos, sizeof(float) * 3);
		memcpy(afOrient, info.afOrient, sizeof(float) * 6);


		dcmWndParam = info.dcmWndParam;

		rescale.fRescaleIntercept = info.rescale.fRescaleIntercept;
		rescale.fRescaleSlope = info.rescale.fRescaleSlope;

		strcpy_s(lpszModality, sizeof(lpszModality) / sizeof(char), info.lpszModality);
	}
	friend std::ostream& operator<< (std::ostream& os, const CImageInfo& rhs);
	//std::ostream& operator<< (std::ostream& os, const CImageInfo& rhs);
};




class MESHDICOM_API CDICOMReader
{
public:
	CDICOMReader();
	~CDICOMReader();

	/**
	 * 
	 * 
	 * \param filename
	 * \return 
	 */	
	bool readImage(const std::string& filename);

	bool readDicomTag(const std::string& filename);


	// read dicom-value by tag

	bool getFloatValues(const DcmTagKey& key, float* pfData, int nNums);
	
	bool getUintValues(const DcmTagKey& key, unsigned int* pData, int nNums);

	std::string getStringValue(const DcmTagKey& key);

	/**
	 * 
	 * 
	 * \return 
	 */
	//bool readVolume(const std::string & dirname);
	const std::vector<short>& getData();

	CImageInfo getTagInfo();
	// generally x
	int imgWidth = 0;
	// generally y
	int imgHeight = 0;
	// generally z
	//int imgDepth = 0;
	// bits per pixel
	int imgBits = 0;	
protected:

	bool _readImage();
	bool _readDicomFile();
	std::string _filename;

	CImageInfo _imgInfo;
	// dcmtk fileformat, to handle reading only once
	DcmFileFormat* _pFileFormat = nullptr;
	
	std::vector<short> _buffer;
	// generally with axial image
	// z - depth - number of files 
	// y - height , x - width
	// (z, y, x) shape
	std::vector<int> _shape;
};


 }