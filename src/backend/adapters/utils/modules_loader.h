#pragma once

#include "../../utils/shared.h"


class ModulesLoader {
public:
	ModulesLoader();
	~ModulesLoader();

	void loadAuthorized();

private:
	std::vector<std::string> m_authorizedModuleNames;

	void addPath(const std::string &bin = nullptr, const std::string &data = nullptr);
	void loadModule(const std::string &bin_path, const std::string &data_path);
	void postLoad();
};