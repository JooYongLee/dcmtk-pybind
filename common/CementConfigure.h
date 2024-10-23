#pragma once
#include <string>
#include <sstream>
namespace meshlibs { ;

// 시멘트갭 입력 데이터 종류 - 
// 원본 스캔 - 마진기준으로 자른 부분 우
enum E_CementInputDataType
{
	E_ORIGINAL_FULL_SCAN = 0,	// source scan
	E_BLOCKOUT_PARTIAL_SCAN,	// 
	E_LIB_ABUTMENT,
	E_LIB_ABUTMENT_BLOCKOUT
};


class CCementPram
{
public:
	CCementPram()
		: fCementGap(0.02f)
		, fExtraCementGap(0.05f)
		, fDistanceTomarginLine(0.80f)
		, fSmoothDistance(0.20f)
		, bDrillingCompensation(false)
		, bRemoveUndercut(false)
		, fDrilRadius(0.65f)
		, fDrillOffset(0.05f)
	{}

	~CCementPram() {};


	float		fCementGap = 0.04f;
	float		fExtraCementGap = 0.06f;
	float		fDistanceTomarginLine = 0.80f;
	float		fSmoothDistance = 0.2f;

	bool		bDrillingCompensation = false;
	bool		bRemoveUndercut = false;
	float		fDrilRadius = 0.65f;
	bool		fDrillOffset = 0.05f;



	float maxOffset() const { return __max(this->fCementGap, this->fExtraCementGap); };
	operator std::string() const
	{
		std::ostringstream le_msg;
		le_msg << "cement:" << fCementGap << "//" << "extra cement : " << fExtraCementGap;
		return std::string(le_msg.str());
	}

	//CCementPram& operator=(const CCementPram &rhs)
	//{
	//	this->fCementGap = rhs.fCementGap;
	//	this->fExtraCementGap = rhs.fExtraCementGap;
	//	this->fDistanceTomarginLine = rhs.fDistanceTomarginLine;
	//	this->fSmoothDistance = rhs.fSmoothDistance;
	//	this->bDrillingCompensation = rhs.bDrillingCompensation;
	//	this->bRemoveUndercut = rhs.bRemoveUndercut;
	//	this->fDrilRadius = rhs.fDrilRadius;
	//	this->fDrillOffset = rhs.fDrillOffset;
	//	return *this;
	//}
};

}