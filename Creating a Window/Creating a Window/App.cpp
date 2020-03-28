#include "pch.h"
#include "App.h"

#include <ppltasks.h>

using namespace concurrency;
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::UI::Core;
using namespace Windows::UI::Input;
using namespace Windows::System;
using namespace Windows::Foundation;
using namespace Windows::Graphics::Display;

//App Class
void App::Initialize(CoreApplicationView^ AppView)
{
	//Calls onActivated() when the Activated event is triggered (it's like an event listener)
	/*
	AppView->Activated+= : The activated member is an event, which is a member of a class that provides it a pointer to a function the "+=" operator is used as it adds a function to the event
	ref new TypedEventHandler : a TypedEventHandler is a generic class that processes events
	<CoreApplicationView^, IActivatedEventArgs^> : As TypedEventHandler is a generic class is does not know what type of parameters will be passed to it so we have to say what the types are, these will match the OnActivated method parameters
	(this, &App::OnActivated); : The TypedEventHandler takes two parameters. It needs to know who will be calling the function and what function to call. 
		App is calling the function so we pass in this and App::OnActivated is the function that needs to be called so we pass in that
	*/
	AppView->Activated += ref new TypedEventHandler<CoreApplicationView^, IActivatedEventArgs^>(this, &App::OnActivated); //Points to the OnActivated function to create and activate the window
}

void App::SetWindow(CoreWindow^ Window)
{

}

void App::Load(String^ EntryPoint)
{

}

void App::Run()
{

}

void App::Uninitialize()
{

}

void App::OnActivated(CoreApplicationView^ CoreAppView, IActivatedEventArgs^ Args)
{
	CoreWindow^ Window = CoreWindow::GetForCurrentThread(); //This returns a hat pointer to the created window, the window is created but needs to be activated
	Window->Activate(); //Activates the window
}


//AppSource Class
IFrameworkView^ AppSource::CreateView()
{
	return ref new App(); //Create an app class and returns it
}


//MTAThread = Multi-Threaded Apartment Thread
/*
Windows 10 requires DirectX to run in MTAThread mode 
Using this before main() forces out program to using this mode
*/
[MTAThread] 

//^ handle declarator - Automatically deletes memory when it is no longer needed, basically a smart pointer
//Array<String^>^ args
/*
takes in an Array of WIN-RT style string objects
*/
int main(Array<String^>^ args) {
	IFrameworkViewSource^ appSource = ref new AppSource();
	CoreApplication::Run(appSource); //Creates an instance of AppSource, takes in a instance of a IFrameWorkViewSource interface class
	return 0;
}
