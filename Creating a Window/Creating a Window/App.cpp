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

	//Adds the suspending function as the handler for the windows suspend event
	CoreApplication::Suspending += ref new EventHandler<SuspendingEventArgs^>(this, &App::Suspending);
	CoreApplication::Resuming += ref new EventHandler<Object^>(this, &App::Resuming);

	WindowClosed = false;
}

void App::SetWindow(CoreWindow^ Window)
{
	//Informs Windows of the pointer pressed event handler
	Window->PointerPressed += ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &App::PointerPressed);
	Window->PointerReleased += ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &App::PointerRelease);
	Window->PointerMoved += ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &App::PointerMoved);
	Window->PointerWheelChanged += ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &App::PointerWheelChanged);

	//Informs Windows of the key stroke event handlers
	Window->KeyDown += ref new TypedEventHandler<CoreWindow^, KeyEventArgs^>(this, &App::KeyDown);
	Window->KeyUp += ref new TypedEventHandler<CoreWindow^, KeyEventArgs^>(this, &App::KeyUp);

	//Informs Windows of the Closed event handlers
	Window->Closed += ref new TypedEventHandler<CoreWindow^, CoreWindowEventArgs^>(this, &App::Closed);
}

void App::Load(String^ EntryPoint)
{

}

void App::Run()
{
	//Obtain a pointer to the window
	CoreWindow^ Window = CoreWindow::GetForCurrentThread();




	while (!WindowClosed) {
		//ProcessEvents()
		/*
		Dispatches Events
		Takes one of four values
		ProcessOneIfPresent : Dispatch the next event waiting in the queue (if applicable)
		ProcessAllIfPresent : Dispatch the next event waiting in the queue and repeat until all waiting events are handled
		ProcessOneAndAllPending : Dispatch all waiting events. if none are waiting wait until another one arrives
		ProcessUntilQuit : Dispatch all events and then repeat, do not stop until windows shuts the program down
		These options are part of the CoreProcessEventsOption enum
		*/
		Window->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessAllIfPresent);
	}

}

void App::Uninitialize()
{

}

void App::OnActivated(CoreApplicationView^ CoreAppView, IActivatedEventArgs^ Args)
{
	CoreWindow^ Window = CoreWindow::GetForCurrentThread(); //This returns a hat pointer to the created window, the window is created but needs to be activated
	Window->Activate(); //Activates the window
}

//Takes two parameters, the first is a pointer to the current window, the second is the details of the event, in this case the mouse position, buttons pressed etc.
void App::PointerPressed(CoreWindow^ Window, PointerEventArgs^ Args)
{
	//MessageDialog Dialog("Thank you for noticing me Senpai.", "UwU");
	//Dialog.ShowAsync();
}

void App::PointerMoved(CoreWindow^ Window, PointerEventArgs^ Args)
{
	//Spawns a Dialog box when the pointer is moved
	/*MessageDialog Dialog("Thank you for noticing me Senpai.", "UwU");
	Dialog.ShowAsync();*/
}

void App::PointerRelease(CoreWindow^ Window, PointerEventArgs^ Args)
{
	//Spawns a Dialog Box when the pointer is released
	//MessageDialog Dialog("Thank you for noticing me Senpai.", "UwU");
	//Dialog.ShowAsync();
}

void App::PointerWheelChanged(CoreWindow^ Window, PointerEventArgs^ Args)
{
	int Wheel = Args->CurrentPoint->Properties->MouseWheelDelta;

	MessageDialog Dialog(Wheel.ToString(), "Scroll Wheel");
	Dialog.ShowAsync();
}

void App::KeyDown(CoreWindow^ Window, KeyEventArgs^ Args)
{
	//Dialog box pops up with the key that was pressed 
	/*MessageDialog Dialog("That was: " + Args->VirtualKey.ToString(), "Key Pressed");
	Dialog.ShowAsync();*/

	//Quits the program when escape is pressed
	if (Args->VirtualKey == VirtualKey::Escape) {
		WindowClosed = true;
	}
}

void App::KeyUp(CoreWindow^ Window, KeyEventArgs^ Args)
{
	if (Args->VirtualKey == VirtualKey::A) {
		MessageDialog Dialog("You Let A Go!", "Let it Go!");
		Dialog.ShowAsync();
	}
}

void App::Suspending(Object^ Sender, SuspendingEventArgs^ Args)
{

}

void App::Resuming(Object^ Sender, Object^ Args)
{

}

void App::Closed(CoreWindow^ Sender, CoreWindowEventArgs^ Args)
{
	WindowClosed = true;
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
