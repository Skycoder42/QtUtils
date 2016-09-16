#include "systemtrayprogressadapter.h"

SystemTrayProgressAdapter::SystemTrayProgressAdapter(QSystemTrayIcon *trayIco, QMovie *animation, bool showTrayOff) :
	ProgressAdapter(),
	tray(trayIco),
	movie(animation),
	ico(trayIco->icon()),
	sTrayOff(showTrayOff)
{
	QObject::connect(this->movie, &QMovie::frameChanged,
					 this, &SystemTrayProgressAdapter::movieUpdate);
}

bool SystemTrayProgressAdapter::showTrayOff() const
{
	return this->sTrayOff;
}

void SystemTrayProgressAdapter::setShowTrayOff(bool showTrayOff, QIcon offIcon)
{
	this->sTrayOff = showTrayOff;
	if(!offIcon.isNull())
		this->ico = offIcon;
}

void SystemTrayProgressAdapter::changeState(QProgressGroup::BarState state, int minimum, int maximum, int value)
{
	Q_UNUSED(minimum)
	Q_UNUSED(maximum)
	Q_UNUSED(value)
	switch(state) {
	case QProgressGroup::Off:
		this->movie->stop();
		if(this->sTrayOff)
			this->tray->setIcon(this->ico);
		else
			this->tray->hide();
		break;
	case QProgressGroup::Indeterminated:
	case QProgressGroup::Active:
	case QProgressGroup::Paused:
	case QProgressGroup::Stopped:
		this->movie->start();
		if(!this->sTrayOff)
			this->tray->show();
		break;
	}
}

void SystemTrayProgressAdapter::setMinimum(int minimum)
{
	Q_UNUSED(minimum)
}

void SystemTrayProgressAdapter::setMaximum(int maximum)
{
	Q_UNUSED(maximum)
}

void SystemTrayProgressAdapter::setRange(int minimum, int maximum)
{
	Q_UNUSED(minimum)
	Q_UNUSED(maximum)
}

void SystemTrayProgressAdapter::setValue(int value)
{
	Q_UNUSED(value)
}

void SystemTrayProgressAdapter::movieUpdate(int frame)
{
	Q_UNUSED(frame)
	this->tray->setIcon(QIcon(this->movie->currentPixmap()));
}
