#pragma once
#include <wrl/client.h>
#include <DirectXColors.h>

#include <iostream>
#include <fstream>
#include <string>

#include "Structs.h"

using namespace Microsoft::WRL;
using namespace Windows::UI::Core;
using namespace Platform;
using namespace DirectX;

class Game
{
public:
	//The device interface is a virtual representation of the video adapter, this is how we access the video memory and create other Direct3D COM objects
	ComPtr<ID3D11Device1> dev;				//The Device interface
	//The device context interface is a control panel for the GPU this allows us to control the rendering sequence and the process that translates the 3d model into a 2d image on the screen (Rasterisation) 
	ComPtr<ID3D11DeviceContext1> devcon;	//Device context interface
	//The swap chain is a series of preprepared frames for the GPU basically double and triple buffering
	ComPtr<IDXGISwapChain1> swapchain;		//Swap Chain Interface

	//The render target is the buffer that needs to be written to
	ComPtr<ID3D11RenderTargetView> rendertarget;

	//Vertex Buffer
	ComPtr<ID3D11Buffer> vertexbuffer;

	//Input layout
	ComPtr<ID3D11InputLayout> inputlayout;

	//Shader Variables
	ComPtr<ID3D11VertexShader> vertexshader;
	ComPtr<ID3D11PixelShader> pixelshader;

	void Initialize();
	void Update();
	void Render();

	void InitGraphics();
	void InitPipeline();

};

