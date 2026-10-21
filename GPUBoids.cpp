
// [LAB3][BOIDS] КАРТА КОДА: GPUBoids.cpp управляет D3D11-ресурсами и этапами симуляции.
// [LAB3][BOIDS] OnD3D11FrameRender() -> GenerateMips() -> AccumulateForces()/AccumulateForcesGS()
// [LAB3][BOIDS] -> AdvanceParticles() -> SwapParticleTextures() -> RenderMeshInstanced().
// [LAB3][BOIDS] Математика Boids (Separation/Alignment/Cohesion и др.) находится в GPUBoids.fx.
// [LAB3][BOIDS] Пояснения помечены [LAB3][BOIDS]; программная логика не изменена.
#include "DXUT.h"
#include "DXUTgui.h"
#include "DXUTsettingsdlg.h"
#include "DXUTcamera.h"
#include "SDKmisc.h"
#include "SDKmesh.h"
#include "d3dx11effect.h"
#include "resource.h"

#include <cstring>
#include <cstdio>

//--------------------------------------------------------------------------------------
// Global variables
//--------------------------------------------------------------------------------------
CModelViewerCamera                  g_Camera;               // A model viewing camera
CDXUTDialogResourceManager          g_DialogResourceManager; // manager for shared resources of dialogs
CD3DSettingsDlg                     g_D3DSettingsDlg;       // Device settings dialog
CDXUTDialog                         g_HUD;                  // manages the 3D UI
CDXUTDialog                         g_SampleUI;             // dialog for sample specific controls
bool                                g_bFirst = true;
bool                                g_bAccumulateWithGS = false;

CDXUTTextHelper*                    g_pTxtHelper = NULL;
ID3DX11Effect*                      g_pEffect11 = NULL;
ID3D11InputLayout*                  g_pParticleVertexLayout = NULL;
ID3D11InputLayout*                  g_pSplatVertexLayout = NULL;
ID3D11InputLayout*                  g_pMeshVertexLayout = NULL;

CDXUTSDKMesh                        g_BoidMesh;
CDXUTSDKMesh                        g_FleeMesh;

ID3D11Buffer*                       g_pParticleBuffer = NULL;
ID3D11Buffer*                       g_pSplatBuffer = NULL;


// [LAB3][BOIDS] Две текстуры состояния агента: From читается шейдерами, To принимает новый кадр.
// [LAB3][BOIDS] Это ping-pong: после обновления From и To меняются ролями без копирования данных.
ID3D11Texture2D*                    g_pParticleDataTextureTo = NULL;
ID3D11ShaderResourceView*           g_pParticleDataTexSRVTo = NULL;
ID3D11RenderTargetView*             g_pParticleDataTexRTVTo = NULL;
ID3D11Texture2D*                    g_pParticleDataTextureFrom = NULL;
ID3D11ShaderResourceView*           g_pParticleDataTexSRVFrom = NULL;
ID3D11RenderTargetView*             g_pParticleDataTexRTVFrom = NULL;
ID3D11Texture2D*                    g_pForceTexture = NULL;
ID3D11ShaderResourceView*           g_pForceTexSRV = NULL;
ID3D11RenderTargetView*             g_pForceTexRTV = NULL;


// [LAB3][BOIDS] Техники GPUBoids.fx: отдельные проходы накопления сил, интегрирования и отрисовки.
// [LAB3][BOIDS] AccumulateForcesGS и AccumulateForces дают один результат двумя способами.
ID3DX11EffectTechnique*              g_pRenderParticles;
ID3DX11EffectTechnique*              g_pAccumulateForces;
ID3DX11EffectTechnique*              g_pAccumulateForcesGS;
ID3DX11EffectTechnique*              g_pAdvanceParticles;
ID3DX11EffectTechnique*              g_pRenderBoids;
ID3DX11EffectTechnique*              g_pRenderMesh;

ID3DX11EffectMatrixVariable*         g_pmWorldViewProj;
ID3DX11EffectMatrixVariable*         g_pmWorld;
ID3DX11EffectMatrixVariable*         g_pmInvView;
ID3DX11EffectScalarVariable*         g_pfGlobalTime;
ID3DX11EffectScalarVariable*         g_pfElapsedTime;
ID3DX11EffectScalarVariable*         g_piTexSize;
ID3DX11EffectScalarVariable*         g_piMaxMip;
ID3DX11EffectScalarVariable*         g_pfParticleRad;
ID3DX11EffectScalarVariable*         g_pfParticleMass;
ID3DX11EffectScalarVariable*         g_pfG;
ID3DX11EffectVectorVariable*         g_pvFleePos;
ID3DX11EffectScalarVariable*         g_pfBoidScale;

ID3DX11EffectShaderResourceVariable* g_pDiffuseTex;
ID3DX11EffectShaderResourceVariable* g_pParticleDataTex;
ID3DX11EffectShaderResourceVariable* g_pForceTex;


// [LAB3][BOIDS] Положение шара, от которого агенты уклоняются через силу Flee().
// [LAB3][BOIDS] Это не конечный автомат агента и не система поиска пути.
DirectX::XMFLOAT4                         g_vFleePos( 700,0,0,1 );


struct PARTICLE_VERTEX
{
    DirectX::XMFLOAT4 Color;
};


struct SPLAT_VERTEX
{
    DirectX::XMFLOAT3 Pos;
    DirectX::XMFLOAT2 Tex;
};


// [LAB3][BOIDS] Число визуализируемых агентов и число элементов, передаваемых в GPU-проходы.
// [LAB3][BOIDS] Сами агенты не представлены объектами C++; динамические данные лежат в текстурах.
#define MAX_PARTICLES 4096
UINT                                g_iTexSize;
UINT                                g_iMaxMip;
float                               g_fParticleMass = 10000.0f * 10000.0f;
float                               g_fGConstant = 6.67300e-11f * 10000.0f;
float                               g_fSpread = 50.0f;
float                               g_fParticleRad = 1.0f;
float                               g_fBoidScale = 3.0f;


//--------------------------------------------------------------------------------------
// UI control IDs
//--------------------------------------------------------------------------------------
#define IDC_TOGGLEFULLSCREEN    1
#define IDC_TOGGLEREF           3
#define IDC_CHANGEDEVICE        4
#define IDC_TOGGLEWARP          5

#define IDC_ACCUMULATEWITHGS	10
#define IDC_BOIDSCALE_STATIC	11
#define IDC_BOIDSCALE			12


//--------------------------------------------------------------------------------------
// Forward declarations 
//--------------------------------------------------------------------------------------
bool CALLBACK ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, void* pUserContext );
void CALLBACK OnFrameMove( double fTime, float fElapsedTime, void* pUserContext );
LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing, void* pUserContext );
void CALLBACK KeyboardProc( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext );
void CALLBACK OnGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext );

bool CALLBACK IsD3D11DeviceAcceptable( const CD3D11EnumAdapterInfo* pAdapterInfo, UINT Output, const CD3D11EnumDeviceInfo* pDeviceInfo, DXGI_FORMAT BackBufferFormat, bool bWindowed, void* pUserContext );
HRESULT CALLBACK OnD3D11CreateDevice( ID3D11Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );
HRESULT CALLBACK OnD3D11SwapChainResized( ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );
void CALLBACK OnD3D11SwapChainReleasing( void* pUserContext );
void CALLBACK OnD3D11DestroyDevice( void* pUserContext );
void CALLBACK OnD3D11FrameRender( ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dContext, double fTime, float fElapsedTime, void* pUserContext );

void InitApp();
void RenderText();

HRESULT CreateParticleTextures( ID3D11Device* pd3dDevice );
HRESULT CreateParticleBuffer( ID3D11Device* pd3dDevice );
HRESULT CreateForceTexture( ID3D11Device* pd3dDevice );
HRESULT CreateSplatBuffer( ID3D11Device* pd3dDevice );

//--------------------------------------------------------------------------------------
// Entry point to the program. Initializes everything and goes into a message processing 
// loop. Idle time is used to render the scene.
//--------------------------------------------------------------------------------------
int WINAPI wWinMain( HINSTANCE , HINSTANCE , LPWSTR , int  )
{
    // Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

    // DXUT will create and use the best device (either D3D9 or D3D11) 
    // that is available on the system depending on which D3D callbacks are set below

    // Set DXUT callbacks
    DXUTSetCallbackDeviceChanging( ModifyDeviceSettings );
    DXUTSetCallbackMsgProc( MsgProc );
    DXUTSetCallbackKeyboard( KeyboardProc );
    DXUTSetCallbackFrameMove( OnFrameMove );
    DXUTSetCallbackD3D11DeviceAcceptable( IsD3D11DeviceAcceptable );
    DXUTSetCallbackD3D11DeviceCreated( OnD3D11CreateDevice );
    DXUTSetCallbackD3D11SwapChainResized( OnD3D11SwapChainResized );
    DXUTSetCallbackD3D11SwapChainReleasing( OnD3D11SwapChainReleasing );
    DXUTSetCallbackD3D11DeviceDestroyed( OnD3D11DestroyDevice );
    DXUTSetCallbackD3D11FrameRender( OnD3D11FrameRender );

    InitApp();
    DXUTInit( true, true, NULL ); // Parse the command line, show msgboxes on error, no extra command line params
    DXUTSetCursorSettings( true, true ); // Show the cursor and clip it when in full screen
    DXUTCreateWindow( L"GPUBoids" );
    DXUTCreateDevice( D3D_FEATURE_LEVEL_11_0, true, 1024, 768 );
    DXUTMainLoop(); // Enter into the DXUT render loop
}


