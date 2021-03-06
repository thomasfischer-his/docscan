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

#ifndef FILEANALYZERMULTIPLEXER_H
#define FILEANALYZERMULTIPLEXER_H

#include "fileanalyzerabstract.h"
#ifdef HAVE_QUAZIP5
#include "fileanalyzerodf.h"
#include "fileanalyzeropenxml.h"
#include "fileanalyzerzip.h"
#endif // HAVE_QUAZIP5
#include "fileanalyzerpdf.h"
#ifdef HAVE_WV2
#include "fileanalyzercompoundbinary.h"
#endif // HAVE_WV2
#include "fileanalyzerjpeg.h"
#include "fileanalyzerjp2.h"
#include "fileanalyzertiff.h"

/**
 * Automatically redirects a file to be analyzed
 * to the right specialized file analysis object.
 *
 * @author Thomas Fischer <thomas.fischer@his.se>
 */
class FileAnalyzerMultiplexer : public FileAnalyzerAbstract
{
    Q_OBJECT
public:
    static const QStringList defaultFilters;

    explicit FileAnalyzerMultiplexer(const QStringList &filters, QObject *parent = nullptr);

    virtual bool isAlive() override;
    virtual void setTextExtraction(TextExtraction textExtraction) override;
    virtual void setAnalyzeEmbeddedFiles(bool enableEmbeddedFilesAnalysis) override;

    void setupJhove(const QString &shellscript);
    void setupVeraPDF(const QString &cliTool);
    void setupPdfBoXValidator(const QString &pdfboxValidatorJavaClass);
    void setupCallasPdfAPilotCLI(const QString &callasPdfAPilotCLI);
    void setupDPFManager(const QString &dpfmangerJFXjar);
    void setupAdobePreflightReportDirectory(const QString &adobePreflightReportDirectory);
    void setupQoppaJPDFPreflightDirectory(const QString &qoppaJPDFPreflightDirectory);
    void setupThreeHeightsValidatorShellCLI(const QString &threeHeightsValidatorShellCLI, const QString &threeHeightsValidatorLicenseKey);

    void setPDFAValidationOptions(const bool validateOnlyPDFAfiles, const bool downgradeToPDFA1b, const FileAnalyzerPDF::XMPPDFConformance enforcedValidationLevel);

public slots:
    virtual void analyzeFile(const QString &filename) override;

    /**
     * Schedule a temporary file for analysis. Being a temporary file,
     * erase this file after the analysis.
     *
     * @param filename filename for later analysis, to be deleted afterwards
     */
    void analyzeTemporaryFile(const QString &filename);

private:
#ifdef HAVE_QUAZIP5
    FileAnalyzerODF m_fileAnalyzerODF;
    FileAnalyzerOpenXML m_fileAnalyzerOpenXML;
    FileAnalyzerZIP m_fileAnalyzerZIP;
#endif // HAVE_QUAZIP5
    FileAnalyzerPDF m_fileAnalyzerPDF;
#ifdef HAVE_WV2
    FileAnalyzerCompoundBinary m_fileAnalyzerCompoundBinary;
#endif // HAVE_WV2
    FileAnalyzerJPEG m_fileAnalyzerJPEG;
    FileAnalyzerJP2 m_fileAnalyzerJP2;
    FileAnalyzerTIFF m_fileAnalyzerTIFF;
    const QStringList &m_filters;

    void uncompressAnalyzefile(const QString &filename, const QString &extension, const QString &uncompressTool);
};

#endif // FILEANALYZERMULTIPLEXER_H
