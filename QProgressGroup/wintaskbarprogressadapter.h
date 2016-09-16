#ifndef WINTASKBARPROGRESSADAPTER_H
#define WINTASKBARPROGRESSADAPTER_H

#include "qprogressgroup.h"
#include <QWinTaskbarProgress>

class WinTaskbarProgressAdapter : public QProgressGroup::ProgressAdapter
{
public:
    WinTaskbarProgressAdapter(QWinTaskbarProgress *bar);

	// ProgressAdapter interface
	void changeState(QProgressGroup::BarState state, int minimum, int maximum, int value) Q_DECL_OVERRIDE;

	void setMinimum(int minimum) Q_DECL_OVERRIDE;
	void setMaximum(int maximum) Q_DECL_OVERRIDE;
	void setRange(int minimum, int maximum) Q_DECL_OVERRIDE;
	void setValue(int value) Q_DECL_OVERRIDE;

private:
	QWinTaskbarProgress *bar;
};

#endif // WINTASKBARPROGRESSADAPTER_H
