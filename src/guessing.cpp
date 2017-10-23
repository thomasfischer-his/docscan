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

#include "guessing.h"

#include <QStringList>
#include <QHash>
#include <QVector>
#include <QRegExp>
#include <QRegularExpression>

#include "general.h"

Guessing::Guessing()
{
    /// nothing
}

QString Guessing::fontToXML(const QString &fontName, const QString &typeName)
{
    QHash<QString, QString> name, beautifiedName, license, technology;
    name[QStringLiteral("")] = fontName;
    license[QStringLiteral("type")] = QStringLiteral("unknown"); ///< default: license type is unknown

    const QString cleanedFontName = QString(fontName).remove(QStringLiteral("#20")).remove(QStringLiteral(".")).remove(QStringLiteral(","));

    /**
     * In Adobe's PDF 1.7 specifications, section 9.6.4, it says:
     * "For a font subset, the PostScript name of the font---the value of the font's BaseFont entry and
     * the font descriptor's FontName entry---shall begin with a tag followed by a plus sign (+).
     * The tag shall consist of exactly six uppercase letters; the choice of letters is arbitrary,
     * but different subsets in the same PDF file shall have different tags.
     */
    const QString fontNameWithoutTag = QString(cleanedFontName).remove(QRegularExpression(QStringLiteral("^[A-Z]{6}[+][/]?")));
    const QString fontNameLower = fontNameWithoutTag.toLower();

    if (fontNameWithoutTag.startsWith(QStringLiteral("Libertine"))) {
        license[QStringLiteral("type")] = QStringLiteral("open");
        license[QStringLiteral("name")] = QStringLiteral("SIL Open Font License;GNU General Public License");
    } else if (fontNameWithoutTag.startsWith(QStringLiteral("Nimbus")) || fontNameWithoutTag.startsWith(QStringLiteral("Dingbats")) || fontNameWithoutTag.startsWith(QStringLiteral("URW Chancery")) || fontNameWithoutTag.startsWith(QStringLiteral("URWChancery")) || fontNameWithoutTag.startsWith(QStringLiteral("URW Palladio")) || fontNameWithoutTag.startsWith(QStringLiteral("URWPalladio")) || fontNameWithoutTag.startsWith(QStringLiteral("URW Bookman")) || fontNameWithoutTag.startsWith(QStringLiteral("URWBookman")) || fontNameWithoutTag.startsWith(QStringLiteral("URW Gothic")) || fontNameWithoutTag.startsWith(QStringLiteral("URWGothic")) || fontNameWithoutTag.startsWith(QStringLiteral("Standard Symbols L")) || fontNameWithoutTag.startsWith(QStringLiteral("StandardSymbolsL")) || fontNameWithoutTag.startsWith(QStringLiteral("StandardSymL"))) {
        license[QStringLiteral("type")] = QStringLiteral("open"); /// URW++, released as GPL and AFPL
        license[QStringLiteral("name")] = QStringLiteral("GNU General Public License;Aladdin Free Public License");
    } else if (fontNameWithoutTag.startsWith(QStringLiteral("URWPalladio"))) {
        license[QStringLiteral("type")] = QStringLiteral("open"); /// URW++, released as GPL and AFPL
    } else if (fontNameWithoutTag.startsWith(QStringLiteral("Kp-")) || fontNameWithoutTag.contains(QStringLiteral("-Kp-"))) {
        license[QStringLiteral("type")] = QStringLiteral("open"); /// 'Johannes Kepler' font based on URW++'s Palladio, by Christophe Caignaert
    } else if (fontNameWithoutTag.startsWith(QStringLiteral("Liberation"))) {
        license[QStringLiteral("type")] = QStringLiteral("open");
        license[QStringLiteral("name")] = QStringLiteral("GNU General Public License v2 with Font Exception");
    } else if (fontNameWithoutTag.startsWith(QStringLiteral("DejaVu"))) {
        license[QStringLiteral("type")] = QStringLiteral("open");
    } else if (fontNameWithoutTag.startsWith(QStringLiteral("Ubuntu"))) {
        license[QStringLiteral("type")] = QStringLiteral("open");
        license[QStringLiteral("name")] = QStringLiteral("Ubuntu Font Licence");
    } else if (fontNameWithoutTag.startsWith(QStringLiteral("Junicode"))) {
        license[QStringLiteral("type")] = QStringLiteral("open");
        license[QStringLiteral("name")] = QStringLiteral("SIL Open Font License");
    } else if (fontNameWithoutTag.startsWith(QStringLiteral("Gentium")) || fontNameWithoutTag.startsWith(QStringLiteral("Artemisia"))) {
        license[QStringLiteral("type")] = QStringLiteral("open");
    } else if (fontNameWithoutTag.startsWith(QStringLiteral("FreeSans")) || fontNameWithoutTag.startsWith(QStringLiteral("FreeSerif")) || fontNameWithoutTag.startsWith(QStringLiteral("FreeMono"))) {
        license[QStringLiteral("type")] = QStringLiteral("open");
    } else if (fontNameWithoutTag.startsWith(QStringLiteral("Vera")) || fontNameWithoutTag.startsWith(QStringLiteral("Bera"))) {
        license[QStringLiteral("type")] = QStringLiteral("open");
    } else if (fontNameWithoutTag.startsWith(QStringLiteral("TeX-")) || fontNameWithoutTag.startsWith(QStringLiteral("TeXPa")) || fontNameWithoutTag.startsWith(QStringLiteral("PazoMath"))) { /// TeX fonts
        license[QStringLiteral("type")] = QStringLiteral("open"); /// most likely
    } else if (fontNameWithoutTag.endsWith(QStringLiteral("circle10"))) { /// TeX fonts: http://tug.ctan.org/info/fontname/texfonts.map
        license[QStringLiteral("type")] = QStringLiteral("open"); /// most likely
    } else if (fontNameWithoutTag.startsWith(QStringLiteral("Old Standard")) || fontNameWithoutTag.startsWith(QStringLiteral("OldStandard"))) {
        /// https://fontlibrary.org/en/font/old-standard
        license[QStringLiteral("type")] = QStringLiteral("open");
        license[QStringLiteral("name")] = QStringLiteral("SIL Open Font License");
    } else if (fontNameWithoutTag.startsWith(QStringLiteral("Computer Modern"))) {
        license[QStringLiteral("type")] = QStringLiteral("open");
        license[QStringLiteral("name")] = QStringLiteral("SIL Open Font License");
    } else if (fontNameWithoutTag.startsWith(QStringLiteral("CMUSerif")) || fontNameLower.startsWith(QStringLiteral("cmr")) || fontNameLower.startsWith(QStringLiteral("cmmi")) || fontNameLower.startsWith(QStringLiteral("cmsy")) || fontNameLower.startsWith(QStringLiteral("stmary")) || fontNameLower.startsWith(QStringLiteral("wasy")) || fontNameWithoutTag.contains(QRegularExpression(QStringLiteral("(^|_)([Cc]mr|[Cc]mmi|[Cc]msy|EUSM|EURM|EUFM|EUEX|CM|SF|MS)[A-Z0-9]+(~[0-9a-f]+)?$")))) { /// TeX fonts
        license[QStringLiteral("type")] = QStringLiteral("open");
        license[QStringLiteral("name")] = QStringLiteral("SIL Open Font License");
    } else if (fontNameWithoutTag.startsWith(QStringLiteral("Marvosym"))) {
        license[QStringLiteral("type")] = QStringLiteral("open");
        license[QStringLiteral("name")] = QStringLiteral("GUST Font License (GFL);LaTeX Project Public License (LPPL)");
    } else if (fontNameWithoutTag.startsWith(QStringLiteral("LMSans")) || fontNameWithoutTag.startsWith(QStringLiteral("LMRoman")) || fontNameWithoutTag.startsWith(QStringLiteral("LMSlanted")) || fontNameWithoutTag.startsWith(QStringLiteral("LMTypewriter")) || fontNameWithoutTag.startsWith(QStringLiteral("LMMath")) || fontNameWithoutTag.startsWith(QStringLiteral("LMMono"))) {
        license[QStringLiteral("type")] = QStringLiteral("open");
        license[QStringLiteral("name")] = QStringLiteral("SIL Open Font License");
    } else if (fontNameWithoutTag.startsWith(QStringLiteral("Helena")) || fontNameWithoutTag.startsWith(QStringLiteral("Peshitta")) || fontNameWithoutTag.startsWith(QStringLiteral("Rosetta")) || fontNameWithoutTag.startsWith(QStringLiteral("Salaam")) || fontNameWithoutTag.startsWith(QStringLiteral("Sylvanus")) || fontNameWithoutTag.startsWith(QStringLiteral("Yehudit"))) {
        license[QStringLiteral("type")] = QStringLiteral("open"); /// Accordance Font
        license[QStringLiteral("name")] = QStringLiteral("SIL Open Font License"); /// https://www.accordancebible.com/Accordance-Font
    } else if (fontNameWithoutTag.startsWith(QStringLiteral("STIX"))) {
        license[QStringLiteral("type")] = QStringLiteral("open");
        license[QStringLiteral("name")] = QStringLiteral("SIL Open Font License");
    } else if (fontNameWithoutTag.startsWith(QStringLiteral("OpenSymbol"))) {
        license[QStringLiteral("type")] = QStringLiteral("open");
        license[QStringLiteral("name")] = QStringLiteral("LGPLv3?");
    } else if (fontNameWithoutTag.startsWith(QStringLiteral("MnSymbol"))) {
        license[QStringLiteral("type")] = QStringLiteral("open");
        license[QStringLiteral("name")] = QStringLiteral("PD");
    } else if (fontNameWithoutTag.startsWith(QStringLiteral("Arimo")) || fontNameWithoutTag.startsWith(QStringLiteral("Tinos"))) {
        /// https://en.wikipedia.org/wiki/Croscore_fonts
        license[QStringLiteral("type")] = QStringLiteral("open");
        license[QStringLiteral("name")] = QStringLiteral("Apache 2.0");
    } else if (fontNameWithoutTag.startsWith(QStringLiteral("DroidSans")) || fontNameWithoutTag.startsWith(QStringLiteral("DroidSerif")) || fontNameWithoutTag.startsWith(QStringLiteral("DroidMono"))) {
        /// https://en.wikipedia.org/wiki/Droid_fonts
        license[QStringLiteral("type")] = QStringLiteral("open");
        license[QStringLiteral("name")] = QStringLiteral("Apache 2.0");
    } else if (fontNameWithoutTag.startsWith(QStringLiteral("Menlo"))) {
        license[QStringLiteral("type")] = QStringLiteral("open"); /// Apple's new font based on Bitstream's Vera
    } else if (fontNameWithoutTag.startsWith(QStringLiteral("Apple"))) {
        license[QStringLiteral("type")] = QStringLiteral("proprietary"); /// Most likely some Apple font
    } else if (fontNameWithoutTag.startsWith(QStringLiteral("Monaco")) || fontNameWithoutTag.startsWith(QStringLiteral("Geneva"))) {
        license[QStringLiteral("type")] = QStringLiteral("proprietary"); /// Apple
    } else if (fontNameWithoutTag.startsWith(QStringLiteral("Chantilly")) || fontNameWithoutTag.startsWith(QStringLiteral("BibleScr"))) {
        license[QStringLiteral("type")] = QStringLiteral("unknown"); /// unknown origin, but seems to be free for download
    } else if (fontNameWithoutTag.startsWith(QStringLiteral("Bohemian-typewriter"))) {
        license[QStringLiteral("type")] = QStringLiteral("unknown"); /// free for non-commercial/personal use: http://www.dafont.com/bohemian-typewriter.font
    } else if (fontNameWithoutTag.startsWith(QStringLiteral("Altera"))) {
        license[QStringLiteral("type")] = QStringLiteral("unknown"); /// Jacob King; free for personal use: http://www.jacobking.org/
    } else if (fontNameWithoutTag.startsWith(QStringLiteral("Antenna"))) {
        license[QStringLiteral("type")] = QStringLiteral("proprietary"); /// Font Bureau
    } else if (fontNameWithoutTag.startsWith(QStringLiteral("AkzidenzGrotesk")) || fontNameWithoutTag.startsWith(QStringLiteral("Akzidenz Grotesk"))) {
        /// https://en.wikipedia.org/wiki/Akzidenz-Grotesk
        license[QStringLiteral("type")] = QStringLiteral("proprietary"); /// Akzidenz-Grotesk by H. Berthold AG
    } else if (fontNameWithoutTag.startsWith(QStringLiteral("URWGrotesk"))) {
        /// https://www.myfonts.com/fonts/urw/grotesk/
        license[QStringLiteral("type")] = QStringLiteral("proprietary"); /// 'URW Grotesk was designed exclusively for URW by Prof. Hermann Zapf in 1985'
    } else if (fontNameWithoutTag.startsWith(QStringLiteral("VladimirScript"))) {
        license[QStringLiteral("type")] = QStringLiteral("proprietary"); /// URW
    } else if (fontNameWithoutTag.startsWith(QStringLiteral("Paperback"))) {
        license[QStringLiteral("type")] = QStringLiteral("proprietary"); /// House Industries
    } else if (fontNameWithoutTag.startsWith(QStringLiteral("Formata"))) {
        license[QStringLiteral("type")] = QStringLiteral("proprietary"); /// Berthold
    } else if (fontNameWithoutTag.startsWith(QStringLiteral("Whitney"))) {
        license[QStringLiteral("type")] = QStringLiteral("proprietary"); /// Hoefler & Frere-Jones
    } else if (fontNameWithoutTag.startsWith(QStringLiteral("DTL"))) {
        license[QStringLiteral("type")] = QStringLiteral("proprietary"); /// Dutch Type Library
    } else if (fontNameWithoutTag.startsWith(QStringLiteral("Meta")) && (fontNameWithoutTag.contains(QStringLiteral("Book")) || fontNameWithoutTag.contains(QStringLiteral("Black")) || fontNameWithoutTag.contains(QStringLiteral("Bold")) || fontNameWithoutTag.contains(QStringLiteral("Normal")) || fontNameWithoutTag.contains(QStringLiteral("Medium")))) {
        license[QStringLiteral("type")] = QStringLiteral("proprietary"); /// FontFont (FF)
    } else if (fontNameWithoutTag.startsWith(QStringLiteral("DIN")) || fontNameWithoutTag.startsWith(QStringLiteral("Sari"))) {
        license[QStringLiteral("type")] = QStringLiteral("proprietary"); /// FF?
    } else if (fontNameWithoutTag.startsWith(QStringLiteral("Gotham")) || fontNameWithoutTag.startsWith(QStringLiteral("NewLibrisSerif"))) {
        license[QStringLiteral("type")] = QStringLiteral("proprietary"); /// fonts used by Forsakringskassan
    } else if (fontNameWithoutTag.startsWith(QStringLiteral("Zapf")) || fontNameWithoutTag.startsWith(QStringLiteral("Frutiger"))) {
        license[QStringLiteral("type")] = QStringLiteral("proprietary");
    } else if (fontNameWithoutTag.startsWith(QStringLiteral("Brandon"))) {
        license[QStringLiteral("type")] = QStringLiteral("proprietary"); /// HvD
    } else if (fontNameWithoutTag.startsWith(QStringLiteral("Staat"))) {
        license[QStringLiteral("type")] = QStringLiteral("proprietary"); /// Timo Kuilder
    } else if (fontNameWithoutTag.startsWith(QStringLiteral("Gulliver"))) {
        license[QStringLiteral("type")] = QStringLiteral("proprietary"); /// Gerard Unger
    } else if (fontNameWithoutTag.startsWith(QStringLiteral("BigCaslon"))) {
        license[QStringLiteral("type")] = QStringLiteral("proprietary"); /// Carter & Cone
    } else if (fontNameWithoutTag.startsWith(QStringLiteral("Publico")) || fontNameWithoutTag.startsWith(QStringLiteral("Guardian Egyp")) || fontNameWithoutTag.startsWith(QStringLiteral("GuardianEgyp"))) {
        license[QStringLiteral("type")] = QStringLiteral("proprietary"); /// Paul Barnes and Christian Schwartz
    } else if (fontNameWithoutTag.startsWith(QStringLiteral("ESRI"))) {
        license[QStringLiteral("type")] = QStringLiteral("proprietary"); /// ESRI (maybe free-as-in-beer fonts?)
    } else if (fontNameWithoutTag.startsWith(QStringLiteral("Interstate"))) {
        license[QStringLiteral("type")] = QStringLiteral("proprietary"); /// Font Bureau
    } else if (fontNameWithoutTag.startsWith(QStringLiteral("BaskOldFace")) || fontNameWithoutTag.startsWith(QStringLiteral("Baskerville"))) {
        license[QStringLiteral("type")] = QStringLiteral("proprietary"); /// Microsoft, URW, and Stephenson Blake
    } else if (fontNameWithoutTag.startsWith(QStringLiteral("Microsoft")) || fontNameWithoutTag.startsWith(QStringLiteral("MS-"))) {
        license[QStringLiteral("type")] = QStringLiteral("proprietary"); /// Microsoft
    } else if (fontNameWithoutTag.startsWith(QStringLiteral("SegoeUI")) || fontNameWithoutTag.startsWith(QStringLiteral("Segoe UI")) || fontNameWithoutTag.startsWith(QStringLiteral("SegoeScript"))) {
        license[QStringLiteral("type")] = QStringLiteral("proprietary"); /// Microsoft
    } else if (fontNameWithoutTag.contains(QStringLiteral("PGothic"))) {
        /// https://www.microsoft.com/typography/fonts/family.aspx?FID=335: 'MS PGothic is a Japanese font with proportional latin in the gothic (sans serif) style'
        license[QStringLiteral("type")] = QStringLiteral("proprietary"); /// Microsoft
    } else if (fontNameLower.startsWith(QStringLiteral("marlett")) || fontNameLower.startsWith(QStringLiteral("impact")) || fontNameLower.startsWith(QStringLiteral("comic sans")) || fontNameLower.startsWith(QStringLiteral("comicsans")) || fontNameLower.startsWith(QStringLiteral("webdings")) || fontNameLower.startsWith(QStringLiteral("windings")) || fontNameWithoutTag.startsWith(QStringLiteral("Arial")) || fontNameLower.startsWith(QStringLiteral("verdana")) || fontNameLower.startsWith(QStringLiteral("timesnewroman")) || fontNameLower.startsWith(QStringLiteral("times new roman")) || fontNameLower.startsWith(QStringLiteral("couriernew")) || fontNameLower.startsWith(QStringLiteral("courier new")) || fontNameLower.startsWith(QStringLiteral("georgia")) || fontNameLower == QStringLiteral("symbol") || fontNameWithoutTag.startsWith(QStringLiteral("MT-Extra")) || fontNameWithoutTag.startsWith(QStringLiteral("MTExtra"))) {
        license[QStringLiteral("type")] = QStringLiteral("proprietary"); /// Microsoft
    } else if (fontNameWithoutTag.startsWith(QStringLiteral("FrenchScript")) || fontNameWithoutTag.startsWith(QStringLiteral("French Script")) || fontNameWithoutTag.startsWith(QStringLiteral("Aldhabi")) || fontNameWithoutTag.startsWith(QStringLiteral("Mead")) || fontNameWithoutTag.startsWith(QStringLiteral("Batang")) || fontNameWithoutTag.startsWith(QStringLiteral("Shruti")) || fontNameWithoutTag.startsWith(QStringLiteral("Nyala")) || fontNameWithoutTag.startsWith(QStringLiteral("Sylfaen")) || fontNameWithoutTag.startsWith(QStringLiteral("BookAntiqua")) || fontNameWithoutTag.startsWith(QStringLiteral("Lucinda")) || fontNameWithoutTag.startsWith(QStringLiteral("Trebuchet")) || fontNameWithoutTag.startsWith(QStringLiteral("Franklin Gothic")) || fontNameWithoutTag.startsWith(QStringLiteral("FranklinGothic")) || fontNameWithoutTag.startsWith(QStringLiteral("Century Schoolbook")) || fontNameWithoutTag.startsWith(QStringLiteral("CenturySchoolbook"))) {
        license[QStringLiteral("type")] = QStringLiteral("proprietary"); /// Microsoft
    } else if (fontNameWithoutTag.startsWith(QStringLiteral("MS Mincho")) || fontNameWithoutTag.startsWith(QStringLiteral("MS-Mincho")) || fontNameWithoutTag.startsWith(QStringLiteral("MSMincho"))) {
        license[QStringLiteral("type")] = QStringLiteral("proprietary"); /// Microsoft/Ricoh/Ryobi Imagix
    } else if (fontNameWithoutTag.startsWith(QStringLiteral("SimSun"))) {
        license[QStringLiteral("type")] = QStringLiteral("proprietary"); /// Microsoft/ZHONGYI
    } else if (fontNameLower.startsWith(QStringLiteral("calibri")) || fontNameLower.startsWith(QStringLiteral("cambria")) || fontNameWithoutTag.startsWith(QStringLiteral("Constantia")) || fontNameWithoutTag.startsWith(QStringLiteral("Candara")) || fontNameWithoutTag.startsWith(QStringLiteral("Corbel")) || fontNameWithoutTag.startsWith(QStringLiteral("Consolas"))) {
        license[QStringLiteral("type")] = QStringLiteral("proprietary"); /// Microsoft ClearType Font Collection
    } else if (fontNameWithoutTag.startsWith(QStringLiteral("Caslon3")) || fontNameWithoutTag.startsWith(QStringLiteral("Caslon 3")) || fontNameWithoutTag.startsWith(QStringLiteral("Papyrus")) || fontNameWithoutTag.startsWith(QStringLiteral("Avenir")) || fontNameWithoutTag.startsWith(QStringLiteral("Plantin")) || fontNameWithoutTag.startsWith(QStringLiteral("MathematicalPi")) || fontNameWithoutTag.startsWith(QStringLiteral("ClearfaceGothic")) || fontNameWithoutTag.startsWith(QStringLiteral("Berling")) /* there may be an URW++ variant */ || fontNameWithoutTag.startsWith(QStringLiteral("Granjon")) || fontNameWithoutTag.startsWith(QStringLiteral("Sabon")) || fontNameWithoutTag.startsWith(QStringLiteral("Folio")) || fontNameWithoutTag.startsWith(QStringLiteral("Futura")) || fontNameWithoutTag.startsWith(QStringLiteral("Soho")) || fontNameWithoutTag.startsWith(QStringLiteral("Eurostile")) || fontNameWithoutTag.startsWith(QStringLiteral("NewCenturySchlbk")) || fontNameWithoutTag.startsWith(QStringLiteral("TradeGothic")) || fontNameLower.startsWith(QStringLiteral("univers")) || fontNameLower.contains(QStringLiteral("palatino"))) {
        license[QStringLiteral("type")] = QStringLiteral("proprietary"); /// Linotype
    } else if (fontNameWithoutTag.endsWith(QStringLiteral("BT")) || fontNameWithoutTag.startsWith(QStringLiteral("BT-"))) {
        license[QStringLiteral("type")] = QStringLiteral("proprietary"); /// Bitstream
    } else if (fontNameWithoutTag.startsWith(QStringLiteral("CharterBT")) || fontNameWithoutTag.startsWith(QStringLiteral("Monospace821")) || fontNameWithoutTag.startsWith(QStringLiteral("Swiss721")) || fontNameWithoutTag.startsWith(QStringLiteral("Humanist777")) || fontNameWithoutTag.startsWith(QStringLiteral("Dutch801")) || fontNameWithoutTag.startsWith(QStringLiteral("Zurich"))) {
        license[QStringLiteral("type")] = QStringLiteral("proprietary"); /// Bitstream
    } else if (fontNameWithoutTag.startsWith(QStringLiteral("BrushScript"))) {
        license[QStringLiteral("type")] = QStringLiteral("proprietary"); /// Bitstream
    } else if (fontNameWithoutTag.startsWith(QStringLiteral("Mathematica"))) {
        license[QStringLiteral("type")] = QStringLiteral("proprietary"); /// Wolfram
    } else if (fontNameWithoutTag.startsWith(QStringLiteral("Helvetica")) && fontNameWithoutTag.contains(QStringLiteral("Neue"))) {
        license[QStringLiteral("type")] = QStringLiteral("proprietary"); /// Neue Helvetica by Linotype
    } else if (fontNameWithoutTag.startsWith(QStringLiteral("Tahoma")) || fontNameWithoutTag.startsWith(QStringLiteral("Helvetica")) || fontNameWithoutTag.startsWith(QStringLiteral("Wingdings"))) {
        license[QStringLiteral("type")] = QStringLiteral("proprietary");
    } else if (fontNameWithoutTag.startsWith(QStringLiteral("Bookman-")) /* not "BookmanOldStyle"? */ || fontNameWithoutTag.startsWith(QStringLiteral("SymbolMT")) || fontNameWithoutTag.startsWith(QStringLiteral("GillAltOneMT"))) {
        license[QStringLiteral("type")] = QStringLiteral("proprietary"); /// MonoType's font as shipped with Windows
    } else if (fontNameWithoutTag.startsWith(QStringLiteral("Ehrhardt")) || fontNameWithoutTag.startsWith(QStringLiteral("Forte")) || fontNameWithoutTag.startsWith(QStringLiteral("Monotype")) || fontNameWithoutTag.startsWith(QStringLiteral("MundoSans")) || fontNameWithoutTag.startsWith(QStringLiteral("OceanSans")) || fontNameLower.startsWith(QStringLiteral("bookmanoldstyle")) || fontNameLower.startsWith(QStringLiteral("centaur")) || fontNameLower.startsWith(QStringLiteral("calisto")) || fontNameLower.startsWith(QStringLiteral("centurygothic")) || fontNameLower.startsWith(QStringLiteral("bembo")) || fontNameWithoutTag.startsWith(QStringLiteral("GillSans")) ||  fontNameWithoutTag.startsWith(QStringLiteral("Rockwell")) || fontNameWithoutTag.startsWith(QStringLiteral("Lucida")) || fontNameWithoutTag.startsWith(QStringLiteral("Perpetua"))) {
        license[QStringLiteral("type")] = QStringLiteral("proprietary"); /// MonoType
    } else if (fontNameWithoutTag.startsWith(QStringLiteral("KunstlerScript")) || fontNameWithoutTag.startsWith(QStringLiteral("AmericanTypewriter")) || fontNameWithoutTag.startsWith(QStringLiteral("ACaslon")) || fontNameWithoutTag.startsWith(QStringLiteral("AdobeCaslon")) || fontNameWithoutTag.startsWith(QStringLiteral("AGaramond")) || fontNameWithoutTag.startsWith(QStringLiteral("GaramondPremrPro")) || fontNameWithoutTag.startsWith(QStringLiteral("EuroSans")) || fontNameWithoutTag.startsWith(QStringLiteral("Minion")) || fontNameWithoutTag.startsWith(QStringLiteral("Myriad"))) {
        license[QStringLiteral("type")] = QStringLiteral("proprietary"); /// Adobe
    } else if (fontNameWithoutTag.startsWith(QStringLiteral("Itc")) || fontNameWithoutTag.startsWith(QStringLiteral("ITC")) || fontNameWithoutTag.endsWith(QStringLiteral("ITC")) || fontNameWithoutTag.contains(QStringLiteral("ITC-")) || fontNameWithoutTag.startsWith(QStringLiteral("AvantGarde")) || fontNameWithoutTag.startsWith(QStringLiteral("Tiepolo")) || fontNameWithoutTag.startsWith(QStringLiteral("Officina")) || fontNameWithoutTag.startsWith(QStringLiteral("Kabel")) || fontNameWithoutTag.startsWith(QStringLiteral("Cheltenham"))) {
        license[QStringLiteral("type")] = QStringLiteral("proprietary"); /// ITC
    } else if (fontNameWithoutTag.startsWith(QStringLiteral("BellGothic"))) {
        license[QStringLiteral("type")] = QStringLiteral("proprietary"); /// Mergenthaler Linotype/AT&T
    } else if (fontNameWithoutTag.startsWith(QStringLiteral("StoneSans"))) {
        license[QStringLiteral("type")] = QStringLiteral("proprietary"); /// ITC or Linotype?
    } else if (fontNameWithoutTag.startsWith(QStringLiteral("TheSans"))) {
        license[QStringLiteral("type")] = QStringLiteral("proprietary"); /// Lucas
    } else if (fontNameWithoutTag.startsWith(QStringLiteral("UU-"))) {
        license[QStringLiteral("type")] = QStringLiteral("proprietary"); /// Commercial font used by University of Uppsala
    } else if (fontNameWithoutTag.endsWith(QStringLiteral("LiU"))) {
        license[QStringLiteral("type")] = QStringLiteral("proprietary"); /// Commercial font used by University of Linkoping
    } else if (fontNameWithoutTag.startsWith(QStringLiteral("Bookman Old Style")) || fontNameWithoutTag.startsWith(QStringLiteral("Gill Sans"))) {
        license[QStringLiteral("type")] = QStringLiteral("proprietary"); /// multiple alternatives
    } else
        license[QStringLiteral("type")] = QStringLiteral("unknown");
    /// The following font names are ambiguous:
    /// - Courier
    /// - Garamond
    /// - Franklin Gothic
    /// - Symbol
    /// More fonts?

    QString bName = fontNameWithoutTag;
    bName.remove(QStringLiteral("#20")).remove(QStringLiteral("\\s")).remove(QStringLiteral(".")).remove(QStringLiteral("_")).remove(QStringLiteral("/"));
    bool bNameChanged = true;
    while (bNameChanged) {
        const QString bNameOriginal = bName;
        bName = bName.trimmed();
        static const QStringList suffixes = QStringList()
                                            << QStringLiteral("BOLD") << QStringLiteral("ITALIC") << QStringLiteral(" Print") << QStringLiteral(" Extra")
                                            << QStringLiteral("-greek")
                                            << QStringLiteral("-regular") << QStringLiteral("-Regular") << QStringLiteral("-Regu") << QStringLiteral("-DisplayRegular")
                                            << QStringLiteral("-KursivHalbfett") << QStringLiteral("-Kursiv") << QStringLiteral("-Boo")
                                            << QStringLiteral("OsF") << QStringLiteral("PS") << QStringLiteral("BE") << QStringLiteral("BEIt") << QStringLiteral("PSMT") << QStringLiteral("MS") << QStringLiteral("SC") << QStringLiteral("LT") << QStringLiteral("LTCom") << QStringLiteral("MTLight") << QStringLiteral("LF") << QStringLiteral("-BT") << QStringLiteral("BT") << QStringLiteral("Bk")
                                            << QStringLiteral("-Normal") << QStringLiteral("-Book") << QStringLiteral("-Md") << QStringLiteral("-MdIt") << QStringLiteral("-MdCn") << QStringLiteral("-Plain") << QStringLiteral("-Medium") << QStringLiteral("-MediumCond") << QStringLiteral("-Medi") << QStringLiteral("-MediumItalic") << QStringLiteral("-Semi") <<  QStringLiteral("-Semibold") << QStringLiteral("-SmbdIt") << QStringLiteral("-SemiCn") << QStringLiteral("-SemiCnIt") << QStringLiteral("-SemiboldSemiCn") << QStringLiteral("-Caps") << QStringLiteral("-Roman") << QStringLiteral("-Roma")
                                            << QStringLiteral("-Demi") << QStringLiteral("-Blk") << QStringLiteral("-Black") << QStringLiteral("-BlackCond") << QStringLiteral("-BlackIt") << QStringLiteral("-Blac") << QStringLiteral("Bla") << QStringLiteral("-BlackSemiCn") << QStringLiteral("-BlackSemiCnIt") << QStringLiteral("-Ultra") << QStringLiteral("-Extra") << QStringLiteral("-ExtraBold") << QStringLiteral("Obl") << QStringLiteral("-Hv") << QStringLiteral("-HvIt") << QStringLiteral("-Heavy") << QStringLiteral("-HeavyCond") << QStringLiteral("-Heav") << QStringLiteral("-Subh") << QStringLiteral("-ItSubh") << QStringLiteral("-MediumItDisp") << QStringLiteral("-MediumDisp") << QStringLiteral("-SemiboldIt") << QStringLiteral("-Capt") << QStringLiteral("-CnIt") << QStringLiteral("-BoldIt") << QStringLiteral("-BoldDisp") << QStringLiteral("-BoldSubh") << QStringLiteral("-BoldCn") << QStringLiteral("-BoldCond") << QStringLiteral("-BoldCondIt") << QStringLiteral("-BoldItal") << QStringLiteral("-BoldItalicB") << QStringLiteral("-BdIt") << QStringLiteral("-Bd") << QStringLiteral("-It") << QStringLiteral("-italic")
                                            << QStringLiteral("-Condensed") << QStringLiteral("-Cond") << QStringLiteral("-CondIt") << QStringLiteral("-Light") << QStringLiteral("-LightSemiCn") << QStringLiteral("-LightSemiCnIt") << QStringLiteral("-Ligh") << QStringLiteral("-Lt") << QStringLiteral("-Slant") << QStringLiteral("-Slant167") << QStringLiteral("-LightCond") << QStringLiteral("Lig") << QStringLiteral("-Narrow") << QStringLiteral("-DmCn") << QStringLiteral("-BoldSemiCnIt") << QStringLiteral("-BoldSemiCn")
                                            << QStringLiteral("-BlackAlternate") << QStringLiteral("-BoldAlternate")
                                            << QStringLiteral("Regular") << QStringLiteral("Compressed") << QStringLiteral("Narrow") << QStringLiteral("Unicode")
                                            << QStringLiteral("Ext") << QStringLiteral("SWA") << QStringLiteral("Std") << QStringLiteral("-Identity-H") << QStringLiteral("-DTC") << QStringLiteral("CE");
        for (const QString &suffix : suffixes) {
            if (bName.endsWith(suffix))
                bName = bName.left(bName.length() - suffix.length());
        }
        static const QVector<QRegExp> suffixesRegExp = QVector<QRegExp>()
                << QRegExp(QStringLiteral("^[1-9][0-9]+E[a-f0-9]{2,5}"))
                << QRegExp(QStringLiteral("(-[0-9])+$"))
                << QRegExp(QStringLiteral("[~][0-9a-f]+$"))
                << QRegExp(QStringLiteral("-Extend\\.[0-9]+$"))
                << QRegExp(QStringLiteral("(Fet|Kursiv)[0-9]+$"))
                << QRegExp(QStringLiteral("[,-]?(Ital(ic)?|Oblique|Black|Bol(dB?)?)$"))
                << QRegExp(QStringLiteral("[,-](BdCn|SC)[0-9]*$")) << QRegExp(QStringLiteral("[,-][A-Z][0-9]$")) << QRegExp(QStringLiteral("_[0-9]+$"))
                << QRegExp(QStringLiteral("[+][A-Z]+$"))
                << QRegExp(QStringLiteral("[*][0-9]+$"));
        for (const QRegExp &suffix : suffixesRegExp) {
            bName.remove(suffix);
        }
        static const QStringList prefixes = QStringList() << QStringLiteral("Bitstream") << QStringLiteral("Microsoft") << QStringLiteral("Monotype");
        for (const QString &prefix : prefixes) {
            if (bName.startsWith(prefix))
                bName = bName.mid(prefix.length());
        }
        static const QRegExp specialCaseSuffix = QRegExp(QStringLiteral("([a-z])[ ]*(MT|T)$"));
        bName.replace(specialCaseSuffix, QStringLiteral("\\1"));
        static const QRegExp teXFonts = QRegExp(QStringLiteral("^((CM|SF|MS)[A-Z]+|(cm)[a-z]+|wasy|stmary|LM(Sans|Roman|Typewriter|Slanted|Math|Mono)[a-zA-Z]*)([0-9]+)$"));
        bName.replace(teXFonts, QStringLiteral("\\1"));
        static const QRegExp adobePrefix = QRegExp(QStringLiteral("^(A|Adobe)(Caslon|Garamond)"));
        bName.replace(adobePrefix, QStringLiteral("\\2"));
        bName.replace(QStringLiteral("Lucida "), QStringLiteral("Lucida"));
        bName.replace(QStringLiteral("Segoe UI"), QStringLiteral("SegoeUI"));
        bName.replace(QStringLiteral("CambriaMath"), QStringLiteral("Cambria"));
        bName.replace(QStringLiteral("Cambria Math"), QStringLiteral("Cambria"));
        bName.replace(QStringLiteral("MetaTabell"), QStringLiteral("Meta"));
        bName.replace(QStringLiteral("MetaText"), QStringLiteral("Meta"));
        static const QVector<QPair<QString, QString> > microsoftNamesWithSpaces = QVector<QPair<QString, QString> >() << QPair<QString, QString>(QStringLiteral("Times New Roman"), QStringLiteral("TimesNewRoman")) << QPair<QString, QString>(QStringLiteral("Courier New"), QStringLiteral("CourierNew")) << QPair<QString, QString>(QStringLiteral("Comic Sans"), QStringLiteral("ComicSans"));
        for (QVector<QPair<QString, QString> >::ConstIterator it = microsoftNamesWithSpaces.constBegin(); it != microsoftNamesWithSpaces.constEnd(); ++it)
            bName.replace(it->first, it->second);
        static const QRegExp sixLettersPlusPrefix = QRegExp(QStringLiteral("^([A-Z]{6}\\+[_]?)([a-zA-Z0-9]{3,})"));
        if (sixLettersPlusPrefix.indexIn(bName) == 0)
            bName = bName.mid(sixLettersPlusPrefix.cap(1).length());
        if (bName.length() > 3 && bName[0] == QChar('*'))
            bName = bName.mid(1);
        bNameChanged = bName != bNameOriginal;
    }
    bName.remove(QStringLiteral("+"));
    if (bName.isEmpty()) bName = fontNameWithoutTag; ///< Upps, removed too many characters?
    beautifiedName[QStringLiteral("")] = DocScan::xmlify(bName);

    QString text = typeName.toLower().remove(QStringLiteral(" "));
    if (text.indexOf(QStringLiteral("truetype")) >= 0)
        technology[QStringLiteral("type")] = QStringLiteral("truetype");
    else if (text.indexOf(QStringLiteral("type0")) >= 0)
        technology[QStringLiteral("type")] = QStringLiteral("type0");
    else if (text.indexOf(QStringLiteral("type1")) >= 0)
        technology[QStringLiteral("type")] = QStringLiteral("type1");
    else if (text.indexOf(QStringLiteral("type3")) >= 0)
        technology[QStringLiteral("type")] = QStringLiteral("type3");

    const QString result = DocScan::formatMap(QStringLiteral("name"), name) + DocScan::formatMap(QStringLiteral("beautified"), beautifiedName) + DocScan::formatMap(QStringLiteral("technology"), technology) + DocScan::formatMap(QStringLiteral("license"), license);

    return result;
}

