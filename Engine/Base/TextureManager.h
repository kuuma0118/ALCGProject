#pragma once
#include "Base/DirectXCommon.h"
#include "Utility/Log.h"
#include "Externals/DirectXTex/DirectXTex.h"
#include "Externals/DirectXTex/d3dx12.h"
#include <array>
#include <cassert>
#include <thread>
#include <vector>

/// <summary>
/// テクスチャの管理クラス
/// </summary>
class TextureManager {
public:
	//ディスクリプタの最大数
	static const size_t kNumDescriptors = 256;

	//インクリメントサイズ
	static uint32_t descriptorSizeSRV;

	/// <summary>
	/// テクスチャ
	/// </summary>
	struct Texture {
		//テクスチャリソース
		Microsoft::WRL::ComPtr<ID3D12Resource> resource = nullptr;
		//中間リソース
		Microsoft::WRL::ComPtr<ID3D12Resource> intermediateResource = nullptr;
		//CPUのディスクリプタハンドル
		D3D12_CPU_DESCRIPTOR_HANDLE srvHandleCPU{};
		//GPUのディスクリプタハンドル
		D3D12_GPU_DESCRIPTOR_HANDLE srvHandleGPU{};
		//テクスチャの名前
		std::string name{};
		//テクスチャハンドル
		uint32_t textureHandle = 0;
	};

	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>シングルトンインスタンス</returns>
	static TextureManager* GetInstance();

	/// <summary>
	/// テクスチャを読み込む
	/// </summary>
	/// <param name="filePath">ファイルパス</param>
	/// <returns></returns>
	static uint32_t Load(const std::string& filePath);

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// ディスクリプタヒープを設定
	/// </summary>
	void SetGraphicsDescriptorHeap();

	/// <summary>
	/// ディスクリプタテーブルを設定
	/// </summary>
	/// <param name="rootParameterIndex">ルートパラメータの番号</param>
	/// <param name="textureHandle">テクスチャ番号</param>
	void SetGraphicsRootDescriptorTable(UINT rootParameterIndex, uint32_t textureHandle);

	/// <summary>
	/// テクスチャの情報を取得
	/// </summary>
	/// <param name="textureHandle">テクスチャハンドル</param>
	/// <returns>テクスチャの情報</returns>
	const D3D12_RESOURCE_DESC GetResourceDesc(uint32_t textureHandle);

	/// <summary>
	/// Instancing用のSRVの作成
	/// </summary>
	/// <param name="instancingResource"></param>
	/// <param name="kNumInstance"></param>
	/// <param name="size"></param>
	/// <returns></returns>
	uint32_t CreateInstancingShaderResourceView(const Microsoft::WRL::ComPtr<ID3D12Resource>& instancingResource, uint32_t kNumInstance, size_t size);

private:
	TextureManager() = default;
	~TextureManager() = default;
	TextureManager(const TextureManager&) = delete;
	TextureManager& operator=(const TextureManager&) = delete;

	/// <summary>
	/// テクスチャを読み込む
	/// </summary>
	/// <param name="filePath">ファイルパス</param>
	/// <returns></returns>
	uint32_t LoadInternal(const std::string& filePath);

	/// <summary>
	/// テクスチャデータの読み込み
	/// </summary>
	/// <param name="filePath">ファイル名</param>
	/// <returns></returns>
	DirectX::ScratchImage LoadTexture(const std::string& filePath);

	/// <summary>
	/// テクスチャのリソースを作成
	/// </summary>
	/// <param name="metadata">テクスチャの情報</param>
	/// <returns>テクスチャのリソース</returns>
	Microsoft::WRL::ComPtr<ID3D12Resource> CreateTextureResource(const DirectX::TexMetadata& metadata);

	/// <summary>
	/// リソースにテクスチャのデータを転送
	/// </summary>
	/// <param name="texture">テクスチャのリソース</param>
	/// <param name="mipImages">テクスチャのデータ</param>
	/// <returns>中間リソース</returns>
	Microsoft::WRL::ComPtr<ID3D12Resource> UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages);

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
	//デバイス
	ID3D12Device* device_ = nullptr;
	//コマンドリスト
	ID3D12GraphicsCommandList* commandList_ = nullptr;
	//ディスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvDescriptorHeap_ = nullptr;
	//テクスチャの配列
	std::array<Texture, kNumDescriptors> textures_{};
	//テクスチャ番号
	uint32_t textureHandle_ = -1;

};
