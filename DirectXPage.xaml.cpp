//
// DirectXPage.xaml.cpp
// Implementation of the DirectXPage class.
//

#include "pch.h"
#include "DirectXPage.xaml.h"
#include <ppltasks.h>

using namespace Kuplung_DX;
using namespace Kuplung_DX::Utilities;

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
	window->KeyDown += ref new Windows::Foundation::TypedEventHandler<Windows::UI::Core::CoreWindow^, Windows::UI::Core::KeyEventArgs^>(this, &DirectXPage::OnKeyDown);
	window->KeyUp += ref new Windows::Foundation::TypedEventHandler<Windows::UI::Core::CoreWindow^, Windows::UI::Core::KeyEventArgs^>(this, &DirectXPage::OnKeyUp);
	CoreWindow::GetForCurrentThread()->Dispatcher->AcceleratorKeyActivated += ref new TypedEventHandler<CoreDispatcher^, AcceleratorKeyEventArgs^>(this, &DirectXPage::OnAcceleratorKeyActivated);
	Windows::Devices::Input::MouseDevice::GetForCurrentView()->MouseMoved += ref new TypedEventHandler<Windows::Devices::Input::MouseDevice^, Windows::Devices::Input::MouseEventArgs^>(this, &DirectXPage::OnMouseMoved);

	DisplayInformation^ currentDisplayInformation = DisplayInformation::GetForCurrentView();

	currentDisplayInformation->DpiChanged += ref new TypedEventHandler<DisplayInformation^, Object^>(this, &DirectXPage::OnDpiChanged);
	currentDisplayInformation->OrientationChanged += ref new TypedEventHandler<DisplayInformation^, Object^>(this, &DirectXPage::OnOrientationChanged);
	DisplayInformation::DisplayContentsInvalidated += ref new TypedEventHandler<DisplayInformation^, Object^>(this, &DirectXPage::OnDisplayContentsInvalidated);
	this->swapChainPanel->CompositionScaleChanged += ref new TypedEventHandler<SwapChainPanel^, Object^>(this, &DirectXPage::OnCompositionScaleChanged);
	this->swapChainPanel->SizeChanged += ref new SizeChangedEventHandler(this, &DirectXPage::OnSwapChainPanelSizeChanged);

	// At this point we have access to the device. 
	// We can create the device-dependent resources.
	this->m_deviceResources = std::make_shared<DX::DeviceResources>();
	this->m_deviceResources->SetSwapChainPanel(this->swapChainPanel);

	// Register our SwapChainPanel to get independent input pointer events
	auto workItemHandler = ref new WorkItemHandler([this](IAsyncAction^) {
		// The CoreIndependentInputSource will raise pointer events for the specified device types on whichever thread it's created on.
		m_coreInput = this->swapChainPanel->CreateCoreIndependentInputSource(
			Windows::UI::Core::CoreInputDeviceTypes::Mouse |
			Windows::UI::Core::CoreInputDeviceTypes::Touch |
			Windows::UI::Core::CoreInputDeviceTypes::Pen
		);

		// Register for pointer events, which will be raised on the background thread.
		m_coreInput->PointerPressed += ref new TypedEventHandler<Object^, PointerEventArgs^>(this, &DirectXPage::OnPointerPressed);
		m_coreInput->PointerMoved += ref new TypedEventHandler<Object^, PointerEventArgs^>(this, &DirectXPage::OnPointerMoved);
		m_coreInput->PointerReleased += ref new TypedEventHandler<Object^, PointerEventArgs^>(this, &DirectXPage::OnPointerReleased);
		m_coreInput->PointerWheelChanged += ref new TypedEventHandler<Object^, PointerEventArgs^>(this, &DirectXPage::OnPointerWheelChanged);

		// Begin processing input messages as they're delivered.
		m_coreInput->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessUntilQuit);
		});

	// Run task on a dedicated high priority background thread.
	this->m_inputLoopWorker = ThreadPool::RunAsync(workItemHandler, WorkItemPriority::High, WorkItemOptions::TimeSliced);

	this->m_main = std::unique_ptr<Kuplung_DXMain>(new Kuplung_DXMain(this->m_deviceResources));
	this->m_main->StartRenderLoop();

	this->availableModels = ref new Platform::Collections::Vector<Kuplung_DX::Models::Shape^>();
	this->availableModels->Append(ref new Kuplung_DX::Models::Shape{ "Triangle", "triangle.objk" });
	this->availableModels->Append(ref new Kuplung_DX::Models::Shape{ "Cone", "cone.objk" });
	this->availableModels->Append(ref new Kuplung_DX::Models::Shape{ "Cube", "cube.objk" });
	this->availableModels->Append(ref new Kuplung_DX::Models::Shape{ "Cylinder", "cylinder.objk" });
	this->availableModels->Append(ref new Kuplung_DX::Models::Shape{ "Grid", "grid.objk" });
	this->availableModels->Append(ref new Kuplung_DX::Models::Shape{ "Ico Sphere", "ico_sphere.objk" });
	this->availableModels->Append(ref new Kuplung_DX::Models::Shape{ "Plane", "plane.objk" });
	this->availableModels->Append(ref new Kuplung_DX::Models::Shape{ "Torus", "torus.objk" });
	this->availableModels->Append(ref new Kuplung_DX::Models::Shape{ "Tube", "tube.objk" });
	this->availableModels->Append(ref new Kuplung_DX::Models::Shape{ "UV Sphere", "uv_sphere.objk" });
	this->availableModels->Append(ref new Kuplung_DX::Models::Shape{ "Monkey Head", "monkey_head.objk" });
	this->availableModels->Append(ref new Kuplung_DX::Models::Shape{ "Epcot", "epcot.objk" });
	this->availableModels->Append(ref new Kuplung_DX::Models::Shape{ "Brick Wall", "brick_wall.objk" });
	this->availableModels->Append(ref new Kuplung_DX::Models::Shape{ "Plane Objects", "plane_objects.objk" });
	this->availableModels->Append(ref new Kuplung_DX::Models::Shape{ "Plane Objects - Large Plane", "plane_objects_large.objk" });
	this->availableModels->Append(ref new Kuplung_DX::Models::Shape{ "Material Ball", "MaterialBall.objk" });
	this->availableModels->Append(ref new Kuplung_DX::Models::Shape{ "Material Ball - Blender", "MaterialBallBlender.objk" });
	this->lvModels->ItemsSource = this->availableModels;

	this->guiObjects = ref new Platform::Collections::Vector<Kuplung_DX::Models::GuiObject^>();
	this->guiObjects->Append(ref new Kuplung_DX::Models::GuiObject{ 0, "General", L"\xE713" });
	this->guiObjects->Append(ref new Kuplung_DX::Models::GuiObject{ 1, "Camera", L"\xE7B3" });
	this->guiObjects->Append(ref new Kuplung_DX::Models::GuiObject{ 2, "Camera Model", L"\xE714" });
	this->guiObjects->Append(ref new Kuplung_DX::Models::GuiObject{ 3, "Grid", L"\xF0E2" });
	this->guiObjects->Append(ref new Kuplung_DX::Models::GuiObject{ 4, "Scene Lights", L"\xE754" });
	this->guiObjects->Append(ref new Kuplung_DX::Models::GuiObject{ 5, "Skybox", L"\xF16E" });
	this->guiObjects->Append(ref new Kuplung_DX::Models::GuiObject{ 6, "Lights", L"\xEA80" });
	this->tvGuiObjects->ItemsSource = this->guiObjects;

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

	this->InitializeGUIControlsValues();

	this->KeyPressed_Alt = false;
	this->KeyPressed_Ctrl = false;
	this->KeyPressed_Shift = false;
	this->MousePressed_Left = false;
	this->MousePressed_Middle = false;
	this->MousePressed_Right = false;

	this->pointerDelta.x = 0.0f;
	this->pointerDelta.y = 0.0f;
}

