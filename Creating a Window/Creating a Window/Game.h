#pragma once
#include <wrl/client.h>
#include <DirectXColors.h>

using namespace Microsoft::WRL;
using namespace Windows::UI::Core;
using namespace DirectX;

class Game
{
public:
	//The device interface is a virtual representation of the video adapter, this is how we access the video memory and create other Direct3D COM objects
	ComPtr<ID3D11Device1> dev;				//The Device interface
	//The device context interface is a control panel for the GPU this allows us to control the rendering sequence and the process that translates the 3d model into a 2d image on the screen (Rasterization) 
	ComPtr<ID3D11DeviceContext1> devcon;	//Device context interface

	void Initialize();
	void Update();
	void Render();
};

