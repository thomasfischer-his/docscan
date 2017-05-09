<xsl:stylesheet version = '1.0' xmlns:xsl='http://www.w3.org/1999/XSL/Transform'>
<xsl:output method="text" omit-xml-declaration="yes" indent="no" encoding="UTF-8"/>

<!--
  Print the unique PDF version numbers and the number of PDF documents that are of
  each version.
-->

<xsl:key name="pdfversions" match="fileformat" use="version" />

<xsl:template match="log">

<!-- column header  -->
<xsl:text>pdfversion	file count</xsl:text>

<xsl:text>
total	</xsl:text>
<xsl:value-of select="count(/log/logitem/fileanalysis[@status='ok' and meta/fileformat/version])" />
<xsl:text>
</xsl:text>

<xsl:for-each select="/log/logitem/fileanalysis[@status='ok']/meta/fileformat[generate-id()=generate-id(key('pdfversions',version))]">
<xsl:sort select="version"/>
<xsl:variable name="cmpto" select="version" />
<xsl:value-of select="version" />
<xsl:text>	</xsl:text>
<xsl:value-of select="count(/log/logitem/fileanalysis[@status='ok' and meta/fileformat/version=$cmpto])" />
<xsl:text>
</xsl:text>
</xsl:for-each>

</xsl:template><!-- match="log" -->

</xsl:stylesheet>
