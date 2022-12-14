#pragma once
class RenderTarget
{
protected:
    int width, height;

    //내가 렌더타겟으로 쓸때 필요한 인터페이스
    ID3D11RenderTargetView* rtv;
    ID3D11DepthStencilView* dsv;


    //쉐이더자원으로 쓸때 인터페이스
    ID3D11ShaderResourceView* rtvSrv;
    ID3D11ShaderResourceView* dsvSrv;

    //자원
    ID3D11Texture2D* rtvTexture;
    ID3D11Texture2D* dsvTexture;

    //창사이즈 조절
    void CreateBackBuffer(float width, float height);
    void DeleteBackBuffer();

public:
    RenderTarget(UINT width = App.GetWidth() ,
        UINT height = App.GetHeight());
    ~RenderTarget();

    void Set(Color clear = Color(0, 0, 0, 1));

    ID3D11ShaderResourceView* GetSRV() { return rtvSrv; }

    //창사이즈 조절
    void ResizeScreen(float width, float height);
};

