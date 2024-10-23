// Added by Makeit, on 2018/04/09.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "mfccommon.h"

// Memo: Makeit - dcmtk�� mbcs�� ������ �ǵ��� �ϱ� ����.
#ifdef UNICODE
#undef UNICODE
#define UNICODE_UNDEF
#endif
#include "dcmtk/dcmdata/dctk.h"
#ifdef UNICODE_UNDEF
#define UNICODE
#undef UNICODE_UNDEF
#endif




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// macro constants

#define	DCM_NLS_AUTO					(-1)
#define	DCM_NLS_OFF						0

#define	DCM_NLS_ISO_IR_6				0
#define	DCM_NLS_ISO_IR_100				1
#define	DCM_NLS_ISO_IR_101				2
#define	DCM_NLS_ISO_IR_109				3
#define	DCM_NLS_ISO_IR_110				4
#define	DCM_NLS_ISO_IR_144				5
#define	DCM_NLS_ISO_IR_127				6
#define	DCM_NLS_ISO_IR_126				7
#define	DCM_NLS_ISO_IR_138				8
#define	DCM_NLS_ISO_IR_148				9
#define	DCM_NLS_ISO_IR_13				10
#define	DCM_NLS_ISO_IR_166				11
#define	DCM_NLS_ISO_IR_14				12
#define	DCM_NLS_ISO_IR_87				13
#define	DCM_NLS_ISO_IR_149				14
#define	DCM_NLS_ISO_IR_58               15   // codepage-50227 ISO 2022 Simplified Chinese => IR 58
#define	DCM_NLS_ISO_IR_58C              16   
#define	DCM_NLS_ISO_IR_192				17

#define	DCM_NLS_COUNT					18

#define	DCM_NLS_ISO_2022_IR_6_87		(DCM_NLS_ISO_IR_6 | DCM_NLS_ISO_IR_87 << 8)		// Japanese: ISO 646 + JIS X 0208
#define	DCM_NLS_ISO_2022_IR_13_87		(DCM_NLS_ISO_IR_13 | DCM_NLS_ISO_IR_87 << 8)	// Japanese: JIS X 0201 + JIS X 0208
#define	DCM_NLS_ISO_2022_IR_6_149		(DCM_NLS_ISO_IR_6 | DCM_NLS_ISO_IR_149 << 8)	// Korean: ISO 646 + KS X 1001

#define	DCM_NLS_ISO_2022_CN             10646   // codepage-50229  ISO 2022 Traditional Chinese (ISO-2022-CN or ISO-2022-CN-EXT)
#define	DCM_NLS_BIG5_CN        	        10647   // codepage-950    big5 ANSI/OEM Traditional Chinese (Taiwan; Hong Kong SAR, PRC); Chinese Traditional (Big5) 
#define	DCM_NLS_ISO_2022_SC             10648   // codepage-50227  ISO 2022 Simplified Chinese
#define	DCM_NLS_GB2312_SC      	        10649   // codepage-936    ANSI/OEM Simplified Chinese (PRC, Singapore); Chinese Simplified (GB2312)

#define	DCM_NLS_DEFAULT					DCM_NLS_ISO_IR_6
#define	DCM_NLS_LATIN1					DCM_NLS_ISO_IR_100
#define	DCM_NLS_LATIN2					DCM_NLS_ISO_IR_101
#define	DCM_NLS_LATIN3					DCM_NLS_ISO_IR_109
#define	DCM_NLS_LATIN4					DCM_NLS_ISO_IR_110
#define	DCM_NLS_CYRILLIC				DCM_NLS_ISO_IR_144
#define	DCM_NLS_ARABIC					DCM_NLS_ISO_IR_127
#define	DCM_NLS_GREEK					DCM_NLS_ISO_IR_126
#define	DCM_NLS_HEBREW					DCM_NLS_ISO_IR_138
#define	DCM_NLS_LATIN5					DCM_NLS_ISO_IR_148
#define	DCM_NLS_JAPANESE0				DCM_NLS_ISO_IR_13
#define	DCM_NLS_THAI					DCM_NLS_ISO_IR_166
#define	DCM_NLS_JAPANESE1				DCM_NLS_ISO_2022_IR_6_87
#define	DCM_NLS_JAPANESE2				DCM_NLS_ISO_2022_IR_13_87
#define	DCM_NLS_JAPANESE				DCM_NLS_JAPANESE2
#define	DCM_NLS_KOREAN					DCM_NLS_ISO_2022_IR_6_149
#define	DCM_NLS_UNICODE_UTF8			DCM_NLS_ISO_IR_192


