#include "pch.h"
#include "Kuplung_DXMain.h"
#include "Common\DirectXHelper.h"

using namespace Kuplung_DX;
using namespace Windows::Foundation;
using namespace Windows::System::Threading;
using namespace Concurrency;

// Loads and initializes application assets when the application is loaded.
Kuplung_DXMain::Kuplung_DXMain(const std::shared_ptr<DX::DeviceResources>& deviceResources) : m_deviceResources(deviceResources), m_pointerLocationX(0.0f) {
	// Register to be notified if the Device is lost or recreated
	m_deviceResources->RegisterDeviceNotify(this);

	// Samples
	m_sampleSceneRenderer = std::unique_ptr<Sample::Sample3DSceneRenderer>(new Sample::Sample3DSceneRenderer(m_deviceResources));
	m_fpsTextRenderer = std::unique_ptr<Sample::SampleFpsTextRenderer>(new Sample::SampleFpsTextRenderer(m_deviceResources));

	// Rendering
	m_renderingManager = std::unique_ptr<Rendering::RenderingManager>(new Rendering::RenderingManager(m_deviceResources));

	// TODO: Change the timer settings if you want something other than the default variable timestep mode.
	// e.g. for 60 FPS fixed timestep update logic, call:
	/*
	m_timer.SetFixedTimeStep(true);
	m_timer.SetTargetElapsedSeconds(1.0 / 60);
	*/
}

Kuplung_DXMain::~Kuplung_DXMain() {
	m_renderingManager.reset();
	m_deviceResources->RegisterDeviceNotify(nullptr);
}

// Updates application state when the window size changes (e.g. device orientation change)
void Kuplung_DXMain::CreateWindowSizeDependentResources()  {
	m_sampleSceneRenderer->CreateWindowSizeDependentResources();
}

void Kuplung_DXMain::StartTracking() {
	if (Kuplung_DX::App::ViewSampleScene)
		m_sampleSceneRenderer->StartTracking();

	m_renderingManager->StartTracking(this->models3D);
}

void Kuplung_DXMain::StopTracking() {
	if (Kuplung_DX::App::ViewSampleScene)
		m_sampleSceneRenderer->StopTracking();

	m_renderingManager->StopTracking(this->models3D);
}

bool Kuplung_DXMain::IsTracking() {
	if (Kuplung_DX::App::ViewSampleScene)
		return m_sampleSceneRenderer->IsTracking();

	return m_renderingManager->IsTracking(this->models3D);
}

void Kuplung_DXMain::StartRenderLoop() {
	// If the animation render loop is already running then do not start another thread.
	if (m_renderLoopWorker != nullptr && m_renderLoopWorker->Status == AsyncStatus::Started)
		return;

	// Create a task that will be run on a background thread.
	auto workItemHandler = ref new WorkItemHandler([this](IAsyncAction ^ action) {
		// Calculate the updated frame and render once per vertical blanking interval.
		while (action->Status == AsyncStatus::Started) {
			critical_section::scoped_lock lock(m_criticalSection);
			Update();
			if (Render())
				m_deviceResources->Present();
		}
	});

	// Run task on a dedicated high priority background thread.
	m_renderLoopWorker = ThreadPool::RunAsync(workItemHandler, WorkItemPriority::High, WorkItemOptions::TimeSliced);
}

void Kuplung_DXMain::StopRenderLoop() {
	m_renderLoopWorker->Cancel();
}

// Updates the application state once per frame.
void Kuplung_DXMain::Update()  {
	ProcessInput();

	// Update scene objects.
	m_timer.Tick([&]() {
		if (Kuplung_DX::App::ViewSampleScene)
			m_sampleSceneRenderer->Update(m_timer);
		if (Kuplung_DX::App::ViewFPSCounter)
			m_fpsTextRenderer->Update(m_timer);

		m_renderingManager->Update(this->models3D, m_timer);
	});
}

// Process all input from the user before updating game state
void Kuplung_DXMain::ProcessInput() {
	if (Kuplung_DX::App::ViewSampleScene)
		m_sampleSceneRenderer->TrackingUpdate(m_pointerLocationX);

	m_renderingManager->TrackingUpdate(this->models3D, m_pointerLocationX);
}

// Renders the current frame according to the current application state.
// Returns true if the frame was rendered and is ready to be displayed.
bool Kuplung_DXMain::Render()  {
	// Don't try to render anything before the first Update.
	if (m_timer.GetFrameCount() == 0)
		return false;

	auto context = m_deviceResources->GetD3DDeviceContext();

	// Reset the viewport to target the whole screen.
	auto viewport = m_deviceResources->GetScreenViewport();
	context->RSSetViewports(1, &viewport);

	// Reset render targets to the screen.
	ID3D11RenderTargetView *const targets[1] = { m_deviceResources->GetBackBufferRenderTargetView() };
	context->OMSetRenderTargets(1, targets, m_deviceResources->GetDepthStencilView());

	// Clear the back buffer and depth stencil view.
	const float bgc[4] = { 70.0f / 255.0f, 70.0f / 255.0f, 70.0f / 255.0f, 255.0f / 255.0f };
	context->ClearRenderTargetView(m_deviceResources->GetBackBufferRenderTargetView(), bgc);
	context->ClearDepthStencilView(m_deviceResources->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// Render the scene objects.
	if (Kuplung_DX::App::ViewSampleScene)
		m_sampleSceneRenderer->Render();
	if (Kuplung_DX::App::ViewFPSCounter)
		m_fpsTextRenderer->Render();

	m_renderingManager->Render(this->models3D);

	return true;
}

// Notifies renderers that device resources need to be released.
void Kuplung_DXMain::OnDeviceLost() {
	m_sampleSceneRenderer->ReleaseDeviceDependentResources();
	m_fpsTextRenderer->ReleaseDeviceDependentResources();
}

// Notifies renderers that device resources may now be recreated.
void Kuplung_DXMain::OnDeviceRestored() {
	m_sampleSceneRenderer->CreateDeviceDependentResources();
	m_fpsTextRenderer->CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();
}

void Kuplung_DXMain::AddModels(std::vector<Kuplung_DX::Models::MeshModel> mms) {
	this->meshModels.insert(end(this->meshModels), begin(mms), end(mms));

	for each (Kuplung_DX::Models::MeshModel m in mms) {
		auto m3d = std::unique_ptr<Kuplung_DX::Rendering::Models::Model3D>(new Kuplung_DX::Rendering::Models::Model3D(m_deviceResources));
		m3d->InitModel3D(m);
		this->models3D.push_back(std::move(m3d));
	}
}

void Kuplung_DXMain::ClearModels() {
	for each (auto& m in this->models3D) {
		m->ReleaseDeviceDependentResources();
	}
	this->meshModels.clear();
	this->models3D.clear();
}