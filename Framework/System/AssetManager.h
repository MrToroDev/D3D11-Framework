#pragma once

#include <string>
#include <map>
#include <nlohmann\json.hpp>

namespace DX {
	enum class AssetType {
		TEXTURE,
		SHADER,
		MODEL,
		FONT,
		SOUND
	};

	struct AssetCache
	{
		AssetType type;
		std::string data;
	};

	class AssetManager
	{
	public:
		/// <summary>
		/// The file that the loader will use must contain those keywords:
		/// models;
		/// textures;
		/// shaders;
		/// fonts;
		/// sounds;
		/// </summary>
		/// <param name="file">-> json file</param>
		void InitResources(std::string file);

		AssetCache GetResource(std::string name);

		std::string GetShader(std::string name);
		std::string GetTexture(std::string name);
		std::string GetModel(std::string name);
		std::string GetFont(std::string name);
		std::string GetSound(std::string name);

		void LoadResource(std::string name, AssetCache& cache);
		void SaveResources();

	private:
		std::map<std::string, AssetCache> m_resources;
		nlohmann::json m_fileData;
		std::string m_fileName;
	};
}