DirectXPage::~DirectXPage() {
	// Stop rendering and processing events on destruction.
	this->m_main->StopRenderLoop();
	this->m_coreInput->Dispatcher->StopProcessEvents();
	this->managerParsers.reset();
}

// Saves the current state of the app for suspend and terminate events.
void DirectXPage::SaveInternalState(IPropertySet^ state) {
	critical_section::scoped_lock lock(this->m_main->GetCriticalSection());
	this->m_deviceResources->Trim();

	// Stop rendering when the app is suspended.
	this->m_main->StopRenderLoop();

	// Put code to save app state here.
}

// Loads the current state of the app for resume events.
void DirectXPage::LoadInternalState(IPropertySet^ state) {
	// Put code to load app state here.

	// Start rendering when the app is resumed.
	this->m_main->StartRenderLoop();
}

// Window event handlers.

void DirectXPage::OnVisibilityChanged(CoreWindow^ sender, VisibilityChangedEventArgs^ args) {
	this->m_windowVisible = args->Visible;
	if (this->m_windowVisible)
		this->m_main->StartRenderLoop();
	else
		this->m_main->StopRenderLoop();
}

void DirectXPage::OnKeyDown(CoreWindow^ window, KeyEventArgs^ args) {
	this->KeyPressed_Ctrl = false;
	this->KeyPressed_Shift = false;

	Windows::System::VirtualKey key = args->VirtualKey;

	if (key == Windows::System::VirtualKey::Control)
		this->KeyPressed_Ctrl = args->KeyStatus.WasKeyDown;
	if (key == Windows::System::VirtualKey::Shift)
		this->KeyPressed_Shift = args->KeyStatus.WasKeyDown;
}

