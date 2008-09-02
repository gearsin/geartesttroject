//----------------------------------------------------------------------------------------------------
#include "StdAFx.h"
#include "Input.h"
#include "GameInput.h"


//----------------------------------------------------------------------------------------------------
cGameInput * g_GameInput = NULL;


//----------------------------------------------------------------------------------------------------
cGameInput::cInputModifier::cInputModifier( eInputGroup pGroup, float pDefVal )
{
	m_Group = pGroup;
	m_DefalutVal = pDefVal;
	m_CurrentValue = 0.0f;
}


//----------------------------------------------------------------------------------------------------
cGameInput::cInputModifier::~cInputModifier()
{

}


//----------------------------------------------------------------------------------------------------
void cGameInput::cInputModifier::AddInputCtrlModifier( eInputDeviceType pInputType, int pKeyCode, float pValue )
{
	m_DeviceType = pInputType;
	m_OnValue = pValue;
	m_InputCode = pKeyCode;
}


//----------------------------------------------------------------------------------------------------
void cGameInput::Create()
{
	new cGameInput();
}


//----------------------------------------------------------------------------------------------------
void cGameInput::Destroy()
{
	if( g_GameInput )
	{
		delete g_GameInput;
		g_GameInput = NULL;
	}
}


//----------------------------------------------------------------------------------------------------
cGameInput::cGameInput()
{
	g_GameInput = this;
	g_GameInput->m_InputManager = new cInputManager();
	AddInputModifier( eInputGroup_Player, eInputControl_Test, 0.0f );
	AddInputCtrlModifier( eInputDeviceType_Keyboard, DIK_H, 1.0f );
}


//----------------------------------------------------------------------------------------------------
cGameInput::~cGameInput()
{
	for( tInputModifierPtrVec::iterator inputIt = m_GameInput.begin(); inputIt != m_GameInput.end(); ++inputIt )
	{
		cInputModifier * inputMod = *inputIt;
		delete inputMod;
		inputMod = NULL;
	}

	//if( m_InputManager )
	//{
	//	delete m_InputManager;
	//	m_InputManager = NULL;
	//}
}


//----------------------------------------------------------------------------------------------------
void cGameInput::AddInputModifier( eInputGroup pGroupType, eInputControl pInputCtrl, float pDefVal )
{
	cInputModifier * inputMod = new cInputModifier( pGroupType, pDefVal );
	m_LastInputModifier = inputMod;
	m_LastInputCtrl = pInputCtrl;
}


//----------------------------------------------------------------------------------------------------
void cGameInput::AddInputCtrlModifier( eInputDeviceType pInputType, int pKeyCode, float pValue )
{
	m_LastInputModifier->AddInputCtrlModifier( pInputType, pKeyCode, pValue );
	m_GameInput.push_back( m_LastInputModifier );
}


//----------------------------------------------------------------------------------------------------
void cGameInput::Update()
{
	m_InputManager->Update();
	for( tInputModifierPtrVec::iterator it = m_GameInput.begin(); it != m_GameInput.end(); ++it )
	{
		bool keyPressed = false;
		cInputModifier * inputMod = *it;
		if( inputMod->m_DeviceType == eInputDeviceType_Keyboard )
		{
			if( m_InputManager->KeyUp( inputMod->m_InputCode ) )
			{
				keyPressed = true;
			}
		}
		else if( inputMod->m_DeviceType == eInputDeviceType_Mouse )
		{
			keyPressed = true;
		}

		//set the value
		if( keyPressed )
		{
			inputMod->m_CurrentValue = inputMod->m_OnValue;
		}
		else
		{
			inputMod->m_CurrentValue = inputMod->m_DefalutVal;
		}
	}
}