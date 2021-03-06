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

#ifndef GENERAL_H
#define GENERAL_H

#include <QString>
#include <QDate>
#include <QHash>

namespace DocScan
{
typedef struct {
    QString name;
    QHash<QString, QString> attributes;
    QString text;
} XMLNode;

/**
 * Serialize a XMLNode struct into plain XML text.
 * @param node XMLNode struct to serialize
 * @return serialized text
 */
QString xmlNodeToText(const XMLNode &node);

/**
 * Make a text XML-safe by rewriting critical symbols such as <, & or, >.
 *
 * @param text plain text
 * @return text encoded as XML
 */
QString xmlify(const QString &text);

/**
 * Make a text XML-safe by rewriting critical symbols such as <, & or, >.
 * In constrast to @see xmlify, it will keep line breaks
 *
 * @param text plain text
 * @return text encoded as XML
 */
QString xmlifyLines(const QString &text);

/**
 * Rewrite XML encodings like &aml; back to plain text like &.
 *
 * @param xml text encoded as XML
 * @return decoded plain text
 */
QString dexmlify(const QString &xml);

QString removeBinaryGarbage(const QString &xml);
QByteArray removeBinaryGarbage(const QByteArray &data);

/**
 * Create an XML representation like
 * '<date epoch="1317333600" base="creation" year="2011" month="9" day="30">2011-09-30</date>'
 * for a given date.
 *
 * @param date date to encode
 * @param base value for the 'base' attribte
 * @return XML representation
 */
QString formatDate(const QDate date, const QString &base);
QString formatDateTime(const QDateTime datetime, const QString &base);

QString formatMap(const QString &key, const QHash<QString, QString> &attrs);

/**
 * Guess mimetype based on filename (most often its extension).
 * For example, filenames 'test.html' or 'test.htm' shall return
 * 'text/html'.
 * If guessing fails, an empty string will be returned.
 *
 * @param filename filemae to based the guessing on
 * @return guessed mimetype or empty string
 */
QString guessMimetype(const QString &filename);

QString extensionForMimetype(const QString &mimetype);
}

#endif // GENERAL_H
