#include "pch.h"
#include "Game.h"

void Game::Initialize()
{
	//Create temporary ComPtr to a device and device context
	/*
	There is no Create Device function for Version 11.1 so we create Version 11 ComPtr and then convert them at a later point
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
}

void Game::Update()
{

}

void Game::Render()
{

}
