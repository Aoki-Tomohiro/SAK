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
	//インクリメントサイズ
	static uint32_t descriptorSizeRTV;
	static uint32_t descriptorSizeSRV;
	static uint32_t descriptorSizeDSV;

	/// <summary>
	/// Resource構造体
	/// </summary>
	struct Texture {
		ComPtr<ID3D12Resource> resource;
		uint32_t rtvIndex;
		uint32_t srvIndex;
	};

	/// <summary>
	/// 頂点データ
	/// </summary>
	struct VertexPosUV {
		Vector4 pos;
		Vector2 texcoord;
	};

	/// <summary>
	/// ブラーデータ
	/// </summary>
	struct BlurData {
		int32_t textureWidth;
		int32_t textureHeight;
		float padding[2];
		float weight[8];
	};

	/// <summary>
	/// ブルーム用データ
	/// </summary>
	struct BloomData {
		//フラグ
		bool enable;
		float padding[3];
	};

	struct LensDistortionData {
		//フラグ
		bool enable;
		//歪みのきつさ
		float tightness;
		//歪みの強さ
		float strength;
	};

	struct VignetteData {
		//フラグ
		bool enable;
		//強度
		float intensity;
	};

	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>シングルトンインスタンス</returns>
	static PostProcess* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 描画前処理
	/// </summary>
	void PreDraw();

	/// <summary>
	/// 描画後処理
	/// </summary>
	void PostDraw();

private:
	/// <summary>
	/// ぼかしの種類
	/// </summary>
	enum BlurState {
		kHorizontal,
		kVertical,
	};

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
	/// ブラー用のPSOの作成
	/// </summary>
	void CreateBlurPipelineStateObject();

	/// <summary>
	/// ポストプロセス用のPSOの作成
	/// </summary>
	void CreatePostProcessPipelineStateObject();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 2パス目の描画前処理
	/// </summary>
	void PreSecondPassDraw();

	/// <summary>
	/// 2パス目の描画
	/// </summary>
	void SecondPassDraw();

	/// <summary>
	/// 2パス目の描画後処理
	/// </summary>
	void PostSecondPassDraw();

	/// <summary>
	/// ぼかし前準備
	/// </summary>
	void PreBlur(BlurState blurState);

	/// <summary>
	/// ぼかし処理
	/// </summary>
	void Blur(BlurState blurState, uint32_t srvIndex);

	/// <summary>
	/// ぼかし後処理
	/// </summary>
	void PostBlur(BlurState blurState);

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
	/// 深度テクスチャの作成
	/// </summary>
	/// <param name="width">幅</param>
	/// <param name="height">高さ</param>
	/// <returns>リソース</returns>
	ComPtr<ID3D12Resource> CreateDepthStencilTextureResource(int32_t width, int32_t height);

	/// <summary>
	/// レンダーターゲットビューの作成
	/// </summary>
	/// <param name="resource">リソース</param>
	/// <param name="format">フォーマット</param>
	/// <returns>RTVIndex</returns>
	uint32_t CreateRenderTargetView(const ComPtr<ID3D12Resource>& resource, DXGI_FORMAT format);

	/// <summary>
	/// シェーダーリソースビューの作成
	/// </summary>
	/// <param name="resource">リソース</param>
	/// <param name="format">フォーマット</param>
	/// <returns>SRVIndex</returns>
	uint32_t CreateShaderResourceView(const ComPtr<ID3D12Resource>& resource, DXGI_FORMAT format);

	/// <summary>
	/// DSVの作成
	/// </summary>
	void CreateDepthStencilView();

	/// <summary>
	/// CPUディスクリプタハンドルを取得
	/// </summary>
	/// <param name="descriptorHeap">ディスクリプタヒープ</param>
	/// <param name="descriptorSize">ディスクリプタのサイズ</param>
	/// <param name="index">ディスクリプタヒープの番号</param>
	/// <returns>CPUディスクリプタハンドル</returns>
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(ID3D12DescriptorHeap* descriptorHeap, const uint32_t descriptorSize, uint32_t index);

	/// <summary>
	/// GPUディスクリプタハンドルを取得
	/// </summary>
	/// <param name="descriptorHeap">ディスクリプタヒープ</param>
	/// <param name="descriptorSize">ディスクリプタのサイズ</param>
	/// <param name="index">ディスクリプタヒープの番号</param>
	/// <returns>GPUディスクリプタハンドル</returns>
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(ID3D12DescriptorHeap* descriptorHeap, const uint32_t descriptorSize, uint32_t index);

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
	ComPtr<ID3D12RootSignature> postProcessRootSignature_;
	//パイプラインステート
	ComPtr<ID3D12PipelineState> pipelineState_;
	std::array<ComPtr<ID3D12PipelineState>, 2> blurPipelineState_;
	ComPtr<ID3D12PipelineState> postProcessPipelineState_;
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
	//深度バッファ
	ComPtr<ID3D12Resource> depthStencilResource_ = nullptr;

	//1パス目用テクスチャ
	Texture firstPassResource_ = { nullptr };
	//2パス目用テクスチャ
	Texture secondPassResource_ = { nullptr };
	//高輝度を書き込むテクスチャ
	Texture highIntensityResource_ = { nullptr };
	//ぼかし用のテクスチャ
	std::array<Texture, 2> blurResources_ = { nullptr };

	//ぼかし用のリソース
	ComPtr<ID3D12Resource> blurConstantBuffer_ = nullptr;
	//Bloom用のリソース
	ComPtr<ID3D12Resource> bloomConstantBuffer_ = nullptr;
	//LensDistortion用のリソース
	ComPtr<ID3D12Resource> lensDistortionConstantBuffer_ = nullptr;
	//Vignette用のリソース
	ComPtr<ID3D12Resource> vignetteConstantBuffer_ = nullptr;

	//PostProcessの設定項目
	bool isPostProcessActive_ = false;
	bool isBloomActive_ = false;
	bool isLensDistortionActive_ = false;
	float lensDistortionTightness_ = 2.5f;
	float lensDistortionStrength_ = -0.1f;
	bool isVignetteActive_ = false;
	float vignetteIntensity_ = 1.0f;

};