#include "qelementmodel.h"
#include <QMetaProperty>

QElementModel::QElementModel(QObject *parent) :
	QAbstractListModel(parent),
	headerTitle(),
	itemRoles(),
	roleIndex(Qt::UserRole + 1),
	elements()
{}

QElementModel::QElementModel(const QStringList &roles, QObject *parent) :
	QElementModel(parent)
{
	this->setRoles(roles);
}

QVariant QElementModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if(section == 0 &&
	   orientation == Qt::Horizontal &&
	   role == Qt::DisplayRole)
		return this->headerTitle;
	else
		return QVariant();
}

int QElementModel::rowCount(const QModelIndex &parent) const
{
	if (parent.isValid())
		return 0;
	else
		return this->elements.size();
}

QHash<int, QByteArray> QElementModel::roleNames() const
{
	QHash<int, QByteArray> res;
	for(auto it = this->itemRoles.constBegin(), end = this->itemRoles.constEnd(); it != end; ++it)
		res.insert(it.value(), it.key().toLatin1());
	return res;
}

QVariant QElementModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid() ||
		index.column() != 0 ||
		index.row() < 0 ||
		index.row() >= this->elements.size())
		return QVariant();

	auto roleName = this->itemRoles.key(role);
	if(roleName.isNull())
		return QVariant();
	else
		return this->elements[index.row()]->property(roleName.toLatin1().constData());
}

bool QElementModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if(this->readonly)
		return false;

	if (!index.isValid() ||
		index.column() != 0 ||
		index.row() < 0 ||
		index.row() >= this->elements.size())
		return false;

	auto roleName = this->itemRoles.key(role);
	if(roleName.isNull())
		return false;
	else {
		this->elements[index.row()]->setProperty(roleName.toLatin1().constData(), value);
		emit dataChanged(index, index, {role});
		return true;
	}
}

Qt::ItemFlags QElementModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return Qt::NoItemFlags;

	auto flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemNeverHasChildren;
	if(!this->readonly)
		flags |= Qt::ItemIsEditable;
	return flags;
}

void QElementModel::append(QObject *object, bool takeOwn)
{
	auto index = this->elements.size();
	this->beginInsertRows(QModelIndex(), index, index);
	this->elements.append(object);
	if(takeOwn)
		object->setParent(this);
	this->endInsertRows();
}

QObject *QElementModel::append(QHash<QString, QVariant> fieldMap)
{
	auto obj = new QObject(this);
	for(auto it = fieldMap.constBegin(), end = fieldMap.constEnd(); it != end; ++it)
		obj->setProperty(it.key().toLatin1(), it.value());
	this->append(obj, false);
	return obj;
}

void QElementModel::insert(int index, QObject *object, bool takeOwn)
{
	this->beginInsertRows(QModelIndex(), index, index);
	this->elements.append(object);
	if(takeOwn)
		object->setParent(this);
	this->endInsertRows();
}

QObject *QElementModel::insert(int index, QHash<QString, QVariant> fieldMap)
{
	auto obj = new QObject(this);
	for(auto it = fieldMap.constBegin(), end = fieldMap.constEnd(); it != end; ++it)
		obj->setProperty(it.key().toLatin1(), it.value());
	this->insert(index, obj, false);
	return obj;
}

void QElementModel::remove(int index)
{
	this->beginRemoveRows(QModelIndex(), index, index);
	auto obj = this->elements.takeAt(index);
	if(obj->parent() == this)
		obj->deleteLater();
	this->endRemoveRows();
}

void QElementModel::move(int fromIndex, int toIndex)
{
	if(this->beginMoveRows(QModelIndex(), fromIndex, fromIndex, QModelIndex(), toIndex)) {
		this->elements.move(fromIndex, toIndex);
		this->endMoveRows();
	}
}

void QElementModel::reset(bool resetRoles)
{
	this->beginResetModel();
	this->elements.clear();
	if(resetRoles) {
		this->itemRoles.clear();
		emit rolesChanged({});
	}
	this->endResetModel();
}

QString QElementModel::header() const
{
	return this->headerTitle;
}

bool QElementModel::isReadonly() const
{
	return this->readonly;
}

QStringList QElementModel::roles() const
{
	return this->itemRoles.keys();
}

int QElementModel::roleId(QString role) const
{
	return this->itemRoles.value(role, -1);
}

void QElementModel::setHeader(QString header)
{
	if (this->headerTitle == header)
		return;

	this->headerTitle = header;
	emit headerChanged(header);
	emit headerDataChanged(Qt::Horizontal, 0, 0);
}

void QElementModel::setReadonly(bool readonly)
{
	if (this->readonly == readonly)
		return;

	this->readonly = readonly;
	emit readonlyChanged(readonly);
}

void QElementModel::setRoles(QStringList roles)
{
	this->itemRoles.clear();
	foreach(auto role, roles)
		this->itemRoles.insert(role, this->roleIndex++);
	emit rolesChanged(roles);
}

void QElementModel::addRole(QString role)
{
	this->itemRoles.insert(role, this->roleIndex++);
}

void QElementModel::removeRole(QString role)
{
	this->itemRoles.remove(role);
}

void QElementModel::initRoles(QMetaObject metaObject, bool includeSuperClass)
{
	QStringList propertyNames;
	for(auto i = includeSuperClass ? 0 : metaObject.propertyOffset(); i < metaObject.propertyCount(); i++) {
		auto property = metaObject.property(i);
		propertyNames.append(QString::fromLatin1(property.name()));
	}
	this->setRoles(propertyNames);
}

void QElementModel::initRoles(QObject *object, bool includeSuperClass)
{
	this->initRoles(*(object->metaObject()), includeSuperClass);
}
