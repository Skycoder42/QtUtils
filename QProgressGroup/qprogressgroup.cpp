#include "qprogressgroup.h"

QProgressGroup::QProgressGroup(QObject *parent) :
	QProgressGroup({}, parent)
{}

QProgressGroup::QProgressGroup(const std::initializer_list<QProgressGroup::ProgressAdapter*> &adapters, QObject *parent) :
	QObject(parent),
	adapterList(adapters),
	state(Off),
	min(0),
	max(100),
	val(0)
{}

QProgressGroup::~QProgressGroup()
{
	qDeleteAll(this->adapterList);
}

QProgressGroup::BarState QProgressGroup::barState() const
{
	return this->state;
}

void QProgressGroup::setBarState(QProgressGroup::BarState barState)
{
    if(this->state == barState)
		return;

	foreach (ProgressAdapter *adapter, this->adapterList) {
		adapter->changeState(barState,
								   this->min,
								   this->max,
								   this->val);
	}

	this->state = barState;
	emit barStateChanged(barState);
}

int QProgressGroup::minimum() const
{
	return this->min;
}

void QProgressGroup::setMinimum(int minimum)
{
	switch (this->state) {
	case Active:
	case Paused:
	case Stopped:
		foreach(ProgressAdapter *adapter, this->adapterList)
			adapter->setMinimum(minimum);
		break;
	default:
		break;
	}
	this->min = minimum;
}

int QProgressGroup::maximum() const
{
	return this->max;
}

void QProgressGroup::setMaximum(int maximum)
{
	switch (this->state) {
	case Active:
	case Paused:
	case Stopped:
		foreach(ProgressAdapter *adapter, this->adapterList)
			adapter->setMaximum(maximum);
		break;
	default:
		break;
	}
	this->max = maximum;
}

void QProgressGroup::setRange(int minimum, int maximum)
{
	switch (this->state) {
	case Active:
	case Paused:
    case Stopped:
		foreach(ProgressAdapter *adapter, this->adapterList)
            adapter->setRange(minimum, maximum);
        break;
	default:
		break;
	}
	this->min = minimum;
	this->max = maximum;
}

int QProgressGroup::value() const
{
	return this->val;
}

void QProgressGroup::setValue(int value)
{
	switch (this->state) {
	case Active:
	case Paused:
	case Stopped:
		foreach(ProgressAdapter *adapter, this->adapterList)
			adapter->setValue(value);
        break;
	default:
		break;
	}
	this->val = value;
	emit valueChanged(value);
}

void QProgressGroup::addAdapter(QProgressGroup::ProgressAdapter *adapter)
{
	this->adapterList.append(adapter);
	adapter->changeState(this->state,
						 this->min,
						 this->max,
						 this->val);
}

bool QProgressGroup::takeAdapter(QProgressGroup::ProgressAdapter *adapter)
{
	return (this->adapterList.removeAll(adapter) > 0);
}

QList<QProgressGroup::ProgressAdapter*> QProgressGroup::adapters() const
{
	return this->adapterList;
}

void QProgressGroup::sync()
{
	foreach (ProgressAdapter *adapter, this->adapterList) {
		adapter->changeState(this->state,
								   this->min,
								   this->max,
								   this->val);
	}
}

void QProgressGroup::reset()
{
	this->setBarState(Off);
	this->setValue(this->min);
}

void QProgressGroup::clearAdapters()
{
	qDeleteAll(this->adapterList);
	this->adapterList.clear();
}
