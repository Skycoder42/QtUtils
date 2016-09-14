#ifndef QELEMENTMODEL_H
#define QELEMENTMODEL_H

#include <QAbstractListModel>
#include <QStringList>
#include <QObjectList>

class QElementModel : public QAbstractListModel
{
	Q_OBJECT

	Q_PROPERTY(QString header READ header WRITE setHeader NOTIFY headerChanged)
	Q_PROPERTY(bool readonly READ isReadonly WRITE setReadonly NOTIFY readonlyChanged)
	Q_PROPERTY(QStringList roles READ roles WRITE setRoles NOTIFY rolesChanged)

public:
	explicit QElementModel(QObject *parent = nullptr);
	explicit QElementModel(const QStringList &roles, QObject *parent = nullptr);

	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	QHash<int, QByteArray> roleNames() const override;

	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
	bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
	Qt::ItemFlags flags(const QModelIndex& index) const override;

	void append(QObject *object, bool takeOwn = true);
	QObject *append(QHash<QString, QVariant> fieldMap);
	void insert(int index, QObject *object, bool takeOwn = true);
	QObject *insert(int index, QHash<QString, QVariant> fieldMap);
	void remove(int index);
	void move(int fromIndex, int toIndex);
	void reset(bool resetRoles = false);

	QString header() const;
	bool isReadonly() const;
	QStringList roles() const;
	int roleId(QString role) const;

public slots:
	void setHeader(QString header);
	void setReadonly(bool readonly);
	void setRoles(QStringList roles);

	void addRole(QString role);
	void removeRole(QString role);
	void initRoles(QMetaObject metaObject, bool includeSuperClass = true);
	void initRoles(QObject *object, bool includeSuperClass = true);

signals:
	void headerChanged(QString header);
	void readonlyChanged(bool readonly);
	void rolesChanged(QStringList roles);

private:
	QString headerTitle;
	bool readonly;
	QHash<QString, int> itemRoles;
	int roleIndex;

	QObjectList elements;
};

#endif // QELEMENTMODEL_H