void DirectXPage::OnKeyUp(CoreWindow^ window, KeyEventArgs^ args) {
	this->KeyPressed_Ctrl = false;
	this->KeyPressed_Shift = false;
}

void DirectXPage::OnMouseMoved(Windows::Devices::Input::MouseDevice^ mouseDevice, Windows::Devices::Input::MouseEventArgs^ args) {
	this->pointerDelta.x = static_cast<float>(args->MouseDelta.X);
	this->pointerDelta.y = static_cast<float>(args->MouseDelta.Y);
}

void DirectXPage::OnAcceleratorKeyActivated(CoreDispatcher^ sender, AcceleratorKeyEventArgs^ args) {
	this->KeyPressed_Alt = false;
	if (args->EventType == CoreAcceleratorKeyEventType::SystemKeyDown && args->VirtualKey == Windows::System::VirtualKey::Menu)
		this->KeyPressed_Alt = args->KeyStatus.IsMenuKeyDown;
}

// DisplayInformation event handlers.

void DirectXPage::OnDpiChanged(DisplayInformation^ sender, Object^ args) {
	critical_section::scoped_lock lock(this->m_main->GetCriticalSection());
	// Note: The value for LogicalDpi retrieved here may not match the effective DPI of the app
	// if it is being scaled for high resolution devices. Once the DPI is set on DeviceResources,
	// you should always retrieve it using the GetDpi method.
	// See DeviceResources.cpp for more details.
	this->m_deviceResources->SetDpi(sender->LogicalDpi);
	this->m_main->CreateWindowSizeDependentResources();
}

void DirectXPage::OnOrientationChanged(DisplayInformation^ sender, Object^ args) {
	critical_section::scoped_lock lock(this->m_main->GetCriticalSection());
	this->m_deviceResources->SetCurrentOrientation(sender->CurrentOrientation);
	this->m_main->CreateWindowSizeDependentResources();
}

void DirectXPage::OnDisplayContentsInvalidated(DisplayInformation^ sender, Object^ args) {
	critical_section::scoped_lock lock(this->m_main->GetCriticalSection());
	this->m_deviceResources->ValidateDevice();
}

