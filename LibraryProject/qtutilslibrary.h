#ifndef QTUTILSLIBRARY_H
#define QTUTILSLIBRARY_H

#include <QtGlobal>
#include <QStringList>
#include <QVersionNumber>

#define ENUM_ID(index, versionMajor, versionMinor, versionPatch) (index << 24) | (QT_VERSION_CHECK(versionMajor, versionMinor, versionPatch) >> 8)

class QtUtilsLibrary
{
public:
	enum SupportedUtils : quint32 {
		DialogMaster = ENUM_ID(1, 2, 0, 0),
		QPropertySettings = ENUM_ID(2, 1, 0, 0),
		QElementModel = ENUM_ID(3, 1, 0, 0)
	};

	static bool ensureUtilAvailable(SupportedUtils util);
	static QVersionNumber utilVersion(SupportedUtils util);
};

#define STATIC_UTIL_AVAILABLE(x) (void)(QtUtilsLibrary::SupportedUtils)x

#endif // QTUTILSLIBRARY_H