//--------------------------------------------------------------------------------------
// Initialize the app 
//--------------------------------------------------------------------------------------
void InitApp()
{
    g_D3DSettingsDlg.Init( &g_DialogResourceManager );
    g_HUD.Init( &g_DialogResourceManager );
    g_SampleUI.Init( &g_DialogResourceManager );

    g_HUD.SetCallback( OnGUIEvent ); int iY = 10;
    g_HUD.AddButton( IDC_TOGGLEFULLSCREEN, L"Toggle full screen", 35, iY, 125, 22 );
    g_HUD.AddButton( IDC_CHANGEDEVICE, L"Change device (F2)", 35, iY += 24, 125, 22, VK_F2 );
    g_HUD.AddButton( IDC_TOGGLEREF, L"Toggle REF (F3)", 35, iY += 24, 125, 22, VK_F3 );
    g_HUD.AddButton( IDC_TOGGLEWARP, L"Toggle WARP (F4)", 35, iY += 24, 125, 22, VK_F4 );

    g_SampleUI.SetCallback( OnGUIEvent ); iY = 10;
    g_SampleUI.AddCheckBox( IDC_ACCUMULATEWITHGS, L"Accumulate With GS", 35, iY += 24, 125, 22, g_bAccumulateWithGS );

    WCHAR str[MAX_PATH];
    swprintf_s( str, MAX_PATH, L"BoidSize: %.2f", g_fBoidScale );
    g_SampleUI.AddStatic( IDC_BOIDSCALE_STATIC, str, 35, iY += 24, 125, 22 );
    g_SampleUI.AddSlider( IDC_BOIDSCALE, 35, iY += 24, 125, 22, 0, 500, ( int )( g_fBoidScale * 100 ) );
}


//--------------------------------------------------------------------------------------
// Called right before creating a D3D9 or D3D11 device, allowing the app to modify the device settings as needed
//--------------------------------------------------------------------------------------
bool CALLBACK ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, void*  )
{
    // For the first device created if its a REF device, optionally display a warning dialog box
    pDeviceSettings->d3d11.SyncInterval = 0; // VSync off; this field is an interval, not DXGI_SWAP_EFFECT.
    //g_D3DSettingsDlg.GetDialogControl()->GetComboBox( DXUTSETTINGSDLG_PRESENT_INTERVAL )->SetEnabled( false );

    //static bool s_bFirstTime = true;
    //if( s_bFirstTime )
    //{
    //    s_bFirstTime = false;
    //    //if( ( DXUT_D3D9_DEVICE == pDeviceSettings->ver && pDeviceSettings->d3d9.DeviceType == D3DDEVTYPE_REF ) ||
    //    //    ( DXUT_D3D11_DEVICE == pDeviceSettings->ver &&
    //    //      pDeviceSettings->d3d11.DriverType == D3D_DRIVER_TYPE_REFERENCE ) )
    //    //    DXUTDisplaySwitchingToREFWarning( pDeviceSettings->ver );
    //}

    return true;
}


//--------------------------------------------------------------------------------------
// Handle updates to the scene.  This is called regardless of which D3D API is used
//--------------------------------------------------------------------------------------
// [LAB3][BOIDS] Обновление управления: WASDQE перемещают шар g_vFleePos, влияющий на Flee().
// [LAB3][BOIDS] Это НЕ шаг симуляции Boids; вычисления выполняются в OnD3D11FrameRender().
void CALLBACK OnFrameMove( double , float fElapsedTime, void*  )
{
    // Update the camera's position based on user input 
    g_Camera.FrameMove( fElapsedTime );

    float fMoveAmt = 200.0f * fElapsedTime;
    if( GetAsyncKeyState( 'A' ) & 0x8000 )
        g_vFleePos.x -= fMoveAmt;
    if( GetAsyncKeyState( 'D' ) & 0x8000 )
        g_vFleePos.x += fMoveAmt;

    if( GetAsyncKeyState( 'W' ) & 0x8000 )
        g_vFleePos.z += fMoveAmt;
    if( GetAsyncKeyState( 'S' ) & 0x8000 )
        g_vFleePos.z -= fMoveAmt;

    if( GetAsyncKeyState( 'E' ) & 0x8000 )
        g_vFleePos.y += fMoveAmt;
    if( GetAsyncKeyState( 'Q' ) & 0x8000 )
        g_vFleePos.y -= fMoveAmt;
}


//--------------------------------------------------------------------------------------
// Handle messages to the application
//--------------------------------------------------------------------------------------
LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing, void*  )
{
    // Pass messages to dialog resource manager calls so GUI state is updated correctly
    *pbNoFurtherProcessing = g_DialogResourceManager.MsgProc( hWnd, uMsg, wParam, lParam );
    if( *pbNoFurtherProcessing )
        return 0;

    // Pass messages to settings dialog if its active
    if( g_D3DSettingsDlg.IsActive() )
    {
        g_D3DSettingsDlg.MsgProc( hWnd, uMsg, wParam, lParam );
        return 0;
    }

    // Give the dialogs a chance to handle the message first
    *pbNoFurtherProcessing = g_HUD.MsgProc( hWnd, uMsg, wParam, lParam );
    if( *pbNoFurtherProcessing )
        return 0;
    *pbNoFurtherProcessing = g_SampleUI.MsgProc( hWnd, uMsg, wParam, lParam );
    if( *pbNoFurtherProcessing )
        return 0;

    // Pass all remaining windows messages to camera so it can respond to user input
    g_Camera.HandleMessages( hWnd, uMsg, wParam, lParam );

    return 0;
}


//--------------------------------------------------------------------------------------
// Handle key presses
//--------------------------------------------------------------------------------------
void CALLBACK KeyboardProc( UINT , bool , bool , void*  )
{
}


//--------------------------------------------------------------------------------------
// Handles the GUI events
//--------------------------------------------------------------------------------------
void CALLBACK OnGUIEvent( UINT , int nControlID, CDXUTControl* , void*  )
{
    switch( nControlID )
    {
        case IDC_TOGGLEFULLSCREEN:
            DXUTToggleFullScreen(); break;
        case IDC_TOGGLEREF:
            DXUTToggleREF(); break;
        case IDC_CHANGEDEVICE:
            g_D3DSettingsDlg.SetActive( !g_D3DSettingsDlg.IsActive() ); break;
        case IDC_TOGGLEWARP:
            DXUTToggleWARP(); break;
        case IDC_ACCUMULATEWITHGS:
            g_bAccumulateWithGS = !g_bAccumulateWithGS; break;

        case IDC_BOIDSCALE:
        {
            g_fBoidScale = ( float )g_SampleUI.GetSlider( IDC_BOIDSCALE )->GetValue() / 100.0f;
            WCHAR str[MAX_PATH];
            swprintf_s( str, MAX_PATH, L"BoidSize: %.2f", g_fBoidScale );
            g_SampleUI.GetStatic( IDC_BOIDSCALE_STATIC )->SetText( str );
        }
            break;
    }
}


//--------------------------------------------------------------------------------------
// Reject any D3D11 devices that aren't acceptable by returning false
//--------------------------------------------------------------------------------------
bool CALLBACK IsD3D11DeviceAcceptable( const CD3D11EnumAdapterInfo* , UINT , const CD3D11EnumDeviceInfo* , DXGI_FORMAT , bool , void*  )
{
    return true;
}


//--------------------------------------------------------------------------------------
// Create any D3D11 resources that aren't dependant on the back buffer
//--------------------------------------------------------------------------------------
// При отказе callback DXUT не вызывает OnD3D11DestroyDevice: освобождаем ресурсы сами.
// Название неудачного этапа и HRESULT показываем сразу, пока сообщение не скрыто DXUT.
HRESULT ReportD3D11DeviceCreateFailure( const char* pStage, HRESULT hr, const char* pDetails = NULL )
{
    char message[4096];
    sprintf_s( message, "Failed to initialize GPUBoids (Direct3D 11).\nStage: %s\nHRESULT: 0x%08X\n%.*s", pStage, (unsigned int)hr, 3072, pDetails ? pDetails : "" );
    OutputDebugStringA( message );
    MessageBoxA( DXUTGetHWND(), message, "GPUBoids - Initialization Error", MB_OK | MB_ICONERROR );
    OnD3D11DestroyDevice( NULL );
    return hr;
}