void DirectXPage::OnPointerPressed(Object^ sender, PointerEventArgs^ e) {
	this->m_main->StartTracking();

	this->MousePressed_Left = false;
	this->MousePressed_Middle = false;
	this->MousePressed_Right = false;

	PointerPoint^ p = e->CurrentPoint;
	PointerPointProperties^ pp = p->Properties;
	this->MousePressed_Left = pp->IsLeftButtonPressed;
	this->MousePressed_Middle = pp->IsMiddleButtonPressed;
	this->MousePressed_Right = pp->IsRightButtonPressed;
}

void DirectXPage::OnPointerMoved(Object^ sender, PointerEventArgs^ e) {
	if (this->m_main->IsTracking())
		this->m_main->TrackingUpdate(e->CurrentPoint->Position.X);

	if (this->MousePressed_Middle) {
		if (e->CurrentPoint->Position.X > 0)
			this->m_main->ManagerObjects->CompCamera->RotateX->point += this->pointerDelta.y;
		if (e->CurrentPoint->Position.X < 0)
			this->m_main->ManagerObjects->CompCamera->RotateX->point += this->pointerDelta.y;
		if (this->m_main->ManagerObjects->CompCamera->RotateX->point > 360.0f)
			this->m_main->ManagerObjects->CompCamera->RotateX->point = 0.0f;
		if (this->m_main->ManagerObjects->CompCamera->RotateX->point < 0.0f)
			this->m_main->ManagerObjects->CompCamera->RotateX->point = 360.0f;

		if (e->CurrentPoint->Position.Y < 0)
			this->m_main->ManagerObjects->CompCamera->RotateY->point += this->pointerDelta.x;
		if (e->CurrentPoint->Position.Y > 0)
			this->m_main->ManagerObjects->CompCamera->RotateY->point += this->pointerDelta.x;
		if (this->m_main->ManagerObjects->CompCamera->RotateY->point > 360.0f)
			this->m_main->ManagerObjects->CompCamera->RotateY->point = 0.0f;
		if (this->m_main->ManagerObjects->CompCamera->RotateY->point < 0.0f)
			this->m_main->ManagerObjects->CompCamera->RotateY->point = 360.0f;
	}

	if (this->KeyPressed_Shift) {
		if (e->CurrentPoint->Position.X < 0)
			this->m_main->ManagerObjects->CompCamera->PositionX->point += 0.1f;
		else if (e->CurrentPoint->Position.X > 0)
			this->m_main->ManagerObjects->CompCamera->PositionX->point -= 0.1f;
		else if (e->CurrentPoint->Position.Y > 0)
			this->m_main->ManagerObjects->CompCamera->PositionY->point += 0.1f;
		else if (e->CurrentPoint->Position.Y < 0)
			this->m_main->ManagerObjects->CompCamera->PositionY->point -= 0.1f;
	}
}

void DirectXPage::OnPointerReleased(Object^ sender, PointerEventArgs^ e) {
	this->m_main->StopTracking();
	this->MousePressed_Left = false;
	this->MousePressed_Middle = false;
	this->MousePressed_Right = false;
}

void DirectXPage::OnPointerWheelChanged(Object^ sender, PointerEventArgs^ e) {
	PointerPoint^ point = e->CurrentPoint;
	int d = point->Properties->MouseWheelDelta / WHEEL_DELTA;

	if (this->KeyPressed_Alt) {
		if (d < 0)
			Kuplung_DX::App::Setting_FOV += 4;
		else if (d > 0)
			Kuplung_DX::App::Setting_FOV -= 4;

		if (Kuplung_DX::App::Setting_FOV > 180)
			Kuplung_DX::App::Setting_FOV = 180;
		if (Kuplung_DX::App::Setting_FOV < -180)
			Kuplung_DX::App::Setting_FOV = -180;
	}
	else
		this->m_main->ManagerObjects->CompCamera->PositionZ->point += d;
}

