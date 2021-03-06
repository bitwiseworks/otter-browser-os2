/**************************************************************************
* Otter Browser: Web browser controlled by the user, not vice-versa.
* Copyright (C) 2014 Martin Rejda <rejdi@otter.ksp.sk>
* Copyright (C) 2014 - 2021 Michal Dutkiewicz aka Emdek <michal@emdek.pl>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
**************************************************************************/

#ifndef OTTER_QTWEBKITPLUGINFACTORY_H
#define OTTER_QTWEBKITPLUGINFACTORY_H

#include <QtCore/QUrl>
#include <QtWebKit/QWebPluginFactory>
#include <QtWidgets/QWidget>

namespace Otter
{

class QtWebKitWebWidget;

class QtWebKitPluginWidget final : public QWidget
{
	Q_OBJECT

public:
	explicit QtWebKitPluginWidget(const QString &mimeType, const QUrl &url, QWidget *parent = nullptr);

protected:
	void changeEvent(QEvent *event) override;
	void paintEvent(QPaintEvent *event) override;
	void enterEvent(QEvent *event) override;
	void leaveEvent(QEvent *event) override;

private:
	QString m_mimeType;
	QString m_url;
	bool m_isHovered;
};

class QtWebKitPluginFactory final : public QWebPluginFactory
{
	Q_OBJECT

public:
	explicit QtWebKitPluginFactory(QtWebKitWebWidget *parent);

	QObject* create(const QString &mimeType, const QUrl &url, const QStringList &argumentNames, const QStringList &argumentValues) const override;
	QList<QWebPluginFactory::Plugin> plugins() const override;

private:
	QtWebKitWebWidget *m_widget;
};

}

#endif
