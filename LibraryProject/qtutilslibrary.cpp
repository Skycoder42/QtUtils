#include "qtutilslibrary.h"

QVersionNumber QtUtilsLibrary::utilVersion(QtUtilsLibrary::SupportedUtils util)
{
	quint32 idNum = util;
	auto major = (idNum >> 16) & 0x000000FF;
	auto minor = (idNum >> 8) & 0x000000FF;
	auto patch = idNum & 0x000000FF;
	return QVersionNumber(major, minor, patch);
}
