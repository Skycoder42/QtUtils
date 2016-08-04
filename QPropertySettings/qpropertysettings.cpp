#include "qpropertysettings.h"
#include "changedmonitorobject.h"
#include <QDebug>

QPropertySettings::QPropertySettings(QObject *parent) :
	QPropertySettings(nullptr, parent)
{}

QPropertySettings::QPropertySettings(QSettings *settings, QObject *parent) :
	QObject(parent),
	settings(settings ? settings : new QSettings(this)),
	indexCounter(0),
	propertyData()
{}

int QPropertySettings::findId(QObject *object, const char *property) const
{
	auto metaProperty = this->getMetaProperty(object, property);
	if(metaProperty.isValid())
		return this->findId(object, metaProperty);
	else
		return -1;
}

int QPropertySettings::findId(QObject *object, const QMetaProperty &property) const
{
	for(auto it = this->propertyData.constBegin(), end = this->propertyData.constEnd(); it != end; ++it) {
		if(it->object == object &&
		   QString(it->property.name()) == property.name())
			return it.key();
	}

	return -1;
}

int QPropertySettings::addProperty(QObject *object, const char *property, const QString &group, const QString &key)
{
	auto metaProperty = this->getMetaProperty(object, property);
	if(metaProperty.isValid())
		return this->addProperty(object, metaProperty, group, key);
	else
		return -1;
}

int QPropertySettings::addProperty(QObject *object, const QMetaProperty &property, QString group, QString key)
{
	int mapId = this->indexCounter++;
	PropertyData data;
	data.object = object;
	data.property = property;

	if(group.isNull()) {
		group = object->objectName();
		if(group.isEmpty())
			group = QString::fromLocal8Bit(object->metaObject()->className());
	}
	if(key.isEmpty())
		key = QString::fromLocal8Bit(property.name());
	if(!group.isEmpty())
		group.append(QLatin1Char('/'));
	data.fullKey = group + key;

	if(property.hasNotifySignal()) {
		data.monitor = new ChangedMonitorObject(mapId, this);
		auto signalMethod = property.notifySignal();
		auto slotMethod = this->getChangedSlot(data.monitor);
		connect(object, signalMethod, data.monitor, slotMethod, Qt::DirectConnection);
		connect(data.monitor, &ChangedMonitorObject::triggered,
				this, &QPropertySettings::saveProperty,
				Qt::QueuedConnection);
	} else
		qWarning("Property without changed signal will not automatically save!");

	this->propertyData.insert(mapId, data);
	this->loadProperty(mapId);
	return mapId;
}

void QPropertySettings::loadProperty(int id)
{
	auto data = this->propertyData.value(id);
	if(!data.object.isNull() &&
	   data.property.isWritable()) {
		auto loadData = this->settings->value(data.fullKey);
		qDebug() << "Loading" << data.fullKey << "with value" << loadData;
		if(loadData.isValid())
			data.property.write(data.object, loadData);
	}
}

void QPropertySettings::loadAll()
{
	foreach (auto key, this->propertyData.keys())
		this->loadProperty(key);
}

void QPropertySettings::saveProperty(int id)
{
	auto data = this->propertyData.value(id);
	if(!data.object.isNull() &&
	   data.property.isReadable()) {
		auto saveData = data.property.read(data.object);
		qDebug() << "Saving" << data.fullKey << "with value" << saveData;
		if(saveData.isValid())
			this->settings->setValue(data.fullKey, saveData);
	}
}

void QPropertySettings::saveAll()
{
	foreach (auto key, this->propertyData.keys())
		this->saveProperty(key);
}

void QPropertySettings::resetProperty(int id, bool resetProperty)
{
	auto data = this->propertyData.value(id);
	if(!data.object.isNull()) {
		qDebug() << "Resetting" << data.fullKey;
		this->settings->remove(data.fullKey);
		if(resetProperty && data.property.isResettable())
			data.property.reset(data.object);
	}
}

void QPropertySettings::resetAll(bool resetProperty)
{
	foreach (auto key, this->propertyData.keys())
		this->resetProperty(key, resetProperty);
}

void QPropertySettings::removeProperty(int id, bool resetSettings)
{
	auto data = this->propertyData.take(id);
	if(!data.fullKey.isNull()) {
		if(resetSettings)
			this->settings->remove(data.fullKey);
	}
	if(data.monitor)
		data.monitor->deleteLater();
}

void QPropertySettings::removeAll(bool resetSettings)
{
	foreach (auto key, this->propertyData.keys())
		this->removeProperty(key, resetSettings);
}

QMetaMethod QPropertySettings::getChangedSlot(ChangedMonitorObject *monitor) const
{
	auto metaObject = monitor->metaObject();
	auto slotIndex = metaObject->indexOfSlot("trigger()");
	return metaObject->method(slotIndex);
}

QMetaProperty QPropertySettings::getMetaProperty(QObject *object, const char *property) const
{
	auto metaObject = object->metaObject();
	auto propIndex = metaObject->indexOfProperty(property);
	if(propIndex >= 0)
		return metaObject->property(propIndex);
	else
		return QMetaProperty();
}



QPropertySettings::PropertyData::PropertyData() :
	object(nullptr),
	property(),
	fullKey(),
	monitor(nullptr)
{}
