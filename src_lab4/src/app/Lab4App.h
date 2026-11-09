#pragma once

#include "pch.h"
#include "platform.h"
#include "render/GUIRHI.h"
#include "ui/Lab4UI.h"
#include "weather/WeatherModel.h"

class CLab4App
{
private:
    struct CWindowCallback : public krystallic::Platform::IPlatformWindowCallback
    {
        krystallic::Platform::IPlatformManager* manager = nullptr;
        bool resizeRequested = true;

        void OnDestroy(const krystallic::Platform::SNativeHandle* window) override;
        void OnSize(const krystallic::Platform::SNativeHandle* window, uint32 width, uint32 height) override;
    };

    krystallic::RefCounted<krystallic::Platform::IPlatformManager> m_platform;
    krystallic::RefCounted<krystallic::Platform::IPlatformWindow> m_window;
    krystallic::RefCounted<krystallic::Platform::IPlatformInputManager> m_inputManager;
    krystallic::RefCounted<krystallic::Platform::IKeyboard> m_keyboard;
    krystallic::RefCounted<krystallic::Platform::IMouse> m_mouse;
    krystallic::RefCounted<krystallic::GUI::IGUIInputContext> m_guiInput;

    CWindowCallback m_windowCallback;
    CGUIRHIFrame m_frame;
    CGUIRHIStrategy m_renderer;
    CLab4UI m_ui;
    CWeatherModel m_weatherModel;

    void* m_nativeWindow = nullptr;
    uint32 m_width = 1060;
    uint32 m_height = 720;
    bool m_inputInitialized = false;
    bool m_f1WasDown = false;

    bool InitializePlatform();
    bool InitializeRenderer();
    bool InitializeInput();
    bool ResizeIfRequested();
    void CopyPlatformInput();
    void Update(float deltaTime);
    bool Render();

public:
    CLab4App() = default;
    ~CLab4App();

    bool Initialize();
    int Run();
    void Shutdown();
};
