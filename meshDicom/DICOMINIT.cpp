// Added by Makeit, on 2018/04/23.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "DICOMINIT.h"

// Memo: Makeit - dcmtk가 mbcs로 컴파일 되도록 하기 위함.
#ifdef UNICODE
#undef UNICODE
#define UNICODE_UNDEF
#endif
#include <dcmtk/dcmjpeg/djdecode.h>			// JPEG decoders
#include <dcmtk/dcmjpls/djdecode.h>			// JPEG-LS decoders
#include <dcmtk/dcmdata/dcrledrg.h>			// RLE decoder
#include <dcmtk/dcmjpeg/djencode.h>			// JPEG encoders
#include <dcmtk/dcmjpls/djencode.h>			// JPEG-LS encoders
#include <dcmtk/dcmdata/dcrleerg.h>			// RLE encoder
#ifdef UNICODE_UNDEF
#define UNICODE
#undef UNICODE_UNDEF
#endif

#include <plog/Log.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Do not make any instance.
DICOMINIT::DICOMINIT()
{
}
DICOMINIT::~DICOMINIT()
{
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Initializing DICOM library
void DICOMINIT::InitializeDICOM(void)
{
	// register global compression codecs
	DJEncoderRegistration::registerCodecs();
	DJLSEncoderRegistration::registerCodecs();
	DcmRLEEncoderRegistration::registerCodecs();

	// register global decompress codecs
	DJDecoderRegistration::registerCodecs();
	DJLSDecoderRegistration::registerCodecs();
	DcmRLEDecoderRegistration::registerCodecs();
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Release DICOM library
void DICOMINIT::ReleaseDICOM(void)
{
	// deregister compression codecs
	DJEncoderRegistration::cleanup();
	DJLSEncoderRegistration::cleanup();
	DcmRLEEncoderRegistration::cleanup();

	// deregister decompress codecs
	DJDecoderRegistration::cleanup();
	DJLSDecoderRegistration::cleanup();
	DcmRLEDecoderRegistration::cleanup();
}
