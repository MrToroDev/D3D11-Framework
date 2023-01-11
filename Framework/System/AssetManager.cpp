#include "AssetManager.h"
#include <fstream>
#include <iostream>
#include "../Logger.h"

using json = nlohmann::json;

void DX::AssetManager::InitResources(std::string file)
{
	try {
		m_fileName = file;
		std::ifstream dataFile(file);
		if (dataFile.is_open() && dataFile.good()) {
			json data = json::parse(dataFile);
			m_fileData = data;

			if (data.find("textures") != data.end()) {
				for (auto& e : data["textures"].items()) {
					AssetCache tempCache;
					tempCache.type = AssetType::TEXTURE;
					tempCache.data = e.value();

					this->m_resources[e.key()] = tempCache;
				}
			}
			
			if (data.find("shaders") != data.end()) {
				for (auto& e : data["shaders"].items()) {
					AssetCache tempCache;
					tempCache.type = AssetType::SHADER;
					tempCache.data = e.value();

					this->m_resources[e.key()] = tempCache;
				}
			}
			
			if (data.find("models") != data.end()) {
				for (auto& e : data["models"].items()) {
					AssetCache tempCache;
					tempCache.type = AssetType::MODEL;
					tempCache.data = e.value();

					this->m_resources[e.key()] = tempCache;
				}
			}

			if (data.find("fonts") != data.end()) {
				for (auto& e : data["fonts"].items()) {
					AssetCache tempCache;
					tempCache.type = AssetType::FONT;
					tempCache.data = e.value();

					this->m_resources[e.key()] = tempCache;
				}
			}

			if (data.find("sounds") != data.end()) {
				for (auto& e : data["sounds"].items()) {
					AssetCache tempCache;
					tempCache.type = AssetType::SOUND;
					tempCache.data = e.value();

					this->m_resources[e.key()] = tempCache;
				}
			}
		}
		else {
			DX::LogCritic("There is not Asset File in folder!");
			return;
		}

		DX::LogInfo(file + " data file opened");
	}
	catch (json::exception& ex) {
		DX::LogError(ex.what());
	}
}

DX::AssetCache DX::AssetManager::GetResource(std::string name)
{
	return m_resources[name];
}

std::string DX::AssetManager::GetShader(std::string name)
{
	for (auto r : m_resources) {
		if (r.second.type == DX::AssetType::SHADER && r.first == name) {
			return m_resources[name].data;
			break;
		}
		else { continue; }
	}

	DX::LogCritic("There is no object called " + name);
	return "<not found>";
}

std::string DX::AssetManager::GetTexture(std::string name)
{
	for (auto r : m_resources) {
		if (r.second.type == DX::AssetType::TEXTURE && r.first == name) {
			return m_resources[name].data;
			break;
		}
		else { continue; }
	}

	DX::LogCritic("There is no object called " + name);
	return "<not found>";
}

std::string DX::AssetManager::GetModel(std::string name)
{
	for (auto r : m_resources) {
		if (r.second.type == DX::AssetType::MODEL && r.first == name) {
			return m_resources[name].data;
			break;
		}
		else { continue; }
	}

	DX::LogCritic("There is no object called " + name);
	return "<not found>";
}

std::string DX::AssetManager::GetFont(std::string name)
{
	for (auto r : m_resources) {
		if (r.second.type == DX::AssetType::FONT && r.first == name) {
			return m_resources[name].data;
			break;
		}
		else { continue; }
	}

	DX::LogCritic("There is no object called " + name);
	return "<not found>";
}

std::string DX::AssetManager::GetSound(std::string name)
{
	for (auto r : m_resources) {
		if (r.second.type == DX::AssetType::SOUND && r.first == name) {
			return m_resources[name].data;
			break;
		}
		else { continue; }
	}

	DX::LogCritic("There is no object called " + name);
	return "<not found>";
}

void DX::AssetManager::LoadResource(std::string name, DX::AssetCache& cache)
{
	m_resources.insert({ name, cache });
}

void DX::AssetManager::SaveResources()
{
	for (auto r : m_resources) {
		if (r.second.type == DX::AssetType::TEXTURE) {
			m_fileData["textures"][r.first] = r.second.data;
		}
		else if (r.second.type == DX::AssetType::SHADER) {
			m_fileData["shaders"][r.first] = r.second.data;
		}
		else if (r.second.type == DX::AssetType::MODEL) {
			m_fileData["models"][r.first] = r.second.data;
		}
	}

	std::ofstream output(m_fileName, std::ios::trunc | std::ios::out);
	output << std::setfill(' ') << std::setw(2) << m_fileData << std::endl;
}
