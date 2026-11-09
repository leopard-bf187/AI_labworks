#include "Lab4App.h"

#include <chrono>
#include <cstdio>

#if defined(KRYSTALLIC_OS_WINNT)
#include <Windows.h>
#endif

using namespace krystallic;
using namespace krystallic::Platform;
using namespace krystallic::GUI;

void CLab4App::CWindowCallback::OnDestroy(const SNativeHandle* window)
{
    (void)window;
    if (manager)
        manager->RequestQuit(0);
}

void CLab4App::CWindowCallback::OnSize(const SNativeHandle* window, uint32 width, uint32 height)
{
    (void)window;
    (void)width;
    (void)height;
    // Win32 platform callbacks can report zero dimensions; query the client
    // area during the next frame rather than resizing inside the callback.
    resizeRequested = true;
}

CLab4App::~CLab4App()
{
    Shutdown();
}

bool CLab4App::InitializePlatform()
{
#if defined(KRYSTALLIC_OS_WINNT)
    SNativeHandle appHandle{GetModuleHandleW(nullptr), -1, NATIVE_HANDLE_APP_INSTANCE};
    if (CreatePlatformManager(Stdlib::g_stdlibDefaultAllocator, &appHandle, m_platform.ReleaseAndGetAddressOf()) != PLATFORM_ERR_OK)
        return false;

    m_windowCallback.manager = m_platform.Get();
    SPlatformWindowDesc desc{};
    desc.width = m_width;
    desc.height = m_height;
    desc.x = 80;
    desc.y = 60;
    desc.style.caption = 1;
    desc.style.sysmenu = 1;
    desc.style.resizable = 0;
    desc.style.visible = 1;
    desc.style.minimizeButton = 1;
    desc.style.maximizeButton = 0;
    desc.backgroundColor.mask = 0x202040;

    if (m_platform->CreateNativeWindow("AI Lab 4 - Fuzzy Weather", &desc, &m_windowCallback, nullptr, m_window.ReleaseAndGetAddressOf()) != PLATFORM_ERR_OK)
        return false;

    SNativeHandle windowHandle{};
    if (m_window->GetNativeHandle(&windowHandle) != PLATFORM_ERR_OK || !windowHandle.ptr)
        return false;

    m_nativeWindow = windowHandle.ptr;
    return true;
#else
    std::fprintf(stderr, "AI_Lab4: this prebuilt renderer currently requires Windows.\n");
    return false;
#endif
}

bool CLab4App::InitializeRenderer()
{
    if (!m_frame.Initialize(m_platform.Get(), m_window.Get(), RHI_BACKEND_OPENGL, m_width, m_height))
        return false;

    m_renderer.Initialize(m_frame.Device(), m_frame.Context(), m_frame.Compiler(), RHI_BACKEND_OPENGL);
    return m_ui.Initialize(&m_renderer, float(m_width), float(m_height), &m_weatherModel.GetFuzzySystem());
}

bool CLab4App::InitializeInput()
{
    if (m_platform->QueryInputManager(m_inputManager.ReleaseAndGetAddressOf()) != PLATFORM_ERR_OK || !m_inputManager.Get())
        return false;

    if (m_inputManager->Initialize() != PLATFORM_ERR_OK)
        return false;

    m_inputInitialized = true;
    if (m_inputManager->GetAvailableMouseCount() != 0)
        m_inputManager->CreateMouse(0, m_mouse.ReleaseAndGetAddressOf());

    if (m_inputManager->GetAvailableKeyboardCount() != 0)
        m_inputManager->CreateKeyboard(0, m_keyboard.ReleaseAndGetAddressOf());

    m_ui.GetContext()->GetInputContext(m_guiInput.ReleaseAndGetAddressOf());
    return m_guiInput.Get() != nullptr;
}

bool CLab4App::Initialize()
{
    if (!InitializePlatform() || !InitializeRenderer() || !InitializeInput())
    {
        std::fprintf(stderr, "AI_Lab4: application initialization failed.\n");
        Shutdown();
        return false;
    }

    if (m_window->Show() != PLATFORM_ERR_OK)
    {
        Shutdown();
        return false;
    }
    return true;
}

bool CLab4App::ResizeIfRequested()
{
    if (!m_windowCallback.resizeRequested)
        return true;

    m_windowCallback.resizeRequested = false;
#if defined(KRYSTALLIC_OS_WINNT)
    RECT client{};
    if (!m_nativeWindow || !GetClientRect(static_cast<HWND>(m_nativeWindow), &client))
        return false;

    const uint32 width = uint32(client.right - client.left);
    const uint32 height = uint32(client.bottom - client.top);
    if (width == 0 || height == 0 || (width == m_width && height == m_height))
        return true;

    if (!m_frame.Resize(width, height))
        return false;

    m_width = width;
    m_height = height;
    m_ui.Resize(float(m_width), float(m_height));
#endif
    return true;
}

