/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtWidgets>
#include "qflowlayout.h"

QFlowLayout::QFlowLayout(QWidget *parent, QMargins margins, int hSpacing, int vSpacing) :
    QLayout(parent),
    m_hSpace(hSpacing),
    m_vSpace(vSpacing)
{
	this->setContentsMargins(margins);
}

QFlowLayout::QFlowLayout(QMargins margins, int hSpacing, int vSpacing) :
    QLayout(NULL),
    m_hSpace(hSpacing),
    m_vSpace(vSpacing)
{
	this->setContentsMargins(margins);
}

QFlowLayout::~QFlowLayout()
{
	qDeleteAll(this->itemList);
}

void QFlowLayout::addItem(QLayoutItem *item)
{
    this->itemList.append(item);
	this->update();
}

void QFlowLayout::insertItem(int pos, QLayoutItem *item)
{
    this->itemList.insert(pos, item);
    this->update();
}

void QFlowLayout::moveItem(int oldPos, int newPos)
{
    this->itemList.move(oldPos, newPos);
    this->update();
}

void QFlowLayout::insertWidget(int pos, QWidget *w)
{
    this->addWidget(w);
    this->itemList.move(this->itemList.size() - 1, pos);
    this->update();
}

int QFlowLayout::horizontalSpacing() const
{
    if (this->m_hSpace >= 0)
        return this->m_hSpace;
    else
        return this->smartSpacing(QStyle::PM_LayoutHorizontalSpacing);
}

int QFlowLayout::verticalSpacing() const
{
    if (this->m_vSpace >= 0)
        return this->m_vSpace;
    else
        return this->smartSpacing(QStyle::PM_LayoutVerticalSpacing);
}

int QFlowLayout::count() const
{
    return this->itemList.size();
}

QLayoutItem *QFlowLayout::itemAt(int index) const
{
    return this->itemList.value(index);
}

QLayoutItem *QFlowLayout::takeAt(int index)
{
    if (index >= 0 && index < this->itemList.size())
        return this->itemList.takeAt(index);
    else
        return 0;
}

Qt::Orientations QFlowLayout::expandingDirections() const
{
    return 0;
}

bool QFlowLayout::hasHeightForWidth() const
{
    return true;
}

int QFlowLayout::heightForWidth(int width) const
{
    int height = this->doLayout(QRect(0, 0, width, 0), true);
    return height;
}

void QFlowLayout::setGeometry(const QRect &rect)
{
    this->QLayout::setGeometry(rect);
    this->doLayout(rect, false);
}

QSize QFlowLayout::sizeHint() const
{
    return this->minimumSize();
}

QSize QFlowLayout::minimumSize() const
{
	QSize size;
	foreach (QLayoutItem *item, this->itemList)
        size = size.expandedTo(item->minimumSize());

	int left;
	int top;
	int right;
	int bottom;
	this->getContentsMargins(&left, &top, &right, &bottom);
	size += QSize(left + right, top + bottom);
    return size;
}

int QFlowLayout::doLayout(const QRect &rect, bool testOnly) const
{
    int left, top, right, bottom;
    this->getContentsMargins(&left, &top, &right, &bottom);
    QRect effectiveRect = rect.adjusted(+left, +top, -right, -bottom);
    int x = effectiveRect.x();
    int y = effectiveRect.y();
    int lineHeight = 0;

	foreach (QLayoutItem *item, this->itemList)
    {
        QWidget *wid = item->widget();

        int spaceX = this->horizontalSpacing();
        if (spaceX == -1)
            spaceX = wid->style()->layoutSpacing(QSizePolicy::PushButton, QSizePolicy::PushButton, Qt::Horizontal);

        int spaceY = this->verticalSpacing();
        if (spaceY == -1)
            spaceY = wid->style()->layoutSpacing(QSizePolicy::PushButton, QSizePolicy::PushButton, Qt::Vertical);

        int nextX = x + item->sizeHint().width() + spaceX;
        if (nextX - spaceX > effectiveRect.right() && lineHeight > 0)
        {
            x = effectiveRect.x();
            y = y + lineHeight + spaceY;
            nextX = x + item->sizeHint().width() + spaceX;
            lineHeight = 0;
        }

        if (!testOnly)
            item->setGeometry(QRect(QPoint(x, y), item->sizeHint()));

        x = nextX;
        lineHeight = qMax(lineHeight, item->sizeHint().height());
    }

    return y + lineHeight - rect.y() + bottom;
}

int QFlowLayout::smartSpacing(QStyle::PixelMetric pm) const
{
    QObject *parent = this->parent();
    if (!parent)
        return -1;
    else if (parent->isWidgetType())
    {
        QWidget *pw = static_cast<QWidget *>(parent);
        return pw->style()->pixelMetric(pm, 0, pw);
    }
    else
        return static_cast<QLayout *>(parent)->spacing();
}