// [LAB3][BOIDS] Инициализация: загрузка HLSL Effect, получение техник и создание GPU-ресурсов.
HRESULT CALLBACK OnD3D11CreateDevice( ID3D11Device* pd3dDevice, const DXGI_SURFACE_DESC* , void*  )
{
    HRESULT hr;

    // DXUT11 хранит немедленный контекст отдельно от ID3D11Device.
    ID3D11DeviceContext* pd3dContext = DXUTGetD3D11DeviceContext();
    hr = g_DialogResourceManager.OnD3D11CreateDevice( pd3dDevice, pd3dContext );
    if( FAILED( hr ) )
        return ReportD3D11DeviceCreateFailure( "DXUT dialog resource initialization", hr );
    hr = g_D3DSettingsDlg.OnD3D11CreateDevice( pd3dDevice );
    if( FAILED( hr ) )
        return ReportD3D11DeviceCreateFailure( "DXUT device settings initialization", hr );
    g_pTxtHelper = new CDXUTTextHelper( pd3dDevice, pd3dContext, &g_DialogResourceManager, 15 );

    // Read the D3DX effect file
    WCHAR str[MAX_PATH];
    hr = DXUTFindDXSDKMediaFileCch( str, MAX_PATH, L"GPUBoids.fx" );
    if( FAILED( hr ) )
        return ReportD3D11DeviceCreateFailure( "Locate GPUBoids.fx", hr );
    UINT dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
    dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif
    // [LAB3][BOIDS] Effects11 компилирует HLSL-исходник .fx через D3DX11CompileEffectFromFile().
    // [LAB3][BOIDS] Функции поведения Seek/Separation/Alignment/Cohesion/Flee/Avoid находятся в GPUBoids.fx.
    ID3DBlob* pEffectErrors = NULL;
    hr = D3DX11CompileEffectFromFile( str, NULL, NULL, dwShaderFlags, 0, pd3dDevice, &g_pEffect11, &pEffectErrors );
    if( FAILED( hr ) )
    {
        const char* pDetails = pEffectErrors ? (const char*)pEffectErrors->GetBufferPointer() : NULL;
        if( pDetails )
            OutputDebugStringA( pDetails );
        HRESULT result = ReportD3D11DeviceCreateFailure( "Compile GPUBoids.fx (Effects11)", hr, pDetails );
        SAFE_RELEASE( pEffectErrors );
        return result;
    }
    if( pEffectErrors )
    {
        OutputDebugStringA( (const char*)pEffectErrors->GetBufferPointer() );
        SAFE_RELEASE( pEffectErrors );
    }

    // Obtain the technique handles
    g_pRenderParticles = g_pEffect11->GetTechniqueByName( "RenderParticles" );
    // [LAB3][BOIDS] Две альтернативы одного этапа: PSForce() + аддитивное смешивание.
    // [LAB3][BOIDS] GS-версия формирует квадраты в geometry shader, обычная берёт готовые треугольники.
    g_pAccumulateForces = g_pEffect11->GetTechniqueByName( "AccumulateForces" );
    g_pAccumulateForcesGS = g_pEffect11->GetTechniqueByName( "AccumulateForcesGS" );
    g_pAdvanceParticles = g_pEffect11->GetTechniqueByName( "AdvanceParticles" );
    g_pRenderBoids = g_pEffect11->GetTechniqueByName( "RenderBoids" );
    g_pRenderMesh = g_pEffect11->GetTechniqueByName( "RenderMesh" );

    // Obtain the parameter handles
    g_pmWorldViewProj = g_pEffect11->GetVariableByName( "g_mWorldViewProj" )->AsMatrix();
    g_pmWorld = g_pEffect11->GetVariableByName( "g_mWorld" )->AsMatrix();
    g_pmInvView = g_pEffect11->GetVariableByName( "g_mInvView" )->AsMatrix();
    g_pfGlobalTime = g_pEffect11->GetVariableByName( "g_fGlobalTime" )->AsScalar();
    g_pfElapsedTime = g_pEffect11->GetVariableByName( "g_fElapsedTime" )->AsScalar();
    g_piTexSize = g_pEffect11->GetVariableByName( "g_iTexSize" )->AsScalar();
    g_piMaxMip = g_pEffect11->GetVariableByName( "g_iMaxMip" )->AsScalar();
    g_pfParticleRad = g_pEffect11->GetVariableByName( "g_fParticleRad" )->AsScalar();
    g_pfParticleMass = g_pEffect11->GetVariableByName( "g_fParticleMass" )->AsScalar();
    g_pfG = g_pEffect11->GetVariableByName( "g_fG" )->AsScalar();
    g_pvFleePos = g_pEffect11->GetVariableByName( "g_vFleePos" )->AsVector();
    g_pfBoidScale = g_pEffect11->GetVariableByName( "g_fBoidScale" )->AsScalar();

    g_pDiffuseTex = g_pEffect11->GetVariableByName( "g_txDiffuse" )->AsShaderResource();
    g_pParticleDataTex = g_pEffect11->GetVariableByName( "g_txParticleData" )->AsShaderResource();
    g_pForceTex = g_pEffect11->GetVariableByName( "g_txForce" )->AsShaderResource();

    // Create our vertex input layout
    const D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    D3DX11_PASS_DESC PassDesc;
    hr = g_pRenderParticles->GetPassByIndex( 0 )->GetDesc( &PassDesc );
    if( FAILED( hr ) )
        return ReportD3D11DeviceCreateFailure( "Read RenderParticles input signature", hr );
    hr = pd3dDevice->CreateInputLayout( layout, sizeof( layout ) / sizeof( layout[0] ),
                                             PassDesc.pIAInputSignature,
                                             PassDesc.IAInputSignatureSize, &g_pParticleVertexLayout );
    if( FAILED( hr ) )
        return ReportD3D11DeviceCreateFailure( "Create particle input layout", hr );

    // Create our splat vertex input layout
    const D3D11_INPUT_ELEMENT_DESC splatlayout[] =
    {
        { "POS",      0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    hr = g_pAccumulateForces->GetPassByIndex( 0 )->GetDesc( &PassDesc );
    if( FAILED( hr ) )
        return ReportD3D11DeviceCreateFailure( "Read AccumulateForces input signature", hr );
    hr = pd3dDevice->CreateInputLayout( splatlayout, sizeof( splatlayout ) / sizeof( splatlayout[0] ),
                                             PassDesc.pIAInputSignature,
                                             PassDesc.IAInputSignatureSize, &g_pSplatVertexLayout );
    if( FAILED( hr ) )
        return ReportD3D11DeviceCreateFailure( "Create splat input layout", hr );

    // Create our mesh vertex input layout
    const D3D11_INPUT_ELEMENT_DESC meshlayout[] =
    {
        { "POS",      0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    hr = g_pRenderBoids->GetPassByIndex( 0 )->GetDesc( &PassDesc );
    if( FAILED( hr ) )
        return ReportD3D11DeviceCreateFailure( "Read RenderBoids input signature", hr );
    hr = pd3dDevice->CreateInputLayout( meshlayout, sizeof( meshlayout ) / sizeof( meshlayout[0] ),
                                             PassDesc.pIAInputSignature,
                                             PassDesc.IAInputSignatureSize, &g_pMeshVertexLayout );
    if( FAILED( hr ) )
        return ReportD3D11DeviceCreateFailure( "Create boid mesh input layout", hr );

    // Create a mesh to avoid
    // [LAB3][BOIDS] Отдельная геометрия: модель boid для instanced drawing и шар для точки Flee().
    hr = g_BoidMesh.Create( pd3dDevice, L"GPUBoids\\boid.sdkmesh" );
    if( FAILED( hr ) )
        return ReportD3D11DeviceCreateFailure( "Load GPUBoids/boid.sdkmesh", hr );
    hr = g_FleeMesh.Create( pd3dDevice, L"misc\\ball.sdkmesh" );
    if( FAILED( hr ) )
        return ReportD3D11DeviceCreateFailure( "Load misc/ball.sdkmesh", hr );

    // Create the seeding particle
    // [LAB3][BOIDS] Состояния в texture array, фиктивный vertex buffer, текстура суммарных сил
    // [LAB3][BOIDS] и геометрия splat-квадратов (для способа накопления без geometry shader).
    hr = CreateParticleTextures( pd3dDevice );
    if( FAILED( hr ) )
        return ReportD3D11DeviceCreateFailure( "Create particle state textures", hr );
    hr = CreateParticleBuffer( pd3dDevice );
    if( FAILED( hr ) )
        return ReportD3D11DeviceCreateFailure( "Create particle vertex buffer", hr );
    hr = CreateForceTexture( pd3dDevice );
    if( FAILED( hr ) )
        return ReportD3D11DeviceCreateFailure( "Create force texture", hr );
    hr = CreateSplatBuffer( pd3dDevice );
    if( FAILED( hr ) )
        return ReportD3D11DeviceCreateFailure( "Create splat vertex buffer", hr );

    // Setup the camera's view parameters
    DirectX::XMVECTOR vecEye = DirectX::XMVectorSet( 0.0f, 0.0f, -g_fSpread * 15.0f, 0.0f);
    DirectX::XMVECTOR vecAt = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
    g_Camera.SetViewParams( vecEye, vecAt );

    return S_OK;
}


//--------------------------------------------------------------------------------------
// Create any D3D11 resources that depend on the back buffer
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnD3D11SwapChainResized( ID3D11Device* pd3dDevice, IDXGISwapChain* , const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void*  )
{
    HRESULT hr = S_OK;

    V_RETURN( g_DialogResourceManager.OnD3D11ResizedSwapChain( pd3dDevice, pBackBufferSurfaceDesc ) );
    V_RETURN( g_D3DSettingsDlg.OnD3D11ResizedSwapChain( pd3dDevice, pBackBufferSurfaceDesc ) );

    // Setup the camera's projection parameters
    float fAspectRatio = pBackBufferSurfaceDesc->Width / ( FLOAT )pBackBufferSurfaceDesc->Height;
    g_Camera.SetProjParams( DirectX::XM_PIDIV4, fAspectRatio, 0.5f, 5000.0f );
    g_Camera.SetWindow( pBackBufferSurfaceDesc->Width, pBackBufferSurfaceDesc->Height );
    g_Camera.SetButtonMasks( 0, MOUSE_WHEEL, MOUSE_LEFT_BUTTON | MOUSE_MIDDLE_BUTTON | MOUSE_RIGHT_BUTTON );

    g_HUD.SetLocation( pBackBufferSurfaceDesc->Width - 170, 0 );
    g_HUD.SetSize( 170, 170 );
    g_SampleUI.SetLocation( pBackBufferSurfaceDesc->Width - 170, pBackBufferSurfaceDesc->Height - 300 );
    g_SampleUI.SetSize( 170, 300 );

    g_bFirst = true;
    return hr;
}


//--------------------------------------------------------------------------------------
// [LAB3][BOIDS] Альтернативная визуализация частиц спрайтами; НЕ вычисляет поведение Boids.
// [LAB3][BOIDS] Основная текущая визуализация использует RenderMeshInstanced().
bool RenderParticles( ID3D11DeviceContext* pd3dContext, const DirectX::XMMATRIX& mView, const DirectX::XMMATRIX& mProj )
{
    DirectX::XMMATRIX mWorldViewProj = DirectX::XMMatrixMultiply( mView, mProj );
    DirectX::XMFLOAT4X4 mWorldViewProjData;
    DirectX::XMStoreFloat4x4( &mWorldViewProjData, mWorldViewProj );

    // Set the Vertex Layout
    pd3dContext->IASetInputLayout( g_pParticleVertexLayout );

    // Set IA parameters
    ID3D11Buffer* pBuffers[1] = { g_pParticleBuffer };
    UINT stride[1] = { sizeof( PARTICLE_VERTEX ) };
    UINT offset[1] = { 0 };
    pd3dContext->IASetVertexBuffers( 0, 1, pBuffers, stride, offset );
    pd3dContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_POINTLIST );

    // Set Effects Parameters
    g_pmWorldViewProj->SetMatrix( &mWorldViewProjData._11 );
    DirectX::XMFLOAT4X4 mInvViewData;
    DirectX::XMStoreFloat4x4( &mInvViewData, DirectX::XMMatrixInverse( nullptr, mView ) );
    g_pmInvView->SetMatrix( &mInvViewData._11 );
    g_piTexSize->SetInt( g_iTexSize );
    g_piMaxMip->SetInt( g_iMaxMip );
    g_pfParticleRad->SetFloat( g_fParticleRad );

    // Set resources
    g_pParticleDataTex->SetResource( g_pParticleDataTexSRVFrom );
    g_pForceTex->SetResource( g_pForceTexSRV );

    // Draw
    D3DX11_TECHNIQUE_DESC techDesc;
    g_pRenderParticles->GetDesc( &techDesc );

    for( UINT p = 0; p < techDesc.Passes; ++p )
    {
        g_pRenderParticles->GetPassByIndex( p )->Apply( 0, pd3dContext );
        pd3dContext->Draw( MAX_PARTICLES, 0 );
    }

    ID3D11ShaderResourceView* pNULLs[4] = {0,0,0,0};
    pd3dContext->VSSetShaderResources( 0, 4, pNULLs );
    pd3dContext->PSSetShaderResources( 0, 4, pNULLs );

    return true;
}


//--------------------------------------------------------------------------------------
// [LAB3][BOIDS] ЭТАП 2A: накопление взаимодействий на GPU через geometry shader.
// [LAB3][BOIDS] Каждая точка становится экранным квадратом; PSForce() вычисляет пару
// [LAB3][BOIDS] Separation() + Avoid(), а аддитивный blend суммирует вклад агентов в g_txForce.
bool AccumulateForcesGS( ID3D11DeviceContext* pd3dContext )
{
    // Clear the force texture
    float ClearColor[4] = {0,0,0,0};
    pd3dContext->ClearRenderTargetView( g_pForceTexRTV, ClearColor );

    // Store the old render targets
    ID3D11RenderTargetView* pOldRTV;
    ID3D11DepthStencilView* pOldDSV;
    pd3dContext->OMGetRenderTargets( 1, &pOldRTV, &pOldDSV );

    // Store the old viewport
    UINT iNumVP = 1;
    D3D11_VIEWPORT OldVP;
    pd3dContext->RSGetViewports( &iNumVP, &OldVP );

    // Set the new render targets
    pd3dContext->OMSetRenderTargets( 1, &g_pForceTexRTV, NULL );

    // Set the new viewport
    D3D11_VIEWPORT NewVP;
    NewVP.Width = g_iTexSize;
    NewVP.Height = g_iTexSize;
    NewVP.TopLeftX = 0;
    NewVP.TopLeftY = 0;
    NewVP.MinDepth = 0.0f;
    NewVP.MaxDepth = 1.0f;
    pd3dContext->RSSetViewports( 1, &NewVP );

    // Set the Vertex Layout
    pd3dContext->IASetInputLayout( g_pParticleVertexLayout );

    // Set IA parameters
    ID3D11Buffer* pBuffers[1] = { g_pParticleBuffer };
    UINT stride[1] = { sizeof( PARTICLE_VERTEX ) };
    UINT offset[1] = { 0 };
    pd3dContext->IASetVertexBuffers( 0, 1, pBuffers, stride, offset );
    pd3dContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_POINTLIST );

    // Set Effects Parameters
    g_piTexSize->SetInt( g_iTexSize );
    g_piMaxMip->SetInt( g_iMaxMip );
    g_pfParticleMass->SetFloat( g_fParticleMass );
    g_pfG->SetFloat( g_fGConstant );
    g_pfParticleRad->SetFloat( g_fParticleRad );

    // Set resources
    g_pParticleDataTex->SetResource( g_pParticleDataTexSRVFrom );

    // Draw
    D3DX11_TECHNIQUE_DESC techDesc;
    // [LAB3][BOIDS] HLSL: technique AccumulateForcesGS -> VSForceGS -> GSForce -> PSForce.
    g_pAccumulateForcesGS->GetDesc( &techDesc );

    for( UINT p = 0; p < techDesc.Passes; ++p )
    {
        g_pAccumulateForcesGS->GetPassByIndex( p )->Apply( 0, pd3dContext );
        pd3dContext->Draw( MAX_PARTICLES, 0 );
    }

    // Restore the old vp
    pd3dContext->RSSetViewports( 1, &OldVP );

    // Restore the old render targets
    pd3dContext->OMSetRenderTargets( 1, &pOldRTV, pOldDSV );
    SAFE_RELEASE( pOldRTV );
    SAFE_RELEASE( pOldDSV );

    ID3D11ShaderResourceView* pNULLs[4] = {0,0,0,0};
    pd3dContext->VSSetShaderResources( 0, 4, pNULLs );
    pd3dContext->PSSetShaderResources( 0, 4, pNULLs );

    return true;
}


//--------------------------------------------------------------------------------------
// [LAB3][BOIDS] ЭТАП 2B: тот же расчёт сил, но без geometry shader.
// [LAB3][BOIDS] Для каждого агента заранее подготовлено по 6 вершин (2 треугольника).
// [LAB3][BOIDS] PSForce() и аддитивное смешивание остаются теми же, что и в варианте GS.
bool AccumulateForces( ID3D11DeviceContext* pd3dContext )
{
    // Clear the force texture
    float ClearColor[4] = {0,0,0,0};
    pd3dContext->ClearRenderTargetView( g_pForceTexRTV, ClearColor );

    // Store the old render targets
    ID3D11RenderTargetView* pOldRTV;
    ID3D11DepthStencilView* pOldDSV;
    pd3dContext->OMGetRenderTargets( 1, &pOldRTV, &pOldDSV );

    // Store the old viewport
    UINT iNumVP = 1;
    D3D11_VIEWPORT OldVP;
    pd3dContext->RSGetViewports( &iNumVP, &OldVP );

    // Set the new render targets
    pd3dContext->OMSetRenderTargets( 1, &g_pForceTexRTV, NULL );

    // Set the new viewport
    D3D11_VIEWPORT NewVP;
    NewVP.Width = g_iTexSize;
    NewVP.Height = g_iTexSize;
    NewVP.TopLeftX = 0;
    NewVP.TopLeftY = 0;
    NewVP.MinDepth = 0.0f;
    NewVP.MaxDepth = 1.0f;
    pd3dContext->RSSetViewports( 1, &NewVP );

    // Set the Vertex Layout
    pd3dContext->IASetInputLayout( g_pSplatVertexLayout );

    // Set IA parameters
    ID3D11Buffer* pBuffers[1] = { g_pSplatBuffer };
    UINT stride[1] = { sizeof( SPLAT_VERTEX ) };
    UINT offset[1] = { 0 };
    pd3dContext->IASetVertexBuffers( 0, 1, pBuffers, stride, offset );
    pd3dContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

    // Set Effects Parameters
    g_piTexSize->SetInt( g_iTexSize );
    g_piMaxMip->SetInt( g_iMaxMip );
    g_pfParticleMass->SetFloat( g_fParticleMass );
    g_pfG->SetFloat( g_fGConstant );
    g_pfParticleRad->SetFloat( g_fParticleRad );

    // Set resources
    g_pParticleDataTex->SetResource( g_pParticleDataTexSRVFrom );

    // Draw
    D3DX11_TECHNIQUE_DESC techDesc;
    // [LAB3][BOIDS] HLSL: technique AccumulateForces -> VSForce -> PSForce; GeometryShader = NULL.
    g_pAccumulateForces->GetDesc( &techDesc );

    for( UINT p = 0; p < techDesc.Passes; ++p )
    {
        g_pAccumulateForces->GetPassByIndex( p )->Apply( 0, pd3dContext );
        pd3dContext->Draw( MAX_PARTICLES * 6, 0 );
    }

    // Restore the old vp
    pd3dContext->RSSetViewports( 1, &OldVP );

    // Restore the old render targets
    pd3dContext->OMSetRenderTargets( 1, &pOldRTV, pOldDSV );
    SAFE_RELEASE( pOldRTV );
    SAFE_RELEASE( pOldDSV );

    ID3D11ShaderResourceView* pNULLs[4] = {0,0,0,0};
    pd3dContext->VSSetShaderResources( 0, 4, pNULLs );
    pd3dContext->PSSetShaderResources( 0, 4, pNULLs );

    return true;
}


//--------------------------------------------------------------------------------------
// [LAB3][BOIDS] ЭТАП 3: запуск интегрирования состояния ВСЕХ агентов на GPU.
// [LAB3][BOIDS] PSAdvance() читает позиции/скорости, складывает силы Boids, ограничивает их
// [LAB3][BOIDS] и записывает новые позицию, направление/скорость и up-вектор в текстуру To.
bool AdvanceParticles( ID3D11DeviceContext* pd3dContext, float fElapsedTime )
{
    // Store the old render targets
    ID3D11RenderTargetView* pOldRTV;
    ID3D11DepthStencilView* pOldDSV;
    pd3dContext->OMGetRenderTargets( 1, &pOldRTV, &pOldDSV );

    // Store the old viewport
    UINT iNumVP = 1;
    D3D11_VIEWPORT OldVP;
    pd3dContext->RSGetViewports( &iNumVP, &OldVP );

    // Set the new render targets
    pd3dContext->OMSetRenderTargets( 1, &g_pParticleDataTexRTVTo, NULL );

    // Set the new viewport
    D3D11_VIEWPORT NewVP;
    NewVP.Width = g_iTexSize;
    NewVP.Height = g_iTexSize;
    NewVP.TopLeftX = 0;
    NewVP.TopLeftY = 0;
    NewVP.MinDepth = 0.0f;
    NewVP.MaxDepth = 1.0f;
    pd3dContext->RSSetViewports( 1, &NewVP );

    // Set the Vertex Layout
    pd3dContext->IASetInputLayout( g_pParticleVertexLayout );

    // Set IA parameters
    ID3D11Buffer* pBuffers[1] = { g_pParticleBuffer };
    UINT stride[1] = { sizeof( PARTICLE_VERTEX ) };
    UINT offset[1] = { 0 };
    pd3dContext->IASetVertexBuffers( 0, 1, pBuffers, stride, offset );
    pd3dContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_POINTLIST );

    // Set Effects Parameters
    g_piTexSize->SetInt( g_iTexSize );
    g_piMaxMip->SetInt( g_iMaxMip );
    g_pfParticleMass->SetFloat( g_fParticleMass );
    g_pfElapsedTime->SetFloat( fElapsedTime );

    // Set resources
    g_pParticleDataTex->SetResource( g_pParticleDataTexSRVFrom );
    g_pForceTex->SetResource( g_pForceTexSRV );

    // Draw
    D3DX11_TECHNIQUE_DESC techDesc;
    // [LAB3][BOIDS] HLSL: technique AdvanceParticles -> VSAdvance -> GSAdvance -> PSAdvance.
    // [LAB3][BOIDS] Один GS-проход создаёт три fullscreen-квадрата: по одному на слой данных.
    g_pAdvanceParticles->GetDesc( &techDesc );

    for( UINT p = 0; p < techDesc.Passes; ++p )
    {
        g_pAdvanceParticles->GetPassByIndex( p )->Apply( 0, pd3dContext );
        // [LAB3][BOIDS] Достаточно одной входной точки: GSAdvance разворачивает её в три квадрата.
        pd3dContext->Draw( 1, 0 );
    }

    // Restore the old vp
    pd3dContext->RSSetViewports( 1, &OldVP );

    // Restore the old render targets
    pd3dContext->OMSetRenderTargets( 1, &pOldRTV, pOldDSV );
    SAFE_RELEASE( pOldRTV );
    SAFE_RELEASE( pOldDSV );

    ID3D11ShaderResourceView* pNULLs[4] = {0,0,0,0};
    pd3dContext->VSSetShaderResources( 0, 4, pNULLs );
    pd3dContext->PSSetShaderResources( 0, 4, pNULLs );

    return true;
}

//--------------------------------------------------------------------------------------
// [LAB3][BOIDS] ЭТАП 5: отрисовка всех агентов методом instancing (не расчёт поведения).
// [LAB3][BOIDS] VSBoids() получает SV_InstanceID, читает положение/ориентацию из texture array
// [LAB3][BOIDS] и строит матрицу ориентации для каждого экземпляра одной и той же модели.
void RenderMeshInstanced( ID3D11DeviceContext* pd3dContext, CDXUTSDKMesh* pMesh, ID3DX11EffectTechnique* pTechnique, const DirectX::XMMATRIX& mView, const DirectX::XMMATRIX& mProj )
{
    ID3D11Buffer* pVB[1];
    UINT Strides[1];
    UINT Offsets[1] = {0};

    pd3dContext->IASetInputLayout( g_pMeshVertexLayout );
    // Render the mesh instanced
    pVB[0] = pMesh->GetVB11( 0, 0 );
    Strides[0] = ( UINT )pMesh->GetVertexStride( 0, 0 );

    pd3dContext->IASetVertexBuffers( 0, 1, pVB, Strides, Offsets );
    pd3dContext->IASetIndexBuffer( pMesh->GetIB11( 0 ), pMesh->GetIBFormat11( 0 ), 0 );

    // Set Effects Parameters
    DirectX::XMFLOAT4X4 mWorldViewProjData;
    DirectX::XMStoreFloat4x4( &mWorldViewProjData, DirectX::XMMatrixMultiply( mView, mProj ) );
    g_pmWorldViewProj->SetMatrix( &mWorldViewProjData._11 );
    g_piTexSize->SetInt( g_iTexSize );
    g_piMaxMip->SetInt( g_iMaxMip );
    g_pfBoidScale->SetFloat( g_fBoidScale );

    // Set resources
    g_pParticleDataTex->SetResource( g_pParticleDataTexSRVFrom );

    SDKMESH_SUBSET* pSubset = NULL;
    D3D11_PRIMITIVE_TOPOLOGY PrimType;
    D3DX11_TECHNIQUE_DESC techDesc;
    pTechnique->GetDesc( &techDesc );
    for( UINT p = 0; p < techDesc.Passes; ++p )
    {
        for( UINT subset = 0; subset < pMesh->GetNumSubsets( 0 ); ++subset )
        {
            pSubset = pMesh->GetSubset( 0, subset );

            PrimType = pMesh->GetPrimitiveType11( ( SDKMESH_PRIMITIVE_TYPE )pSubset->PrimitiveType );
            pd3dContext->IASetPrimitiveTopology( PrimType );

            pTechnique->GetPassByIndex( p )->Apply( 0, pd3dContext );
            // [LAB3][BOIDS] Один instanced draw на subset: MAX_PARTICLES экземпляров одной boid-модели.
            pd3dContext->DrawIndexedInstanced( ( UINT )pSubset->IndexCount, MAX_PARTICLES, ( UINT )pSubset->IndexStart, ( INT )pSubset->VertexStart, 0 );
        }
    }

}

//--------------------------------------------------------------------------------------
// [LAB3][BOIDS] D3D11: CDXUTSDKMesh::Render() не принимает Effects11 technique,
// [LAB3][BOIDS] поэтому шар рисуем вручную, применяя эффект для каждого subset.
void RenderFleeMesh( ID3D11DeviceContext* pd3dContext )
{
    pd3dContext->IASetInputLayout( g_pMeshVertexLayout );
    D3DX11_TECHNIQUE_DESC techDesc;
    g_pRenderMesh->GetDesc( &techDesc );

    for( UINT mesh = 0; mesh < g_FleeMesh.GetNumMeshes(); ++mesh )
    {
        ID3D11Buffer* pVB = g_FleeMesh.GetVB11( mesh, 0 );
        UINT stride = g_FleeMesh.GetVertexStride( mesh, 0 );
        UINT offset = 0;
        pd3dContext->IASetVertexBuffers( 0, 1, &pVB, &stride, &offset );
        pd3dContext->IASetIndexBuffer( g_FleeMesh.GetIB11( mesh ), g_FleeMesh.GetIBFormat11( mesh ), 0 );

        for( UINT subset = 0; subset < g_FleeMesh.GetNumSubsets( mesh ); ++subset )
        {
            SDKMESH_SUBSET* pSubset = g_FleeMesh.GetSubset( mesh, subset );
            pd3dContext->IASetPrimitiveTopology( g_FleeMesh.GetPrimitiveType11( ( SDKMESH_PRIMITIVE_TYPE )pSubset->PrimitiveType ) );
            ID3D11ShaderResourceView* pDiffuse = NULL;
            if( pSubset->MaterialID != INVALID_MATERIAL )
            {
                SDKMESH_MATERIAL* pMaterial = g_FleeMesh.GetMaterial( ( UINT )pSubset->MaterialID );
                if( !IsErrorResource( pMaterial->pDiffuseRV11 ) )
                    pDiffuse = pMaterial->pDiffuseRV11;
            }
            g_pDiffuseTex->SetResource( pDiffuse );

            for( UINT pass = 0; pass < techDesc.Passes; ++pass )
            {
                g_pRenderMesh->GetPassByIndex( pass )->Apply( 0, pd3dContext );
                pd3dContext->DrawIndexed( ( UINT )pSubset->IndexCount, ( UINT )pSubset->IndexStart, ( INT )pSubset->VertexStart );
            }
        }
    }
}

//--------------------------------------------------------------------------------------
// [LAB3][BOIDS] ЭТАП 4: ping-pong. Не копируем 3D-состояние на CPU: меняем указатели и SRV/RTV.
// [LAB3][BOIDS] В следующем кадре вчерашняя To-текстура становится входной From.
void SwapParticleTextures()
{
    ID3D11Texture2D* Temp1 = g_pParticleDataTextureTo;
    ID3D11ShaderResourceView* Temp2 = g_pParticleDataTexSRVTo;
    ID3D11RenderTargetView* Temp3 = g_pParticleDataTexRTVTo;

    g_pParticleDataTextureTo = g_pParticleDataTextureFrom;
    g_pParticleDataTexSRVTo = g_pParticleDataTexSRVFrom;
    g_pParticleDataTexRTVTo = g_pParticleDataTexRTVFrom;

    g_pParticleDataTextureFrom = Temp1;
    g_pParticleDataTexSRVFrom = Temp2;
    g_pParticleDataTexRTVFrom = Temp3;
}

//--------------------------------------------------------------------------------------
// [LAB3][BOIDS] ОСНОВНОЙ ЦИКЛ BOIDS: этапы ниже идут последовательно на каждом кадре.
// [LAB3][BOIDS] CPU организует GPU-проходы; сами поведенческие формулы выполняются в GPUBoids.fx.
void CALLBACK OnD3D11FrameRender( ID3D11Device* , ID3D11DeviceContext* pd3dContext, double , float fElapsedTime, void*  )
{
    float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0 };

    // If the settings dialog is being shown, then
    // render it instead of rendering the app's scene
    if( g_D3DSettingsDlg.IsActive() )
    {
        g_D3DSettingsDlg.OnRender( fElapsedTime );
        return;
    }

    ID3D11RenderTargetView* pRTV = DXUTGetD3D11RenderTargetView();
    pd3dContext->ClearRenderTargetView( pRTV, ClearColor );
    ID3D11DepthStencilView* pDSV = DXUTGetD3D11DepthStencilView();
    pd3dContext->ClearDepthStencilView( pDSV, D3D11_CLEAR_DEPTH, 1.0, 0 );

    g_pvFleePos->SetFloatVector( &g_vFleePos.x );

    // Average the velocities and positions
    // [LAB3][BOIDS] ЭТАП 1: mipmap-усреднение текстур состояния; старший mip применяется
    // [LAB3][BOIDS] для глобальной (не локальной) Cohesion/Alignment в PSAdvance().
    pd3dContext->GenerateMips( g_pParticleDataTexSRVFrom );

    // Accumulate forces
    // [LAB3][BOIDS] ЭТАП 2: попарные вклады Separation + Avoid, с накоплением в force texture.
    // [LAB3][BOIDS] Переключатель в UI изменяет только способ построения квадратов (GS / без GS).
    if( g_bAccumulateWithGS )
        AccumulateForcesGS( pd3dContext );
    else
        AccumulateForces( pd3dContext );

    // Apply forces to particles
    // [LAB3][BOIDS] ЭТАП 3: в PSAdvance складываются Separation/Avoid + Seek + Cohesion
    // [LAB3][BOIDS] + Alignment + Flee, затем изменяется положение каждого агента.
    AdvanceParticles( pd3dContext, fElapsedTime );

    // Swap the ping-pong buffers
    // [LAB3][BOIDS] ЭТАП 4: новый набор состояний становится текущим.
    SwapParticleTextures();

    // DXUT11 camera хранит D3DXMATRIX: копируем совместимый набор из 16 float без aliasing cast.
    DirectX::XMFLOAT4X4 viewData;
    DirectX::XMFLOAT4X4 projData;
    DirectX::XMMATRIX mView = DirectX::XMLoadFloat4x4( &viewData );
    DirectX::XMMATRIX mProj = DirectX::XMLoadFloat4x4( &projData );

    // Render the boids
    // [LAB3][BOIDS] ЭТАП 5: рисуем mesh экземплярами, используя обновлённую позицию каждого boid.
    RenderMeshInstanced( pd3dContext, &g_BoidMesh, g_pRenderBoids, mView, mProj );

    // render the mesh
    pd3dContext->IASetInputLayout( g_pMeshVertexLayout );
    DirectX::XMMATRIX mWorld = DirectX::XMMatrixTranslation( g_vFleePos.x, g_vFleePos.y, g_vFleePos.z );
    float fScale = 50.0f;
    DirectX::XMMATRIX mScale = DirectX::XMMatrixScaling( fScale, fScale, fScale );
    DirectX::XMMATRIX mWorldViewProj = DirectX::XMMatrixMultiply( DirectX::XMMatrixMultiply( DirectX::XMMatrixMultiply( mScale, mWorld ), mView ), mProj );
    DirectX::XMFLOAT4X4 mWorldViewProjData;
    DirectX::XMFLOAT4X4 mWorldData;
    DirectX::XMStoreFloat4x4( &mWorldViewProjData, mWorldViewProj );
    DirectX::XMStoreFloat4x4( &mWorldData, mWorld );
    g_pmWorldViewProj->SetMatrix( &mWorldViewProjData._11 );
    g_pmWorld->SetMatrix( &mWorldData._11 );
    // [LAB3][BOIDS] Отображение управляемого шара, координата которого поступает в Flee().
    RenderFleeMesh( pd3dContext );

    DXUT_BeginPerfEvent( DXUT_PERFEVENTCOLOR, L"HUD / Stats" );
    RenderText();
    g_HUD.OnRender( fElapsedTime );
    g_SampleUI.OnRender( fElapsedTime );
    DXUT_EndPerfEvent();
}


