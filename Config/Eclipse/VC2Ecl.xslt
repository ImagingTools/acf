<?xml version="1.0"?>
<xsl:stylesheet xmlns:xsl = "http://www.w3.org/1999/XSL/Transform" version = "1.0" >
	<xsl:param name = "SourcePath">..</xsl:param>

	<xsl:output method = "xml"/>

	<xsl:template match = "*">
		<xsl:apply-templates/>
	</xsl:template>

	<xsl:template match = "VisualStudioProject">
		<xsl:element name = "projectDescription">
			<xsl:element name = "name">
				<xsl:value-of select = "@Name" />
			</xsl:element>
			<xsl:element name = "comment"/>
			<xsl:element name = "project"/>
			<xsl:element name = "buildSpec">
				<xsl:element name = "buildCommand">
					<xsl:element name = "name">org.eclipse.cdt.managedbuilder.core.genmakebuilder</xsl:element>
					<xsl:element name = "triggers">clean,full,incremental,</xsl:element>
					<xsl:element name = "arguments">
						<xsl:element name = "dictionary">
							<xsl:element name = "key">?name?</xsl:element>
							<xsl:element name = "value"></xsl:element>
						</xsl:element>
						<xsl:element name = "dictionary">
							<xsl:element name = "key">org.eclipse.cdt.make.core.append_environment</xsl:element>
							<xsl:element name = "value">true</xsl:element>
						</xsl:element>
						<xsl:element name = "dictionary">
							<xsl:element name = "key">org.eclipse.cdt.make.core.buildArguments</xsl:element>
							<xsl:element name = "value"></xsl:element>
						</xsl:element>
						<xsl:element name = "dictionary">
							<xsl:element name = "key">org.eclipse.cdt.make.core.buildCommand</xsl:element>
							<xsl:element name = "value">make</xsl:element>
						</xsl:element>
						<xsl:element name = "dictionary">
							<xsl:element name = "key">org.eclipse.cdt.make.core.buildLocation</xsl:element>
							<xsl:element name = "value">${workspace_loc:/iser/Debug}</xsl:element>
						</xsl:element>
						<xsl:element name = "dictionary">
							<xsl:element name = "key">org.eclipse.cdt.make.core.contents</xsl:element>
							<xsl:element name = "value">org.eclipse.cdt.make.core.activeConfigSettings</xsl:element>
						</xsl:element>
						<xsl:element name = "dictionary">
							<xsl:element name = "key">org.eclipse.cdt.make.core.enableAutoBuild</xsl:element>
							<xsl:element name = "value">false</xsl:element>
						</xsl:element>
						<xsl:element name = "dictionary">
							<xsl:element name = "key">org.eclipse.cdt.make.core.enableCleanBuild</xsl:element>
							<xsl:element name = "value">true</xsl:element>
						</xsl:element>
						<xsl:element name = "dictionary">
							<xsl:element name = "key">org.eclipse.cdt.make.core.enableFullBuild</xsl:element>
							<xsl:element name = "value">true</xsl:element>
						</xsl:element>
						<xsl:element name = "dictionary">
							<xsl:element name = "key">org.eclipse.cdt.make.core.stopOnError</xsl:element>
							<xsl:element name = "value">true</xsl:element>
						</xsl:element>
						<xsl:element name = "dictionary">
							<xsl:element name = "key">org.eclipse.cdt.make.core.useDefaultBuildCmd</xsl:element>
							<xsl:element name = "value">true</xsl:element>
						</xsl:element>
					</xsl:element>
				</xsl:element>
				<xsl:element name = "buildCommand">
					<xsl:element name = "name">org.eclipse.cdt.managedbuilder.core.ScannerConfigBuilder</xsl:element>
					<xsl:element name = "arguments"></xsl:element>
				</xsl:element>
			</xsl:element>
			<xsl:element name = "natures">
				<xsl:element name = "nature">org.eclipse.cdt.core.ccnature</xsl:element>
				<xsl:element name = "nature">org.eclipse.cdt.managedbuilder.core.ScannerConfigNature</xsl:element>
				<xsl:element name = "nature">org.eclipse.cdt.managedbuilder.core.managedBuildNature</xsl:element>
				<xsl:element name = "nature">org.eclipse.cdt.core.cnature</xsl:element>
			</xsl:element>
			<xsl:element name = "linkedResources">
				<xsl:apply-templates/>
			</xsl:element>
		</xsl:element>
	</xsl:template>

	<xsl:template match = "File">
		<xsl:variable name = "FilePath"><xsl:value-of select = "substring-after(@RelativePath,'..\')"/></xsl:variable>
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
				<xsl:otherwise>
					Garbages
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>

		<xsl:if test = "starts-with(@RelativePath,'..\')">
			<xsl:element name = "link">
				<xsl:element name = "name">
					<xsl:value-of select = "normalize-space($GroupName)"/><xsl:text>/</xsl:text><xsl:value-of select = "$FileName"/>
				</xsl:element>
				<xsl:element name = "type">1</xsl:element>
				<xsl:element name = "locationURI">
					<xsl:value-of select = "$SourcePath"/>
					<xsl:text>/</xsl:text>
					<xsl:value-of select = "translate($FilePath,'\','/')"/>
				</xsl:element>
			</xsl:element>
		</xsl:if>
	</xsl:template>
</xsl:stylesheet> 
