#include <d3d11.h>
#include <d2d1.h>
#include <directxmath.h>

struct VERTEX
{
      float X, Y, Z;      // position
      DirectX::XMFLOAT4 Color;    // color
};

/*
VERTEX OurVertices[] =
{
    {0.0f, 0.5f, 0.0f, DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)},
    {0.45f, -0.5, 0.0f, DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)},
    {-0.45f, -0.5f, 0.0f, DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)}
};
*/
#define SCREEN_WIDTH  1920
#define SCREEN_HEIGHT 1080

class DXApp
{
    
public:

    ID2D1Factory* pD2DFactory = NULL;
    // global declarations
    IDXGISwapChain *swapchain = nullptr;             // the pointer to the swap chain interface
    ID3D11Device *dev = nullptr;                     // the pointer to our Direct3D device interface
    ID3D11DeviceContext *devcon = nullptr;           // the pointer to our Direct3D device context
    ID3D11RenderTargetView *backbuffer = nullptr;    // the pointer to our back buffer

    //Constructor
    DXApp(HWND hWnd)
    {   
        static_assert(sizeof(float) == sizeof(FLOAT));


        // create a struct to hold information about the swap chain
        DXGI_SWAP_CHAIN_DESC scd;

        // clear out the struct for use
        ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

        // fill the swap chain description struct
        scd.BufferCount = 1;                                    // one back buffer
        scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color
        scd.BufferDesc.Width = SCREEN_WIDTH;                    // set the back buffer width
        scd.BufferDesc.Height = SCREEN_HEIGHT;                  // set the back buffer height
        scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // how swap chain is to be used
        scd.OutputWindow = hWnd;                                // the window to be used
        scd.SampleDesc.Count = 4;                               // how many multisamples
        scd.Windowed = FALSE;                                    // windowed/full-screen mode
        scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;     // allow full-screen switching

        // create a device, device context and swap chain using the information in the scd struct
        D3D11CreateDeviceAndSwapChain(NULL,
                                    D3D_DRIVER_TYPE_HARDWARE,
                                    NULL,
                                    NULL,
                                    NULL,
                                    NULL,
                                    D3D11_SDK_VERSION,
                                    &scd,
                                    &swapchain,
                                    &dev,
                                    NULL,
                                    &devcon);
         InitBackBuffer();
            
    HRESULT hr = D2D1CreateFactory(
        D2D1_FACTORY_TYPE_SINGLE_THREADED,
        &pD2DFactory
        );
        
    }

    
    ~DXApp()
    {
        swapchain->SetFullscreenState(FALSE, NULL);    // switch to windowed mode
        // close and release all existing COM objects

        pD2DFactory->Release();

        swapchain->Release();
        dev->Release();
        backbuffer->Release();
        devcon->Release();
    }
    void InitBackBuffer();

    // this is the function used to render a single frame
    void RenderFrame()
    {
        // clear the back buffer to a deep blue
        
        
        devcon->ClearRenderTargetView(backbuffer, DirectX::XMVECTOR{0.0f, 0.2f, 0.4f, 1.0f}.m128_f32);
        
        RECT rc;
        GetClientRect(hwnd, &rc);



        // switch the back buffer and the front buffer
        swapchain->Present(0, 0);
    }

};