//--------------------------------------------------------------------------------------
// Render the help and statistics text
//--------------------------------------------------------------------------------------
void RenderText()
{
    g_pTxtHelper->Begin();
    g_pTxtHelper->SetInsertionPos( 2, 0 );
    g_pTxtHelper->SetForegroundColor( DirectX::XMVectorSet( 0.0f, 1.0f, 0.0f, 1.0f ) );
    g_pTxtHelper->DrawTextLine( DXUTGetFrameStats( DXUTIsVsyncEnabled() ) );
    g_pTxtHelper->DrawTextLine( DXUTGetDeviceStats() );
    g_pTxtHelper->SetForegroundColor(DirectX::XMVectorSet(1.0f, 0.5f, 0.0f, 1.0f));
    g_pTxtHelper->DrawTextLine( L"Use A,D,S,W,Q,E keys to move the ball" );
    g_pTxtHelper->End();
}


//--------------------------------------------------------------------------------------
// Release D3D11 resources created in OnD3D11ResizedSwapChain 
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D11SwapChainReleasing( void*  )
{
    g_DialogResourceManager.OnD3D11ReleasingSwapChain();
}


//--------------------------------------------------------------------------------------
// Release D3D11 resources created in OnD3D11CreateDevice 
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D11DestroyDevice( void*  )
{
    g_DialogResourceManager.OnD3D11DestroyDevice();
    g_D3DSettingsDlg.OnD3D11DestroyDevice();
    DXUTGetGlobalResourceCache().OnDestroyDevice();
    SAFE_DELETE( g_pTxtHelper );
    SAFE_RELEASE( g_pEffect11 );
    SAFE_RELEASE( g_pParticleVertexLayout );
    SAFE_RELEASE( g_pSplatVertexLayout );
    SAFE_RELEASE( g_pMeshVertexLayout );
    SAFE_RELEASE( g_pParticleBuffer );
    SAFE_RELEASE( g_pSplatBuffer );

    SAFE_RELEASE( g_pParticleDataTextureTo );
    SAFE_RELEASE( g_pParticleDataTexSRVTo );
    SAFE_RELEASE( g_pParticleDataTexRTVTo );
    SAFE_RELEASE( g_pParticleDataTextureFrom );
    SAFE_RELEASE( g_pParticleDataTexSRVFrom );
    SAFE_RELEASE( g_pParticleDataTexRTVFrom );
    SAFE_RELEASE( g_pForceTexture );
    SAFE_RELEASE( g_pForceTexSRV );
    SAFE_RELEASE( g_pForceTexRTV );

    g_BoidMesh.Destroy();
    g_FleeMesh.Destroy();
}


