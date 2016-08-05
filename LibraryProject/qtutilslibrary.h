#ifndef QTUTILSLIBRARY_H
#define QTUTILSLIBRARY_H

#include <QtGlobal>
#include <QStringList>
#include <QVersionNumber>

class QtUtilsLibrary
{
public:
	enum SupportedUtils : quint32 {
		DialogMaster = (0x01 << 24) | (0x020000 >> 8),
		QPropertySettings = (0x02 << 24) | (0x010000 >> 8)
	};

	static QVersionNumber utilVersion(SupportedUtils util);
};

#define ENSURE_UTIL_AVAILABLE(x) (void)(QtUtilsLibrary::SupportedUtils)x

#endif // QTUTILSLIBRARY_H
