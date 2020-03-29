#include "pch.h"
#include "Game.h"

//This function loads a shader file into a array
Platform::Array<byte>^ LoadShaderFile(std::string File)
{
	Array<byte >^ FileData = nullptr;

	// open the file
	std::ifstream VertexFile(File, std::ios::in | std::ios::binary | std::ios::ate);

	// if open was successful
	if (VertexFile.is_open())
	{
		// find the length of the file
		int Length = (int)VertexFile.tellg();

		// collect the file data
		FileData = ref new Array<byte>(Length);
		VertexFile.seekg(0, std::ios::beg);
		VertexFile.read(reinterpret_cast<char*>(FileData->Data), Length);
		VertexFile.close();
	}

	return FileData;
}

void Game::Initialize()
{
	//Create temporary ComPtr to a device and device context
	/*
	There is no Create Device function for Version 11.1 so we create Version 11 ComPtr and then convert them at a later point

	ComPtr
	COM : Component Object Model is a way of coding so that each script does not depend on another class to work, essentially each class is a lego brick, it dosent care what pieces are attached to it
	It just works with whatever 
	You create new COM objects using the ComPtr class this takes ina type which you wish to cast the ComPtr to.
	*/
	ComPtr<ID3D11Device> dev11;
	ComPtr<ID3D11DeviceContext> devcon11;

	//D3D11CreateDevice
	/*
	Initialize Direct3D and create the device and device context objects
	Param IDXGIAdapter : This is a pointer to a interface to describe the graphics adapter (leaving this as nullptr means Direct3D chooses the Adapter for us)
	Param D3D_DRIVER_TYPE : This tells Direct3D what type of acceleration to use. (D3D_DRIVER_TYPE_HARDWARE means use hardware accelerated graphics)
	Param HMODULE : This allows us to build our own software engine (leaving this as nullptr as we want to program the hardware directly)
	Param FLAGS : This allows us to set advanced flags for our program such as BGRA colour space support and single threaded processing
	Param D3D_FEATURE_LEVEL : Each major version of Direct3D has a series of hardware requirements (like ray tracing requires RTX) (nullptr is used here as we are not using advanced features)
	Param SDKVersion : This says what version of Direct3D we are using in this case it is D3D11_SDK_VERSION 
	Param ID3D11Device : This is a pointer to a pointer to the device interface, this will create an object for us (we pass through dev11 here as that is the device we want creating)
	Param D3D_FEATURE_LEVEL : This is a pointer to a feature level variable when the function is completed this variable will be filled with the flag of the highest feature level that was found. 
	Param ID3D11DeviceContext : This is a pointer to a pointer to the device context, this is the device context that we want to be created. 
	*/
	D3D11CreateDevice(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&dev11,
		nullptr,
		&devcon11);

	//Convert the pointers from DirectX Version 11 to DirectX Version 11.1
	//As
	/*
	This function is a member of the ComPtr class, its job is to store the interface it represents into another ComPtr
	The ID3D11CreateDevice function created the ID3D11Device and ID3D11DeviceContext but we need ID3D11Device1 and ID3D11DeviceContext1
	This is why we access the dev11 ComPtr and turn it into a ID3D11Device1 by passing in the memory address of the dev variable, same for devcon11
	*/
	dev11.As(&dev);
	devcon11.As(&devcon);

	//Getting the swap chain
	//First, convert a ID3D11Device1 into a IDXGIDevice1
	ComPtr<IDXGIDevice1> dxgiDevice;
	dev.As(&dxgiDevice);

	//Second, use the IDXGIDevice1 interface to get access to the adapter
	//IDXGIAdapter is a virtual representation of the video card, the parent of this object is the factory that was used to create our device
	ComPtr<IDXGIAdapter> dxgiAdapter;
	dxgiDevice->GetAdapter(&dxgiAdapter);

	//Third, use the IDXGIAdapter interface to get access to the factory (Factory is a COM object that can be used to create other COM objects
	//IDXGIFactory2 is a interface to our factory from this interface we can create the swap chain, the 2 at the end indicates this is the version 2 of the interface
	ComPtr<IDXGIFactory2> dxgiFactory;
	//GetParent() : This allows us to access the factory used for out adapter and the device, it has two parameters the type of interface we are obtaining and a pointer to store the address in
	//__uuidof() : This is a COM function for the unique identifying code of a COM object, we use it here to tell GetParent what type of interface we are obtaining
	dxgiAdapter->GetParent(__uuidof(IDXGIFactory2), &dxgiFactory);

	DXGI_SWAP_CHAIN_DESC1 scd = { 0 };
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;		//How the swap chain should be used
	scd.BufferCount = 2;									//a front and back buffer (double buffering)
	scd.Format = DXGI_FORMAT_B8G8R8A8_UNORM;				//Most common swap chain format
	scd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;		//Recommended flip mode
	scd.SampleDesc.Count = 1;								//Disables Anti Aliasing

	CoreWindow^ Window = CoreWindow::GetForCurrentThread();

	dxgiFactory->CreateSwapChainForCoreWindow(
		dev.Get(),								//Device
		reinterpret_cast<IUnknown*>(Window),	//Windoq
		&scd,									//Swap Chain Description
		nullptr,								//allows us to restrict rendering to a specific monitor
		&swapchain);							//pointer to the swap chain variable

	ComPtr<ID3D11Texture2D> backbuffer;

	//GetBuffer is a function that finds the back buffer we want to obtain and creates an interface that points to it
	swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), &backbuffer); //Gets the address of the back buffer and saves the address in our backbuffer variable

	//Create a render target
	dev->CreateRenderTargetView(backbuffer.Get(), nullptr, &rendertarget);

	D3D11_VIEWPORT viewport = { 0 };

	//Determines the width and height of the view port
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = Window->Bounds.Width;
	viewport.Height = Window->Bounds.Height;

	devcon->RSSetViewports(1, &viewport);

	InitGraphics();
	InitPipeline();
}

