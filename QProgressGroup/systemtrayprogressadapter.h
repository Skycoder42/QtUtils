#ifndef SYSTEMTRAYPROGRESSADAPTER_H
#define SYSTEMTRAYPROGRESSADAPTER_H

#include <QObject>
#include "qprogressgroup.h"
#include <QSystemTrayIcon>
#include <QMovie>

class SystemTrayProgressAdapter : public QObject, public QProgressGroup::ProgressAdapter
{
	Q_OBJECT

	Q_PROPERTY(bool showTrayOff READ showTrayOff WRITE setShowTrayOff)
public:
	SystemTrayProgressAdapter(QSystemTrayIcon *trayIco, QMovie *animation, bool showTrayOff = true);

	bool showTrayOff() const;
	void setShowTrayOff(bool showTrayOff, QIcon offIcon = QIcon());

	// ProgressAdapter interface
	void changeState(QProgressGroup::BarState state, int minimum, int maximum, int value) Q_DECL_OVERRIDE;

	void setMinimum(int minimum) Q_DECL_OVERRIDE;
	void setMaximum(int maximum) Q_DECL_OVERRIDE;
	void setRange(int minimum, int maximum) Q_DECL_OVERRIDE;
	void setValue(int value) Q_DECL_OVERRIDE;

private slots:
	void movieUpdate(int frame);

private:
	QSystemTrayIcon *tray;
	QMovie *movie;
	QIcon ico;
	bool sTrayOff;
};

#endif // SYSTEMTRAYPROGRESSADAPTER_H