//--------------------------------------------------------------------------------------
float RPercent()
{
    float ret = ( float )( ( rand() % 10000 ) - 5000 );
    return ret / 5000.0f;
}


//--------------------------------------------------------------------------------------
// This helper function loads a group of particles
//--------------------------------------------------------------------------------------
// [LAB3][BOIDS] Инициализация стартовой группы агентов внутри сферы Spread вокруг Center.
// [LAB3][BOIDS] Каждому задаются позиция, стартовое направление и исходный вектор вверх.
void LoadParticles( DirectX::XMFLOAT4* pPositions, DirectX::XMFLOAT4* pVelocities, DirectX::XMFLOAT4* pUps, DirectX::XMFLOAT3 Center, DirectX::XMFLOAT4 Velocity, float Spread, UINT NumParticles )
{
    for( UINT i = 0; i < NumParticles; i++ )
    {
        DirectX::XMFLOAT3 Delta( Spread, Spread, Spread );

        while( DirectX::XMVectorGetX( DirectX::XMVector3LengthSq( DirectX::XMLoadFloat3( &Delta ) ) ) > Spread * Spread )
        {
            Delta.x = RPercent() * Spread;
            Delta.y = RPercent() * Spread;
            Delta.z = RPercent() * Spread;
        }

        pPositions[i].x = Center.x + Delta.x;
        pPositions[i].y = Center.y + Delta.y;
        pPositions[i].z = Center.z + Delta.z;
        pPositions[i].w = 1.0f;

        pVelocities[i] = Velocity;

        pUps[i] = DirectX::XMFLOAT4( 0, 1, 0, 1 );
    }
}