void DirectXPage::OnCompositionScaleChanged(SwapChainPanel^ sender, Object^ args) {
	critical_section::scoped_lock lock(this->m_main->GetCriticalSection());
	this->m_deviceResources->SetCompositionScale(sender->CompositionScaleX, sender->CompositionScaleY);
	this->m_main->CreateWindowSizeDependentResources();
}

void DirectXPage::OnSwapChainPanelSizeChanged(Object^ sender, SizeChangedEventArgs^ e) {
	critical_section::scoped_lock lock(this->m_main->GetCriticalSection());
	this->m_deviceResources->SetLogicalSize(e->NewSize);
	this->m_main->CreateWindowSizeDependentResources();
}

void DirectXPage::AddToLog(Platform::String^ message) {
	LogMessage += message;
}

#pragma region MainMenu
void Kuplung_DX::DirectXPage::MenuGUIControls_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e) {
	this->panelGUI->Visibility = panelGUI->Visibility == Windows::UI::Xaml::Visibility::Visible ? Windows::UI::Xaml::Visibility::Collapsed : Windows::UI::Xaml::Visibility::Visible;
}

void Kuplung_DX::DirectXPage::MenuSceneControls_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e) {
	this->panelModels->Visibility = panelModels->Visibility == Windows::UI::Xaml::Visibility::Visible ? Windows::UI::Xaml::Visibility::Collapsed : Windows::UI::Xaml::Visibility::Visible;
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

void Kuplung_DX::DirectXPage::MenuNew_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e) {
	this->m_main->ClearModels();
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
	Kuplung_DX::Models::Shape^ m = this->availableModels->GetAt(lvModels->SelectedIndex);
	Platform::String^ modelFile = Kuplung_DX::App::ApplicationPath + "\\..\\Resources\\Objects\\Shapes\\" + m->Filename;
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

void Kuplung_DX::DirectXPage::tvGuiObjects_Tapped(Platform::Object^ sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs^ e) {
	auto item = (Kuplung_DX::Models::GuiObject^)this->tvGuiObjects->SelectedItem;
	this->pnlGuiGeneral->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
	this->pnlGuiCamera->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
	this->pnlGuiCameraModel->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
	this->pnlGuiGrid->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
	this->pnlGuiSceneLights->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
	this->pnlGuiSkybox->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
	this->pnlGuiLights->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
	switch (item->Id)
	{
	case 1:
		this->pnlGuiCamera->Visibility = Windows::UI::Xaml::Visibility::Visible;
		break;
	case 2:
		this->pnlGuiCameraModel->Visibility = Windows::UI::Xaml::Visibility::Visible;
		break;
	case 3:
		this->pnlGuiGrid->Visibility = Windows::UI::Xaml::Visibility::Visible;
		break;
	case 4:
		this->pnlGuiSceneLights->Visibility = Windows::UI::Xaml::Visibility::Visible;
		break;
	case 5:
		this->pnlGuiSkybox->Visibility = Windows::UI::Xaml::Visibility::Visible;
		break;
	case 6:
		this->pnlGuiLights->Visibility = Windows::UI::Xaml::Visibility::Visible;
		break;
	default:
		this->pnlGuiGeneral->Visibility = Windows::UI::Xaml::Visibility::Visible;
		break;
	}
	this->svGUIControlsPanels->Height = Kuplung_DX::App::GUI_ControlsPanelsHeight - this->headerGUI->ActualHeight - this->btnResetGuiValues->ActualHeight - this->tvGuiObjects->ActualHeight - 80;
}

#pragma region GUI Controls events
void Kuplung_DX::DirectXPage::ButtonResetValuesGuiControls_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e) {
	Kuplung_DX::App::Setting_FOV = 45.0f;
	Kuplung_DX::App::Setting_RatioWidth = 4.0f;
	Kuplung_DX::App::Setting_RatioHeight = 3.0f;
	Kuplung_DX::App::Setting_PlaneClose = 0.1f;
	Kuplung_DX::App::Setting_PlaneFar = 100.0f;
	Kuplung_DX::App::GridSize = 30;
	Kuplung_DX::App::GridUnitSize = 1;
	Kuplung_DX::App::ShowGrid = true;

	this->m_main->ManagerObjects->CompCamera->InitProperties();
	this->m_main->ManagerObjects->CompGrid->InitProperties();

	this->InitializeGUIControlsValues();
}

