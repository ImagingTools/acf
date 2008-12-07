<?xml version="1.0" standalone="no"?>
<xsl:stylesheet xmlns:xsl = "http://www.w3.org/1999/XSL/Transform" version = "1.0" >
	<xsl:param name = "SourcePath">..</xsl:param>
	<xsl:param name = "CompilerCode"/>

	<xsl:template match = "Tool" mode="IncludePath">
		<xsl:param name = "UserParam" select="''"/>

		<xsl:call-template name = "ParsePathList">
			<xsl:with-param name = "ToParse" select="@AdditionalIncludeDirectories"/>
			<xsl:with-param name = "Mode" select="'EnvOnly'"/>
			<xsl:with-param name = "UserParam" select="$UserParam"/>
		</xsl:call-template>
	</xsl:template>

	<xsl:template match = "Tool" mode="LinkerPath">
		<xsl:param name = "UserParam" select="''"/>

		<xsl:call-template name = "ParsePathList">
			<xsl:with-param name = "ToParse" select="@AdditionalLibraryDirectories"/>
			<xsl:with-param name = "Mode" select="'EnvOnly'"/>
			<xsl:with-param name = "UserParam" select="$UserParam"/>
		</xsl:call-template>
	</xsl:template>

	<xsl:template match = "Tool" mode="LinkerLib">
		<xsl:param name = "UserParam" select="''"/>

		<xsl:call-template name = "ParsePathList">
			<xsl:with-param name = "ToParse" select="@AdditionalDependencies"/>
			<xsl:with-param name = "Mode" select="'Lib'"/>
			<xsl:with-param name = "Separator" select="' '"/>
			<xsl:with-param name = "UserParam" select="$UserParam"/>
		</xsl:call-template>
	</xsl:template>

	<xsl:template name = "ParsePathList">
		<!--Three modes are supported: 'Normal', 'EnvOnly' and 'Lib'-->
		<xsl:param name = "ToParse"/>
		<xsl:param name = "Mode" select="'Normal'"/>
		<xsl:param name = "Separator" select="';'"/>
		<xsl:param name = "UserParam" select="''"/>

		<xsl:if test="string-length($ToParse)>1">
			<xsl:choose>
				<xsl:when test="contains($ToParse, $Separator)">
					<xsl:variable name = "incString" select="substring-before($ToParse, $Separator)"/>
					<xsl:call-template name = "InsertListValue">
						<xsl:with-param name = "ToInsert" select="$incString"/>
						<xsl:with-param name = "Mode" select="$Mode"/>
						<xsl:with-param name = "UserParam" select="$UserParam"/>
					</xsl:call-template>
					<xsl:call-template name = "ParsePathList">
						<xsl:with-param name = "ToParse" select="substring-after($ToParse, $Separator)"/>
						<xsl:with-param name = "Mode" select="$Mode"/>
						<xsl:with-param name = "Separator" select="$Separator"/>
						<xsl:with-param name = "UserParam" select="$UserParam"/>
					</xsl:call-template>
				</xsl:when>
				<xsl:otherwise>
					<xsl:call-template name = "InsertListValue">
						<xsl:with-param name = "ToInsert" select="$ToParse"/>
						<xsl:with-param name = "Mode" select="$Mode"/>
						<xsl:with-param name = "UserParam" select="$UserParam"/>
					</xsl:call-template>
			</xsl:otherwise>
			</xsl:choose>
		</xsl:if>
	</xsl:template>

	<xsl:template name = "InsertListValue">
		<xsl:param name = "ToInsert"/>
		<xsl:param name = "Mode" select="'Normal'"/>
		<xsl:param name = "UserParam" select="''"/>

		<xsl:variable name = "CorrectedPath">
			<xsl:choose>
				<xsl:when test = "$Mode='Lib'">
					<xsl:choose>
						<xsl:when test="contains($ToInsert, '.lib')">
							<xsl:value-of select="translate(concat('lib', substring-before($ToInsert, '.lib'), '.a'), '()\', '{}/')"/>
						</xsl:when>
						<xsl:when test="contains($ToInsert, '.dll')">
							<xsl:value-of select="translate(concat('lib', substring-before($ToInsert, '.lib'), '.prl'), '()\', '{}/')"/>
						</xsl:when>
					</xsl:choose>
				</xsl:when>
				<xsl:otherwise>
					<xsl:choose>
						<xsl:when test="contains($ToInsert, 'VC8')">
							<xsl:value-of select="translate(concat(substring-before($ToInsert, 'VC8'), '$CompilerCode' ,substring-after($ToInsert, 'VC8')), '()\', '{}/')"/>
						</xsl:when>
						<xsl:otherwise>
							<xsl:value-of select="translate($ToInsert, '()\', '{}/')"/>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>

		<xsl:if test="(string-length($CorrectedPath) > 0) and ($Mode!='EnvOnly' or contains($CorrectedPath, '$'))">
			<xsl:call-template name = "InsertValue">
				<xsl:with-param name = "Path" select="$CorrectedPath"/>
			</xsl:call-template>
		</xsl:if>
	</xsl:template>

	<xsl:template match = "File" mode="Sources">
		<xsl:param name = "UserParam" select="''"/>

		<xsl:variable name = "FilePath">
			<xsl:value-of select = "substring-after(@RelativePath,'..\')"/>
		</xsl:variable>
		<xsl:variable name = "FileName">
			<xsl:choose>
				<xsl:when test = "contains(@RelativePath,'generated\')">
					<xsl:value-of select = "substring-after(@RelativePath,'generated\')"/>
				</xsl:when>
				<xsl:when test = "starts-with(@RelativePath,'..\')">
					<xsl:value-of select = "substring-after(@RelativePath,'..\')"/>
				</xsl:when>
				<xsl:otherwise>
					<xsl:value-of select = "@RelativePath"/>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:variable name = "GroupName">
			<xsl:choose>
				<xsl:when test = "contains(@RelativePath,'generated\')">
					Generated
				</xsl:when>
				<xsl:when test = "contains(@RelativePath,'.h')">
					Headers
				</xsl:when>
				<xsl:when test = "contains(@RelativePath,'.cpp')">
					Sources
				</xsl:when>
				<xsl:when test = "contains(@RelativePath,'.ui')">
					Forms
				</xsl:when>
				<xsl:when test = "contains(@RelativePath,'.qrc')">
					Resources
				</xsl:when>
				<xsl:otherwise>
					Garbages
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:if test = "starts-with(@RelativePath, '..\')">
			<xsl:call-template name = "InsertSourceFile">
				<xsl:with-param name = "Name" select="$FileName"/>
				<xsl:with-param name = "FilePath" select="concat($SourcePath, '/', $FilePath)"/>
				<xsl:with-param name = "GroupName" select="normalize-space($GroupName)"/>
				<xsl:with-param name = "UserParam" select="$UserParam"/>
			</xsl:call-template>
		</xsl:if>
	</xsl:template>
</xsl:stylesheet> 
