#pragma once

#include <DX\StepTimer.h>
#include <DX\DeviceResources.h>
#include <Sample\Sample3DSceneRenderer.h>
#include <Sample\SampleLines.h>
#include <Sample\SampleFpsTextRenderer.h>
#include <Rendering\RenderingManager.h>
#include <Rendering\ObjectsManager.h>
#include <Models\Shape.h>
#include <Rendering\Models\Model3D.h>
#include <DX\DeviceResources.h>

// Renders Direct2D and 3D content on the screen.
namespace Kuplung_DX
{
	class Kuplung_DXMain : public DX::IDeviceNotify
	{
	public:
		Kuplung_DXMain(const std::shared_ptr<DX::DeviceResources>& deviceResources);
		~Kuplung_DXMain();
		void CreateWindowSizeDependentResources();
		void StartTracking();
		void TrackingUpdate(float positionX) { m_pointerLocationX = positionX; }
		void StopTracking();
		bool IsTracking();
		void StartRenderLoop();
		void StopRenderLoop();
		Concurrency::critical_section& GetCriticalSection() { return m_criticalSection; }

		// IDeviceNotify
		virtual void OnDeviceLost();
		virtual void OnDeviceRestored();

		void AddModels(std::vector<Kuplung_DX::Models::MeshModel> mms);
		void ClearModels();

		std::unique_ptr<Rendering::ObjectsManager> ManagerObjects;

	private:
		void ProcessInput();
		void Update();
		bool Render();

		// Cached pointer to device resources.
		std::shared_ptr<DX::DeviceResources> m_deviceResources;

		// TODO: Replace with your own content renderers.
		std::unique_ptr<Sample::Sample3DSceneRenderer> m_sampleSceneRenderer;
		std::unique_ptr<Sample::SampleLines> m_sampleLines;
		std::unique_ptr<Sample::SampleFpsTextRenderer> m_fpsTextRenderer;
		std::unique_ptr<Rendering::RenderingManager> m_renderingManager;

		Windows::Foundation::IAsyncAction^ m_renderLoopWorker;
		Concurrency::critical_section m_criticalSection;

		// Rendering loop timer.
		DX::StepTimer m_timer;

		// Track current input pointer position.
		float m_pointerLocationX;

		std::vector<Models::MeshModel> meshModels;
		std::vector<std::unique_ptr<Rendering::Models::Model3D>> models3D;
	};
}