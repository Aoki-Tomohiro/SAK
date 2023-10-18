#pragma once
#include "Base/DirectXCommon.h"
#include "Base/TextureManager.h"
#include "3D/Matrix/ViewProjection.h"
#include <dxcapi.h>
#include <fstream>
#include <list>
#include <string>
#include <sstream>
#pragma comment(lib,"dxcompiler.lib")

/// <summary>
/// パーティクル
/// </summary>
class Particle {
private:
	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
	/// <summary>
	/// 頂点データ
	/// </summary>
	struct VertexData {
		Vector4 position;
		Vector2 texcoord;
		Vector3 normal;
	};

	/// <summary>
	/// マテリアルデータ構造体
	/// </summary>
	struct MaterialData {
		std::string textureFilePath;
	};

	/// <summary>
	/// モデルデータ構造体
	/// </summary>
	struct ModelData {
		std::vector<VertexData> vertices;
		MaterialData material;
		std::string name;
	};

	/// <summary>
	/// マテリアルの構造体
	/// </summary>
	struct ConstBuffDataMaterial {
		Vector4 color{};
		Matrix4x4 uvTransform{};
	};

	/// <summary>
	/// ワールドトランスフォームの構造体
	/// </summary>
	struct ConstBuffDataWorldTransform {
		Matrix4x4 world;
	};

	/// <summary>
	/// 静的初期化
	/// </summary>
	static void StaticInitialize();

	/// <summary>
	/// 静的メンバ変数の解放
	/// </summary>
	static void Release();

	/// <summary>
	/// 描画前処理
	/// </summary>
	static void PreDraw();

	/// <summary>
	/// 描画後処理
	/// </summary>
	static void PostDraw();

	/// <summary>
	/// パーティクルモデルの作成
	/// </summary>
	/// <param name="directoryPath"></param>
	/// <param name="filename"></param>
	/// <returns></returns>
	static Particle* CreateFromOBJ(const std::string& directoryPath, const std::string& filename, uint32_t kNumInstance);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection"></param>
	void Draw(const ViewProjection& viewProjection);

private:
	/// <summary>
	/// DXCの初期化
	/// </summary>
	static void InitializeDXC();

	/// <summary>
	/// シェーダーをコンパイルする
	/// </summary>
	/// <param name="filePath">Compilerするshaderのファイルパス</param>
	/// <param name="profile">Compilerで使用するProfile</param>
	/// <returns>実行用のバイナリ</returns>
	static ComPtr<IDxcBlob> CompileShader(
		const std::wstring& filePath,
		const wchar_t* profile);

	/// <summary>
	/// PSOの作成
	/// </summary>
	static void CreatePipelineStateObject();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="kNumInstance"></param>
	void Initialize(uint32_t kNumInstance, std::vector<VertexData>& vertices);

	/// <summary>
	/// 頂点データの作成
	/// </summary>
	void CreateVertexResource(std::vector<VertexData>& vertices);

	/// <summary>
	/// マテリアルリソース用の作成
	/// </summary>
	void CreateMaterialResource();

	/// <summary>
	/// ワールドトランスフォームのCBVを作成
	/// </summary>
	void CreateWorldTransform(uint32_t kNumInstance);

	/// <summary>
	/// Objファイルの読み込み
	/// </summary>
	/// <param name="directoryPath">ディレクトリ名</param>
	/// <param name="filename">ファイル名</param>
	/// <returns></returns>
	ModelData LoadObjFile(const std::string& directoryPath, const std::string& filename);

	/// <summary>
	/// mtlファイルの読み込み
	/// </summary>
	/// <param name="directoryPath">ディレクトリ名</param>
	/// <param name="filename">ファイル名</param>
	/// <returns></returns>
	MaterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename);

private:
	//デバイス
	static ID3D12Device* sDevice_;
	//コマンドリスト
	static ID3D12GraphicsCommandList* sCommandList_;
	//DXC
	static ComPtr<IDxcUtils> sDxcUtils_;
	static ComPtr<IDxcCompiler3> sDxcCompiler_;
	static ComPtr<IDxcIncludeHandler> sIncludeHandler_;
	//RootSignature
	static ComPtr<ID3D12RootSignature> sRootSignature_;
	//PipelineStateObject
	static ComPtr<ID3D12PipelineState> sGraphicsPipelineState_;
	//モデルデータ
	static std::list<ModelData> modelDatas_;
	//頂点データ
	std::vector<VertexData> vertices_{};
	//頂点バッファ
	ComPtr<ID3D12Resource> vertexResource_ = nullptr;
	//頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};
	//マテリアル用のリソース
	ComPtr<ID3D12Resource> materialResource_ = nullptr;
	//Instancing用のWorldTransform
	ComPtr<ID3D12Resource> instancingResource_ = nullptr;
	//Instancing用のSRVハンドル
	D3D12_GPU_DESCRIPTOR_HANDLE instancingSrvHandleGPU_{};
	//インスタンス数
	uint32_t kNumInstance_ = 0;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0;

};