// 
// Created by fcx@pingxingyun.com
// 2020-02-07 15:43
//
#include <iostream>
#include "application.h"
#include <algorithm>
#include "lark_xr/xr_config.h"
#include "lark_xr/xr_latency_collector.h"
#include <sys/stat.h>
#include <sys/types.h>
#include "lark_xr/lk_common_types.h"

/******************************************************/
// 配置参数
// SDK ID 可以自助申请,也可以联系商务获取。要注意SDKID与服务器授权码不是同一个。
// https://www.pingxingyun.com/console/#/
#define LARK_SDK_ID "Your sdk id"
// 服务器地址
#define SERVER_IP   "192.168.0.55"
// 服务器端口号
#define SERVER_PORT 8181
// 云端应用APPID
#define APP_ID      "888075472670294016"
/******************************************************/

namespace {
    // Default camera values
    const float YAW = -90.0f;
    const float PITCH = 0.0f;
    const float SPEED = 2.5f;
    const float SENSITIVITY = 0.1f;
}

using namespace lark;

Application::Application()
{};

Application::~Application() {
    xr_client_.UnRegisterObserver();
    xr_client_.Release();
};

bool Application::InitDX(ID3D11Device* device) {
    // config video
    XRConfig::set_bitrate(50 * 1000);

    XRConfig::set_render_width(1920);
    XRConfig::set_render_height(1080);  

    XRConfig::set_fps(72);
    larkFoveatedRendering fov{ false };
    XRConfig::set_foveated_rendering(fov);

    xr_client_.RegisterObserver(this);
    xr_client_.Init(true);
    xr_client_.InitD3D11Device(device);
    xr_client_.InitSdkAuthorization(LARK_SDK_ID);

    xr_client_.SetServerAddr(SERVER_IP, SERVER_PORT);

    // init cam.
    cam_yaw_ = YAW;
    cam_pitch_ = PITCH;
    return true;
}

bool Application::RunMainLoop(ID3D11DeviceContext* context, ID3D11RenderTargetView*  renderTargetView) {
    if (xr_client_.media_ready()) {
        bool res = xr_client_.WaitFroNewFrame(50);
        if (!res) {
            return false;
        }

        larkxrTrackingFrame trackingFrame = {};
        bool has_new_frame = xr_client_.Render(&trackingFrame);

        if (has_new_frame) {
            context->ClearRenderTargetView(renderTargetView, DirectX::Colors::MidnightBlue);

            xr_client_.Draw(LARKXR_EYE_BOTH);

            lark::XRLatencyCollector::Instance().Submit(trackingFrame.frameIndex, 0);
        }
        else {
            return false;
        }
    }
    else {
        context->ClearRenderTargetView(renderTargetView, DirectX::Colors::MintCream);
        return true;
    }
    
    return true;
}

bool Application::ReleaseGl()
{
    xr_client_.ReleaseD3D11Device();
    xr_client_.OnDestory();
    xr_client_.Release();
    // TODO
    // xr_client_.Release();
    return false;
}

bool Application::HandleInput(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    
    static POINTS ptsEnd;          // new endpoint 

    switch (message)
    {
    case WM_KEYDOWN:
        {
            // 移动摄像机
            const float speed = 0.05;
            switch (wParam)
            {
            case 0x57: // W
                break;
            case 0x41: // A
                break;
            case 0x53: // S
                break;
            case 0x44: // D
                break;
            default:
                break;
            }
            // update view
        }
        break;
    case WM_KEYUP:
        break;
    case WM_LBUTTONDOWN:
        break;
    case WM_MOUSEMOVE:
        if (xr_client_.is_connected() && (wParam & MK_LBUTTON)) {
            POINTS current = MAKEPOINTS(lParam);
            POINTS rel;
            rel.x = current.x - ptsEnd.x;
            rel.y = current.y - ptsEnd.y;
            ptsEnd = current;

            std::cout << "x " << ptsEnd.x << " y " << ptsEnd.y << " rel x " << rel.x << " rel y " << rel.y << std::endl;

            float xoffset = rel.x;
            xoffset *= SENSITIVITY;
            cam_yaw_ += xoffset;

            std::cout << "xoffset " << xoffset << " cam_yaw_ " << cam_yaw_ << std::endl;

            // update view
        }
        break;
    case WM_LBUTTONUP:
        if (!xr_client_.is_connected()) {
            xr_client_.EnterAppli(APP_ID);
        }
        break;
    case WM_RBUTTONDOWN:
        break;
    case WM_RBUTTONUP:
        break;
    case WM_MBUTTONDOWN:
        break;
    case WM_MBUTTONUP:
        xr_client_.Close();
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return true;
}

void Application::OnConnected()
{
    std::cout << "on connected " << std::endl;
}

void Application::OnClose(int code)
{
    std::cout << "on close " << code << std::endl;
}

void Application::OnError(int errCode, const char* msg)
{
    std::cout << "on error " << errCode << " msg "  << msg << std::endl;
}

void Application::OnInfo(int infoCode, const char* info)
{
    std::cout << "on info " << infoCode << " " << info << std::endl;
}

void Application::RequestTrackingInfo()
{
    //LOGV("RequestTrackingInfo");
    static uint64_t frameIndex = 0;
    frameIndex++;
    larkxrTrackingDevicePairFrame tracking = {};
    tracking.frameIndex = frameIndex;
    tracking.devicePair.hmdPose.isConnected = true;
    /* invert view matrix */
    // send test pose;
    xr_client_.SendDevicePair(tracking);
}
