//
// Created by erik9 on 5/11/2024.
//

#pragma once


// Begin automatically forwarding exports to the original dxgi.dll functions
namespace byteMender::proxies::dxgi {
    #pragma comment(linker, "/export:ApplyCompatResolutionQuirking=dxgi_o.ApplyCompatResolutionQuirking,@1")
    #pragma comment(linker, "/export:CompatString=dxgi_o.CompatString,@2")
    #pragma comment(linker, "/export:CompatValue=dxgi_o.CompatValue,@3")
    #pragma comment(linker, "/export:DXGIDumpJournal=dxgi_o.DXGIDumpJournal,@4")
    #pragma comment(linker, "/export:PIXBeginCapture=dxgi_o.PIXBeginCapture,@5")
    #pragma comment(linker, "/export:PIXEndCapture=dxgi_o.PIXEndCapture,@6")
    #pragma comment(linker, "/export:PIXGetCaptureState=dxgi_o.PIXGetCaptureState,@7")
    #pragma comment(linker, "/export:SetAppCompatStringPointer=dxgi_o.SetAppCompatStringPointer,@8")
    #pragma comment(linker, "/export:UpdateHMDEmulationStatus=dxgi_o.UpdateHMDEmulationStatus,@9")
    #pragma comment(linker, "/export:CreateDXGIFactory1=dxgi_o.CreateDXGIFactory1,@10")
    #pragma comment(linker, "/export:CreateDXGIFactory2=dxgi_o.CreateDXGIFactory2,@11")
    #pragma comment(linker, "/export:CreateDXGIFactory=dxgi_o.CreateDXGIFactory,@12")
    #pragma comment(linker, "/export:DXGID3D10CreateDevice=dxgi_o.DXGID3D10CreateDevice,@13")
    #pragma comment(linker, "/export:DXGID3D10CreateLayeredDevice=dxgi_o.DXGID3D10CreateLayeredDevice,@14")
    #pragma comment(linker, "/export:DXGID3D10GetLayeredDeviceSize=dxgi_o.DXGID3D10GetLayeredDeviceSize,@15")
    #pragma comment(linker, "/export:DXGID3D10RegisterLayers=dxgi_o.DXGID3D10RegisterLayers,@16")
    #pragma comment(linker, "/export:VerQueryDXGIDeclareAdapterRemovalSupportValueW=dxgi_o.VerQueryDXGIDeclareAdapterRemovalSupportValueW,@17")
    #pragma comment(linker, "/export:DXGIDisableVBlankVirtualization=dxgi_o.DXGIDisableVBlankVirtualization,@18")
    #pragma comment(linker, "/export:DXGIGetDebugInterface1=dxgi_o.DXGIGetDebugInterface1,@19")
    #pragma comment(linker, "/export:DXGIReportAdapterConfiguration=dxgi_o.DXGIReportAdapterConfiguration,@20")
}

