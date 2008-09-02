//----------------------------------------------------------------------------------------------------
#include "StdAfx.h"
#include "Input.h"


//----------------------------------------------------------------------------------------------------
#define KEY_DOWN 0x80 


//----------------------------------------------------------------------------------------------------
const char * cInput::m_DeviceName[] = 
{ 
	"Unknown",
	"Keyboard",
	"Mouse",
	"GamePad"
};
std::string cInputManager::m_ErrorMsg;


//----------------------------------------------------------------------------------------------------
cInput * cInput::CreateInputDevice( const DIDEVICEINSTANCE *pDDI, LPDIRECTINPUT8 pInputInterface )
{
	// Set the device's data format:
    DIDATAFORMAT dataFormat;
	memset( &dataFormat, 0, sizeof( DIDATAFORMAT ) );
	cInput * input = NULL;
	switch( GET_DIDEVICE_TYPE( pDDI->dwDevType ) )
	{
		  case DI8DEVTYPE_KEYBOARD:
			 dataFormat = c_dfDIKeyboard;
			 input = new cKeyboardInput( pDDI, pInputInterface, dataFormat );
			 break;

		  case DI8DEVTYPE_MOUSE:
			dataFormat = c_dfDIMouse;
			input = new cMouseInput( pDDI, pInputInterface, dataFormat );
			input->m_DeviceType = eInputDeviceType_Mouse;
			break;

		  case DI8DEVTYPE_JOYSTICK:
		  case DI8DEVTYPE_GAMEPAD:
			  //TODO:: still to be implemented
			dataFormat = c_dfDIJoystick;
			break;

		  default:
			  Assert( false, "Unknown input device\n" );
			break;	
	}
	return input;
}

//----------------------------------------------------------------------------------------------------
cInput::cInput( const DIDEVICEINSTANCE *pDDI, LPDIRECTINPUT8 pInputInterface, DIDATAFORMAT & pDataFormat, 
				eInputDeviceType pInputType )
{
	m_DeviceInstance = *pDDI;
	m_DeviceType = pInputType;
	m_BufferedDevice = false;
	HRESULT hr;
	//create the input device
	hr = pInputInterface->CreateDevice( m_DeviceInstance.guidInstance, &m_InputDevice, NULL );
	Assert( hr == DI_OK, "Cannot create input device %s.\n", m_DeviceName[m_DeviceType] );

	m_DeviceCaps.dwSize = sizeof( DIDEVCAPS );
	hr = m_InputDevice->GetCapabilities( &m_DeviceCaps );
	Assert( hr == DI_OK, "  Failed to get the capabilities of the input device %s.\n", m_DeviceName[m_DeviceType] );

	if( m_BufferedDevice )
	{
		// IMPORTANT STEP TO USE BUFFERED DEVICE DATA!
		//
		// DirectInput uses unbuffered I/O (buffer size = 0) by default.
		// If you want to read buffered data, you need to set a nonzero
		// buffer size.
		//
		// Set the buffer size to DINPUT_BUFFERSIZE (defined above) elements.
		//
		// The buffer size is a DWORD property associated with the device.
		DIPROPDWORD dipdw;
		dipdw.diph.dwSize       = sizeof(DIPROPDWORD);
		dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
		dipdw.diph.dwObj        = 0;
		dipdw.diph.dwHow        = DIPH_DEVICE;
		dipdw.dwData            = 256; // Arbitary buffer size

		hr = m_InputDevice->SetProperty( DIPROP_BUFFERSIZE, &dipdw.diph );
		Assert( hr == DI_OK, "  Failed to set buffet data property for %s.\n", m_DeviceName[m_DeviceType] );
	}

	hr = m_InputDevice->SetDataFormat( &pDataFormat );
	Assert( hr == DI_OK, "  Failed to set data format for %s.\n", m_DeviceName[m_DeviceType] );
	AcquireDevice();
}