void Game::Update()
{

}

void Game::Render()
{	
	//Sets our render target object as the active render target
	devcon->OMSetRenderTargets(1, rendertarget.GetAddressOf(), nullptr);

	//clear the back buffer to a deep blue colour
	float color[4] = { 0.0f,0.2f,0.4f,1.0f };
	//Takes an address to the current rendertarget and then sets the background color
	devcon->ClearRenderTargetView(rendertarget.Get(), color);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	devcon->IASetVertexBuffers(0, 1, vertexbuffer.GetAddressOf(), &stride, &offset);

	devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	devcon->Draw(3, 0);

	//Switches the back buffer and the front buffer
	swapchain->Present(1, 0);
}

void Game::InitGraphics()
{
	Vertex OurVerticies[] = {
		{0.0f,0.5f,0.0f},
		{0.45f,-0.5f,0.0f},
		{-0.45f,-0.5f,0.0f}
	};

	//Initialize everything to 0
	D3D11_BUFFER_DESC bd = { 0 };
	bd.ByteWidth = sizeof(Vertex) * ARRAYSIZE(OurVerticies);	//This is the size of each vertex in memory
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;					//This is the type of buffer that we want Direct3D to make

	D3D11_SUBRESOURCE_DATA srd = { OurVerticies, 0, 0 };		//This is a struct containing a description of the data that we want to store in the vertex buffer

	dev->CreateBuffer(&bd, &srd, &vertexbuffer);				//This creates the buffer, this takes a reference to the buffer description and the source data for the buffer and then we store the created buffer in the vertexbuffer passed in
}

void Game::InitPipeline()
{
	//Load shader information
	Platform::Array<byte>^ VSFile = LoadShaderFile("VertexShader.cso");
	Array<byte>^ PSFile = LoadShaderFile("PixelShader.cso");

	//Creating the shader objects
	dev->CreateVertexShader(VSFile->Data, VSFile->Length, nullptr, &vertexshader);
	dev->CreatePixelShader(PSFile->Data, PSFile->Length, nullptr, &pixelshader);

	//Setting these to be the active shaders
	//SetShader() : Takes a address to the shader
	devcon->VSSetShader(vertexshader.Get(), nullptr, 0);
	devcon->PSSetShader(pixelshader.Get(), nullptr, 0);


	//Creating the input layout
	D3D11_INPUT_ELEMENT_DESC ied[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0,0,D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	dev->CreateInputLayout(ied, ARRAYSIZE(ied), VSFile->Data, VSFile->Length, &inputlayout);
	devcon->IASetInputLayout(inputlayout.Get());
}


