#pragma once

#include "pch.h"
#include "Common\DeviceResources.h"
#include <iostream>

using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::UI::Core;
using namespace Windows::UI::Popups;
using namespace Windows::System;
using namespace Windows::Foundation;
using namespace Windows::Graphics::Display;
using namespace Platform;

//ref classes
/*
Cannot use public variables, must use properties instad and then utilise setters/getters to access them
Cannot use public constructors, unless the sealed keyword is also used
*/
//sealed keyword
/*
Sealed allows a ref class to have a public constructor
A sealed class cannot be inherited by another class
*/
//App Class
/*
The app class is a gateway for all things Windows
Windows uses this class as a event handler, any events that happen (such as closing the window or resizing) will be told to us through this class
*/
ref class App sealed : public IFrameworkView {
public:
	//All these methods are called by IFrameworkView from behind the scenes
	virtual void Initialize(CoreApplicationView^ AppView); //Gives us access to low level properties and notifications
	virtual void SetWindow(CoreWindow^ Window); //Allows us to setup window notifications such as key strokes and mouse movements
	virtual void Load(String^ EntryPoint); //Allows us to load in graphics, sound effects, and allocate memory
	virtual void Run(); //This is where the game loop is
	virtual void Uninitialize(); //Cleans up any allocated memory created during the Load method

	void OnActivated(CoreApplicationView^ CoreAppView, IActivatedEventArgs^ Args);
};

//IFrameworkViewSource
/*
IFrameworkViewSource is a simple WinRT interface class
The job of this class is to set up our application with windows and to prepare a second class IFrameworkView
*/
ref class AppSource sealed : IFrameworkViewSource {
public:
	//This method is called behind the scenes by IFrameworkViewSource for us
	virtual IFrameworkView^ CreateView(); //Creates and returns a ref new instance of the App class (which inherits from the IFrameworkView class)
};

