//
// DirectXPage.xaml.cpp
// Implementation of the DirectXPage class.
//

#include "pch.h"
#include "DirectXPage.xaml.h"
#include <ppltasks.h>

using namespace Kuplung_DX;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Graphics::Display;
using namespace Windows::System::Threading;
using namespace Windows::UI::Core;
using namespace Windows::UI::Input;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;
using namespace concurrency;

DirectXPage::DirectXPage() : m_windowVisible(true), m_coreInput(nullptr) {
	InitializeComponent();

	// Register event handlers for page lifecycle.
	CoreWindow^ window = Window::Current->CoreWindow;

	window->VisibilityChanged += ref new TypedEventHandler<CoreWindow^, VisibilityChangedEventArgs^>(this, &DirectXPage::OnVisibilityChanged);

	DisplayInformation^ currentDisplayInformation = DisplayInformation::GetForCurrentView();

	currentDisplayInformation->DpiChanged += ref new TypedEventHandler<DisplayInformation^, Object^>(this, &DirectXPage::OnDpiChanged);
	currentDisplayInformation->OrientationChanged += ref new TypedEventHandler<DisplayInformation^, Object^>(this, &DirectXPage::OnOrientationChanged);
	DisplayInformation::DisplayContentsInvalidated += ref new TypedEventHandler<DisplayInformation^, Object^>(this, &DirectXPage::OnDisplayContentsInvalidated);
	swapChainPanel->CompositionScaleChanged += ref new TypedEventHandler<SwapChainPanel^, Object^>(this, &DirectXPage::OnCompositionScaleChanged);
	swapChainPanel->SizeChanged += ref new SizeChangedEventHandler(this, &DirectXPage::OnSwapChainPanelSizeChanged);

	// At this point we have access to the device. 
	// We can create the device-dependent resources.
	m_deviceResources = std::make_shared<DX::DeviceResources>();
	m_deviceResources->SetSwapChainPanel(swapChainPanel);

	// Register our SwapChainPanel to get independent input pointer events
	auto workItemHandler = ref new WorkItemHandler([this](IAsyncAction^) {
		// The CoreIndependentInputSource will raise pointer events for the specified device types on whichever thread it's created on.
		m_coreInput = swapChainPanel->CreateCoreIndependentInputSource(
			Windows::UI::Core::CoreInputDeviceTypes::Mouse |
			Windows::UI::Core::CoreInputDeviceTypes::Touch |
			Windows::UI::Core::CoreInputDeviceTypes::Pen
		);

		// Register for pointer events, which will be raised on the background thread.
		m_coreInput->PointerPressed += ref new TypedEventHandler<Object^, PointerEventArgs^>(this, &DirectXPage::OnPointerPressed);
		m_coreInput->PointerMoved += ref new TypedEventHandler<Object^, PointerEventArgs^>(this, &DirectXPage::OnPointerMoved);
		m_coreInput->PointerReleased += ref new TypedEventHandler<Object^, PointerEventArgs^>(this, &DirectXPage::OnPointerReleased);

		// Begin processing input messages as they're delivered.
		m_coreInput->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessUntilQuit);
		});

	// Run task on a dedicated high priority background thread.
	m_inputLoopWorker = ThreadPool::RunAsync(workItemHandler, WorkItemPriority::High, WorkItemOptions::TimeSliced);

	m_main = std::unique_ptr<Kuplung_DXMain>(new Kuplung_DXMain(m_deviceResources));
	m_main->StartRenderLoop();

	this->availableModels = ref new Platform::Collections::Vector<Kuplung_DX::Models::Model3D^>();
	this->availableModels->Append(ref new Kuplung_DX::Models::Model3D{ "Triangle", "triangle.obj" });
	this->availableModels->Append(ref new Kuplung_DX::Models::Model3D{ "Cone", "cone.obj" });
	this->availableModels->Append(ref new Kuplung_DX::Models::Model3D{ "Cube", "cube.obj" });
	this->availableModels->Append(ref new Kuplung_DX::Models::Model3D{ "Cylinder", "cylinder.obj" });
	this->availableModels->Append(ref new Kuplung_DX::Models::Model3D{ "Grid", "grid.obj" });
	this->availableModels->Append(ref new Kuplung_DX::Models::Model3D{ "Ico Sphere", "ico_sphere.obj" });
	this->availableModels->Append(ref new Kuplung_DX::Models::Model3D{ "Plane", "plane.obj" });
	this->availableModels->Append(ref new Kuplung_DX::Models::Model3D{ "Torus", "torus.obj" });
	this->availableModels->Append(ref new Kuplung_DX::Models::Model3D{ "Tube", "tube.obj" });
	this->availableModels->Append(ref new Kuplung_DX::Models::Model3D{ "UV Sphere", "uv_sphere.obj" });
	this->availableModels->Append(ref new Kuplung_DX::Models::Model3D{ "Monkey Head", "monkey_head.obj" });
	this->availableModels->Append(ref new Kuplung_DX::Models::Model3D{ "Epcot", "epcot.obj" });
	this->availableModels->Append(ref new Kuplung_DX::Models::Model3D{ "Brick Wall", "brick_wall.obj" });
	this->availableModels->Append(ref new Kuplung_DX::Models::Model3D{ "Plane Objects", "plane_objects.obj" });
	this->availableModels->Append(ref new Kuplung_DX::Models::Model3D{ "Plane Objects - Large Plane", "plane_objects_large.obj" });
	this->availableModels->Append(ref new Kuplung_DX::Models::Model3D{ "Material Ball", "MaterialBall.obj" });
	this->availableModels->Append(ref new Kuplung_DX::Models::Model3D{ "Material Ball - Blender", "MaterialBallBlender.obj" });
	this->lvModels->ItemsSource = this->availableModels;

	this->showLogWindow = true;

	this->panelLog->Width = Kuplung_DX::App::LogWindowWidth;
	this->panelLog->Height = Kuplung_DX::App::LogWindowHeight;
	this->svLog->Width = Kuplung_DX::App::LogWindowWidth;
	this->svLog->Height = Kuplung_DX::App::LogWindowHeight - 40;
	this->nbLogWidth->Text = Kuplung_DX::Utilities::CXXUtils::ConvertInt32ToPlatformString(Kuplung_DX::App::LogWindowWidth);
	this->nbLogHeight->Text = Kuplung_DX::Utilities::CXXUtils::ConvertInt32ToPlatformString(Kuplung_DX::App::LogWindowHeight);

	this->managerParsers = std::make_unique<Importers::FileModelManager>();
	this->managerParsers->init(
		[this](float progress) {
			this->DoProgress(progress);
		}
	);
}

