#pragma once

#include <string>
#include <map>
#include <vector>

namespace DX
{
	class ConfigFile
	{
	private:
		struct KeyData
		{
			std::string name;
			std::string value;
		};

		std::map<std::string, std::vector<KeyData>> keywords_data;
		std::string filename;
		bool isFileFound;
		void ParseFile(std::string f);

	public:
		ConfigFile();
		ConfigFile(std::string file);
		~ConfigFile();

		void OpenFile(std::string file);
		void SaveFileData(); // TODO: need to be implemented

		bool FileNotFound();

		int get_integer(std::string header, std::string key);
		double get_double(std::string header, std::string key);
		float get_float(std::string header, std::string key);
		bool get_bool(std::string header, std::string key);
		std::string get_string(std::string header, std::string key);
	};
}