//----------------------------------------------------------------------------------------------------
cInput::~cInput()
{
	if( m_InputDevice )
	{
		m_InputDevice->Unacquire();
		m_InputDevice->Release();
		m_InputDevice = NULL;
	}
}


//----------------------------------------------------------------------------------------------------
bool cInput::AcquireDevice()
{
	//Log( " Try to Acquired Device\n" );
	DWORD flag = 0;
	if( m_DeviceType == eInputDeviceType_Keyboard )
	{
		flag = DISCL_FOREGROUND | DISCL_NONEXCLUSIVE;
	}
	else //if mouse
	{
		flag = DISCL_FOREGROUND | DISCL_EXCLUSIVE | DISCL_NOWINKEY;
	}

	HRESULT hr;
	// Now we will set the flags and options for our keyboard object.
	hr = m_InputDevice->SetCooperativeLevel( DXUTGetHWND(), flag );
	if( hr != DI_OK )
	{
	  return false;
	}

	// Now we call Acquire() to gain access to our input device.
	hr = m_InputDevice->Acquire();
	if( hr != DI_OK )
	{
		//Log( " Device %s was not aquired====\n", m_DeviceName[m_DeviceType] );
		//error code
		//result = SUCCEEDED( code );
		 //switch ( code )
		 //{
		 //   case DIERR_INVALIDPARAM:      reason = "Invalid parameter"  ; break;
		 //   case DIERR_NOTINITIALIZED:    reason = "Not initialized"; break;
		 //   case DIERR_OTHERAPPHASPRIO:   reason = "Other app has priority"; break;
		 //   case S_FALSE:                 reason = "Already acquired"; break;
		 //   default:                      reason = "Unknown error"; break;
		 //}
		return false;
	}
	else
	{
		//Log( " Device %s aquired====\n", m_DeviceName[m_DeviceType] );
	}
	return true;
}


//----------------------------------------------------------------------------------------------------
cKeyboardInput::cKeyboardInput( const DIDEVICEINSTANCE *pDDI, LPDIRECTINPUT8 pInputInterface, 
							   DIDATAFORMAT & pDataFormat  ) : cInput( pDDI, pInputInterface, pDataFormat, eInputDeviceType_Keyboard )
{
   memset( m_Keys, 0, sizeof(m_Keys) );
   memset( m_oldKeys, 0, sizeof(m_oldKeys) );
}


//----------------------------------------------------------------------------------------------------
cKeyboardInput::~cKeyboardInput()
{
}


//----------------------------------------------------------------------------------------------------
void cKeyboardInput::UpdateInput()
{
	// Copy old keys so we can determine when a button is raised.
	memcpy( m_oldKeys, m_Keys, sizeof(m_Keys) );
	SecureZeroMemory( m_Keys, sizeof( m_Keys ) );
	HRESULT hr = m_InputDevice->GetDeviceState( sizeof( m_Keys ), &m_Keys );
	//DIDEVICEOBJECTDATA didod[ m_KeySize ];  // Receives buffered data 
	//DWORD dele = 256;
	//HREFTYPE hr = m_InputDevice->GetDeviceData( sizeof(DIDEVICEOBJECTDATA), didod, &dele, 0 );
	switch( hr )
	{
		case DIERR_NOTACQUIRED:
		case DIERR_INPUTLOST:
			AcquireDevice();
			break;
		case DIERR_INVALIDPARAM:
			Assert( false, "Invalide parameter passed to device: %s ", m_DeviceName[m_DeviceType] );
			break;
	}
}


//----------------------------------------------------------------------------------------------------
bool cKeyboardInput::KeyDown( int pKeyIdx )
{
	return ( ( m_Keys[pKeyIdx] & KEY_DOWN ) ? true : false );
}


//----------------------------------------------------------------------------------------------------
bool cKeyboardInput::KeyWasDown( int pKeyIdx )
{
	return ( ( m_oldKeys[pKeyIdx] & KEY_DOWN ) ? true : false );
}


