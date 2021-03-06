/*
    This file is part of DocScan.

    DocScan is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    DocScan is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with DocScan.  If not, see <https://www.gnu.org/licenses/>.


    Copyright (2017) Thomas Fischer <thomas.fischer@his.se>, senior
    lecturer at University of Skövde, as part of the LIM-IT project.

 */

#include "general.h"

#include <QRegExp>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QStringList>
#include <QDebug>

namespace DocScan
{

QString xmlNodeToText(const XMLNode &node)
{
    QString result = QString(QChar('<')).append(node.name);
    QStringList attributes = node.attributes.keys();
    attributes.sort();
    for (QStringList::ConstIterator it = attributes.constBegin(); it != attributes.constEnd(); ++it)
        result.append(QString(QStringLiteral(" %1=\"%2\"")).arg(*it, xmlify(node.attributes[*it])));

    if (node.text.isEmpty())
        result.append(QStringLiteral(" />\n"));
    else {
        result.append(QStringLiteral(">\n"));
        result.append(node.text);
        result.append(QString(QStringLiteral("</%1>\n")).arg(node.name));
    }

    return result;
}

QString xmlify(const QString &text)
{
    QString result = text;
    result = result.replace(QChar(0x0009) /** horizontal tab */, QChar(0x0020)).replace(QChar(0x000a) /** line feed */, QChar(0x0020)).replace(QChar(0x000d) /** cardridge return */, QChar(0x0020)).trimmed();
    result = result.remove(QRegExp(QStringLiteral("[^-^[]'a-z0-9,;.:_+\\}{@|* !\"#%&/()=?åäöü]"), Qt::CaseInsensitive));

    /// remove unprintable or control characters
    for (int i = 0; i < result.length(); ++i)
        if (result[i].unicode() < 0x0020 || result[i].unicode() >= 0x0100 || !result[i].isPrint()) {
            result = result.left(i) + result.mid(i + 1);
            --i;
        }

    result = result.replace(QChar('&'), QStringLiteral("&amp;"));
    result = result.replace(QChar('<'), QStringLiteral("&lt;")).replace(QChar('>'), QStringLiteral("&gt;"));
    result = result.replace(QChar('"'), QStringLiteral("&quot;")).replace(QChar('\''), QStringLiteral("&apos;"));
    result = result.simplified();
    return result;
}

QStringList splitLines(QString input)
{
    QStringList result;
    QTextStream ts(&input, QIODevice::ReadOnly);
    while (!ts.atEnd())
        result << ts.readLine();
    return result;
}

QString xmlifyLines(const QString &text) {
    const QStringList lines = splitLines(text);
    QString result;
    for (const QString &line : lines) {
        if (!result.isEmpty())
            result.append(QChar('\n'));
        result.append(xmlify(line));
    }
    return result;
}

QString dexmlify(const QString &xml)
{
    QString result = xml;
    result = result.replace(QStringLiteral("&lt;"), QChar('<')).replace(QStringLiteral("&gt;"), QChar('>'));
    result = result.replace(QStringLiteral("&quot;"), QChar('"'));
    result = result.replace(QStringLiteral("&apos;"), QChar('\''));
    result = result.replace(QStringLiteral("&amp;"), QChar('&'));
    return result;
}

QString removeBinaryGarbage(const QString &xml)
{
    QString result;
    for (const QChar &c : xml) {
        if (c.unicode() == 0x00ad) /// soft hyphen
            result.append(QChar('-'));
        else if (c.unicode() == 9 || c.unicode() == 10 || c.unicode() == 13 || c.unicode() == 38 || c.unicode() == 59 || c.unicode() == 60 || c.unicode() == 62 || (c.unicode() >= 0x20 && !c.isNonCharacter() && !c.isNull() && c.isPrint()))
            result.append(c);
        else
            qDebug() << "Found garbage character with unicode number " << c.unicode();
    }
    return result;
}

QByteArray removeBinaryGarbage(const QByteArray &data)
{
    QByteArray result; result.reserve(data.size());
    for (int i = 0; i < data.size(); ++i) {
        const unsigned char b = data[i];
        if ((b & 128) == 0) {
            /// Plain ASCII, most-significant bit not set
            if (b >= 32 || b == 9 || b == 10 || b == 13) {
                /// Skip lower ASCII characters except for tab, line break, and cardrige return
                result.append(b);
            }
        } else if ((b & 224) == 192 && i < data.size() - 1 && (data[i + 1] & 192) == 128) {
            /// Two-byte UTF-8 data
            result.append(b);
            ++i;
            result.append(data[i]);
        } else if ((b & 240) == 224 && i < data.size() - 2 && (data[i + 1] & 192) == 128 && (data[i + 2] & 192) == 128) {
            /// Three-byte UTF-8 data

            /// Skipping those sequences, most likely garbage
            i += 2;
            /*
            result.append(b);
            ++i;
            result.append(data[i]);
            ++i;
            result.append(data[i]);
            */
        } else if ((b & 248) == 240 && i < data.size() - 3 && (data[i + 1] & 192) == 128 && (data[i + 2] & 192) == 128 && (data[i + 3] & 192) == 128) {
            /// Four-byte UTF-8 data

            /// Skipping those sequences, most likely garbage
            i += 3;
            /*
            result.append(b);
            ++i;
            result.append(data[i]);
            ++i;
            result.append(data[i]);
            ++i;
            result.append(data[i]);
            */
        } else
            qWarning() << "Removing invalid byte from QByteArray";
    }
    return result;
}

QString formatDate(const QDate date, const QString &base)
{
    return QString(QStringLiteral("<date epoch=\"%6\"%5 day=\"%3\" month=\"%2\" year=\"%1\">%4</date>\n")).arg(date.year()).arg(date.month()).arg(date.day()).arg(date.toString(Qt::ISODate), base.isEmpty() ? QString() : QStringLiteral(" base=\"") + base + QStringLiteral("\"")).arg(QDateTime(date).toTime_t());
}

QString formatDateTime(const QDateTime datetime, const QString &base) {
    return QString(QStringLiteral("<date epoch=\"%6\"%5 day=\"%3\" month=\"%2\" year=\"%1\" hour=\"%7\" minute=\"%8\" second=\"%9\">%4</date>\n")).arg(datetime.date().year()).arg(datetime.date().month()).arg(datetime.date().day()).arg(datetime.toString(Qt::ISODate), base.isEmpty() ? QString() : QStringLiteral(" base=\"") + base + QStringLiteral("\"")).arg(datetime.toTime_t()).arg(datetime.time().hour()).arg(datetime.time().minute()).arg(datetime.time().second());
}

QString formatMap(const QString &key, const QHash<QString, QString> &attrs)
{
    if (attrs.isEmpty()) return QStringLiteral("");

    const QString body = DocScan::xmlify(attrs[QStringLiteral("")]);
    QString result = QString(QStringLiteral("<%1")).arg(key);
    for (QHash<QString, QString>::ConstIterator it = attrs.constBegin(); it != attrs.constEnd(); ++it)
        if (!it.key().isEmpty())
            result.append(QString(QStringLiteral(" %1=\"%2\"")).arg(it.key(), DocScan::xmlify(it.value())));

    if (body.isEmpty())
        result.append(" />\n");
    else
        result.append(">").append(body).append(QString(QStringLiteral("</%1>\n")).arg(key));

    return result;
}

QString guessMimetype(const QString &filename) {
    if (filename.isEmpty())
        return QString();

    const QString lowerFilename = filename.toLower();
    static const QRegularExpression fileExtensionRegExp(QStringLiteral("\\.[a-z0-9]{1,4}$"));
    const QString fileExtension = fileExtensionRegExp.match(lowerFilename).captured(0);

    if (fileExtension.isEmpty()) {
        qWarning() << "Filename is empty";
        return QString();
    } else if (fileExtension == QStringLiteral(".htm") || fileExtension == QStringLiteral(".html"))
        return QStringLiteral("text/html");
    else if (fileExtension == QStringLiteral(".pdf"))
        return QStringLiteral("application/pdf");
    else if (fileExtension == QStringLiteral(".xml"))
        return QStringLiteral("application/xml");
    else if (fileExtension == QStringLiteral(".jpg") || fileExtension == QStringLiteral(".jpeg") || fileExtension == QStringLiteral(".jpe") || fileExtension == QStringLiteral(".jfif"))
        return QStringLiteral("image/jpeg");
    else if (fileExtension == QStringLiteral(".jp2") || fileExtension == QStringLiteral(".jpf") || fileExtension == QStringLiteral(".jpx"))
        return QStringLiteral("image/jp2");
    else if (fileExtension == QStringLiteral(".png"))
        return QStringLiteral("image/png");
    else if (fileExtension == QStringLiteral(".tif") || fileExtension == QStringLiteral(".tiff"))
        return QStringLiteral("image/tiff");
    else {
        qWarning() << "Cannot guess mimetype for filename " << filename;
        return QString();
    }
}

QString extensionForMimetype(const QString &mimetype) {
    if (mimetype.isEmpty()) {
        qWarning() << "Mimetype is empty";
        return QStringLiteral(".data");
    } else if (mimetype == QStringLiteral("text/html"))
        return QStringLiteral(".html");
    else if (mimetype == QStringLiteral("application/pdf"))
        return QStringLiteral(".pdf");
    else if (mimetype == QStringLiteral("application/xml"))
        return QStringLiteral(".xml");
    else if (mimetype == QStringLiteral("image/jpeg"))
        return QStringLiteral(".jpeg");
    else if (mimetype == QStringLiteral("image/jp2"))
        return QStringLiteral(".jp2");
    else if (mimetype == QStringLiteral("image/png"))
        return QStringLiteral(".png");
    else if (mimetype == QStringLiteral("image/tiff"))
        return QStringLiteral(".tiff");
    else {
        qWarning() << "Don't know file extension for mimetype" << mimetype;
        return QStringLiteral(".data");
    }
}

} // end of namespace
