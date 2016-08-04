#include "changedmonitorobject.h"

ChangedMonitorObject::ChangedMonitorObject(int id, QObject *parent) :
	QObject(parent),
	id(id)
{}

void ChangedMonitorObject::trigger() {
	emit triggered(id);
}
