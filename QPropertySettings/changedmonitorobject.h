#ifndef CHANGEDMONITOROBJECT_H
#define CHANGEDMONITOROBJECT_H

#include <QObject>

class ChangedMonitorObject : public QObject
{
	Q_OBJECT

public:
	ChangedMonitorObject(int id, QObject *parent = nullptr);

public slots:
	void trigger();

signals:
	void triggered(int id);

private:
	const int id;
};

#endif // CHANGEDMONITOROBJECT_H
