#pragma once
#include <iostream>
#include <Eigen>
//#include <Eigen/src/Core/Matrix.h>
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif // _USE_MATH_DEFINES
#include <math.h>


//#include "version.h"
//#include "logger.h"

//#define PYTHON_BUILD
//#ifdef PYTHON_BUILD
//#define AUTOCROWN_API
//#else
//https://mania14.tistory.com/6

#ifdef MESHLIBS_EXPORTS
#define MESHLIBS_API __declspec(dllexport)
#else
#define MESHLIBS_API __declspec(dllimport)
#endif


#ifdef AUTOCROWN_EXPORTS
#pragma warning(disable:4251)
#endif 


//https://stackoverflow.com/questions/3767869/adding-message-to-assert
#ifdef NDEBUG
#   define ASSERT_MSG(condition, message) \
    do { \
        if (! (condition)) { \
            std::cerr << "Assertion `" #condition "` failed in " << __FILE__ \
                      << " line " << __LINE__ << ": " << message << std::endl; \
            std::terminate(); \
        } \
    } while (false)
#else
#   define ASSERT_MSG(condition, message) do { } while (false)
#endif


#define SAFE_DELETE(x)			if(x){delete x;x=nullptr;}
#define SAFE_DELETE_ARRAY(x)	if(x){delete[] x;x=nullptr;}
#define SAFE_DELETE_VTK(p)		{if((p)) {(p)->Delete(); (p)=nullptr;}}

//template class AUTOCROWN_API Eigen::Matrix<double, -1, -1, 0, -1, -1>;
//AUTOCROWN_TEMPLATE template class AUTOCROWN_API Eigen::Matrix<double, -1, -1, 0, -1, -1>;
//AUTOCROWN_TEMPLATE template class AUTOCROWN_API Eigen::Matrix<float, -1, -1, 0, -1, -1>;
//AUTOCROWN_TEMPLATE template class AUTOCROWN_API std::vector<int>;
//AUTOCROWN_TEMPLATE template class AUTOCROWN_API std::vector<int>;

#define _180_PER_PI			180. / M_PI
#define _PI_PER_180			M_PI / 180.

#define TEETH_HALF_COUNT			14

#define TEETH_FULL_COUNT			28
// anatomy 정의 & 픽스쳐 & 크라운
#define MAX_NUM_ANATOMY				20
// verte
#define MAX_NUM_LANDMARK			256
// x, y, z dimension
#define DIMENSION_POSE				3
// 11번부터 48번 치아에 대한 총 수
#define TOTAL_COUNT_OF_TEETH		28 // 7 X 4

#define EYE_DATA {1., 0., 0., 0.,\
				0., 1., 0., 0.,\
				0., 0., 1., 0.,\
				0., 0., 0., 1.}

// 치아 번호 확인
#define CHECK_NUMBER(x)		assert(( 0 <= x ^ 10 && x % 10 <= 7 &&\
                                    1 <= x / 10 && x / 10 <= 4))
// int64 정의 matrix
typedef Eigen::Matrix<long long, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> MatrixXll;
typedef MatrixXll MatrixXlr;
typedef Eigen::Matrix<long long, Eigen::Dynamic, Eigen::Dynamic> MatrixXl;

// int64 정의 matrix
typedef Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> MatrixXir;

typedef Eigen::Vector<long long, Eigen::Dynamic> VectorXl;
typedef Eigen::RowVector<long long, Eigen::Dynamic> RowVectorXl;

// Eigen default matrix는 ColMajor, numpy는 RowMajor로 동작?하기 때문에 
// RowMajor로 직접 지정. suffix에 row에 앞자 r을 "MatrixXd  + r" 로해서 명명
// 참조 https://eigen.tuxfamily.org/dox-devel/group__TopicStorageOrders.html 
// raw 버퍼로 받을때는 Map<MatrixXdr> 로 받을것, reshaped 함수 사용하면 안됨
typedef Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>  MatrixXdr;

typedef Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>  MatrixXfr;


typedef Eigen::Matrix<bool, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> MatrixXbr;
typedef Eigen::Matrix<bool, Eigen::Dynamic, Eigen::Dynamic> MatrixXb;

typedef Eigen::Vector<bool, Eigen::Dynamic> VectorXb;

typedef Eigen::Vector<unsigned char, Eigen::Dynamic> VectorXu;

typedef Eigen::RowVector<long long, 3> RowVector3l;
typedef Eigen::RowVector<long long, 2> RowVector2l;

typedef Eigen::RowVector3f Vec3f;
typedef Eigen::RowVector3i Vec3i;
typedef Eigen::RowVector3d Vec3d;
typedef unsigned char VOXEL;
typedef Eigen::Vector<VOXEL, Eigen::Dynamic> ImageData;

// https://202psj.tistory.com/1010
enum class VoxelFlagsEnum {
	NONE = 0,
	SURFACE = 255,
	//FILLED = 0x01 << 0,        // Set this bit when a voxel cell is not empty. Unset this bit when this voxel is empty.
	//INSIDE = 0x01 << 1,        // Voxel center is inside the mesh surface , TIGHT
	//SURFACE = 0x01 << 2,        // Voxel is generated from surface in the surface voxelizing pass
	//SOLID = 0x01 << 3,        // Voxel is generated in solid flood filling in the solid voxelizing pass
	//CLIPPED = 0x01 << 4,        // Voxel is clipped
};

//extern AUTOCROWN_API const int UPPER_TEETH[TEETH_HALF_COUNT];
//extern AUTOCROWN_API const int LOWER_TEETH[TEETH_HALF_COUNT];
/////////////////////////////////////////////////////////////////////////////
namespace meshlibs {;

using namespace Eigen;


template<typename T>
inline T rad2deg(T x)
{
	return x * _180_PER_PI;
}

template<typename T>
inline T deg2rad(T x)
{
	return x * _PI_PER_180;
}

template<typename T>
T deg_to_rad(T x)
{
	return x * (M_PI / 180.);
}


template<typename T>
T rad_to_deg(T x)
{
	return x * (180. / M_PI);
}
//
//inline bool log_initialized_message()
//{
//	LOGI.printf("\n"\
//		"========================================================================================\n"\
//		"====================  %s : %s===========================\n"\
//		"========================================================================================\n"\
//		, VER_FILE_DESCRIPTION_STR, VER_FILE_VERSION_STR);
//	
//
//	
//}
//
//inline bool initialize_logger(plog::IAppender *pAppender, plog::Severity maxSeverity = plog::debug)
//{
//	if (plog::get() == nullptr)
//	{
//		plog::init(maxSeverity, pAppender);
//		log_initialized_message();
//	}
//	return true;
//}
}
//#endif // COMMON_DEF_H_