//--------------------------------------------------------------------------------------
// This helper function creates the texture array that will store all of the particle
// data.  Position, Velocity.
//--------------------------------------------------------------------------------------
// [LAB3][BOIDS] ХРАНЕНИЕ СОСТОЯНИЯ: две одинаковые Texture2DArray для ping-pong симуляции.
// [LAB3][BOIDS] Слой 0 = position.xyz, слой 1 = direction.xyz + speed в w, слой 2 = up.xyz.
// [LAB3][BOIDS] Все слои имеют mip-уровни; по последнему PSAdvance получает усреднения.
HRESULT CreateParticleTextures( ID3D11Device* pd3dDevice )
{
    HRESULT hr = S_OK;

    UINT SideSize = ( UINT )sqrtf( MAX_PARTICLES ) + 1;
    g_iTexSize = SideSize;
    // [LAB3][BOIDS] Три слоя — не три агента, а три типа данных для каждого агента.
    UINT ArraySize = 3;

    // Create the texture
    // [LAB3][BOIDS] Полная цепочка mipmap применяется как быстрое приближение глобального среднего.
    UINT MipLevels = ( UINT )( ( logf( ( float )SideSize ) / logf( 2 ) ) + 1 );
    g_iMaxMip = MipLevels - 1;
    D3D11_TEXTURE2D_DESC dstex;
    dstex.Width = SideSize;
    dstex.Height = SideSize;
    dstex.MipLevels = MipLevels;
    dstex.ArraySize = ArraySize;
    dstex.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    dstex.Usage = D3D11_USAGE_DEFAULT;
    dstex.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
    dstex.CPUAccessFlags = 0;
    dstex.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
    dstex.SampleDesc.Count = 1;
    dstex.SampleDesc.Quality = 0;

    // Initialize the data in the buffers
    DirectX::XMFLOAT4** ppData = new DirectX::XMFLOAT4*[3 * MipLevels];
    UINT subResource = 0;
    for( UINT i = 0; i < 3; i++ )
    {
        UINT Size = SideSize;
        for( UINT m = 0; m < MipLevels; m++ )
        {
            UINT MaxTextureParticles = Size * Size;

            ppData[subResource] = new DirectX::XMFLOAT4[ MaxTextureParticles ];
            if( !ppData[subResource] )
                return E_OUTOFMEMORY;

            subResource ++;
            Size = Size >> 1;
        }
    }

    srand( timeGetTime() );

    UINT MaxTextureParticles = SideSize * SideSize;
    // [LAB3][BOIDS] Две стартовые группы с противоположными направлениями движения.
    // Two clumps
    LoadParticles( ppData[0], ppData[1 * MipLevels], ppData[2 * MipLevels], DirectX::XMFLOAT3( g_fSpread, 0, 0 ), DirectX::XMFLOAT4( 0, 0, -1, 0 ), g_fSpread, MaxTextureParticles / 2 );
    LoadParticles( &ppData[0][MaxTextureParticles / 2], &ppData[1 * MipLevels][MaxTextureParticles / 2], &ppData[2 * MipLevels][MaxTextureParticles / 2], DirectX::XMFLOAT3( -g_fSpread, 0, 0 ), DirectX::XMFLOAT4( 0, 0, 1, 0 ), g_fSpread, MaxTextureParticles / 2 );

    D3D11_SUBRESOURCE_DATA* pInitData = new D3D11_SUBRESOURCE_DATA[3 * MipLevels];

    subResource = 0;
    for( UINT i = 0; i < 3; i++ )
    {
        UINT Size = SideSize;
        for( UINT m = 0; m < MipLevels; m++ )
        {
            pInitData[subResource].pSysMem = ppData[subResource];
            pInitData[subResource].SysMemPitch = sizeof( DirectX::XMFLOAT4 ) * Size;
            pInitData[subResource].SysMemSlicePitch = sizeof( DirectX::XMFLOAT4 ) * Size * Size;

            Size = Size >> 1;
            subResource ++;
        }
    }

    V_RETURN( pd3dDevice->CreateTexture2D( &dstex, pInitData, &g_pParticleDataTextureTo ) );
    V_RETURN( pd3dDevice->CreateTexture2D( &dstex, pInitData, &g_pParticleDataTextureFrom ) );
    for( UINT i = 0; i < 3 * MipLevels; i++ )
        SAFE_DELETE_ARRAY( ppData[i] );
    SAFE_DELETE_ARRAY( ppData );
    SAFE_DELETE_ARRAY( pInitData );

    // Create the resource view
    D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
    ZeroMemory( &SRVDesc, sizeof( SRVDesc ) );
    SRVDesc.Format = dstex.Format;
    SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
    SRVDesc.Texture2DArray.ArraySize = ArraySize;
    SRVDesc.Texture2DArray.FirstArraySlice = 0;
    SRVDesc.Texture2DArray.MipLevels = MipLevels;
    SRVDesc.Texture2DArray.MostDetailedMip = 0;
    V_RETURN( pd3dDevice->CreateShaderResourceView( g_pParticleDataTextureTo, &SRVDesc, &g_pParticleDataTexSRVTo ) );
    V_RETURN( pd3dDevice->CreateShaderResourceView( g_pParticleDataTextureFrom, &SRVDesc,
                                                    &g_pParticleDataTexSRVFrom ) );

    // Create the render target view
    D3D11_RENDER_TARGET_VIEW_DESC RTVDesc;
    ZeroMemory( &RTVDesc, sizeof( RTVDesc ) );
    RTVDesc.Format = dstex.Format;
    RTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
    RTVDesc.Texture2DArray.ArraySize = ArraySize;
    RTVDesc.Texture2DArray.FirstArraySlice = 0;
    RTVDesc.Texture2DArray.MipSlice = 0;
    V_RETURN( pd3dDevice->CreateRenderTargetView( g_pParticleDataTextureTo, &RTVDesc, &g_pParticleDataTexRTVTo ) );
    V_RETURN( pd3dDevice->CreateRenderTargetView( g_pParticleDataTextureFrom, &RTVDesc, &g_pParticleDataTexRTVFrom ) );

    return hr;
}


