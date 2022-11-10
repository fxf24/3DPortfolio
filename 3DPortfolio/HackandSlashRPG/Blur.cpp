#include "stdafx.h"

BlurManager::BlurManager()
{
    {
        D3D11_BUFFER_DESC desc = { 0 };
        desc.ByteWidth = sizeof(Blur);
        desc.Usage = D3D11_USAGE_DYNAMIC;
        desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;//상수버퍼
        desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        desc.MiscFlags = 0;
        desc.StructureByteStride = 0;
        HRESULT hr = D3D->GetDevice()->CreateBuffer(&desc, NULL, &blurBuffer);
        D3D->GetDC()->PSSetConstantBuffers(10, 1, &blurBuffer);
        assert(SUCCEEDED(hr));
    }
}

BlurManager::~BlurManager()
{
    SafeRelease(blurBuffer);
}

void BlurManager::Update()
{
    ImGui::SliderInt("Select", &blur.select, 0, 7);
    ImGui::SliderInt("Count", &blur.count, 0, 200);
    ImGui::SliderFloat("Width", &blur.width, 0, App.GetWidth());
    ImGui::SliderFloat("Height", &blur.height, 0, App.GetHeight());
    ImGui::ColorEdit3("BlendColor", (float*)&blur.blendColor);
    ImGui::SliderFloat2("Center", (float*)&blur.center, 0, 2000.0f);
    ImGui::SliderFloat("Radius", (float*)&blur.radius, 0, 2000.0f);
}

void BlurManager::Set()
{
   D3D11_MAPPED_SUBRESOURCE mappedResource;
   D3D->GetDC()->Map(blurBuffer, 0, D3D11_MAP_WRITE_DISCARD,0,&mappedResource);
   memcpy_s(mappedResource.pData, sizeof(Blur), &blur, sizeof(Blur));
   D3D->GetDC()->Unmap(blurBuffer, 0);
}
