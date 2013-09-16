#include "AppInfo.h"

AppInfo* AppInfo::mInstance = 0;

AppInfo* AppInfo::instance()
{
	if(mInstance == 0)
	{
		mInstance = new AppInfo;
	}
	return mInstance;
}

AppInfo::AppInfo()
{

}

void AppInfo::setAppPath(const std::string& appPath)
{
	mAppPath = appPath;
}

const std::string& AppInfo::appPath()
{
	return mAppPath;
}

std::string AppInfo::appDir()
{
    return "";
}

std::string AppInfo::toUTF8(std::string localString )
{
    return "";
}

std::string AppInfo::fromUTF8( std::string unicodeString )
{
	return "";
}