//--------------------------------------------------------------------------------------
// [LAB3][BOIDS] Промежуточная Texture2D: один texel на агента, значение = сумма pairwise-сил.
// [LAB3][BOIDS] RTV служит для GPU-накопления, SRV — для чтения в PSAdvance().
HRESULT CreateForceTexture( ID3D11Device* pd3dDevice )
{
    HRESULT hr = S_OK;

    UINT SideSize = ( UINT )sqrtf( MAX_PARTICLES ) + 1;
    g_iTexSize = SideSize;
    UINT ArraySize = 1;

    // Create the texture
    D3D11_TEXTURE2D_DESC dstex;
    dstex.Width = SideSize;
    dstex.Height = SideSize;
    dstex.MipLevels = 1;
    dstex.ArraySize = ArraySize;
    dstex.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    dstex.Usage = D3D11_USAGE_DEFAULT;
    dstex.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
    dstex.CPUAccessFlags = 0;
    dstex.MiscFlags = 0;
    dstex.SampleDesc.Count = 1;
    dstex.SampleDesc.Quality = 0;

    // Initialize the data in the buffers
    UINT MaxTextureParticles = SideSize * SideSize;
    DirectX::XMFLOAT4* pData = new DirectX::XMFLOAT4[ MaxTextureParticles ];
    if( !pData )
        return E_OUTOFMEMORY;

    for( UINT i = 0; i < MaxTextureParticles; i++ )
    {
        pData[i] = DirectX::XMFLOAT4( 0, 0, 0, 1 );
    }
    D3D11_SUBRESOURCE_DATA InitData;
    InitData.pSysMem = pData;
    InitData.SysMemPitch = sizeof( DirectX::XMFLOAT4 ) * SideSize;
    InitData.SysMemSlicePitch = sizeof( DirectX::XMFLOAT4 ) * MaxTextureParticles;
    V_RETURN( pd3dDevice->CreateTexture2D( &dstex, &InitData, &g_pForceTexture ) );
    SAFE_DELETE_ARRAY( pData );

    // Create the resource view
    D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
    ZeroMemory( &SRVDesc, sizeof( SRVDesc ) );
    SRVDesc.Format = dstex.Format;
    SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    SRVDesc.Texture2DArray.ArraySize = ArraySize;
    SRVDesc.Texture2DArray.FirstArraySlice = 0;
    SRVDesc.Texture2DArray.MipLevels = 1;
    SRVDesc.Texture2DArray.MostDetailedMip = 0;
    V_RETURN( pd3dDevice->CreateShaderResourceView( g_pForceTexture, &SRVDesc, &g_pForceTexSRV ) );

    // Create the render target view
    D3D11_RENDER_TARGET_VIEW_DESC RTVDesc;
    ZeroMemory( &RTVDesc, sizeof( RTVDesc ) );
    RTVDesc.Format = dstex.Format;
    RTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    RTVDesc.Texture2DArray.ArraySize = ArraySize;
    RTVDesc.Texture2DArray.FirstArraySlice = 0;
    RTVDesc.Texture2DArray.MipSlice = 0;
    V_RETURN( pd3dDevice->CreateRenderTargetView( g_pForceTexture, &RTVDesc, &g_pForceTexRTV ) );

    return hr;
}

