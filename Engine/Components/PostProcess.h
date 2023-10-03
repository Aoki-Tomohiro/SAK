#pragma once
#include "Base/DirectXCommon.h"
#include "Utility/MathFunction.h"
#include <array>
#include <dxcapi.h>
#include <vector>
#pragma comment(lib,"dxcompiler.lib")

/// <summary>
/// ポストプロセス
/// </summary>
class PostProcess {
private:
	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
	/// <summary>
	/// 頂点データ
	/// </summary>
	struct VertexPosUV {
		Vector4 pos;
		Vector2 texcoord;
	};

	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>シングルトンインスタンス</returns>
	static PostProcess* GetInstance();

	void Initialize();

	void Update();

	void Draw();

private:
	/// <summary>
	/// DXCの初期化
	/// </summary>
	void InitializeDXC();

	/// <summary>
	/// シェーダーをコンパイルする
	/// </summary>
	/// <param name="filePath">Compilerするshaderのファイルパス</param>
	/// <param name="profile">Compilerで使用するProfile</param>
	/// <returns>実行用のバイナリ</returns>
	ComPtr<IDxcBlob> CompileShader(
		const std::wstring& filePath,
		const wchar_t* profile);

	/// <summary>
	/// PSOの作成
	/// </summary>
	void CreatePipelineStateObject();

	/// <summary>
	/// マルチパス用のリソースの作成
	/// </summary>
	/// <param name="width">幅</param>
	/// <param name="height">高さ</param>
	/// <param name="format">フォーマット</param>
	/// <param name="clearColor">クリアカラー</param>
	/// <returns>リソース</returns>
	ComPtr<ID3D12Resource> CreateTextureResource(uint32_t width, uint32_t height, DXGI_FORMAT format, const float* clearColor);

	/// <summary>
	/// レンダーターゲットビューの作成
	/// </summary>
	/// <param name="resource">リソース</param>
	/// <param name="format">フォーマット</param>
	/// <returns>RTVハンドル</returns>
	uint32_t CreateRenderTargetView(ComPtr<ID3D12Resource> resource, DXGI_FORMAT format);

	/// <summary>
	/// シェーダーリソースビューの作成
	/// </summary>
	/// <param name="resource">リソース</param>
	/// <param name="format">フォーマット</param>
	/// <returns>SRVハンドル</returns>
	uint32_t CreateShaderResourceView(ComPtr<ID3D12Resource> resource, DXGI_FORMAT format);

private:
	//DirectXCommon
	DirectXCommon* dxCommon_ = nullptr;
	//DXCompiler
	ComPtr<IDxcUtils> dxcUtils_;
	ComPtr<IDxcCompiler3> dxcCompiler_;
	ComPtr<IDxcIncludeHandler> includeHandler_;
	//ルートシグネチャ
	ComPtr<ID3D12RootSignature> rootSignature_;
	ComPtr<ID3D12RootSignature> blurRootSignature_;
	ComPtr<ID3D12RootSignature> finalPassRootSignature_;
	//パイプラインステート
	ComPtr<ID3D12PipelineState> pipelineState_;
	std::array<ComPtr<ID3D12PipelineState>, 2> blurPipelineState_;
	ComPtr<ID3D12PipelineState> finalPassPipelineState_;
	//コマンドリスト
	ID3D12GraphicsCommandList* commandList_;
	//デバイス
	ID3D12Device* device_;

	//頂点
	std::vector<VertexPosUV> vertices_{};
	//頂点バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource_ = nullptr;
	//頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};

	//マルチパス用のRTVディスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> multiPassRTVDescriptorHeap_ = nullptr;
	uint32_t rtvIndex_ = -1;
	//マルチパス用のSRVディスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> multiPassSRVDescriptorHeap_ = nullptr;
	uint32_t srvIndex_ = -1;
	//マルチパス用のDSVディスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> multiPassDSVDescriptorHeap_ = nullptr;
	//マルチパス用の深度バッファ
	ComPtr<ID3D12Resource> depthStencilResource_ = nullptr;



};