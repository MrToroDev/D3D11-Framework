#include "ConfigFile.h"
#include <sstream>
#include <fstream>
#include <stack>

std::string getStringBetweenChars(std::string& text, char startC, char endC) {
    std::stack<int> dels;
    for (int i = 0; i < text.size(); i++) {
        // If opening delimiter
        // is encountered
        if (text[i] == startC) {
            dels.push(i);
        }

        // If closing delimiter
        // is encountered
        else if (text[i] == endC && !dels.empty()) {

            // Extract the position
            // of opening delimiter
            int pos = dels.top();

            dels.pop();

            // Length of substring
            int len = i - 1 - pos;

            // Extract the substring
            std::string ans = text.substr(
                pos + 1, len);

            return ans;
        }
    }
}

void DX::ConfigFile::ParseFile(std::string _file)
{
	std::ifstream file(_file);

	if (file.good())
	{
        isFileFound = true;
		std::string line;
		while (std::getline(file, line))
		{
			if (
				line.find("//") != std::string::npos ||
				line.find("#") != std::string::npos
				) continue;

            if (line.find("[") != std::string::npos)
            {
                std::istringstream ss(line);
                std::string text = ss.str();

                std::string headerName = getStringBetweenChars(text, '[', ']');

                if (this->keywords_data.count(headerName) > 0) continue;
                else {
                    std::vector<KeyData> keywords;

                    std::string keyName;
                    char op;
                    std::string value;
                    while (file >> keyName >> op >> value) {
                        if (keyName.find("[") != std::string::npos) break;

                        keywords.push_back({ keyName, value });
                    }
                    this->keywords_data.insert({ headerName, keywords });
                    file.clear();
                    file.seekg(0, std::ifstream::beg);
                }
            }
		}
    }
    else {
        isFileFound = false;
    }

	file.close();
}

DX::ConfigFile::ConfigFile()
{
}

DX::ConfigFile::ConfigFile(std::string file)
{
    filename = file;
    ParseFile(file);
}

DX::ConfigFile::~ConfigFile()
{
}

void DX::ConfigFile::OpenFile(std::string file)
{
    filename = file;
    ParseFile(file);
}

void DX::ConfigFile::SaveFileData()
{
    std::ofstream file(filename);

    // TODO: need to be implemented
}

bool DX::ConfigFile::FileNotFound()
{
    return isFileFound;
}

int DX::ConfigFile::get_integer(std::string header, std::string keyword)
{
    auto h = keywords_data.find(header);
    KeyData foundData;
    if (h != keywords_data.end()) {
        for (auto key : h->second) {
            if (key.name == keyword) foundData = key;
            else continue;
        }
    }
    else {
        throw std::exception(std::string("Cannot find the header " + header).c_str());
    }

    return std::stoi(foundData.value);
}

double DX::ConfigFile::get_double(std::string header, std::string keyword)
{
    auto h = keywords_data.find(header);
    KeyData foundData;
    if (h != keywords_data.end()) {
        for (auto key : h->second) {
            if (key.name == keyword) foundData = key;
            else continue;
        }
    }
    else {
        throw std::exception(std::string("Cannot find the header " + header).c_str());
    }

    return std::stod(foundData.value);
}

float DX::ConfigFile::get_float(std::string header, std::string keyword)
{
    auto h = keywords_data.find(header);
    KeyData foundData;
    if (h != keywords_data.end()) {
        for (auto key : h->second) {
            if (key.name == keyword) foundData = key;
            else continue;
        }
    }
    else {
        throw std::exception(std::string("Cannot find the header " + header).c_str());
    }

    return std::stof(foundData.value);
}

bool DX::ConfigFile::get_bool(std::string header, std::string keyword)
{
    auto h = keywords_data.find(header);
    KeyData foundData;
    if (h != keywords_data.end()) {
        for (auto key : h->second) {
            if (key.name == keyword) foundData = key;
            else continue;
        }
    }
    else {
        throw std::exception(std::string("Cannot find the header " + header).c_str());
    }

    return (bool)std::stoi(foundData.value);
}

std::string DX::ConfigFile::get_string(std::string header, std::string keyword)
{
    auto h = keywords_data.find(header);
    KeyData foundData;
    if (h != keywords_data.end()) {
        for (auto key : h->second) {
            if (key.name == keyword) foundData = key;
            else continue;
        }
    }
    else {
        throw std::exception(std::string("Cannot find the header " + header).c_str());
    }

    return foundData.value;
}