//--------------------------------------------------------------------------------------
// This helper function creates 3 vertex buffers.  The first is used to seed the
// particle system.  The second two are used as output and intput buffers alternatively
// for the GS particle system.  Since a buffer cannot be both an input to the GS and an
// output of the GS, we must ping-pong between the two.
//--------------------------------------------------------------------------------------
// [LAB3][BOIDS] Вершины для запуска GPU-проходов; реальные позиции boids хранятся НЕ здесь.
// [LAB3][BOIDS] Оригинальный комментарий выше про «3 vertex buffers» не соответствует этой функции.
HRESULT CreateParticleBuffer( ID3D11Device* pd3dDevice )
{
    HRESULT hr = S_OK;

    D3D11_BUFFER_DESC vbdesc =
    {
        MAX_PARTICLES * sizeof( PARTICLE_VERTEX ),
        D3D11_USAGE_DEFAULT,
        D3D11_BIND_VERTEX_BUFFER,
        0,
        0
    };
    D3D11_SUBRESOURCE_DATA vbInitData;
    ZeroMemory( &vbInitData, sizeof( D3D11_SUBRESOURCE_DATA ) );

    PARTICLE_VERTEX* pVertices = new PARTICLE_VERTEX[ MAX_PARTICLES ];
    if( !pVertices )
        return E_OUTOFMEMORY;

    for( UINT i = 0; i < MAX_PARTICLES; i++ )
    {
        pVertices[i].Color = DirectX::XMFLOAT4( 1, 1, 0.2f, 1 );
    }

    vbInitData.pSysMem = pVertices;
    V_RETURN( pd3dDevice->CreateBuffer( &vbdesc, &vbInitData, &g_pParticleBuffer ) );
    SAFE_DELETE_ARRAY( pVertices );

    return hr;
}


// [LAB3][BOIDS] Готовые экранные квадраты: 6 вершин на агента для AccumulateForces() без GS.
// [LAB3][BOIDS] Служат проходу накопления сил; это не геометрия самих boid-моделей.
HRESULT CreateSplatBuffer( ID3D11Device* pd3dDevice )
{
    HRESULT hr = S_OK;

    D3D11_BUFFER_DESC vbdesc =
    {
        MAX_PARTICLES * 6 * sizeof( SPLAT_VERTEX ),
        D3D11_USAGE_DEFAULT,
        D3D11_BIND_VERTEX_BUFFER,
        0,
        0
    };

    D3D11_SUBRESOURCE_DATA vbInitData;
    ZeroMemory( &vbInitData, sizeof( D3D11_SUBRESOURCE_DATA ) );

    SPLAT_VERTEX* pVertices = new SPLAT_VERTEX[ MAX_PARTICLES * 6 ];
    if( !pVertices )
        return E_OUTOFMEMORY;

    UINT index = 0;
    for( UINT i = 0; i < MAX_PARTICLES; i++ )
    {
        pVertices[index].Pos = DirectX::XMFLOAT3( -1, 1, 0 );
        pVertices[index].Tex = DirectX::XMFLOAT2( 0, 0 ); index++;
        pVertices[index].Pos = DirectX::XMFLOAT3( 1, 1, 0 );
        pVertices[index].Tex = DirectX::XMFLOAT2( 1, 0 ); index++;
        pVertices[index].Pos = DirectX::XMFLOAT3( -1, -1, 0 );
        pVertices[index].Tex = DirectX::XMFLOAT2( 0, 1 ); index++;

        pVertices[index].Pos = DirectX::XMFLOAT3( -1, -1, 0 );
        pVertices[index].Tex = DirectX::XMFLOAT2( 0, 1 ); index++;
        pVertices[index].Pos = DirectX::XMFLOAT3( 1, 1, 0 );
        pVertices[index].Tex = DirectX::XMFLOAT2( 1, 0 ); index++;
        pVertices[index].Pos = DirectX::XMFLOAT3( 1, -1, 0 );
        pVertices[index].Tex = DirectX::XMFLOAT2( 1, 1 ); index++;

    }

    vbInitData.pSysMem = pVertices;
    V_RETURN( pd3dDevice->CreateBuffer( &vbdesc, &vbInitData, &g_pSplatBuffer ) );
    SAFE_DELETE_ARRAY( pVertices );

    return hr;
}