void Kuplung_DX::DirectXPage::InitializeGUIControlsValues() {
	this->slSetting_FOV->Value = Kuplung_DX::App::Setting_FOV;
	this->slSetting_RatioWidth->Value = Kuplung_DX::App::Setting_RatioWidth;
	this->slSetting_RatioHeight->Value = Kuplung_DX::App::Setting_RatioHeight;
	this->slSetting_PlaneClose->Value = Kuplung_DX::App::Setting_PlaneClose;
	this->slSetting_PlaneFar->Value = Kuplung_DX::App::Setting_PlaneFar;

	this->slLAMEyeX->Value = this->m_main->ManagerObjects->CompCamera->EyeSettings->View_Eye[0];
	this->slLAMEyeY->Value = this->m_main->ManagerObjects->CompCamera->EyeSettings->View_Eye[1];
	this->slLAMEyeZ->Value = this->m_main->ManagerObjects->CompCamera->EyeSettings->View_Eye[2];
	this->slLAMCenterX->Value = this->m_main->ManagerObjects->CompCamera->EyeSettings->View_Center[0];
	this->slLAMCenterY->Value = this->m_main->ManagerObjects->CompCamera->EyeSettings->View_Center[1];
	this->slLAMCenterZ->Value = this->m_main->ManagerObjects->CompCamera->EyeSettings->View_Center[2];
	this->slLAMUpX->Value = this->m_main->ManagerObjects->CompCamera->EyeSettings->View_Up[0];
	this->slLAMUpY->Value = this->m_main->ManagerObjects->CompCamera->EyeSettings->View_Up[0];
	this->slLAMUpZ->Value = this->m_main->ManagerObjects->CompCamera->EyeSettings->View_Up[0];
	this->slCameraPositionX->Value = this->m_main->ManagerObjects->CompCamera->PositionX->point;
	this->slCameraPositionY->Value = this->m_main->ManagerObjects->CompCamera->PositionY->point;
	this->slCameraPositionZ->Value = this->m_main->ManagerObjects->CompCamera->PositionZ->point;
	this->slCameraRotateX->Value = this->m_main->ManagerObjects->CompCamera->RotateX->point;
	this->slCameraRotateY->Value = this->m_main->ManagerObjects->CompCamera->RotateY->point;
	this->slCameraRotateZ->Value = this->m_main->ManagerObjects->CompCamera->RotateZ->point;
	this->slCameraRotateCenterX->Value = this->m_main->ManagerObjects->CompCamera->RotateCenterX->point;
	this->slCameraRotateCenterY->Value = this->m_main->ManagerObjects->CompCamera->RotateCenterY->point;
	this->slCameraRotateCenterZ->Value = this->m_main->ManagerObjects->CompCamera->RotateCenterZ->point;

	this->slGridSize->Value = Kuplung_DX::App::GridSize;
	this->slGridUnitSize->Value = Kuplung_DX::App::GridUnitSize;
	this->chkShowGrid->IsChecked = Kuplung_DX::App::ShowGrid;
	this->slGridPositionX->Value = this->m_main->ManagerObjects->CompGrid->PositionX->point;
	this->slGridPositionY->Value = this->m_main->ManagerObjects->CompGrid->PositionY->point;
	this->slGridPositionZ->Value = this->m_main->ManagerObjects->CompGrid->PositionZ->point;
}

#pragma region General
void Kuplung_DX::DirectXPage::slSetting_FOV_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e) {
	Kuplung_DX::App::Setting_FOV = (float)this->slSetting_FOV->Value;
}