QString Guessing::programToXML(const QString &program) {
    const QString text = program.toLower();
    QHash<QString, QString> xml;
    xml[QStringLiteral("")] = program;
    bool checkOOoVersion = false;

    if (text.indexOf(QStringLiteral("dvips")) >= 0) {
        static const QRegExp radicaleyeVersion("\\b\\d+\\.\\d+[a-z]*\\b");
        xml[QStringLiteral("manufacturer")] = QStringLiteral("radicaleye");
        xml[QStringLiteral("product")] = QStringLiteral("dvips");
        if (radicaleyeVersion.indexIn(text) >= 0)
            xml[QStringLiteral("version")] = radicaleyeVersion.cap(0);
    } else if (text.indexOf(QStringLiteral("ghostscript")) >= 0) {
        static const QRegExp ghostscriptVersion("\\b\\d+(\\.\\d+)+\\b");
        xml[QStringLiteral("manufacturer")] = QStringLiteral("artifex");
        xml[QStringLiteral("product")] = QStringLiteral("ghostscript");
        if (ghostscriptVersion.indexIn(text) >= 0)
            xml[QStringLiteral("version")] = ghostscriptVersion.cap(0);
    } else if (text.startsWith(QStringLiteral("cairo "))) {
        static const QRegExp cairoVersion("\\b\\d+(\\.\\d+)+\\b");
        xml[QStringLiteral("manufacturer")] = QStringLiteral("cairo");
        xml[QStringLiteral("product")] = QStringLiteral("cairo");
        if (cairoVersion.indexIn(text) >= 0)
            xml[QStringLiteral("version")] = cairoVersion.cap(0);
    } else if (text.indexOf(QStringLiteral("pdftex")) >= 0) {
        static const QRegExp pdftexVersion("\\d+(\\.\\d+)+\\b");
        xml[QStringLiteral("manufacturer")] = QStringLiteral("pdftex");
        xml[QStringLiteral("product")] = QStringLiteral("pdftex");
        if (pdftexVersion.indexIn(text) >= 0)
            xml[QStringLiteral("version")] = pdftexVersion.cap(0);
    } else if (text.indexOf(QStringLiteral("xetex")) >= 0) {
        static const QRegExp xetexVersion("\\d+(\\.\\d+)+\\b");
        xml[QStringLiteral("manufacturer")] = QStringLiteral("xetex");
        xml[QStringLiteral("product")] = QStringLiteral("xetex");
        if (xetexVersion.indexIn(text) >= 0)
            xml[QStringLiteral("version")] = xetexVersion.cap(0);
    } else if (text.indexOf(QStringLiteral("latex")) >= 0) {
        xml[QStringLiteral("manufacturer")] = QStringLiteral("latex");
        xml[QStringLiteral("product")] = QStringLiteral("latex");
    } else if (text.indexOf(QStringLiteral("dvipdfm")) >= 0) {
        static const QRegExp dvipdfmVersion("\\b\\d+(\\.\\d+)+[a-z]*\\b");
        xml[QStringLiteral("manufacturer")] = QStringLiteral("dvipdfm");
        xml[QStringLiteral("product")] = QStringLiteral("dvipdfm");
        if (dvipdfmVersion.indexIn(text) >= 0)
            xml[QStringLiteral("version")] = dvipdfmVersion.cap(0);
    } else if (text.indexOf(QStringLiteral("tex output")) >= 0 || text == QStringLiteral("tex") || text.startsWith(QStringLiteral("tex "))) {
        static const QRegExp texVersion("\\b\\d+([.:]\\d+)+\\b");
        xml[QStringLiteral("manufacturer")] = QStringLiteral("tex");
        xml[QStringLiteral("product")] = QStringLiteral("tex");
        if (texVersion.indexIn(text) >= 0)
            xml[QStringLiteral("version")] = texVersion.cap(0);
    } else if (text.startsWith(QStringLiteral("gnuplot "))) {
        static const QRegExp gnuplotVersion("\\b(\\d+([.]\\d+)*)( patchlevel (\\d+))?\\b");
        xml[QStringLiteral("product")] = QStringLiteral("gnuplot");
        if (gnuplotVersion.indexIn(text) >= 0) {
            xml[QStringLiteral("version")] = gnuplotVersion.cap(1);
            if (!gnuplotVersion.cap(4).isEmpty())
                xml[QStringLiteral("version")] += QStringLiteral("p") + gnuplotVersion.cap(4);
        }
    } else if (text.indexOf(QStringLiteral("koffice")) >= 0) {
        static const QRegExp kofficeVersion("/(d+([.]\\d+)*)\\b");
        xml[QStringLiteral("manufacturer")] = QStringLiteral("kde");
        xml[QStringLiteral("product")] = QStringLiteral("koffice");
        if (kofficeVersion.indexIn(text) >= 0)
            xml[QStringLiteral("version")] = kofficeVersion.cap(1);
    } else if (text.indexOf(QStringLiteral("calligra")) >= 0) {
        static const QRegExp calligraVersion("/(d+([.]\\d+)*)\\b");
        xml[QStringLiteral("manufacturer")] = QStringLiteral("kde");
        xml[QStringLiteral("product")] = QStringLiteral("calligra");
        if (calligraVersion.indexIn(text) >= 0)
            xml[QStringLiteral("version")] = calligraVersion.cap(1);
    } else if (text.indexOf(QStringLiteral("abiword")) >= 0) {
        xml[QStringLiteral("manufacturer")] = QStringLiteral("abisource");
        xml[QStringLiteral("product")] = QStringLiteral("abiword");
    } else if (text.indexOf(QStringLiteral("office_one")) >= 0) {
        checkOOoVersion = true;
        xml[QStringLiteral("product")] = QStringLiteral("office_one");
        xml[QStringLiteral("based-on")] = QStringLiteral("openoffice");
    } else if (text.indexOf(QStringLiteral("infraoffice")) >= 0) {
        checkOOoVersion = true;
        xml[QStringLiteral("product")] = QStringLiteral("infraoffice");
        xml[QStringLiteral("based-on")] = QStringLiteral("openoffice");
    } else if (text.indexOf(QStringLiteral("aksharnaveen")) >= 0) {
        checkOOoVersion = true;
        xml[QStringLiteral("product")] = QStringLiteral("aksharnaveen");
        xml[QStringLiteral("based-on")] = QStringLiteral("openoffice");
    } else if (text.indexOf(QStringLiteral("redoffice")) >= 0) {
        checkOOoVersion = true;
        xml[QStringLiteral("manufacturer")] = QStringLiteral("china");
        xml[QStringLiteral("product")] = QStringLiteral("redoffice");
        xml[QStringLiteral("based-on")] = QStringLiteral("openoffice");
    } else if (text.indexOf(QStringLiteral("sun_odf_plugin")) >= 0) {
        checkOOoVersion = true;
        xml[QStringLiteral("manufacturer")] = QStringLiteral("oracle");
        xml[QStringLiteral("product")] = QStringLiteral("odfplugin");
        xml[QStringLiteral("based-on")] = QStringLiteral("openoffice");
    } else if (text.indexOf(QStringLiteral("libreoffice")) >= 0) {
        checkOOoVersion = true;
        xml[QStringLiteral("manufacturer")] = QStringLiteral("tdf");
        xml[QStringLiteral("product")] = QStringLiteral("libreoffice");
        xml[QStringLiteral("based-on")] = QStringLiteral("openoffice");
    }  else if (text.indexOf(QStringLiteral("lotus symphony")) >= 0) {
        static const QRegExp lotusSymphonyVersion("Symphony (\\d+(\\.\\d+)*)");
        xml[QStringLiteral("manufacturer")] = QStringLiteral("ibm");
        xml[QStringLiteral("product")] = QStringLiteral("lotus-symphony");
        xml[QStringLiteral("based-on")] = QStringLiteral("openoffice");
        if (lotusSymphonyVersion.indexIn(text) >= 0)
            xml[QStringLiteral("version")] = lotusSymphonyVersion.cap(1);
    }  else if (text.indexOf(QStringLiteral("Lotus_Symphony")) >= 0) {
        checkOOoVersion = true;
        xml[QStringLiteral("manufacturer")] = QStringLiteral("ibm");
        xml[QStringLiteral("product")] = QStringLiteral("lotus-symphony");
        xml[QStringLiteral("based-on")] = QStringLiteral("openoffice");
    } else if (text.indexOf(QStringLiteral("openoffice")) >= 0) {
        checkOOoVersion = true;
        if (text.indexOf(QStringLiteral("staroffice")) >= 0) {
            xml[QStringLiteral("manufacturer")] = QStringLiteral("oracle");
            xml[QStringLiteral("based-on")] = QStringLiteral("openoffice");
            xml[QStringLiteral("product")] = QStringLiteral("staroffice");
        } else if (text.indexOf(QStringLiteral("broffice")) >= 0) {
            xml[QStringLiteral("product")] = QStringLiteral("broffice");
            xml[QStringLiteral("based-on")] = QStringLiteral("openoffice");
        } else if (text.indexOf(QStringLiteral("neooffice")) >= 0) {
            xml[QStringLiteral("manufacturer")] = QStringLiteral("planamesa");
            xml[QStringLiteral("product")] = QStringLiteral("neooffice");
            xml[QStringLiteral("based-on")] = QStringLiteral("openoffice");
        } else {
            xml[QStringLiteral("manufacturer")] = QStringLiteral("oracle");
            xml[QStringLiteral("product")] = QStringLiteral("openoffice");
        }
    } else if (text == QStringLiteral("writer") || text == QStringLiteral("calc") || text == QStringLiteral("impress")) {
        /// for Creator/Editor string
        xml[QStringLiteral("manufacturer")] = QStringLiteral("oracle;tdf");
        xml[QStringLiteral("product")] = QStringLiteral("openoffice;libreoffice");
        xml[QStringLiteral("based-on")] = QStringLiteral("openoffice");
    } else if (text.startsWith(QStringLiteral("pdfscanlib "))) {
        static const QRegExp pdfscanlibVersion("v(\\d+(\\.\\d+)+)\\b");
        xml[QStringLiteral("manufacturer")] = QStringLiteral("kodak?");
        xml[QStringLiteral("product")] = QStringLiteral("pdfscanlib");
        if (pdfscanlibVersion.indexIn(text) >= 0)
            xml[QStringLiteral("version")] = pdfscanlibVersion.cap(1);
    } else if (text.indexOf(QStringLiteral("framemaker")) >= 0) {
        static const QRegExp framemakerVersion("\\b\\d+(\\.\\d+)+(\\b|\\.|p\\d+)");
        xml[QStringLiteral("manufacturer")] = QStringLiteral("adobe");
        xml[QStringLiteral("product")] = QStringLiteral("framemaker");
        if (framemakerVersion.indexIn(text) >= 0)
            xml[QStringLiteral("version")] = framemakerVersion.cap(0);
    } else if (text.contains(QStringLiteral("distiller"))) {
        static const QRegExp distillerVersion("\\b\\d+(\\.\\d+)+\\b");
        xml[QStringLiteral("manufacturer")] = QStringLiteral("adobe");
        xml[QStringLiteral("product")] = QStringLiteral("distiller");
        if (distillerVersion.indexIn(text) >= 0)
            xml[QStringLiteral("version")] = distillerVersion.cap(0);
    } else if (text.startsWith(QStringLiteral("pdflib plop"))) {
        static const QRegExp plopVersion("\\b\\d+(\\.\\d+)+\\b");
        xml[QStringLiteral("manufacturer")] = QStringLiteral("pdflib");
        xml[QStringLiteral("product")] = QStringLiteral("plop");
        if (plopVersion.indexIn(text) >= 0)
            xml[QStringLiteral("version")] = plopVersion.cap(0);
    } else if (text.startsWith(QStringLiteral("pdflib"))) {
        static const QRegExp pdflibVersion("\\b\\d+(\\.[0-9p]+)+\\b");
        xml[QStringLiteral("manufacturer")] = QStringLiteral("pdflib");
        xml[QStringLiteral("product")] = QStringLiteral("pdflib");
        if (pdflibVersion.indexIn(text) >= 0)
            xml[QStringLiteral("version")] = pdflibVersion.cap(0);
    } else if (text.indexOf(QStringLiteral("pdf library")) >= 0) {
        static const QRegExp pdflibraryVersion("\\b\\d+(\\.\\d+)+\\b");
        xml[QStringLiteral("manufacturer")] = QStringLiteral("adobe");
        xml[QStringLiteral("product")] = QStringLiteral("pdflibrary");
        if (pdflibraryVersion.indexIn(text) >= 0)
            xml[QStringLiteral("version")] = pdflibraryVersion.cap(0);
    } else if (text.indexOf(QStringLiteral("pdfwriter")) >= 0) {
        static const QRegExp pdfwriterVersion("\\b\\d+(\\.\\d+)+\\b");
        xml[QStringLiteral("manufacturer")] = QStringLiteral("adobe");
        xml[QStringLiteral("product")] = QStringLiteral("pdfwriter");
        if (pdfwriterVersion.indexIn(text) >= 0)
            xml[QStringLiteral("version")] = pdfwriterVersion.cap(0);
    } else if (text.indexOf(QStringLiteral("easypdf")) >= 0) {
        static const QRegExp easypdfVersion("\\b\\d+(\\.\\d+)+\\b");
        xml[QStringLiteral("manufacturer")] = QStringLiteral("bcl");
        xml[QStringLiteral("product")] = QStringLiteral("easypdf");
        if (easypdfVersion.indexIn(text) >= 0)
            xml[QStringLiteral("version")] = easypdfVersion.cap(0);
    } else if (text.contains(QStringLiteral("pdfmaker"))) {
        static const QRegExp pdfmakerVersion(" \\d+(\\.\\d+)*\\b");
        xml[QStringLiteral("manufacturer")] = QStringLiteral("adobe");
        xml[QStringLiteral("product")] = QStringLiteral("pdfmaker");
        if (pdfmakerVersion.indexIn(text) >= 0)
            xml[QStringLiteral("version")] = pdfmakerVersion.cap(0);
    } else if (text.startsWith(QStringLiteral("fill-in "))) {
        static const QRegExp fillInVersion("\\b\\d+(\\.\\d+)+\\b");
        xml[QStringLiteral("manufacturer")] = QStringLiteral("textcenter");
        xml[QStringLiteral("product")] = QStringLiteral("fill-in");
        if (fillInVersion.indexIn(text) >= 0)
            xml[QStringLiteral("version")] = fillInVersion.cap(0);
    } else if (text.startsWith(QStringLiteral("itext "))) {
        static const QRegExp iTextVersion("\\b((\\d+)(\\.\\d+)+)\\b");
        xml[QStringLiteral("manufacturer")] = QStringLiteral("itext");
        xml[QStringLiteral("product")] = QStringLiteral("itext");
        if (iTextVersion.indexIn(text) >= 0) {
            xml[QStringLiteral("version")] = iTextVersion.cap(0);
            bool ok = false;
            const int majorVersion = iTextVersion.cap(2).toInt(&ok);
            if (ok && majorVersion > 0) {
                if (majorVersion <= 4)
                    xml[QStringLiteral("license")] = QStringLiteral("MPL;LGPL");
                else if (majorVersion >= 5)
                    xml[QStringLiteral("license")] = QStringLiteral("commercial;AGPLv3");
            }
        }
    } else if (text.startsWith(QStringLiteral("amyuni pdf converter "))) {
        static const QRegExp amyunitVersion("\\b\\d+(\\.\\d+)+\\b");
        xml[QStringLiteral("manufacturer")] = QStringLiteral("amyuni");
        xml[QStringLiteral("product")] = QStringLiteral("pdfconverter");
        if (amyunitVersion.indexIn(text) >= 0)
            xml[QStringLiteral("version")] = amyunitVersion.cap(0);
    } else if (text.indexOf(QStringLiteral("pdfout v")) >= 0) {
        static const QRegExp pdfoutVersion("v(\\d+(\\.\\d+)+)\\b");
        xml[QStringLiteral("manufacturer")] = QStringLiteral("verypdf");
        xml[QStringLiteral("product")] = QStringLiteral("docconverter");
        if (pdfoutVersion.indexIn(text) >= 0)
            xml[QStringLiteral("version")] = pdfoutVersion.cap(1);
    } else if (text.indexOf(QStringLiteral("jaws pdf creator")) >= 0) {
        static const QRegExp pdfcreatorVersion("v(\\d+(\\.\\d+)+)\\b");
        xml[QStringLiteral("manufacturer")] = QStringLiteral("jaws");
        xml[QStringLiteral("product")] = QStringLiteral("pdfcreator");
        if (pdfcreatorVersion.indexIn(text) >= 0)
            xml[QStringLiteral("version")] = pdfcreatorVersion.cap(1);
    } else if (text.startsWith(QStringLiteral("arbortext "))) {
        static const QRegExp arbortextVersion("\\d+(\\.\\d+)+)");
        xml[QStringLiteral("manufacturer")] = QStringLiteral("ptc");
        xml[QStringLiteral("product")] = QStringLiteral("arbortext");
        if (arbortextVersion.indexIn(text) >= 0)
            xml[QStringLiteral("version")] = arbortextVersion.cap(0);
    } else if (text.contains(QStringLiteral("3b2"))) {
        static const QRegExp threeB2Version("\\d+(\\.[0-9a-z]+)+");
        xml[QStringLiteral("manufacturer")] = QStringLiteral("ptc");
        xml[QStringLiteral("product")] = QStringLiteral("3b2");
        if (threeB2Version.indexIn(text) >= 0)
            xml[QStringLiteral("version")] = threeB2Version.cap(0);
    } else if (text.startsWith(QStringLiteral("3-heights"))) {
        static const QRegExp threeHeightsVersion("\\b\\d+(\\.\\d+)+");
        xml[QStringLiteral("manufacturer")] = QStringLiteral("pdftoolsag");
        xml[QStringLiteral("product")] = QStringLiteral("3-heights");
        if (threeHeightsVersion.indexIn(text) >= 0)
            xml[QStringLiteral("version")] = threeHeightsVersion.cap(0);
    } else if (text.contains(QStringLiteral("abcpdf"))) {
        xml[QStringLiteral("manufacturer")] = QStringLiteral("websupergoo");
        xml[QStringLiteral("product")] = QStringLiteral("abcpdf");
    } else if (text.indexOf(QStringLiteral("primopdf")) >= 0) {
        xml[QStringLiteral("manufacturer")] = QStringLiteral("nitro");
        xml[QStringLiteral("product")] = QStringLiteral("primopdf");
        xml[QStringLiteral("based-on")] = QStringLiteral("nitropro");
    } else if (text.indexOf(QStringLiteral("nitro")) >= 0) {
        xml[QStringLiteral("manufacturer")] = QStringLiteral("nitro");
        xml[QStringLiteral("product")] = QStringLiteral("nitropro");
    } else if (text.indexOf(QStringLiteral("pdffactory")) >= 0) {
        static const QRegExp pdffactoryVersion("\\b\\d+(\\.\\d+)+\\b");
        xml[QStringLiteral("manufacturer")] = QStringLiteral("softwarelabs");
        xml[QStringLiteral("product")] = QStringLiteral("pdffactory");
        if (pdffactoryVersion.indexIn(text) >= 0)
            xml[QStringLiteral("version")] = pdffactoryVersion.cap(0);
    } else if (text.startsWith(QStringLiteral("ibex pdf"))) {
        static const QRegExp ibexVersion("\\b\\d+(\\.\\[0-9/]+)+\\b");
        xml[QStringLiteral("manufacturer")] = QStringLiteral("visualprogramming");
        xml[QStringLiteral("product")] = QStringLiteral("ibexpdfcreator");
        if (ibexVersion.indexIn(text) >= 0)
            xml[QStringLiteral("version")] = ibexVersion.cap(0);
    } else if (text.startsWith(QStringLiteral("arc/info")) || text.startsWith(QStringLiteral("arcinfo"))) {
        static const QRegExp arcinfoVersion("\\b\\d+(\\.\\d+)+\\b");
        xml[QStringLiteral("manufacturer")] = QStringLiteral("esri");
        xml[QStringLiteral("product")] = QStringLiteral("arcinfo");
        if (arcinfoVersion.indexIn(text) >= 0)
            xml[QStringLiteral("version")] = arcinfoVersion.cap(0);
    } else if (text.startsWith(QStringLiteral("paperport "))) {
        static const QRegExp paperportVersion("\\b\\d+(\\.\\d+)+\\b");
        xml[QStringLiteral("manufacturer")] = QStringLiteral("nuance");
        xml[QStringLiteral("product")] = QStringLiteral("paperport");
        if (paperportVersion.indexIn(text) >= 0)
            xml[QStringLiteral("version")] = paperportVersion.cap(0);
    } else if (text.indexOf(QStringLiteral("indesign")) >= 0) {
        static const QRegExp indesignVersion("\\b\\d+(\\.\\d+)+\\b");
        xml[QStringLiteral("manufacturer")] = QStringLiteral("adobe");
        xml[QStringLiteral("product")] = QStringLiteral("indesign");
        if (indesignVersion.indexIn(text) >= 0)
            xml[QStringLiteral("version")] = indesignVersion.cap(0);
        else {
            static const QRegExp csVersion(QStringLiteral("\\bCS(\\d*)\\b"));
            if (csVersion.indexIn(text) >= 0) {
                bool ok = false;
                double versionNumber = csVersion.cap(1).toDouble(&ok);
                if (csVersion.cap(0) == QStringLiteral("CS"))
                    xml[QStringLiteral("version")] = QStringLiteral("3.0");
                else if (ok && versionNumber > 1) {
                    versionNumber += 2;
                    xml[QStringLiteral("version")] = QString::number(versionNumber, 'f', 1);
                }
            } else {
                static const QRegExp ccVersion(QStringLiteral("\\bCC\\s*(\\d+([.]\\d+)*)\\b"));
                if (ccVersion.indexIn(text) >= 0) {
                    bool ok = false;
                    double versionNumber = ccVersion.cap(1).toDouble(&ok);
                    if (ccVersion.cap(0) == QStringLiteral("CC"))
                        xml[QStringLiteral("version")] = QStringLiteral("9.2");
                    else if (ok) {
                        if (versionNumber == 2014)
                            xml[QStringLiteral("version")] = QStringLiteral("10");
                        else if (versionNumber == 2014.1)
                            xml[QStringLiteral("version")] = QStringLiteral("10.1");
                        else if (versionNumber == 2014.2)
                            xml[QStringLiteral("version")] = QStringLiteral("10.2");
                        else if (versionNumber == 2015)
                            xml[QStringLiteral("version")] = QStringLiteral("11.0");
                        else if (versionNumber == 2015.1)
                            xml[QStringLiteral("version")] = QStringLiteral("11.1");
                        else if (versionNumber == 2015.2)
                            xml[QStringLiteral("version")] = QStringLiteral("11.2");
                        else if (versionNumber == 2015.4)
                            xml[QStringLiteral("version")] = QStringLiteral("11.4");
                        else if (versionNumber == 2017)
                            xml[QStringLiteral("version")] = QStringLiteral("12.0");
                        else if (versionNumber == 2017.1)
                            xml[QStringLiteral("version")] = QStringLiteral("12.1");
                    }
                }
            }
        }
    } else if (text.indexOf(QStringLiteral("illustrator")) >= 0) {
        static const QRegExp illustratorVersion("\\b\\d+(\\.\\d+)+\\b");
        xml[QStringLiteral("manufacturer")] = QStringLiteral("adobe");
        xml[QStringLiteral("product")] = QStringLiteral("illustrator");
        if (illustratorVersion.indexIn(text) >= 0)
            xml[QStringLiteral("version")] = illustratorVersion.cap(0);
        else {
            static const QRegExp csVersion(QStringLiteral("\\bCS(\\d*)\\b"));
            if (csVersion.indexIn(text) >= 0) {
                bool ok = false;
                double versionNumber = csVersion.cap(1).toDouble(&ok);
                if (csVersion.cap(0) == QStringLiteral("CS"))
                    xml[QStringLiteral("version")] = QStringLiteral("11.0");
                else if (ok && versionNumber > 1) {
                    versionNumber += 10;
                    xml[QStringLiteral("version")] = QString::number(versionNumber, 'f', 1);
                }
            }
        }
    } else if (text.indexOf(QStringLiteral("pagemaker")) >= 0) {
        static const QRegExp pagemakerVersion("\\b\\d+(\\.\\d+)+\\b");
        xml[QStringLiteral("manufacturer")] = QStringLiteral("adobe");
        xml[QStringLiteral("product")] = QStringLiteral("pagemaker");
        if (pagemakerVersion.indexIn(text) >= 0)
            xml[QStringLiteral("version")] = pagemakerVersion.cap(0);
    } else if (text.indexOf(QStringLiteral("acrobat capture")) >= 0) {
        static const QRegExp acrobatCaptureVersion("\\b\\d+(\\.\\d+)+\\b");
        xml[QStringLiteral("manufacturer")] = QStringLiteral("adobe");
        xml[QStringLiteral("product")] = QStringLiteral("acrobatcapture");
        if (acrobatCaptureVersion.indexIn(text) >= 0)
            xml[QStringLiteral("version")] = acrobatCaptureVersion.cap(0);
    } else if (text.indexOf(QStringLiteral("acrobat pro")) >= 0) {
        static const QRegExp acrobatProVersion("\\b\\d+(\\.\\d+)+\\b");
        xml[QStringLiteral("manufacturer")] = QStringLiteral("adobe");
        xml[QStringLiteral("product")] = QStringLiteral("acrobatpro");
        if (acrobatProVersion.indexIn(text) >= 0)
            xml[QStringLiteral("version")] = acrobatProVersion.cap(0);
    } else if (text.indexOf(QStringLiteral("acrobat")) >= 0) {
        static const QRegExp acrobatVersion("\\b\\d+(\\.\\d+)+\\b");
        xml[QStringLiteral("manufacturer")] = QStringLiteral("adobe");
        xml[QStringLiteral("product")] = QStringLiteral("acrobat");
        if (acrobatVersion.indexIn(text) >= 0)
            xml[QStringLiteral("version")] = acrobatVersion.cap(0);
    } else if (text.indexOf(QStringLiteral("livecycle")) >= 0) {
        static const QRegExp livecycleVersion("\\b\\d+(\\.\\d+)+[a-z]?\\b");
        xml[QStringLiteral("manufacturer")] = QStringLiteral("adobe");
        int regExpPos;
        if ((regExpPos = livecycleVersion.indexIn(text)) >= 0)
            xml[QStringLiteral("version")] = livecycleVersion.cap(0);
        if (regExpPos <= 0)
            regExpPos = 1024;
        QString product = text;
        xml[QStringLiteral("product")] = product.left(regExpPos - 1).remove(QStringLiteral("adobe")).remove(livecycleVersion.cap(0)).remove(QStringLiteral(" ")) + QLatin1Char('?');
    } else if (text.startsWith(QStringLiteral("adobe photoshop elements"))) {
        xml[QStringLiteral("manufacturer")] = QStringLiteral("adobe");
        xml[QStringLiteral("product")] = QStringLiteral("photoshopelements");
    } else if (text.startsWith(QStringLiteral("adobe photoshop"))) {
        static const QRegExp photoshopVersion("\\bCS|(CS)?\\d+(\\.\\d+)+\\b");
        xml[QStringLiteral("manufacturer")] = QStringLiteral("adobe");
        xml[QStringLiteral("product")] = QStringLiteral("photoshop");
        if (photoshopVersion.indexIn(text) >= 0)
            xml[QStringLiteral("version")] = photoshopVersion.cap(0);
    } else if (text.indexOf(QStringLiteral("adobe")) >= 0) {
        /// some unknown Adobe product
        static const QRegExp adobeVersion("\\b\\d+(\\.\\d+)+\\b");
        xml[QStringLiteral("manufacturer")] = QStringLiteral("adobe");
        if (adobeVersion.indexIn(text) >= 0)
            xml[QStringLiteral("version")] = adobeVersion.cap(0);
        QString product = text;
        xml[QStringLiteral("product")] = product.remove(QStringLiteral("adobe")).remove(adobeVersion.cap(0)).remove(QStringLiteral(" ")) + QLatin1Char('?');
    } else if (text.contains(QStringLiteral("pages"))) {
        xml[QStringLiteral("manufacturer")] = QStringLiteral("apple");
        xml[QStringLiteral("product")] = QStringLiteral("pages");
    } else if (text.contains(QStringLiteral("keynote"))) {
        static const QRegExp keynoteVersion("\\b\\d+(\\.\\d+)+\\b");
        xml[QStringLiteral("manufacturer")] = QStringLiteral("apple");
        xml[QStringLiteral("product")] = QStringLiteral("keynote");
        if (keynoteVersion.indexIn(text) >= 0)
            xml[QStringLiteral("version")] = keynoteVersion.cap(0);
    } else if (text.indexOf(QStringLiteral("quartz")) >= 0) {
        static const QRegExp quartzVersion("\\b\\d+(\\.\\d+)+\\b");
        xml[QStringLiteral("manufacturer")] = QStringLiteral("apple");
        xml[QStringLiteral("product")] = QStringLiteral("quartz");
        if (quartzVersion.indexIn(text) >= 0)
            xml[QStringLiteral("version")] = quartzVersion.cap(0);
    } else if (text.indexOf(QStringLiteral("pscript5.dll")) >= 0 || text.indexOf(QStringLiteral("pscript.dll")) >= 0) {
        static const QRegExp pscriptVersion("\\b\\d+(\\.\\d+)+\\b");
        xml[QStringLiteral("manufacturer")] = QStringLiteral("microsoft");
        xml[QStringLiteral("product")] = QStringLiteral("pscript");
        xml[QStringLiteral("opsys")] = QStringLiteral("windows");
        if (pscriptVersion.indexIn(text) >= 0)
            xml[QStringLiteral("version")] = pscriptVersion.cap(0);
    } else if (text.indexOf(QStringLiteral("quarkxpress")) >= 0) {
        static const QRegExp quarkxpressVersion("\\b\\d+(\\.\\d+)+\\b");
        xml[QStringLiteral("manufacturer")] = QStringLiteral("quark");
        xml[QStringLiteral("product")] = QStringLiteral("xpress");
        if (quarkxpressVersion.indexIn(text) >= 0)
            xml[QStringLiteral("version")] = quarkxpressVersion.cap(0);
    } else if (text.indexOf(QStringLiteral("pdfcreator")) >= 0) {
        static const QRegExp pdfcreatorVersion("\\b\\d+(\\.\\d+)+\\b");
        xml[QStringLiteral("manufacturer")] = QStringLiteral("pdfforge");
        xml[QStringLiteral("product")] = QStringLiteral("pdfcreator");
        xml[QStringLiteral("opsys")] = QStringLiteral("windows");
        if (pdfcreatorVersion.indexIn(text) >= 0)
            xml[QStringLiteral("version")] = pdfcreatorVersion.cap(0);
    } else if (text.startsWith(QStringLiteral("stamppdf batch"))) {
        static const QRegExp stamppdfbatchVersion("\\b\\d+(\\.\\d+)+\\b");
        xml[QStringLiteral("manufacturer")] = QStringLiteral("appligent");
        xml[QStringLiteral("product")] = QStringLiteral("stamppdfbatch");
        if (stamppdfbatchVersion.indexIn(text) >= 0)
            xml[QStringLiteral("version")] = stamppdfbatchVersion.cap(0);
    } else if (text.startsWith(QStringLiteral("xyenterprise "))) {
        static const QRegExp xyVersion("\\b(\\d+(\\.\\[0-9a-z])+)( patch \\S*\\d)?\\b");
        xml[QStringLiteral("manufacturer")] = QStringLiteral("dakota");
        xml[QStringLiteral("product")] = QStringLiteral("xyenterprise");
        if (xyVersion.indexIn(text) >= 0)
            xml[QStringLiteral("version")] = xyVersion.cap(1);
    } else if (text.startsWith(QStringLiteral("edocprinter "))) {
        static const QRegExp edocprinterVersion("ver (\\d+(\\.\\d+)+)\\b");
        xml[QStringLiteral("manufacturer")] = QStringLiteral("itek");
        xml[QStringLiteral("product")] = QStringLiteral("edocprinter");
        if (edocprinterVersion.indexIn(text) >= 0)
            xml[QStringLiteral("version")] = edocprinterVersion.cap(1);
    } else if (text.startsWith(QStringLiteral("pdf code "))) {
        static const QRegExp pdfcodeVersion("\\b(\\d{8}}|d+(\\.\\d+)+)\\b");
        xml[QStringLiteral("manufacturer")] = QStringLiteral("europeancommission");
        xml[QStringLiteral("product")] = QStringLiteral("pdfcode");
        if (pdfcodeVersion.indexIn(text) >= 0)
            xml[QStringLiteral("version")] = pdfcodeVersion.cap(1);
    } else if (text.indexOf(QStringLiteral("pdf printer")) >= 0) {
        xml[QStringLiteral("manufacturer")] = QStringLiteral("bullzip");
        xml[QStringLiteral("product")] = QStringLiteral("pdfprinter");
    } else if (text.contains(QStringLiteral("aspose")) && text.contains(QStringLiteral("words"))) {
        static const QRegExp asposewordsVersion("\\b\\d+(\\.\\d+)+\\b");
        xml[QStringLiteral("manufacturer")] = QStringLiteral("aspose");
        xml[QStringLiteral("product")] = QStringLiteral("aspose.words");
        if (asposewordsVersion.indexIn(text) >= 0)
            xml[QStringLiteral("version")] = asposewordsVersion.cap(0);
    } else if (text.contains(QStringLiteral("arcmap"))) {
        static const QRegExp arcmapVersion("\\b\\d+(\\.\\d+)+\\b");
        xml[QStringLiteral("manufacturer")] = QStringLiteral("esri");
        xml[QStringLiteral("product")] = QStringLiteral("arcmap");
        if (arcmapVersion.indexIn(text) >= 0)
            xml[QStringLiteral("version")] = arcmapVersion.cap(0);
    } else if (text.contains(QStringLiteral("ocad"))) {
        static const QRegExp ocadVersion("\\b\\d+(\\.\\d+)+\\b");
        xml[QStringLiteral("manufacturer")] = QStringLiteral("ocad");
        xml[QStringLiteral("product")] = QStringLiteral("ocad");
        if (ocadVersion.indexIn(text) >= 0)
            xml[QStringLiteral("version")] = ocadVersion.cap(0);
    } else if (text.contains(QStringLiteral("gnostice"))) {
        static const QRegExp gnosticeVersion("\\b[v]?\\d+(\\.\\d+)+\\b");
        xml[QStringLiteral("manufacturer")] = QStringLiteral("gnostice");
        if (gnosticeVersion.indexIn(text) >= 0)
            xml[QStringLiteral("version")] = gnosticeVersion.cap(0);
        QString product = text;
        xml[QStringLiteral("product")] = product.remove(QStringLiteral("gnostice")).remove(gnosticeVersion.cap(0)).remove(QStringLiteral(" ")) + QLatin1Char('?');
    } else if (text.contains(QStringLiteral("canon"))) {
        static const QRegExp canonVersion("\\b[v]?\\d+(\\.\\d+)+\\b");
        xml[QStringLiteral("manufacturer")] = QStringLiteral("canon");
        if (canonVersion.indexIn(text) >= 0)
            xml[QStringLiteral("version")] = canonVersion.cap(0);
        QString product = text;
        xml[QStringLiteral("product")] = product.remove(QStringLiteral("canon")).remove(canonVersion.cap(0)).remove(QStringLiteral(" ")) + QLatin1Char('?');
    } else if (text.startsWith(QStringLiteral("creo"))) {
        xml[QStringLiteral("manufacturer")] = QStringLiteral("creo");
        QString product = text;
        xml[QStringLiteral("product")] = product.remove(QStringLiteral("creo")).remove(QStringLiteral(" ")) + QLatin1Char('?');
    } else if (text.contains(QStringLiteral("apogee"))) {
        xml[QStringLiteral("manufacturer")] = QStringLiteral("agfa");
        xml[QStringLiteral("product")] = QStringLiteral("apogee");
    } else if (text.contains(QStringLiteral("ricoh"))) {
        xml[QStringLiteral("manufacturer")] = QStringLiteral("ricoh");
        const int i = text.indexOf(QStringLiteral("aficio"));
        if (i >= 0)
            xml[QStringLiteral("product")] = text.mid(i).remove(QLatin1Char(' '));
    } else if (text.contains(QStringLiteral("toshiba")) || text.contains(QStringLiteral("mfpimglib"))) {
        static const QRegExp toshibaVersion("\\b[v]?\\d+(\\.\\d+)+\\b");
        xml[QStringLiteral("manufacturer")] = QStringLiteral("toshiba");
        if (toshibaVersion.indexIn(text) >= 0)
            xml[QStringLiteral("version")] = toshibaVersion.cap(0);
        QString product = text;
        xml[QStringLiteral("product")] = product.remove(QStringLiteral("toshiba")).remove(toshibaVersion.cap(0)).remove(QStringLiteral(" ")) + QLatin1Char('?');
    } else if (text.startsWith(QStringLiteral("hp ")) || text.startsWith(QStringLiteral("hewlett packard "))) {
        xml[QStringLiteral("manufacturer")] = QStringLiteral("hewlettpackard");
        QString product = text;
        xml[QStringLiteral("product")] = product.remove(QStringLiteral("hp ")).remove(QStringLiteral("hewlett packard")).remove(QStringLiteral(" ")) + QLatin1Char('?');
    } else if (text.startsWith(QStringLiteral("xerox "))) {
        xml[QStringLiteral("manufacturer")] = QStringLiteral("xerox");
        QString product = text;
        xml[QStringLiteral("product")] = product.remove(QStringLiteral("xerox ")).remove(QStringLiteral(" ")) + QLatin1Char('?');
    } else if (text.startsWith(QStringLiteral("kodak "))) {
        xml[QStringLiteral("manufacturer")] = QStringLiteral("kodak");
        QString product = text;
        xml[QStringLiteral("product")] = product.remove(QStringLiteral("kodak ")).remove(QStringLiteral("scanner: ")).remove(QStringLiteral(" ")) + QLatin1Char('?');
    } else if (text.contains(QStringLiteral("konica")) || text.contains(QStringLiteral("minolta"))) {
        static const QRegExp konicaMinoltaVersion("\\b[v]?\\d+(\\.\\d+)+\\b");
        xml[QStringLiteral("manufacturer")] = QStringLiteral("konica;minolta");
        if (konicaMinoltaVersion.indexIn(text) >= 0)
            xml[QStringLiteral("version")] = konicaMinoltaVersion.cap(0);
        QString product = text;
        xml[QStringLiteral("product")] = product.remove(QStringLiteral("konica")).remove(QStringLiteral("minolta")).remove(konicaMinoltaVersion.cap(0)).remove(QStringLiteral(" ")) + QLatin1Char('?');
    } else if (text.contains(QStringLiteral("corel"))) {
        static const QRegExp corelVersion("\\b[v]?\\d+(\\.\\d+)+\\b");
        xml[QStringLiteral("manufacturer")] = QStringLiteral("corel");
        if (corelVersion.indexIn(text) >= 0)
            xml[QStringLiteral("version")] = corelVersion.cap(0);
        QString product = text;
        xml[QStringLiteral("product")] = product.remove(QStringLiteral("corel")).remove(corelVersion.cap(0)).remove(QStringLiteral(" ")) + QLatin1Char('?');
    } else if (text.contains(QStringLiteral("scansoft pdf create"))) {
        static const QRegExp scansoftVersion("\\b([a-zA-Z]+[ ])?[A-Za-z0-9]+\\b");
        xml[QStringLiteral("manufacturer")] = QStringLiteral("scansoft");
        xml[QStringLiteral("product")] = QStringLiteral("pdfcreate");
        if (scansoftVersion.indexIn(text) >= 0)
            xml[QStringLiteral("version")] = scansoftVersion.cap(0);
    } else if (text.contains(QStringLiteral("alivepdf"))) {
        static const QRegExp alivepdfVersion("\\b\\d+(\\.\\d+)+( RC)?\\b");
        xml[QStringLiteral("manufacturer")] = QStringLiteral("thibault.imbert");
        xml[QStringLiteral("product")] = QStringLiteral("alivepdf");
        if (alivepdfVersion.indexIn(text) >= 0)
            xml[QStringLiteral("version")] = alivepdfVersion.cap(0);
        xml[QStringLiteral("opsys")] = QStringLiteral("flash");
    } else if (text == QStringLiteral("google")) {
        xml[QStringLiteral("manufacturer")] = QStringLiteral("google");
        xml[QStringLiteral("product")] = QStringLiteral("docs");
    } else if (text.startsWith(QStringLiteral("apache "))) {
        xml[QStringLiteral("manufacturer")] = QStringLiteral("apache");
        if (text.contains(QStringLiteral(" fop ")))
            xml[QStringLiteral("product")] = QStringLiteral("fop");
        static const QRegExp apacheVersion("\\bVersion (\\d+(\\.\\d+)+\\b");
        if (apacheVersion.indexIn(text, Qt::CaseInsensitive) >= 0)
            xml[QStringLiteral("version")] = apacheVersion.cap(0);
    } else if (text.startsWith(QStringLiteral("pdftk "))) {
        xml[QStringLiteral("manufacturer")] = QStringLiteral("pdflabs");
        xml[QStringLiteral("product")] = QStringLiteral("pdftk");
        static const QRegExp pdftkVersion(" (\\d+(\\.\\d+)*\\b");
        if (pdftkVersion.indexIn(text, Qt::CaseInsensitive) >= 0)
            xml[QStringLiteral("version")] = pdftkVersion.cap(0);
    } else if (text.startsWith(QStringLiteral("pdfmerge!"))) {
        xml[QStringLiteral("manufacturer")] = QStringLiteral("lulusoftware");
        xml[QStringLiteral("product")] = QStringLiteral("pdfmerge");
    } else if (!text.contains(QStringLiteral("words"))) {
        static const QRegExp microsoftProducts("powerpoint|excel|word|outlook|visio|access");
        static const QRegExp microsoftVersion("\\b(starter )?(20[01][0-9]|1?[0-9]\\.[0-9]+|9[5-9])\\b");
        if (microsoftProducts.indexIn(text) >= 0) {
            xml[QStringLiteral("manufacturer")] = QStringLiteral("microsoft");
            xml[QStringLiteral("product")] = microsoftProducts.cap(0);
            if (!xml.contains(QStringLiteral("version")) && microsoftVersion.indexIn(text) >= 0)
                xml[QStringLiteral("version")] = microsoftVersion.cap(2);
            if (!xml.contains(QStringLiteral("subversion")) && !microsoftVersion.cap(1).isEmpty())
                xml[QStringLiteral("subversion")] = microsoftVersion.cap(1);

            if (text.contains(QStringLiteral("Macintosh")) || text.contains(QStringLiteral("Mac OS X")))
                xml[QStringLiteral("opsys")] = QStringLiteral("macosx");
            else
                xml[QStringLiteral("opsys")] = QStringLiteral("windows?");
        }
    }

    if (checkOOoVersion) {
        /// Looks like "Win32/2.3.1"
        static const QRegExp OOoVersion1("[a-z]/(\\d(\\.\\d+)+)(_beta|pre)?[$a-z]", Qt::CaseInsensitive);
        if (OOoVersion1.indexIn(text) >= 0)
            xml[QStringLiteral("version")] = OOoVersion1.cap(1);
        else {
            /// Fallback: conventional version string like "3.0"
            static const QRegExp OOoVersion2("\\b\\d+(\\.\\d+)+\\b", Qt::CaseInsensitive);
            if (OOoVersion2.indexIn(text) >= 0)
                xml[QStringLiteral("version")] = OOoVersion2.cap(0);
        }

        if (text.indexOf(QStringLiteral("unix")) >= 0)
            xml[QStringLiteral("opsys")] = QStringLiteral("generic-unix");
        else if (text.indexOf(QStringLiteral("linux")) >= 0)
            xml[QStringLiteral("opsys")] = QStringLiteral("linux");
        else if (text.indexOf(QStringLiteral("win32")) >= 0)
            xml[QStringLiteral("opsys")] = QStringLiteral("windows");
        else if (text.indexOf(QStringLiteral("solaris")) >= 0)
            xml[QStringLiteral("opsys")] = QStringLiteral("solaris");
        else if (text.indexOf(QStringLiteral("freebsd")) >= 0)
            xml[QStringLiteral("opsys")] = QStringLiteral("bsd");
    }

    if (!xml.contains(QStringLiteral("manufacturer")) && (text.contains(QStringLiteral("adobe")) || text.contains(QStringLiteral("acrobat"))))
        xml[QStringLiteral("manufacturer")] = QStringLiteral("adobe");

    if (!xml.contains(QStringLiteral("opsys"))) {
        /// automatically guess operating system
        if (text.contains(QStringLiteral("macint")))
            xml[QStringLiteral("opsys")] = QStringLiteral("macosx");
        else if (text.contains(QStringLiteral("solaris")))
            xml[QStringLiteral("opsys")] = QStringLiteral("solaris");
        else if (text.contains(QStringLiteral("linux")))
            xml[QStringLiteral("opsys")] = QStringLiteral("linux");
        else if (text.contains(QStringLiteral("windows")) || text.contains(QStringLiteral("win32")) || text.contains(QStringLiteral("win64")))
            xml[QStringLiteral("opsys")] = QStringLiteral("windows");
    }

    const QString result = DocScan::formatMap(QStringLiteral("name"), xml);

    return result;
}
