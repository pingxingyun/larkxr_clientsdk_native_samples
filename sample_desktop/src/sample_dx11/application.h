// 
// Created by fcx@pingxingyun.com
// 2020-02-07 15:43
//
#pragma once
#ifndef APPLICATION_INCLUDE
#define APPLICATION_INCLUDE

#include "lark_xr/xr_client.h"
#include "lark_xr/xr_tracking_frame.h"
#include <d3d11_1.h>
#include <directxcolors.h>

class Application: public lark::XRClientObserverWrap {
public:
    struct MouseEvent {
        bool left;
        bool right;
        bool mid;
        bool wheelup;
        bool wheeldown;

        int xPos;
        int yPos;
        int xRel;
        int yRel;
    };


    Application();
    ~Application();
    
    bool InitDX(ID3D11Device* device);
    bool RunMainLoop(ID3D11DeviceContext* context, ID3D11RenderTargetView* renderTargetView);
    bool ReleaseGl();

    bool HandleInput(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    // xr client callback
    virtual void OnConnected() override;
    virtual void OnClose(int code) override;
    virtual void OnError(int errCode, const char* msg) override;
    virtual void OnInfo(int infoCode, const char* info) override;
    // request fetch tracking info
    virtual void RequestTrackingInfo() override;
private:
    lark::XRClient xr_client_;
    bool need_update_ = false;
    MouseEvent last_mouse_event_{};

    uint32_t companion_window_width_ = 1280;
    uint32_t companion_window_height_ = 720;

    // euler Angles
    float cam_yaw_;
    float cam_pitch_;
};
#endif // APPLICATION_INCLUDE
