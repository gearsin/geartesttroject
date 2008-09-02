//----------------------------------------------------------------------------------------------------
#ifndef GAMEINPUT_H
#define GAMEINPUT_H


//------------------------------------------------------------------------------------------------------------------
#include "Input.h"


//----------------------------------------------------------------------------------------------------
enum eInputGroup
{
	eInputGroup_Gui,
	eInputGroup_Player,
	eInputGroup_Camera
};


//----------------------------------------------------------------------------------------------------
enum eInputControl
{
	eInputControl_Test = 0,
};


//----------------------------------------------------------------------------------------------------
class cGameInput
{
public:
	static void Create();
	static void Destroy();
	bool IsKeyDown( eInputControl pControl );
	float GetValue( eInputControl pControl );
	void Update();

private:
	cGameInput();
	virtual ~cGameInput();
	void AddInputModifier( eInputGroup pGroup, eInputControl pInputCtrl, float pDefVal );
	void AddInputCtrlModifier( eInputDeviceType pInputType, int pKeyCode, float pValue );

private:
	class cInputModifier
	{
		public:
			cInputModifier( eInputGroup pGroup, float pDefVal );
			~cInputModifier();
			void AddInputCtrlModifier( eInputDeviceType pInputType, int pKeyCode, float pValue );

		public:
			eInputGroup			m_Group;
			eInputDeviceType	m_DeviceType;
			int					m_InputCode;
			float				m_DefalutVal;
			float				m_OnValue;
			float				m_CurrentValue;
	};
	typedef std::vector< cInputModifier * > tInputModifierPtrVec;

private:
	tInputModifierPtrVec	m_GameInput;
	eInputControl			m_LastInputCtrl;
	cInputModifier	*		m_LastInputModifier;
	cInputManager	*		m_InputManager;

protected:

};


//----------------------------------------------------------------------------------------------------
extern cGameInput * g_GameInput;


//----------------------------------------------------------------------------------------------------
inline bool cGameInput::IsKeyDown( eInputControl pControl )
{
	return ( m_GameInput[pControl]->m_CurrentValue == m_GameInput[pControl]->m_OnValue );
}


//----------------------------------------------------------------------------------------------------
inline float cGameInput::GetValue( eInputControl pControl )
{
	return m_GameInput[pControl]->m_CurrentValue;
}


//----------------------------------------------------------------------------------------------------
#endif