#define	MAX_NAME_LENGTH					(256)
#define	MAX_TEXT_LENGTH					(10240)

#define	LF							((BYTE)'\x0A')
#define	FF							((BYTE)'\x0C')
#define	CR							((BYTE)'\x0D')
#define	ESC							((BYTE)'\x1B')
#define	SS2							((BYTE)'\x8E')
#define	SS3							((BYTE)'\x8F')

static const char* ISO_IR[DCM_NLS_COUNT] =
{
	"ISO_IR 6",			// 0
	"ISO_IR 100",		// 1
	"ISO_IR 101",		// 2
	"ISO_IR 109",		// 3
	"ISO_IR 110",		// 4
	"ISO_IR 144",		// 5
	"ISO_IR 127",		// 6
	"ISO_IR 126",		// 7
	"ISO_IR 138",		// 8
	"ISO_IR 148",		// 9
	"ISO_IR 13",		// 10
	"ISO_IR 166",		// 11
	"ISO_IR 14",		// 12
	"ISO_IR 87",		// 13
	"ISO_IR 149",		// 14
	"ISO_IR 58",		// 15
	"ISO_IR 58",		// 16
	"ISO_IR 192",		// 17
};

static const char* ISO_2022[DCM_NLS_COUNT] =
{
	"ISO 2022 IR 6",	// 0
	"ISO 2022 IR 100",	// 1
	"ISO 2022 IR 101",	// 2
	"ISO 2022 IR 109",	// 3
	"ISO 2022 IR 110",	// 4
	"ISO 2022 IR 144",	// 5
	"ISO 2022 IR 127",	// 6
	"ISO 2022 IR 126",	// 7
	"ISO 2022 IR 138",	// 8
	"ISO 2022 IR 148",	// 9
	"ISO 2022 IR 13",	// 10
	"ISO 2022 IR 166",	// 11
	"ISO 2022 IR 14",	// 12
	"ISO 2022 IR 87",	// 13
	"ISO 2022 IR 149",	// 14
	"ISO 2022 IR 58",	// 15
	"ISO 2022 IR 58",	// 16
	"ISO 2022 IR 192",	// 17
};

