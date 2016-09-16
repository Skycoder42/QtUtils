#include "progressbaradapter.h"

ProgressBarAdapter::ProgressBarAdapter(QProgressBar *bar) :
    ProgressAdapter(),
	bar(bar)
{}

void ProgressBarAdapter::changeState(QProgressGroup::BarState state, int minimum, int maximum, int value)
{
	switch(state) {
	case QProgressGroup::Off:
		this->bar->setRange(0, 1);
		this->bar->setValue(0);
		this->bar->setTextVisible(false);
		break;
	case QProgressGroup::Indeterminated:
		this->bar->setRange(0, 0);
		this->bar->setTextVisible(false);
		break;
	case QProgressGroup::Active:
	case QProgressGroup::Paused:
	case QProgressGroup::Stopped:
		this->bar->setRange(minimum, maximum);
		this->bar->setValue(value);
		this->bar->setTextVisible(true);
		break;
	}
}

void ProgressBarAdapter::setMinimum(int minimum)
{
    this->bar->setMinimum(minimum);
}

void ProgressBarAdapter::setMaximum(int maximum)
{
    this->bar->setMaximum(maximum);
}

void ProgressBarAdapter::setRange(int minimum, int maximum)
{
    this->bar->setRange(minimum, maximum);
}

void ProgressBarAdapter::setValue(int value)
{
    this->bar->setValue(value);
}