void CLab4App::CopyPlatformInput()
{
    if (!m_guiInput.Get())
        return;

    GUI_INPUT_STATE& state = m_guiInput->m_state;
    const SIMDMath::CVector2 previousPos = state.m_mousePos;
    state.m_mouseDelta = SIMDMath::CVector2(0.0f, 0.0f);
    state.m_wheel = 0.0f;

    if (m_mouse.Get())
    {
        SMouseState mouseState{};
        if (m_mouse->GetRawState(&mouseState) == PLATFORM_ERR_OK)
        {
            state.m_wheel = float(mouseState.wheelDelta);
            for (uint i = 0; i < 5; ++i)
                state.m_mouseButton[i] = (mouseState.buttons & (1u << i)) ? 1 : 0;
        }
    }

#if defined(KRYSTALLIC_OS_WINNT)
    POINT cursor{};
    const HWND hwnd = static_cast<HWND>(m_nativeWindow);
    if (hwnd && GetCursorPos(&cursor) && ScreenToClient(hwnd, &cursor))
    {
        state.m_mousePos = SIMDMath::CVector2(float(cursor.x), float(cursor.y));
        state.m_mouseDelta = state.m_mousePos - previousPos;
    }

    if (GetForegroundWindow() != hwnd)
    {
        for (uint i = 0; i < 5; ++i)
            state.m_mouseButton[i] = 0;
    }
#endif

    if (m_keyboard.Get())
    {
        SKeyboardState keyboardState{};
        if (m_keyboard->GetRawState(&keyboardState) == PLATFORM_ERR_OK)
        {
            for (uint i = 0; i < INPUT_KEY_STATE_CAPACITY; ++i)
                state.m_keys[i] = keyboardState.IsKeyDown(i) ? 1 : 0;

            state.m_shift = keyboardState.IsKeyDown(KEYCODE_LSHIFT) || keyboardState.IsKeyDown(KEYCODE_RSHIFT);
            state.m_ctrl = keyboardState.IsKeyDown(KEYCODE_LCTRL) || keyboardState.IsKeyDown(KEYCODE_RCTRL);
            state.m_alt = keyboardState.IsKeyDown(KEYCODE_LALT) || keyboardState.IsKeyDown(KEYCODE_RALT);
            state.m_super = keyboardState.IsKeyDown(KEYCODE_LGUI) || keyboardState.IsKeyDown(KEYCODE_RGUI);
        }
    }

    m_guiInput->CommitState();
}

void CLab4App::Update(float deltaTime)
{
    m_inputManager->Update();
    CopyPlatformInput();

    if (m_keyboard.Get() && m_keyboard->IsKeyPressed(KEYCODE_ESCAPE))
        m_platform->RequestQuit(0);

    // Use a key-down transition instead of relying on the per-device Raw Input
    // pressed event. On Windows, use the focused window's key state as a
    // fallback when keyboard device enumeration or Raw Input misses F1.
    bool f1Down = m_keyboard.Get() && m_keyboard->IsKeyDown(KEYCODE_F1);
#if defined(KRYSTALLIC_OS_WINNT)
    if (m_nativeWindow && GetForegroundWindow() == static_cast<HWND>(m_nativeWindow))
        f1Down = f1Down || ((GetAsyncKeyState(VK_F1) & 0x8000) != 0);
    else
        f1Down = false;
#endif
    if (f1Down && !m_f1WasDown)
        m_ui.ToggleDiagnostics();
    m_f1WasDown = f1Down;

    m_ui.Update(deltaTime);

    if (m_ui.ConsumeCalculationRequest())
    {
        SWeatherInput input{};
        m_ui.GetWeatherInput(input);
        SWeatherResult result{};
        const EWeatherStatus status = m_weatherModel.Evaluate(input, result);
        m_ui.SetWeatherResult(result, status == EWeatherStatus::Success ? CWeatherModel::GetCategoryText(result.category) : nullptr);
        if (status != EWeatherStatus::Success)
            std::fprintf(stderr, "AI_Lab4: weather evaluation status %u.\n", static_cast<unsigned int>(status));
    }
}

bool CLab4App::Render()
{
    return m_frame.Present(m_ui.GetContext());
}

int CLab4App::Run()
{
    if (!m_platform.Get())
        return 1;

    auto previous = std::chrono::steady_clock::now();
    int exitCode = 0;

    while (m_platform->PollEvents())
    {
        if (!ResizeIfRequested())
        {
            std::fprintf(stderr, "AI_Lab4: swap-chain resize failed.\n");
            exitCode = 1;
            break;
        }

        if (m_width == 0 || m_height == 0)
            continue;

        const auto now = std::chrono::steady_clock::now();
        float deltaTime = std::chrono::duration<float>(now - previous).count();
        previous = now;
        if (deltaTime > 0.1f)
            deltaTime = 0.1f;

        Update(deltaTime);
        if (!Render())
        {
            std::fprintf(stderr, "AI_Lab4: frame presentation failed.\n");
            exitCode = 1;
            break;
        }
    }

    Shutdown();
    return exitCode;
}

void CLab4App::Shutdown()
{
    m_f1WasDown = false;
    m_guiInput.Reset();
    m_mouse.Reset();
    m_keyboard.Reset();

    if (m_inputManager.Get() && m_inputInitialized)
        m_inputManager->Shutdown();
    m_inputInitialized = false;
    m_inputManager.Reset();

    m_ui.Shutdown();
    m_renderer.OnRenderDestroy();
    m_frame.Shutdown();
    m_window.Reset();
    m_windowCallback.manager = nullptr;
    m_platform.Reset();
    m_nativeWindow = nullptr;
}