static const char* ISO_2022_ESC[] =
{
	"\x1B\x28\x42",		//  0: ISO 2022 IR 6: ESC ( B
	"\x1B\x2D\x41",		//  1: ISO 2022 IR 100: ESC - A
	"\x1B\x2D\x42",		//  2: ISO 2022 IR 101: ESC - B
	"\x1B\x2D\x43",		//  3: ISO 2022 IR 109: ESC - C
	"\x1B\x2D\x44",		//  4: ISO 2022 IR 110: ESC - D
	"\x1B\x2D\x4C",		//  5: ISO 2022 IR 144: ESC - L
	"\x1B\x2D\x47",		//  6: ISO 2022 IR 127: ESC - G
	"\x1B\x2D\x46",		//  7: ISO 2022 IR 126: ESC - F
	"\x1B\x2D\x48",		//  8: ISO 2022 IR 138: ESC - H
	"\x1B\x2D\x4D",		//  9: ISO 2022 IR 148: ESC - M
	"\x1B\x29\x49",		// 10: ISO 2022 IR 13: ESC ) I
	"\x1B\x2D\x54",		// 11: ISO 2022 IR 166: ESC -
	"\x1B\x28\x4A",		// 12: ISO 2022 IR 14: ESC ( J
	"\x1B\x24\x42",		// 13: ISO 2022 IR 87: ESC $ B
	"\x1B\x24\x29\x43",	// 14: ISO 2022 IR 149: ESC $ ) C
	"\x1B\x24\x29\x41",	// 15: ISO 2022 IR 58 : ESC $ ) A
	"\x1B\x24\x41",	    // 16: ISO 2022 IR 58 : ESC $ A     
	"",					// 17
};




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Search DICOM element in DcmTk DICOM element.
static DcmElement* SearchDcmElement(const DcmTagKey &tag, DcmObject *obj)
{
	DcmStack stack;

	if ((obj ) && (obj->search(tag, stack, ESM_fromHere, false /* searchIntoSub */) == EC_Normal))
		return (DcmElement *)stack.top();

	return nullptr;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// static function used in CDICOMReader::ReadDICOMTags() only.
static BOOL ReadUint32Values(DcmDataset* dSet, const DcmTagKey& key, UINT* pData, int nNums)
{
	// asserting
	ASSERT(dSet != nullptr);
	ASSERT(pData != nullptr);
	ASSERT(nNums > 0);

	DcmElement* elem = ::SearchDcmElement( key, dSet );

	if (elem)
	{
		Uint32 nVal = 0;
		Uint16 nVal16 = 0;

		for (int i = 0; i < nNums; i++)
		{
			if ( elem->getUint16(nVal16, i) == EC_Normal )
			{
				nVal = nVal16;
			}
			else if ( elem->getUint32(nVal, i) != EC_Normal )
			{
				return FALSE;
			}

			pData[i] = static_cast<UINT>( nVal );
		}
		return TRUE;
	}

	return FALSE;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// static function used in CDICOMReader::ReadDICOMTags() only.
static BOOL ReadFloatValues(DcmDataset* dSet, const DcmTagKey& key, float* pfData, int nNums)
{
	// asserting
	ASSERT(dSet != nullptr);
	ASSERT(pfData != nullptr);
	ASSERT(nNums > 0);

	DcmElement* elem = ::SearchDcmElement( key, dSet );

	if (elem)
	{
		double dVal;

		for (int i = 0; i < nNums; i++)
		{
			if (elem->getFloat64(dVal, i) != EC_Normal)
				return FALSE;

			pfData[i] = static_cast<float>(dVal);
		}
		return TRUE;
	}

	return FALSE;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// static function used in CDICOMReader::ReadDICOMTags() only.
static CString ReadStringValue(DcmDataset* dSet, const DcmTagKey& key)
{
	// asserting
	ASSERT(dSet != nullptr);

	DcmElement* elem = ::SearchDcmElement(key, dSet);

	if (elem)
	{
		char *pStr = nullptr;
		if (elem->getString(pStr) != EC_Normal)
			return "";

		// �̷��� �ϸ� ���α׷����� ������ ��� locale�� ��ȯ�ȴ�. �̰� �´°�������...
		//return CString(pStr);

		// Memo: Makeit - Locale ������ ��� ���� System local language�� ��ȯ�ϵ��� �Ѵ�.
		// (���α׷����� ������ ������� ����ϵ��� �����Ͽ� �׽�Ʈ �ߴ��� �ѱ��� �� ��������.
		// �츮 ���α׷��� �ش� �� �������� �ʴ� ��� ����� ǥ�õǵ��� ������ �� �ִ�. �׷� ��� �׳��� ���ڵ���
		// ���� ? ��ȣ�� �ٸ� ���·� ������ ǥ�õ� �� �ִ�. �׷��� local language�� �켱������ ����ϱ�� ��)
		//	* �Ʒ� NLSDecoding �Լ��� ����ϰ� ������, CBCT���� ��κ� ����� encoding�� ���� �ʴ´�.
		//	  ���߿� �̱�ó�� �پ��� �� ����ϴ� ������ ����ϴ� ��쿡�� �ݵ�� �ذ�å�� ã�ƾ� ��.
		TCHAR awszConv[MAX_PATH] = { 0, };

		if ( pStr )
		{
			//MultiByteToWideChar( CP_ACP, 0, pStr, (int)strlen(pStr), awszConv, MAX_PATH );
			strcpy(awszConv, pStr);
		}

		return CString(awszConv);
	}

	return "";
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Unicode�� �ƴ� MBCS(�ٱ���) ó���� ���ؼ� ���ڿ� decoding. Unicode�θ� ��ȯ�Ѵ�.
static CString NLSDecoding( const char* pszText )
{
	CString strOutValue;

	if ( pszText == nullptr )
	{
		// ��� �ִ� ��찡 ���� �߻��ϸ� �Ʒ� assert�� �����Ұ�. Ȯ���� ���ؼ� �ӽ÷� ���ܵд�.
		ASSERT(0);
		return strOutValue;
	}

	const BYTE* pTextStart = (const BYTE*)pszText;
	const BYTE* pTextEnd = pTextStart + strlen( pszText );

	// local members for while() routine.
	UINT nCurrentNLS = DCM_NLS_ISO_IR_6;
	wchar_t szWC[2] = { 0, };
	char szMC[3] = { 0, };
	int nLength = 0;
	int nR, nC;
	BYTE c1, c2;


	auto _ConvertWCtoC = [](const wchar_t* str) -> char*
	{
		//��ȯ�� char* ���� ����
		//char* pStr;

		//�Է¹��� wchar_t ������ ���̸� ����
		int strSize = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);

		//char* �޸� �Ҵ�
		static char* pStr = new char[MAX_PATH];

		//�� ��ȯ
		WideCharToMultiByte(CP_ACP, 0, str, -1, pStr, strSize, 0, 0);

		return pStr;
	};

	// Byte ������ Ư�� ��ȣ�� Ȯ���ϰ� unicode�� ��ȯ�Ѵ�.
	while( *pTextStart && pTextStart < pTextEnd )
	{
		if ( *pTextStart == ESC )
		{
			for ( int nIndex = 0; nIndex < DCM_NLS_COUNT; nIndex++ )
			{
				if ( ::memcmp( pTextStart, ISO_2022_ESC[nIndex], strlen( ISO_2022_ESC[nIndex] ) ) == 0 )
				{
					nCurrentNLS = nIndex;
					break;
				}
			}
			while( *pTextStart )
			{
				if ( *pTextStart++ >= 0x40 )
					break;
			}
		}
		else
		{
			switch( nCurrentNLS )
			{
				// ISO 646 (G0)
				case DCM_NLS_ISO_IR_6:
				default:
					szMC[0] = (*pTextStart++);
					szMC[1] = 0;
					// �ش� OS�� ���� ó��.
					nLength = MultiByteToWideChar( CP_ACP, 0, szMC, 1, szWC, 2 );
					szWC[nLength] = 0;
					strOutValue += _ConvertWCtoC(szWC);
					break;

				case DCM_NLS_ISO_IR_13:		// JIS X 0201 (G1)
				case DCM_NLS_ISO_IR_14:		// JIS X 0201 (G0)
					szMC[0] = (*pTextStart++);
					szMC[1] = 0;

					// ANSI/OEM Japanese; Japanese (Shift-JIS)
					nLength = MultiByteToWideChar( 932, 0, szMC, 1, szWC, 2 );
					szWC[nLength] = 0;

					strOutValue += _ConvertWCtoC(szWC);
					break;

				// JIS X 0208 (G0)
				case DCM_NLS_ISO_IR_87:
					// when j is from 33 to 94 (red/orange), s = (j+1)/2 + 112
					// when j is from 97 to 126 (blue/turquoise), s = (j+1)/2 + 176
					// when j is odd (red/blue), t = k + 31, plus one more if k > 95
					// when j is even (orange/turquoise), t = k + 126 
					// j is first byte of raw JIS, k is second byte of raw JIS
					// s is first byte of SJIS, t is second byte of SJIS
					c1 = *pTextStart++;
					c2 = *pTextStart++;
					if (c2)
					{
						nR = c1 < 95 ? 112 : 176;
						nC = c1 % 2 ? (c2 > 95 ? 32 : 31) : 126;
						szMC[0] = (char)( ((c1 + 1) >> 1) + nR );
						szMC[1] = (char)( (c2 + nC) );
						szMC[2] = 0;
					}
					else
					{
						szMC[0] = c1;
						szMC[1] = 0;
					}

					// ANSI/OEM Japanese; Japanese (Shift-JIS)
					nLength = MultiByteToWideChar( 932, 0, szMC, (int)strlen(szMC), szWC, 2);
					szWC[nLength] = 0;
					strOutValue += _ConvertWCtoC(szWC);
					break;

				// KS X 1001 (G1)
				case DCM_NLS_ISO_IR_149:
					if ( *pTextStart < 0xA0 )
					{
						nCurrentNLS = DCM_NLS_ISO_IR_6;
						continue;
					}
					c1 = (*pTextStart++);
					c2 = (*pTextStart++);
					if (c2)
					{
						szMC[0] = c1;
						szMC[1] = c2;
						szMC[2] = 0;
					}
					else
					{
						szMC[0] = c1;
						szMC[1] = 0;
					}

					// KS_C_5601-1987 ANSI/OEM Korean (Unified Hangul Code)
					nLength = MultiByteToWideChar( 949, 0, szMC, (int)strlen(szMC), szWC, 2 );
					szWC[nLength] = 0;
					strOutValue += _ConvertWCtoC(szWC);
					break;

				// Chinese IR 58
				case DCM_NLS_ISO_IR_58:
				case DCM_NLS_ISO_IR_58C:
					c1 = (*pTextStart++);
					c2 = (*pTextStart++);
					if ( c2 != 0 )
					{
						szMC[0] = c1;
						szMC[1] = c2;
						szMC[2] = 0;
					}
					else
					{
						szMC[0] = c1;
						szMC[1] = 0;
					}

					// codepage-50227 ISO 2022 Simplified Chinese
					nLength = MultiByteToWideChar( 50227, 0, szMC, (int)strlen(szMC), szWC, 2 );
					szWC[nLength] = 0;
					strOutValue += _ConvertWCtoC(szWC);
					break;				
			}
		}
	}

	return strOutValue;
}
