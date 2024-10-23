// ���� ifdef ����� DLL���� ���������ϴ� �۾��� ���� �� �ִ� ��ũ�θ� ����� 
// ǥ�� ����Դϴ�. �� DLL�� ��� �ִ� ������ ��� ����ٿ� ���ǵ� _EXPORTS ��ȣ��
// �����ϵǸ�, �ٸ� ������Ʈ������ �� ��ȣ�� ������ �� �����ϴ�.
// �̷��� �ϸ� �ҽ� ���Ͽ� �� ������ ��� �ִ� �ٸ� ��� ������Ʈ������ 
// MESHDICOM_API �Լ��� DLL���� �������� ������ ����, �� DLL��
// �� DLL�� �ش� ��ũ�η� ���ǵ� ��ȣ�� ���������� ������ ���ϴ�.
#ifdef MESHDICOM_EXPORTS
#define MESHDICOM_API __declspec(dllexport)
#else
#define MESHDICOM_API __declspec(dllimport)
#endif


// �� Ŭ������ meshDicom.dll���� ������ ���Դϴ�.
//class MESHDICOM_API CmeshDicom {
//public:
//	CmeshDicom(void);
//	// TODO: ���⿡ �޼��带 �߰��մϴ�.
//};
//
//extern MESHDICOM_API int nmeshDicom;
//
//MESHDICOM_API int fnmeshDicom(void);
#include "meshDicom/DicomReader.h"
#include "meshDicom/VolumeDicomReader.h"

namespace plog {
	class IAppender;
	enum Severity;
}
namespace meshlibs {
	namespace meshDicom
	{
		MESHDICOM_API bool initLogger(plog::IAppender* appender, const plog::Severity &sev = plog::Severity(0));
	}
}