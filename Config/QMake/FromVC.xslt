<?xml version="1.0"?>
<xsl:stylesheet xmlns:xsl = "http://www.w3.org/1999/XSL/Transform" version = "1.0" >
	<xsl:import href="../VC8/VCHelper.xslt"/>

	<xsl:output method = "text" indent="yes" encoding="utf-8"/>
	<xsl:strip-space elements="*"/>

	<xsl:param name = "SpecialCharReplace" select = "'()/'"/>
	<xsl:param name = "ExtraIncludes"></xsl:param>
	<xsl:param name = "ProjectIncludes"></xsl:param>

	<xsl:template match = "*">
		<xsl:param name = "UserParam" select="''"/>

		<xsl:apply-templates>
			<xsl:with-param name="UserParam" select="$UserParam"/>
		</xsl:apply-templates>
	</xsl:template>

	<xsl:template match = "VisualStudioProject">
		<xsl:variable name="ProductName">
			<xsl:value-of select="translate(//VisualStudioProject/@Name, '.', '_')"/>
		</xsl:variable>
		<xsl:variable name = "OutputDir">
			<xsl:choose>
				<xsl:when test="Configurations/Configuration/@ConfigurationType='1'">"../../../Bin/$(CONFIGURATION)Xcd"</xsl:when>
				<xsl:when test="Configurations/Configuration/@ConfigurationType='2'">"../../../Bin/$(CONFIGURATION)Xcd"</xsl:when>
				<xsl:when test="Configurations/Configuration/@ConfigurationType='4'">"../../../Lib/$(CONFIGURATION)Xcd"</xsl:when>
				<xsl:otherwise>$(BUILD_DIR)/$(CONFIGURATION)</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:text>TARGET = </xsl:text><xsl:value-of select="$ProductName"/>
		<xsl:choose>
			<xsl:when test="Configurations/Configuration/@ConfigurationType='1'">
				<xsl:text>
TEMPLATE = app
</xsl:text>
			</xsl:when>
			<xsl:when test="Configurations/Configuration/@ConfigurationType='2'">
				<xsl:text>
TEMPLATE = lib
</xsl:text>
				<xsl:text>
CONFIG += dll
</xsl:text>
				<xsl:if test = "contains(Configurations/Configuration/Tool/@OutputFile, '.arp')">
					<xsl:text>
TARGET_EXT = arp
</xsl:text>
				</xsl:if>
			</xsl:when>
			<xsl:when test="Configurations/Configuration/@ConfigurationType='4'">
				<xsl:text>
TEMPLATE = lib
CONFIG += staticlib
</xsl:text>
			</xsl:when>
		</xsl:choose>
		<xsl:text>
debug {
</xsl:text>
		<xsl:if test="contains(Configurations/Configuration[2]/@OutputDirectory, '$(ConfigurationName)VC8')">
			<xsl:text>	DESTDIR = </xsl:text><xsl:value-of select="concat(substring-before(Configurations/Configuration[2]/@OutputDirectory, '$(ConfigurationName)VC8'), 'DebugQMake')"/>
			<xsl:text>
</xsl:text>
		</xsl:if>
		<xsl:if test="not(Configurations/Configuration/@ConfigurationType='4')">
			<xsl:text>	LIBS += </xsl:text>
			<xsl:apply-templates mode="LinkerPath" select="Configurations/Configuration[2]/*">
				<xsl:with-param name = "UserParam" select="'DebugQMake'"/>
			</xsl:apply-templates>
			<xsl:text>
</xsl:text>
			<xsl:text>	LIBS += </xsl:text>
			<xsl:apply-templates mode="LinkerLib" select="Configurations/Configuration[2]/*"/>
			<xsl:text>
</xsl:text>
		</xsl:if>
		<xsl:text>}
release {
</xsl:text>
		<xsl:if test="contains(Configurations/Configuration[1]/@OutputDirectory, '$(ConfigurationName)VC8')">
			<xsl:text>	DESTDIR = </xsl:text><xsl:value-of select="concat(substring-before(Configurations/Configuration[2]/@OutputDirectory, '$(ConfigurationName)VC8'), 'ReleaseQMake')"/>
			<xsl:text>
</xsl:text>
		</xsl:if>
		<xsl:if test="not(Configurations/Configuration/@ConfigurationType='4')">
			<xsl:text>	LIBS += </xsl:text>
			<xsl:apply-templates mode="LinkerPath" select="Configurations/Configuration[1]/*">
				<xsl:with-param name = "UserParam" select="'ReleaseQMake'"/>
			</xsl:apply-templates>
			<xsl:text>
</xsl:text>
			<xsl:text>	LIBS += </xsl:text>
			<xsl:apply-templates mode="LinkerLib" select="Configurations/Configuration[1]/*"/>
			<xsl:text>
</xsl:text>
		</xsl:if>
		<xsl:text>}

UI_DIR = ../Generated
MOC_DIR = ../Generated
RCC_DIR = ../Generated

QT += </xsl:text>
		<xsl:apply-templates mode="LinkerLib" select="Configurations/Configuration[1]/*">
			<xsl:with-param name = "UserParam" select="'QT'"/>
		</xsl:apply-templates>
		<xsl:text>

