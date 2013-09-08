#ifndef APPINFO_H
#define APPINFO_H

#include <string>

class AppInfo
{
public:
	static AppInfo* instance();
	void setAppPath(const std::string& appPath);
	const std::string& appPath();
	std::string appDir();
	std::string toUnicode(std::string text);
protected:
	AppInfo();
	static AppInfo* mInstance;
	std::string mAppPath;
};

#endif //APPINFO_H