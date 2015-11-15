/********************************************************************************
**
**	Copyright (C) 2007-2015 Witold Gantzke & Kirill Lepskiy
**
**	This file is part of the ACF Toolkit.
**
**	This file may be used under the terms of the GNU Lesser
**	General Public License version 2.1 as published by the Free Software
**	Foundation and appearing in the file LicenseLGPL.txt included in the
**	packaging of this file.  Please review the following information to
**	ensure the GNU Lesser General Public License version 2.1 requirements
**	will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
**	If you are unsure which license is appropriate for your use, please
**	contact us at info@imagingtools.de.
**
** 	See http://www.ilena.org or write info@imagingtools.de for further
** 	information about the ACF.
**
********************************************************************************/


#pragma once


// Qt includes
#include <QtCore/QtGlobal>
#include <QtCore/QMap>
#include <QtCore/QEvent>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QGraphicsEffect>
#include <QtWidgets/QWidget>
#else
#include <QtGui/QGraphicsEffect>
#include <QtGui/QWidget>
#endif

// ACF includes
#include "istd/TIFactory.h"


namespace iwidgets
{


/*
	Helper class for creating widget decorations based on graphics effects, such drop shadow.
*/
class CFocusDecorator: public QObject
{
	Q_OBJECT
public:
	typedef istd::TIFactory<QGraphicsEffect> GraphicsEffectFactory;

	CFocusDecorator(QObject* parentPtr = NULL);
	virtual ~CFocusDecorator();

	bool RegisterWidget(QWidget* widgetPtr, GraphicsEffectFactory* effectFactoryPtr);
	void UnregisterWidget(QWidget* widgetPtr);
	void UnregisterAllWidgets();

protected:
	// reimplemented (QObject)
	virtual bool eventFilter(QObject* objectPtr, QEvent* eventPtr);

private Q_SLOTS:
	void OnObjectDestroyed(QObject* objectPtr);

private:
	typedef QMap<QObject*, GraphicsEffectFactory*> WidgetEffectsMap;
	WidgetEffectsMap m_widgetEffectsMap;
};


} // namespace iwidgets