void Kuplung_DX::DirectXPage::slSetting_RatioWidth_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e) {
	Kuplung_DX::App::Setting_RatioWidth = (float)this->slSetting_RatioWidth->Value;
}

void Kuplung_DX::DirectXPage::slSetting_RatioHeight_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e) {
	Kuplung_DX::App::Setting_RatioHeight = (float)this->slSetting_RatioHeight->Value;
}

void Kuplung_DX::DirectXPage::slSetting_PlaneClose_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e) {
	if (!XMScalarNearEqual(Kuplung_DX::App::Setting_PlaneFar, (float)this->slSetting_PlaneClose->Value, 0.00001f))
		Kuplung_DX::App::Setting_PlaneClose = (float)this->slSetting_PlaneClose->Value;
}

void Kuplung_DX::DirectXPage::slSetting_PlaneFar_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e) {
	if (!XMScalarNearEqual(Kuplung_DX::App::Setting_PlaneClose, (float)this->slSetting_PlaneFar->Value, 0.00001f))
		Kuplung_DX::App::Setting_PlaneFar = (float)this->slSetting_PlaneFar->Value;
}
#pragma endregion

#pragma region Camera
void Kuplung_DX::DirectXPage::slLAMEyeX_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e) {
	this->m_main->ManagerObjects->CompCamera->EyeSettings->View_Eye = {
		(float)this->slLAMEyeX->Value,
		this->m_main->ManagerObjects->CompCamera->EyeSettings->View_Eye[1],
		this->m_main->ManagerObjects->CompCamera->EyeSettings->View_Eye[2],
	};
}

void Kuplung_DX::DirectXPage::slLAMEyeY_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e) {
	this->m_main->ManagerObjects->CompCamera->EyeSettings->View_Eye = {
		this->m_main->ManagerObjects->CompCamera->EyeSettings->View_Eye[0],
		(float)this->slLAMEyeY->Value,
		this->m_main->ManagerObjects->CompCamera->EyeSettings->View_Eye[2]
	};
}

void Kuplung_DX::DirectXPage::slLAMEyeZ_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e) {
	this->m_main->ManagerObjects->CompCamera->EyeSettings->View_Eye = {
		this->m_main->ManagerObjects->CompCamera->EyeSettings->View_Eye[0],
		this->m_main->ManagerObjects->CompCamera->EyeSettings->View_Eye[1],
		(float)this->slLAMEyeZ->Value
	};
}

void Kuplung_DX::DirectXPage::slLAMCenterX_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e) {
	this->m_main->ManagerObjects->CompCamera->EyeSettings->View_Center = {
		(float)this->slLAMCenterX->Value,
		this->m_main->ManagerObjects->CompCamera->EyeSettings->View_Center[1],
		this->m_main->ManagerObjects->CompCamera->EyeSettings->View_Center[2]
	};
}

void Kuplung_DX::DirectXPage::slLAMCenterY_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e) {
	this->m_main->ManagerObjects->CompCamera->EyeSettings->View_Center = {
		this->m_main->ManagerObjects->CompCamera->EyeSettings->View_Center[0],
		(float)this->slLAMCenterY->Value,
		this->m_main->ManagerObjects->CompCamera->EyeSettings->View_Center[2]
	};
}

void Kuplung_DX::DirectXPage::slLAMCenterZ_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e) {
	this->m_main->ManagerObjects->CompCamera->EyeSettings->View_Center = {
		this->m_main->ManagerObjects->CompCamera->EyeSettings->View_Center[0],
		this->m_main->ManagerObjects->CompCamera->EyeSettings->View_Center[1],
		(float)this->slLAMCenterZ->Value
	};
}

void Kuplung_DX::DirectXPage::slLAMUpX_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e) {
	this->m_main->ManagerObjects->CompCamera->EyeSettings->View_Up = {
		(float)this->slLAMUpX->Value,
		this->m_main->ManagerObjects->CompCamera->EyeSettings->View_Up[1],
		this->m_main->ManagerObjects->CompCamera->EyeSettings->View_Up[2],
	};
}