</xsl:text>
		<xsl:call-template name = "ParsePathList">
			<xsl:with-param name = "ToParse" select="$ProjectIncludes"/>
			<xsl:with-param name = "Mode" select="'ProjIncludes'"/>
		</xsl:call-template>
		<xsl:text>
</xsl:text>
		<xsl:text>INCLUDEPATH += </xsl:text>
		<xsl:call-template name = "ParsePathList">
			<xsl:with-param name = "ToParse" select="$ExtraIncludes"/>
		</xsl:call-template>
		<xsl:apply-templates mode ="IncludePath" select="Configurations/Configuration[1]">
		</xsl:apply-templates>
		<xsl:text>

</xsl:text>
		<xsl:apply-templates mode="Sources" select="Files/Filter/*">
			<xsl:with-param name="UserParam" select="'.Headers'"/>
		</xsl:apply-templates>

		<xsl:apply-templates mode="Sources" select="Files/Filter/*">
			<xsl:with-param name="UserParam" select="'.Sources'"/>
		</xsl:apply-templates>

		<xsl:apply-templates mode="Sources" select="Files/Filter/*">
			<xsl:with-param name="UserParam" select="'.Forms'"/>
		</xsl:apply-templates>

		<xsl:apply-templates mode="Sources" select="Files/Filter/*">
			<xsl:with-param name="UserParam" select="'.Resources'"/>
		</xsl:apply-templates>
	</xsl:template>

	<!--Implementation of imported helper stylesheet-->

	<xsl:template name = "InsertSourceFile">
		<xsl:param name = "Name"/>
		<xsl:param name = "FilePath"/>
		<xsl:param name = "GroupName" select="Sources"/>
		<xsl:param name = "UserParam"/>

		<xsl:if test="$UserParam = concat('.', $GroupName)">
			<xsl:choose>
				<xsl:when test="$GroupName='Sources'">
					<xsl:text>SOURCES += </xsl:text>
					<xsl:value-of select="$FilePath"/>
					<xsl:text>
</xsl:text>
				</xsl:when>
				<xsl:when test="$GroupName='Headers'">
					<xsl:text>HEADERS += </xsl:text>
					<xsl:value-of select="$FilePath"/>
					<xsl:text>
</xsl:text>
				</xsl:when>
				<xsl:when test="$GroupName='Forms'">
					<xsl:text>FORMS += </xsl:text>
					<xsl:value-of select="$FilePath"/>
					<xsl:text>
</xsl:text>
				</xsl:when>
				<xsl:when test="$GroupName='Resources'">
					<xsl:text>RESOURCES += </xsl:text>
					<xsl:value-of select="$FilePath"/>
					<xsl:text>
</xsl:text>
				</xsl:when>
			</xsl:choose>
		</xsl:if>
	</xsl:template>

	<xsl:template name = "InsertValue">
		<xsl:param name = "Path"/>
		<xsl:param name = "Mode"/>
		<xsl:param name = "UserParam" select="''"/>
		<xsl:variable name="SmallName" select="translate($Path, 'ABCDEFGHIJKLMNOPQRSTUVWXYZ.', 'abcdefghijklmnopqrstuvwxyz_')"/>

		<xsl:choose>
			<xsl:when test = "$Mode='ProjIncludes'">
				<xsl:text>include(</xsl:text>
				<xsl:value-of select="$Path"/>
				<xsl:text>)
</xsl:text>
			</xsl:when>
			<xsl:when test = "$Mode='Lib'">
				<xsl:choose>
					<xsl:when test="$UserParam='QT'">
						<xsl:if test="starts-with($SmallName, 'qt')">
							<xsl:if test="contains($SmallName, '4')">
								<xsl:value-of select="substring-after(substring-before($SmallName, '4'), 'qt')"/>
							</xsl:if>
							<xsl:if test="not(contains($SmallName, '4'))">
								<xsl:value-of select="substring-after($SmallName, 'qt')"/>
							</xsl:if>
							<xsl:text> </xsl:text>
						</xsl:if>
					</xsl:when>
					<xsl:otherwise>
						<xsl:if test="not(starts-with($SmallName, 'qt'))">
							<xsl:value-of select="$Path"/>
							<xsl:text> </xsl:text>
						</xsl:if>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:when>
			<xsl:otherwise>
				<xsl:if test = "not(contains($Path, '(QTDIR)') or contains($Path, 'Generated'))">
					<xsl:if test = "$Mode='LinkerPath'">
						<xsl:text>-L</xsl:text>
					</xsl:if>
					<xsl:if test="contains($Path, '$(ConfigurationName)')">
						<xsl:value-of select = "substring-before($Path, '$(ConfigurationName)')"/>
						<xsl:value-of select = "$UserParam"/>
					</xsl:if>
					<xsl:if test = "not(contains($Path, '$(ConfigurationName)'))">
						<xsl:value-of select = "$Path"/>
					</xsl:if>
					<xsl:text> </xsl:text>
				</xsl:if>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>
</xsl:stylesheet>