DirectXPage::~DirectXPage() {
	// Stop rendering and processing events on destruction.
	m_main->StopRenderLoop();
	m_coreInput->Dispatcher->StopProcessEvents();
	this->managerParsers.reset();
}

// Saves the current state of the app for suspend and terminate events.
void DirectXPage::SaveInternalState(IPropertySet^ state) {
	critical_section::scoped_lock lock(m_main->GetCriticalSection());
	m_deviceResources->Trim();

	// Stop rendering when the app is suspended.
	m_main->StopRenderLoop();

	// Put code to save app state here.
}

// Loads the current state of the app for resume events.
void DirectXPage::LoadInternalState(IPropertySet^ state) {
	// Put code to load app state here.

	// Start rendering when the app is resumed.
	m_main->StartRenderLoop();
}

// Window event handlers.

void DirectXPage::OnVisibilityChanged(CoreWindow^ sender, VisibilityChangedEventArgs^ args) {
	m_windowVisible = args->Visible;
	if (m_windowVisible)
		m_main->StartRenderLoop();
	else
		m_main->StopRenderLoop();
}

// DisplayInformation event handlers.

void DirectXPage::OnDpiChanged(DisplayInformation^ sender, Object^ args) {
	critical_section::scoped_lock lock(m_main->GetCriticalSection());
	// Note: The value for LogicalDpi retrieved here may not match the effective DPI of the app
	// if it is being scaled for high resolution devices. Once the DPI is set on DeviceResources,
	// you should always retrieve it using the GetDpi method.
	// See DeviceResources.cpp for more details.
	m_deviceResources->SetDpi(sender->LogicalDpi);
	m_main->CreateWindowSizeDependentResources();
}

void DirectXPage::OnOrientationChanged(DisplayInformation^ sender, Object^ args) {
	critical_section::scoped_lock lock(m_main->GetCriticalSection());
	m_deviceResources->SetCurrentOrientation(sender->CurrentOrientation);
	m_main->CreateWindowSizeDependentResources();
}

void DirectXPage::OnDisplayContentsInvalidated(DisplayInformation^ sender, Object^ args) {
	critical_section::scoped_lock lock(m_main->GetCriticalSection());
	m_deviceResources->ValidateDevice();
}

void DirectXPage::OnPointerPressed(Object^ sender, PointerEventArgs^ e) {
	// When the pointer is pressed begin tracking the pointer movement.
	m_main->StartTracking();
}

void DirectXPage::OnPointerMoved(Object^ sender, PointerEventArgs^ e) {
	// Update the pointer tracking code.
	if (m_main->IsTracking())
		m_main->TrackingUpdate(e->CurrentPoint->Position.X);
}

void DirectXPage::OnPointerReleased(Object^ sender, PointerEventArgs^ e) {
	// Stop tracking pointer movement when the pointer is released.
	m_main->StopTracking();
}

void DirectXPage::OnCompositionScaleChanged(SwapChainPanel^ sender, Object^ args) {
	critical_section::scoped_lock lock(m_main->GetCriticalSection());
	m_deviceResources->SetCompositionScale(sender->CompositionScaleX, sender->CompositionScaleY);
	m_main->CreateWindowSizeDependentResources();
}

void DirectXPage::OnSwapChainPanelSizeChanged(Object^ sender, SizeChangedEventArgs^ e) {
	critical_section::scoped_lock lock(m_main->GetCriticalSection());
	m_deviceResources->SetLogicalSize(e->NewSize);
	m_main->CreateWindowSizeDependentResources();
}

