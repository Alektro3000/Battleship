
#include "DXApp.h"
#include "d3dcompiler.h"
using namespace DirectX;
void DXApp::InitBackBuffer()
{

    // get the address of the back buffer
    ID3D11Texture2D *pBackBuffer;
    swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID *)&pBackBuffer);

    // use the back buffer address to create the render target
    dev->CreateRenderTargetView(pBackBuffer, NULL, &backbuffer);
    pBackBuffer->Release();

    // set the render target as the back buffer
    devcon->OMSetRenderTargets(1, &backbuffer, NULL);

    // Set the viewport
    D3D11_VIEWPORT viewport;
    ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = SCREEN_WIDTH;
    viewport.Height = SCREEN_HEIGHT;

    devcon->RSSetViewports(1, &viewport);
}

void DXApp::InitPipeline()
{
    // load and compile the two shaders
    ID3DBlob *VS, *PS;
    D3DCompileFromFile((LPCWSTR)L"shaders.shader", 0, 0, (LPCSTR)"VShader", 
                        (LPCSTR)L"vs_4_0", 0, 0, &VS, nullptr);
    D3DCompileFromFile((LPCWSTR)L"shaders.shader", 0, 0, (LPCSTR)"PShader", 
                        (LPCSTR)L"ps_4_0", 0, 0, &PS, nullptr);

                        
    // encapsulate both shaders into shader objects
    dev->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), nullptr, &pVS);
    dev->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), nullptr, &pPS);

    
    // set the shader objects
    devcon->VSSetShader(pVS, 0, 0);
    devcon->PSSetShader(pPS, 0, 0);
    
    
}
