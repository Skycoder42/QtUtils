#include "wintaskbarprogressadapter.h"

WinTaskbarProgressAdapter::WinTaskbarProgressAdapter(QWinTaskbarProgress *bar) :
    ProgressAdapter(),
    bar(bar)
{}

void WinTaskbarProgressAdapter::changeState(QProgressGroup::BarState state, int minimum, int maximum, int value)
{
	switch(state) {
	case QProgressGroup::Off:
		this->bar->hide();
		break;
	case QProgressGroup::Indeterminated:
		this->bar->setRange(0, 0);
		this->bar->setValue(0);
		this->bar->show();
		break;
	case QProgressGroup::Active:
		this->bar->setRange(minimum, maximum);
		this->bar->setValue(value);
		this->bar->resume();
		this->bar->show();
		break;
	case QProgressGroup::Paused:
		this->bar->setRange(minimum, maximum);
		this->bar->setValue(value);
		this->bar->pause();
		this->bar->show();
		break;
	case QProgressGroup::Stopped:
		this->bar->setRange(minimum, maximum);
		this->bar->setValue(value);
		this->bar->stop();
		this->bar->show();
		break;
	}
}

void WinTaskbarProgressAdapter::setMinimum(int minimum)
{
    this->bar->setMinimum(minimum);
}

void WinTaskbarProgressAdapter::setMaximum(int maximum)
{
    this->bar->setMaximum(maximum);
}

void WinTaskbarProgressAdapter::setRange(int minimum, int maximum)
{
    this->bar->setRange(minimum, maximum);
}

void WinTaskbarProgressAdapter::setValue(int value)
{
    this->bar->setValue(value);
}