void DirectXPage::AddToLog(Platform::String^ message) {
	LogMessage += message;
}

#pragma region MainMenu
void Kuplung_DX::DirectXPage::MenuGUIControls_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e) {
	panelGUI->Visibility = panelGUI->Visibility == Windows::UI::Xaml::Visibility::Visible ? Windows::UI::Xaml::Visibility::Collapsed : Windows::UI::Xaml::Visibility::Visible;
}

void Kuplung_DX::DirectXPage::MenuSceneControls_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e) {
	panelModels->Visibility = panelModels->Visibility == Windows::UI::Xaml::Visibility::Visible ? Windows::UI::Xaml::Visibility::Collapsed : Windows::UI::Xaml::Visibility::Visible;
}

void Kuplung_DX::DirectXPage::MenuCube_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e) {
	Kuplung_DX::App::ViewSampleScene = !Kuplung_DX::App::ViewSampleScene;
}

void Kuplung_DX::DirectXPage::MenuFPSCounter_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e) {
	Kuplung_DX::App::ViewFPSCounter = !Kuplung_DX::App::ViewFPSCounter;
}

void Kuplung_DX::DirectXPage::MenuShowLogWindow_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e) {
	this->showLogWindow = !this->showLogWindow;
	this->panelLog->Visibility = this->showLogWindow ? Windows::UI::Xaml::Visibility::Visible : Windows::UI::Xaml::Visibility::Collapsed;
}
#pragma endregion

void DirectXPage::LogInfo(Object^ parameter, bool addToLog) {
	auto paraString = parameter->ToString();
	auto formattedText = std::wstring(paraString->Data()).append(L"\r\n");
	OutputDebugString(formattedText.c_str());
	if (addToLog) {
		this->LogMessage = parameter->ToString() + L"\r\n" + this->LogMessage;
		this->txtLog->Text = this->LogMessage;
	}
}

void Kuplung_DX::DirectXPage::LogWindowClear_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e) {
	this->LogMessage = "";
	this->txtLog->Text = "";
}

void Kuplung_DX::DirectXPage::logSize_KeyDown(Platform::Object^ sender, Windows::UI::Xaml::Input::KeyRoutedEventArgs^ e) {
	if (e->Key == Windows::System::VirtualKey::Enter)
		this->LogWindowResize_Click(nullptr, nullptr);
}

void Kuplung_DX::DirectXPage::LogWindowResize_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e) {
	int32 w = Kuplung_DX::Utilities::CXXUtils::ConvertPlatformStringToInt32(this->nbLogWidth->Text);
	int32 h = Kuplung_DX::Utilities::CXXUtils::ConvertPlatformStringToInt32(this->nbLogHeight->Text);

	this->panelLog->Width = w;
	this->panelLog->Height = h;
	this->svLog->Width = w;
	this->svLog->Height = h - 40;
}

void Kuplung_DX::DirectXPage::lvModels_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::SelectionChangedEventArgs^ e) {
	this->pnlLoading->Visibility = Windows::UI::Xaml::Visibility::Visible;
	this->grdLoading->Width = Window::Current->CoreWindow->Bounds.Width;
	this->grdLoading->Height = Window::Current->CoreWindow->Bounds.Height;
	Kuplung_DX::Models::Model3D^ m = this->availableModels->GetAt(lvModels->SelectedIndex);
	Platform::String^ modelFile = Kuplung_DX::App::ApplicationPath + "\\Objects\\Shapes\\" + m->Filename;
	this->LogInfo("Opening " + modelFile + " ... ", true);
	auto workItemHandler = ref new WorkItemHandler([this, modelFile](IAsyncAction^) {
			this->ParseModelAsync(modelFile);
		});
	auto r = ThreadPool::RunAsync(workItemHandler, WorkItemPriority::High, WorkItemOptions::TimeSliced);

}

void Kuplung_DX::DirectXPage::ParseModelAsync(Platform::String^ modelFile) {
	std::vector<Kuplung_DX::Models::MeshModel> mms = this->managerParsers->parse(Kuplung_DX::Utilities::CXXUtils::PlatformStringToString(modelFile), std::vector<std::string>());
	this->pnlLoading->Dispatcher->RunAsync(Windows::UI::Core::CoreDispatcherPriority::Normal, ref new Windows::UI::Core::DispatchedHandler([this, mms]
		{
			this->pnlLoading->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
			this->m_main->AddModels(mms);
		}));
}

void Kuplung_DX::DirectXPage::DoProgress(float progress) {
	Kuplung_DX::DirectXPage^ ctx = this;
	Platform::String^ p = Kuplung_DX::Utilities::CXXUtils::StringToPlatformString(Kuplung_DX::Utilities::CXXUtils::StringFormat("Processing ... %0.2f%%", progress));
	Windows::ApplicationModel::Core::CoreApplication::MainView->CoreWindow->Dispatcher->RunAsync(Windows::UI::Core::CoreDispatcherPriority::Normal, ref new Windows::UI::Core::DispatchedHandler([ctx, p]()
		{
			//ctx->LogInfo(p, true);
			ctx->txtLoading->Text = p;
		}));
}
