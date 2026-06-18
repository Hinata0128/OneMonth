#pragma once

//警告についてのコード分析を無効にする.4005:再定義.
#pragma warning(disable:4005)

//ヘッダ読込.
#include <D3DX11.h>
#include <D3D11.h>
#include <D3DX10.h>	//「D3DX～」の定義使用時に必要.
#include <D3D10.h>


//ライブラリ読み込み.
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx10.lib")	//「D3DX～」の定義使用時に必要.


/**************************************************
*	DirectX11 セットアップ.
*	シングルトン設計にしている.
*	どこでも呼べるようにした.
**/

enum class DEPTH_MODE {
	OFF,        // 深度テスト無効
	ON,         // 通常の深度テスト (LESS)
	SKYBOX,     // スカイボックス用 (LESS_EQUAL)
};

class DirectX11
{
public:
	~DirectX11();

	//インスタンス取得(唯一のアクセス経路).
//※関数の前にstaticを付けることでインスタンス生成しなくても使用できる.
	static DirectX11* GetInstance()
	{
		//唯一のインスタンスを作成する.
		//※staticで作成されたので2回目以降は、下の1行は無視される.
		static DirectX11 s_Instance;	//s_:staticの意味.
		return &s_Instance;
	}

	//DirectX11構築.
	HRESULT Create(HWND hWnd);
	//DirectX11解放.
	void Release();

	//深度（Ｚ）テストON/OFF切り替え.
	void SetDepth( bool flag );
	//アルファブレンドON/OFF切り替え.
	void SetAlphaBlend( bool flag );

	//バックバッファクリア関数.
	void ClearBackBuffer();
	//表示.
	void Present();

	//デバイスを取得.
	ID3D11Device* GetDevice() const { return m_pDevice11; }
	//デバイスコンテキストを取得.
	ID3D11DeviceContext* GetContext() const { return m_pContext11; }

	HWND GethWnd() const { return m_hWnd; }

	void SetDepth(DEPTH_MODE mode);

	// 行列の取得
	const D3DXMATRIX& GetViewMatrix() const { return m_ViewMatrix; }
	const D3DXMATRIX& GetProjectionMatrix() const { return m_ProjMatrix; }

	// 行列の設定（カメラのUpdateなどで呼ぶ用）
	void SetViewMatrix(const D3DXMATRIX& view) { m_ViewMatrix = view; }
	void SetProjectionMatrix(const D3DXMATRIX& proj) { m_ProjMatrix = proj; }

	// ビューポートを取得（コンテキストから直接取得する安全な書き方）
	D3D11_VIEWPORT GetViewport() const {
		UINT numViewports = 1;
		D3D11_VIEWPORT vp;
		m_pContext11->RSGetViewports(&numViewports, &vp);
		return vp;
	}
private:
	DirectX11();
	DirectX11(const DirectX11& rhs) = delete;
	//代入演算子によるコピーを禁止する.
	//operator(オペレータ):演算子のオーバーロードで、演算の中身を拡張できる.
	DirectX11& operator = (const DirectX11& rhs) = delete;


private:
	//デバイスとスワップチェイン作成.
	HRESULT CreateDeviceAndSwapChain(
		HWND hWnd, UINT uFPS, UINT uWidth, UINT uHeight );

	//バックバッファ作成:カラー用レンダーターゲットビュー作成.
	HRESULT CreateColorBackBufferRTV();

	//バックバッファ作成:デプスステンシル用レンダーターゲットビュー作成.
	HRESULT CreateDepthStencilBackBufferRTV();

	//ラスタライザ作成.
	HRESULT CreateRasterizer();

	//デプスステンシルステート作成.
	HRESULT CreateDepthStencilState();

	//アルファブレンドステート作成.
	HRESULT CreateAlphaBlendState();

private:
	ID3D11Device*				m_pDevice11;			//デバイスオブジェクト.
	ID3D11DeviceContext*		m_pContext11;			//デバイスコンテキスト.
	IDXGISwapChain*				m_pSwapChain;			//スワップチェーン.
	ID3D11RenderTargetView*		m_pBackBuffer_TexRTV;	//レンダーターゲットビュー.
	ID3D11Texture2D*			m_pBackBuffer_DSTex;	//デプスステンシル用テクスチャ.
	ID3D11DepthStencilView*		m_pBackBuffer_DSTexDSV;	//デプスステンシルビュー.

	//深度（Ｚ）テスト設定.
	ID3D11DepthStencilState*	m_pDepthStencilStateOn;		//有効設定.
	ID3D11DepthStencilState*	m_pDepthStencilStateOff;	//無効設定.
	
	ID3D11DepthStencilState* m_pDepthStencilStateSkybox;

	//アルファブレンド.
	ID3D11BlendState*		m_pAlphaBlendOn;	//有効設定.
	ID3D11BlendState*		m_pAlphaBlendOff;	//無効設定.

	HWND m_hWnd;

	D3DXMATRIX m_ViewMatrix;       //ビュー行列.
	D3DXMATRIX m_ProjMatrix;       //プロジェクション行列.
};