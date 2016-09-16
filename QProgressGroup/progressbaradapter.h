#ifndef PROGRESSBARADAPTER_H
#define PROGRESSBARADAPTER_H

#include "qprogressgroup.h"
#include <QProgressBar>

class ProgressBarAdapter : public QProgressGroup::ProgressAdapter
{
public:
    ProgressBarAdapter(QProgressBar *bar);

	// ProgressAdapter interface
	void changeState(QProgressGroup::BarState state, int minimum, int maximum, int value) Q_DECL_OVERRIDE;

	void setMinimum(int minimum) Q_DECL_OVERRIDE;
	void setMaximum(int maximum) Q_DECL_OVERRIDE;
	void setRange(int minimum, int maximum) Q_DECL_OVERRIDE;
	void setValue(int value) Q_DECL_OVERRIDE;

private:
	QProgressBar *bar;
};

#endif // PROGRESSBARADAPTER_H