//----------------------------------------------------------------------------------------------------
bool cKeyboardInput::KeyUp( int pKeyIdx )
{
	return ( !( m_Keys[pKeyIdx] & KEY_DOWN ) && m_Keys[pKeyIdx] != m_oldKeys[pKeyIdx] );
}


//----------------------------------------------------------------------------------------------------
cMouseInput::cMouseInput( const DIDEVICEINSTANCE *pDDI, LPDIRECTINPUT8 pInputInterface, 
						 DIDATAFORMAT & pDataFormat  ) : cInput( pDDI, pInputInterface, pDataFormat, eInputDeviceType_Mouse  )
{
   memset( &m_MouseState, 0, sizeof(m_MouseState) );
   memset( &m_oldMouseState, 0, sizeof(m_oldMouseState) );
}


//----------------------------------------------------------------------------------------------------
void cMouseInput::UpdateInput()
{
	memcpy( &m_oldMouseState, &m_MouseState, sizeof( m_MouseState ) );
	HRESULT hr = m_InputDevice->GetDeviceState(  sizeof( DIMOUSESTATE ), &m_MouseState );
	switch( hr )
	{
		case DIERR_NOTACQUIRED:
		case DIERR_INPUTLOST:
			AcquireDevice();
			break;
		case DIERR_INVALIDPARAM:
			Assert( false, "Invalide parameter passed to device: %s ", m_DeviceName[m_DeviceType] );
			break;
	}
}


//----------------------------------------------------------------------------------------------------
bool cMouseInput::MouseButtonDown( eMouseButton pButton )
{
	return ( m_MouseState.rgbButtons[pButton] & KEY_DOWN ? true : false );
}


//----------------------------------------------------------------------------------------------------
bool cMouseInput::MouseButtonUp( eMouseButton pButton )
{
	return ( !( m_MouseState.rgbButtons[pButton] & KEY_DOWN ) && 
				m_MouseState.rgbButtons[pButton] != m_oldMouseState.rgbButtons[pButton] );
}


//----------------------------------------------------------------------------------------------------
cMouseInput::~cMouseInput()
{
}


//----------------------------------------------------------------------------------------------------
cInputManager::cInputManager()
{
	InitInputManager();
}


//----------------------------------------------------------------------------------------------------
cInputManager::~cInputManager()
{
	//release all the input devices
	for( tInputPtrVector::iterator inDvIt = m_InputDevices.begin(); inDvIt != m_InputDevices.end(); ++inDvIt )
	{
		cInput * input_device = *inDvIt;
		delete input_device;
		input_device = NULL;
	}
	
	//release all the input resources
	if( m_InputInterface )
	{
		m_InputInterface->Release();
		m_InputInterface = NULL;
	}
}

//----------------------------------------------------------------------------------------------------
bool cInputManager::InitInputManager()
{
   // Now we are ready to create the input system.  Call DirectInput8Create() function
   // to create the Direct Input object.  This function needs the HINSTANCE object, the
   // version, what is called a unique identifier, the LPDIRECTINPUT8 object, and
   // the last field is used for COM.  Check out the DirectX SDK for more on that one.

	HRESULT hr;
	hr = DirectInput8Create( DXUTGetHINSTANCE(), DIRECTINPUT_VERSION, IID_IDirectInput8, 
							(void**)& m_InputInterface, NULL );
	
	//error
	switch( hr )
	{
		case DIERR_BETADIRECTINPUTVERSION:
		{
			cInputManager::m_ErrorMsg = "DIERR_BETADIRECTINPUTVERSION";
			break;
		}
		case DIERR_INVALIDPARAM:
		{
			//TODO:: handle this error
			cInputManager::m_ErrorMsg = "DIERR_INVALIDPARAM";
			break;
		}
		case DIERR_OLDDIRECTINPUTVERSION:
		{
			cInputManager::m_ErrorMsg = "DIERR_OLDDIRECTINPUTVERSION";
			break;
		}
		case DIERR_OUTOFMEMORY:
		{
			cInputManager::m_ErrorMsg = "DIERR_OUTOFMEMORY";
			break;
		}
		default:
		{
			//TODO:: handle this error
			cInputManager::m_ErrorMsg = "No Error";
			break;
		}
	}
	EnumerateDevices();
return ( hr == DI_OK );
}


