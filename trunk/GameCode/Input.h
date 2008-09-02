//----------------------------------------------------------------------------------------------------
#ifndef INPUT_H
#define INPUT_H


//----------------------------------------------------------------------------------------------------
enum eInputDeviceType
{
	eInputDeviceType_Unknown = 0,
	eInputDeviceType_Keyboard,
	eInputDeviceType_Mouse,
	eInputDeviceType_GamePad
};


//----------------------------------------------------------------------------------------------------
enum eMouseButton
{
	eMouseButton_Left = 0,
	eMouseButton_Right,
	eMouseButton_Middle,
	//More to add
};


//----------------------------------------------------------------------------------------------------
class cInput
{
public:
	static cInput * CreateInputDevice( const DIDEVICEINSTANCE * pDDI, LPDIRECTINPUT8 pInputInterface );
	virtual ~cInput();
	virtual void UpdateInput() = 0;

	virtual bool KeyDown( int pKey );
	virtual bool KeyWasDown( int pKey );
	virtual bool KeyUp( int pKey );
	virtual bool MouseButtonUp( eMouseButton pButton );
	virtual bool MouseButtonDown( eMouseButton pButton );

	//get the device type
	eInputDeviceType GetDeviceType();

protected:
	cInput( const DIDEVICEINSTANCE * pDDI, LPDIRECTINPUT8 pInputInterface, DIDATAFORMAT & pDataFormat,
			eInputDeviceType pInputType );
	bool AcquireDevice();
	bool UnacquireDevice();

	static const char * m_DeviceName[];
protected:
	LPDIRECTINPUTDEVICE8 m_InputDevice;	
	DIDEVICEINSTANCE     m_DeviceInstance;
	DIDEVCAPS            m_DeviceCaps;
	eInputDeviceType	 m_DeviceType;
	bool				 m_BufferedDevice;

private:
//inline bool isPolled()
//{
//   //return true;
//   return ( mDeviceCaps.dwFlags & DIDC_POLLEDDEVICE );
};


//----------------------------------------------------------------------------------------------------
//typedef std::map< cInput *, eInputDeviceType> tInputDevicePtrMap; 
typedef std::vector< cInput * > tInputPtrVector;


//----------------------------------------------------------------------------------------------------
class cKeyboardInput : public cInput
{
public:
	cKeyboardInput( const DIDEVICEINSTANCE * pDDI, LPDIRECTINPUT8 pInputInterface, DIDATAFORMAT & pDataFormat );
	virtual ~cKeyboardInput();
	virtual void UpdateInput();
	virtual bool KeyDown( int pKey );
	virtual bool KeyWasDown( int pKey );
	virtual bool KeyUp( int pKey );

private:
	static const int m_KeySize = 256;
	char m_Keys[m_KeySize];                   // All the keys on a keyboard.
	char m_oldKeys[m_KeySize];                // All the old ( prv Frame ) keys on a keyboard.	  
};


//----------------------------------------------------------------------------------------------------
class cMouseInput : public cInput
{
public:
	cMouseInput( const DIDEVICEINSTANCE * pDDI, LPDIRECTINPUT8 pInputInterface, DIDATAFORMAT & pDataFormat  );
	~cMouseInput();
	virtual void UpdateInput();
	virtual bool MouseButtonUp( eMouseButton pButton );
	virtual bool MouseButtonDown( eMouseButton pButton );
	
private:
	DIMOUSESTATE			m_MouseState;       // Object to hold the state of the mouse.
	DIMOUSESTATE			m_oldMouseState;    // Previous mouse state.
};


//----------------------------------------------------------------------------------------------------
class cInputManager
{
public:
//	static bool CreateInputManager();
//	static void DestroyInputManager();
	cInputManager();
	virtual ~cInputManager();

	static const char * GetErrorMsg();
	void Update();
	
	//get key/button state
	bool KeyDown( int pKey );
	bool KeyWasDown( int pKey );
	bool KeyUp( int pKey );
	bool MouseButtonUp( eMouseButton pButton );
	bool MouseButtonDown( eMouseButton pButton );
	
private:
	//init the device inter face and enumerate all connected devices
	bool InitInputManager();
	static BOOL CALLBACK EnumDevicesProc( const DIDEVICEINSTANCE* pDDI, LPVOID pVRef );
	void AddDevice( cInput * pInput );
	void EnumerateDevices();

	//get the input type
	cInput * GetInputDevice( eInputDeviceType pDeviceType );
	
private:
	static std::string		m_ErrorMsg;
	LPDIRECTINPUT8			m_InputInterface;
	tInputPtrVector			m_InputDevices;
	//TODO :: enumerate all the connected device and intialize it
	//Put the support for Gamepad / joystic
};


//----------------------------------------------------------------------------------------------------
inline eInputDeviceType cInput::GetDeviceType()
{
	return m_DeviceType;
}


//----------------------------------------------------------------------------------------------------
inline bool cInput::KeyWasDown( int pKeyIdx ) 
{
	return false;
}


//----------------------------------------------------------------------------------------------------
inline bool cInput::KeyDown( int pKeyIdx )
{
	return false;
}


//----------------------------------------------------------------------------------------------------
inline bool cInput::KeyUp( int pKeyIdx )
{
	return false;
}


//----------------------------------------------------------------------------------------------------
inline bool cInput::MouseButtonDown(eMouseButton pButton)
{
	return false;
}


//----------------------------------------------------------------------------------------------------
inline bool cInput::MouseButtonUp(eMouseButton pButton)
{
	return false;
}


//----------------------------------------------------------------------------------------------------
inline const char * cInputManager::GetErrorMsg()
{
	return m_ErrorMsg.c_str();
}


//----------------------------------------------------------------------------------------------------
inline void cInputManager::AddDevice( cInput *pInput )
{
	m_InputDevices.push_back( pInput );
}


//----------------------------------------------------------------------------------------------------
#endif