#ifndef QPROPERTYSETTINGS_H
#define QPROPERTYSETTINGS_H

#include <QObject>
#include <QSettings>
#include <QPointer>
#include <QSignalMapper>
#include <QMetaProperty>
class ChangedMonitorObject;

class QPropertySettings : public QObject
{
	Q_OBJECT
public:
	explicit QPropertySettings(QObject *parent = nullptr);
	explicit QPropertySettings(QSettings *settings, QObject *parent = nullptr);

	int findId(QObject *object, const char *property) const;
	int findId(QObject *object, const QMetaProperty &property) const;

	int addProperty(QObject *object, const char *property, const QString &group = QString(), const QString &key = QString());
	int addProperty(QObject *object, const QMetaProperty &property, QString group = QString(), QString key = QString());

public slots:
	void loadProperty(int id);
	void loadAll();
	void saveProperty(int id);
	void saveAll();
	void resetProperty(int id, bool resetProperty = true);
	void resetAll(bool resetProperty = true);
	void removeProperty(int id, bool resetSettings = true);
	void removeAll(bool resetSettings = true);

private:
	struct PropertyData {
		PropertyData();

		QPointer<QObject> object;
		QMetaProperty property;
		QString fullKey;

		ChangedMonitorObject *monitor;
	};

	QSettings *settings;
	int indexCounter;
	QHash<int, PropertyData> propertyData;

	QMetaMethod getChangedSlot(ChangedMonitorObject *monitor) const;
	QMetaProperty getMetaProperty(QObject *object, const char *property) const;
};

#endif // QPROPERTYSETTINGS_H