//----------------------------------------------------------------------------------------------------
BOOL CALLBACK cInputManager::EnumDevicesProc( const DIDEVICEINSTANCE *pDDI, LPVOID pVRef )
{
	cInputManager * inputManager = (cInputManager*) pVRef;
	cInput * inputDevice = cInput::CreateInputDevice( pDDI, inputManager->m_InputInterface );
	inputManager->AddDevice( inputDevice );
	return (DIENUM_CONTINUE);
}


//----------------------------------------------------------------------------------------------------
void cInputManager::EnumerateDevices()
{
	//TODO :: Assert on not intializing
	m_InputInterface->EnumDevices( DI8DEVTYPE_KEYBOARD, EnumDevicesProc, this, DIEDFL_ATTACHEDONLY );
	m_InputInterface->EnumDevices( DI8DEVTYPE_MOUSE, EnumDevicesProc, this, DIEDFL_ATTACHEDONLY );
	//TODO:: handle these devices
	//m_InputInterface->EnumDevices( DI8DEVTYPE_JOYSTICK, EnumDevicesProc, this, DIEDFL_ATTACHEDONLY );
	//m_InputInterface->EnumDevices( DI8DEVTYPE_GAMEPAD, EnumDevicesProc, this, DIEDFL_ATTACHEDONLY );
}


//----------------------------------------------------------------------------------------------------
void cInputManager::Update()
{
	for( tInputPtrVector::iterator inDevIt = m_InputDevices.begin(); inDevIt != m_InputDevices.end(); ++inDevIt )
	{
		cInput * inDevice = * inDevIt;
		inDevice->UpdateInput();
	}

}


//----------------------------------------------------------------------------------------------------
cInput * cInputManager::GetInputDevice( eInputDeviceType pDeviceType )
{
	cInput * rqudInputDevice = NULL;
	for( tInputPtrVector::iterator inDevIt = m_InputDevices.begin(); inDevIt != m_InputDevices.end(); ++inDevIt )
	{
		cInput * inputDevice = *inDevIt;
		if( inputDevice->GetDeviceType() == pDeviceType )
		{
			rqudInputDevice = inputDevice;
			break;
		}
	}

	return rqudInputDevice;
}


//----------------------------------------------------------------------------------------------------
bool cInputManager::KeyDown( int pKey )
{
	cInput * keyInputDev = GetInputDevice( eInputDeviceType_Keyboard );
	assert( keyInputDev != NULL );
	return	keyInputDev->KeyDown( pKey );
}


//----------------------------------------------------------------------------------------------------
bool cInputManager::KeyUp( int pKey )
{
	cInput * keyInputDev = GetInputDevice( eInputDeviceType_Keyboard );
	assert( keyInputDev != NULL );
	return	keyInputDev->KeyUp( pKey );
}


//----------------------------------------------------------------------------------------------------
bool cInputManager::MouseButtonDown( eMouseButton pButton )
{
	cInput * mouseInputDev = GetInputDevice( eInputDeviceType_Mouse );
	assert( mouseInputDev != NULL );
	return	mouseInputDev->MouseButtonDown( pButton );
}


//----------------------------------------------------------------------------------------------------
bool cInputManager::MouseButtonUp( eMouseButton pButton )
{
	cInput * mouseInputDev = GetInputDevice( eInputDeviceType_Mouse );
	assert( mouseInputDev != NULL );
	return	mouseInputDev->MouseButtonUp( pButton );
}