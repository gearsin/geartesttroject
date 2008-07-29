//------------------------------------------------------------------------------------------------------------------
#ifndef CALLBACK_H
#define CALLBACK_H


//------------------------------------------------------------------------------------------------------------------
bool    CALLBACK IsDeviceAcceptable( D3DCAPS9* pCaps, D3DFORMAT pAdapterFormat, D3DFORMAT pBackbufferFormat, bool pWindowed, void * pUserContext );
void    CALLBACK OnDestroyDevice( void * pUserContext );
void	CALLBACK OnLostDevice( void * pUserContext );
void	CALLBACK OnUpdateGame( double pTime, float pElapsedTime, void * pUserContext );
void	CALLBACK OnFrameRender( IDirect3DDevice9 * pDevice, double pTime, float pElapsedTime, void * pUserContext );
void	CALLBACK OnKeyPress( UINT pChar, bool pKeyDown, bool pAltDown, void * pUserContext );
bool	CALLBACK OnModifyDeviceSettings( DXUTDeviceSettings * pDeviceSettings, void * pUserContext );
HRESULT CALLBACK OnCreateDevice( IDirect3DDevice9 * pDevice, const D3DSURFACE_DESC * pBackBufferSurfaceDesc, void * pUserContext );
HRESULT CALLBACK OnResetDevice( IDirect3DDevice9 * pDevice, const D3DSURFACE_DESC * pBackBufferSurfaceDesc, void * pUserContext );
LRESULT CALLBACK MsgProc( HWND pHwnd, UINT pMsg, WPARAM pWparam, LPARAM pLparam, bool * pNoFurtherProcessing, void * pUserContext );


//------------------------------------------------------------------------------------------------------------------
#endif