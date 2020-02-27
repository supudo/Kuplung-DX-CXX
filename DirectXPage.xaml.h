//
// DirectXPage.xaml.h
// Declaration of the DirectXPage class.
//

#pragma once

#include "DirectXPage.g.h"

#include "Common\DeviceResources.h"
#include "Kuplung_DXMain.h"
#include "Models\Model3D.h"
#include "Utilities\CXXUtils.h"
#include "Rendering\RenderingManager.h"
#include "Importers\FileModelManager.h"

namespace Kuplung_DX
{
	/// <summary>
	/// A page that hosts a DirectX SwapChainPanel.
	/// </summary>
	public ref class DirectXPage sealed
	{
	public:
		DirectXPage();
		virtual ~DirectXPage();

		void SaveInternalState(Windows::Foundation::Collections::IPropertySet^ state);
		void LoadInternalState(Windows::Foundation::Collections::IPropertySet^ state);

		void AddToLog(Platform::String^ message);

		property Platform::String^ LogMessage;

	private:
		// XAML low-level rendering event handler.
		void OnRendering(Platform::Object^ sender, Platform::Object^ args);

		// Window event handlers.
		void OnVisibilityChanged(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::VisibilityChangedEventArgs^ args);

		// DisplayInformation event handlers.
		void OnDpiChanged(Windows::Graphics::Display::DisplayInformation^ sender, Platform::Object^ args);
		void OnOrientationChanged(Windows::Graphics::Display::DisplayInformation^ sender, Platform::Object^ args);
		void OnDisplayContentsInvalidated(Windows::Graphics::Display::DisplayInformation^ sender, Platform::Object^ args);

		// Other event handlers.
		void OnCompositionScaleChanged(Windows::UI::Xaml::Controls::SwapChainPanel^ sender, Object^ args);
		void OnSwapChainPanelSizeChanged(Platform::Object^ sender, Windows::UI::Xaml::SizeChangedEventArgs^ e);

		// Track our independent input on a background worker thread.
		Windows::Foundation::IAsyncAction^ m_inputLoopWorker;
		Windows::UI::Core::CoreIndependentInputSource^ m_coreInput;

		// Independent input handling functions.
		void OnPointerPressed(Platform::Object^ sender, Windows::UI::Core::PointerEventArgs^ e);
		void OnPointerMoved(Platform::Object^ sender, Windows::UI::Core::PointerEventArgs^ e);
		void OnPointerReleased(Platform::Object^ sender, Windows::UI::Core::PointerEventArgs^ e);

		// Resources used to render the DirectX content in the XAML page background.
		std::shared_ptr<DX::DeviceResources> m_deviceResources;
		std::unique_ptr<Kuplung_DXMain> m_main; 
		bool m_windowVisible;

		std::unique_ptr<Kuplung_DX::Rendering::RenderingManager> managerRendering;
		std::unique_ptr<Kuplung_DX::Importers::FileModelManager> managerParsers;

		std::vector<Kuplung_DX::Models::MeshModel> meshModels;
		Platform::String^ CurrentModelFile;
		Platform::Collections::Vector<Kuplung_DX::Models::Model3D^>^ availableModels;
		Platform::Boolean showLogWindow;

		void LogInfo(Object^ parameter, bool addToLog);
		void DoProgress(float progress);
		void ParseModelAsync(Platform::String^ modelFile);

		void MenuGUIControls_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void MenuSceneControls_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void MenuCube_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void MenuFPSCounter_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void MenuShowLogWindow_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void LogWindowResize_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void LogWindowClear_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void logSize_KeyDown(Platform::Object^ sender, Windows::UI::Xaml::Input::KeyRoutedEventArgs^ e);

		void lvModels_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::SelectionChangedEventArgs^ e);
	};
}

