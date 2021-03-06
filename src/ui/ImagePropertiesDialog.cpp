/**************************************************************************
* Otter Browser: Web browser controlled by the user, not vice-versa.
* Copyright (C) 2013 - 2021 Michal Dutkiewicz aka Emdek <michal@emdek.pl>
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

#include "ImagePropertiesDialog.h"
#include "../core/Utils.h"

#include "ui_ImagePropertiesDialog.h"

#include <QtCore/QBuffer>
#include <QtCore/QMimeDatabase>
#include <QtGui/QImageReader>

namespace Otter
{

ImagePropertiesDialog::ImagePropertiesDialog(const QUrl &url, const QMap<ImageProperty, QVariant> &properties, QIODevice *device, QWidget *parent) : Dialog(parent),
	m_ui(new Ui::ImagePropertiesDialog)
{
	m_ui->setupUi(this);
	m_ui->addressLabelWidget->setText(url.toDisplayString());
	m_ui->addressLabelWidget->setUrl(url);
	m_ui->sizeLabelWidget->setText(tr("Unknown"));
	m_ui->typeLabelWidget->setText(tr("Unknown"));
	m_ui->fileSizeLabelWidget->setText(tr("Unknown"));
	m_ui->alternativeTextLabelWidget->setText(properties.value(AlternativeTextProperty).toString());
	m_ui->longDescriptionLabelWidget->setText(properties.value(LongDescriptionProperty).toString());

	if (!device && url.scheme() == QLatin1String("data"))
	{
		const QString imageData(url.path());
		QBuffer *buffer(new QBuffer(this));
		buffer->setData(QByteArray::fromBase64(imageData.mid(imageData.indexOf(QLatin1String("base64,")) + 7).toUtf8()));

		device = buffer;
	}

	QSize size(properties.contains(SizeProperty) ? properties[SizeProperty].toSize() : QSize());
	int depth(properties.contains(DepthProperty) ? properties[DepthProperty].toInt() : -1);
	int frames(1);

	if (device)
	{
		QImageReader reader(device);

		frames = reader.imageCount();

		const QImage image(reader.read());

		size = image.size();

		depth = image.depth();

		device->reset();

		m_ui->fileSizeLabelWidget->setText(Utils::formatUnit(device->size(), false, 2, true));
		m_ui->typeLabelWidget->setText(QMimeDatabase().mimeTypeForData(device).comment());

		device->deleteLater();
	}

	if (size.isValid())
	{
		if (depth > 0)
		{
			if (frames > 1)
			{
				m_ui->sizeLabelWidget->setText(tr("%1 x %2 pixels @ %3 bits per pixel in %n frame(s)", "", frames).arg(size.width()).arg(size.height()).arg(depth));
			}
			else
			{
				m_ui->sizeLabelWidget->setText(tr("%1 x %2 pixels @ %3 bits per pixel").arg(size.width()).arg(size.height()).arg(depth));
			}
		}
		else
		{
			m_ui->sizeLabelWidget->setText(tr("%1 x %2 pixels").arg(size.width()).arg(size.height()));
		}
	}

	setMinimumWidth(400);
}

ImagePropertiesDialog::~ImagePropertiesDialog()
{
	delete m_ui;
}

void ImagePropertiesDialog::changeEvent(QEvent *event)
{
	QDialog::changeEvent(event);

	if (event->type() == QEvent::LanguageChange)
	{
		m_ui->retranslateUi(this);
	}
}

void ImagePropertiesDialog::setButtonsVisible(bool visible)
{
	m_ui->buttonBox->setVisible(visible);
}

}
