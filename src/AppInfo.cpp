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

void AppInfo::setAppPath(const QString& appPath)
{
	mAppPath = appPath;
}

const QString& AppInfo::appPath()
{
	return mAppPath;
}

QString AppInfo::appDir()
{
	QFileInfo fileInfo(mAppPath);
	return fileInfo.dir().absolutePath();
}
