#ifndef QPROGRESSGROUP_H
#define QPROGRESSGROUP_H

#include <QtGlobal>
#include <QObject>
#include <QList>
#include <initializer_list>

class QProgressGroup : public QObject
{
	Q_OBJECT

	Q_PROPERTY(BarState barState READ barState WRITE setBarState NOTIFY barStateChanged)
	Q_PROPERTY(int minimum READ minimum WRITE setMinimum)
	Q_PROPERTY(int maximum READ maximum WRITE setMaximum)
	Q_PROPERTY(int value READ value WRITE setValue NOTIFY valueChanged)
	Q_PROPERTY(QList<QProgressGroup::ProgressAdapter*> adapters READ adapters RESET clearAdapters)
public:
	enum BarState {
		Off,
		Indeterminated,
		Active,
		Paused,
		Stopped
	};
	Q_ENUM(BarState)

	class ProgressAdapter {
	public:
		virtual inline ~ProgressAdapter(){}
		virtual void changeState(QProgressGroup::BarState state,
									   int minimum,
									   int maximum,
									   int value) = 0;

		virtual void setMinimum(int minimum) = 0;
		virtual void setMaximum(int maximum) = 0;
		virtual void setRange(int minimum, int maximum) = 0;

		virtual void setValue(int value) = 0;
	};

	explicit QProgressGroup(QObject *parent = 0);
	QProgressGroup(const std::initializer_list<ProgressAdapter *> &adapters, QObject *parent = 0);
	~QProgressGroup();

	BarState barState() const;
	int minimum() const;
	int maximum() const;
	int value() const;

	void addAdapter(ProgressAdapter *adapter);
	bool takeAdapter(ProgressAdapter *adapter);

	QList<ProgressAdapter*> adapters() const;

public slots:
	void setBarState(BarState barState);
	void setMinimum(int minimum);
	void setMaximum(int maximum);
	void setRange(int minimum, int maximum);
	void setValue(int value);

	void sync();
	void reset();

	void clearAdapters();

signals:
	void barStateChanged(BarState barState);
	void valueChanged(int value);

private:
	QList<ProgressAdapter*> adapterList;
	BarState state;
	int min;
	int max;
	int val;
};

#endif // QPROGRESSGROUP_H
