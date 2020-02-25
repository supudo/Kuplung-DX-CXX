﻿//
// App.xaml.h
// Declaration of the App class.
//

#pragma once

#include "App.g.h"
#include "DirectXPage.xaml.h"

namespace Kuplung_DX {
	/// <summary>
	/// Provides application-specific behavior to supplement the default Application class.
	/// </summary>
	ref class App sealed {
	public:
		App();
		virtual void OnLaunched(Windows::ApplicationModel::Activation::LaunchActivatedEventArgs^ e) override;

		static property Platform::String^ ApplicationPath;
		static property Platform::Boolean ViewSampleScene;
		static property Platform::Boolean ViewFPSCounter;
		static property int LogWindowWidth;
		static property int LogWindowHeight;

		void LogInfo(Object^ parameter);
	
	internal:
		void LogError(std::string msg);

	private:
		void OnSuspending(Platform::Object^ sender, Windows::ApplicationModel::SuspendingEventArgs^ e);
		void OnResuming(Platform::Object ^sender, Platform::Object ^args);
		void OnNavigationFailed(Platform::Object ^sender, Windows::UI::Xaml::Navigation::NavigationFailedEventArgs ^e);
		DirectXPage^ m_directXPage;

		void KuplungInitializeSettings();
	};
}