void Kuplung_DX::DirectXPage::slLAMUpY_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e) {
	this->m_main->ManagerObjects->CompCamera->EyeSettings->View_Up = {
		this->m_main->ManagerObjects->CompCamera->EyeSettings->View_Up[0],
		(float)this->slLAMUpY->Value,
		this->m_main->ManagerObjects->CompCamera->EyeSettings->View_Up[2]
	};
}

void Kuplung_DX::DirectXPage::slLAMUpZ_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e) {
	this->m_main->ManagerObjects->CompCamera->EyeSettings->View_Up = {
		this->m_main->ManagerObjects->CompCamera->EyeSettings->View_Up[0],
		this->m_main->ManagerObjects->CompCamera->EyeSettings->View_Up[1],
		(float)this->slLAMUpZ->Value
	};
}

void Kuplung_DX::DirectXPage::slCameraPositionX_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e) {
	this->m_main->ManagerObjects->CompCamera->PositionX->point = (float)this->slCameraPositionX->Value;
}

void Kuplung_DX::DirectXPage::slCameraPositionY_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e) {
	this->m_main->ManagerObjects->CompCamera->PositionY->point = (float)this->slCameraPositionY->Value;
}

void Kuplung_DX::DirectXPage::slCameraPositionZ_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e) {
	this->m_main->ManagerObjects->CompCamera->PositionZ->point = (float)this->slCameraPositionZ->Value;
}

void Kuplung_DX::DirectXPage::slCameraRotateX_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e) {
	this->m_main->ManagerObjects->CompCamera->RotateX->point = (float)this->slCameraRotateX->Value;
}

void Kuplung_DX::DirectXPage::slCameraRotateY_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e) {
	this->m_main->ManagerObjects->CompCamera->RotateY->point = (float)this->slCameraRotateY->Value;
}

void Kuplung_DX::DirectXPage::slCameraRotateZ_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e) {
	this->m_main->ManagerObjects->CompCamera->RotateZ->point = (float)this->slCameraRotateZ->Value;
}

void Kuplung_DX::DirectXPage::slCameraRotateCenterX_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e) {
	this->m_main->ManagerObjects->CompCamera->RotateCenterX->point = (float)this->slCameraRotateCenterX->Value;
}

void Kuplung_DX::DirectXPage::slCameraRotateCenterY_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e) {
	this->m_main->ManagerObjects->CompCamera->RotateCenterY->point = (float)this->slCameraRotateCenterY->Value;
}

void Kuplung_DX::DirectXPage::slCameraRotateCenterZ_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e) {
	this->m_main->ManagerObjects->CompCamera->RotateCenterZ->point = (float)this->slCameraRotateCenterZ->Value;
}
#pragma endregion

#pragma region Grid
void Kuplung_DX::DirectXPage::slGridSize_PointerCaptureLost(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e) {
	Kuplung_DX::App::GridSize = (int)this->slGridSize->Value;
}

void Kuplung_DX::DirectXPage::slGridUnitSize_PointerCaptureLost(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e) {
	Kuplung_DX::App::GridUnitSize = (int)this->slGridUnitSize->Value;
}

void Kuplung_DX::DirectXPage::chkShowGrid_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e) {
	Kuplung_DX::App::ShowGrid = this->chkShowGrid->IsChecked->Value;
}

void Kuplung_DX::DirectXPage::slGridPositionX_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e) {
	this->m_main->ManagerObjects->CompGrid->PositionX->point = (float)this->slGridPositionX->Value;
}

void Kuplung_DX::DirectXPage::slGridPositionY_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e) {
	this->m_main->ManagerObjects->CompGrid->PositionY->point = (float)this->slGridPositionY->Value;
}

void Kuplung_DX::DirectXPage::slGridPositionZ_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e) {
	this->m_main->ManagerObjects->CompGrid->PositionZ->point = (float)this->slGridPositionZ->Value;
}
#pragma endregion
#pragma endregion
