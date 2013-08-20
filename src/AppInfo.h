#ifndef APPINFO_H
#define APPINFO_H

#include <QtCore/QtCore>

class AppInfo
{
public:
	static AppInfo* instance();

	void setAppPath(const QString& appPath);
	const QString& appPath();
	QString appDir();
protected:
	AppInfo();
	static AppInfo* mInstance;

	QString mAppPath;
};

#endif //APPINFO_H