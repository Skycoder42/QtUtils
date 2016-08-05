#include "qtutilslibrary.h"

static QVector<quint8> knownUtils = {
	0x01,//DialogMaster
	0x02//QPropertySettings
};

bool QtUtilsLibrary::ensureUtilAvailable(QtUtilsLibrary::SupportedUtils util)
{
	quint32 idNum = util;
	return knownUtils.contains((quint8)((idNum >> 24) & 0x000000FF));
}

QVersionNumber QtUtilsLibrary::utilVersion(QtUtilsLibrary::SupportedUtils util)
{
	quint32 idNum = util;
	auto major = (idNum >> 16) & 0x000000FF;
	auto minor = (idNum >> 8) & 0x000000FF;
	auto patch = idNum & 0x000000FF;
	return QVersionNumber(major, minor, patch);
}
