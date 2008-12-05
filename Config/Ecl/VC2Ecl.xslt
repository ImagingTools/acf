<?xml version="1.0"?>
<xsl:stylesheet xmlns:xsl = "http://www.w3.org/1999/XSL/Transform" version = "1.0" >
	<xsl:import href="C:/Work/Projects/Development/ACF/Config/VC8/VCHelper.xslt"/>

	<xsl:output method = "xml" indent="yes" encoding="utf-8"/>
	<xsl:strip-space elements="*"/>

	<xsl:template match = "*">
		<xsl:apply-templates/>
	</xsl:template>

	<xsl:template match = "VisualStudioProject">
		<projectDescription>
			<name><xsl:value-of select="//VisualStudioProject/@Name"/></name>
			<comment></comment>
			<projects>
			</projects>
			<buildSpec>
				<buildCommand>
					<name>org.eclipse.cdt.managedbuilder.core.genmakebuilder</name>
					<triggers>clean,full,incremental,</triggers>
					<arguments>
						<dictionary>
							<key>?name?</key>
							<value></value>
						</dictionary>
						<dictionary>
							<key>org.eclipse.cdt.make.core.append_environment</key>
							<value>true</value>
						</dictionary>
						<dictionary>
							<key>org.eclipse.cdt.make.core.buildArguments</key>
							<value></value>
						</dictionary>
						<dictionary>
							<key>org.eclipse.cdt.make.core.buildCommand</key>
							<value>make</value>
						</dictionary>
						<dictionary>
							<key>org.eclipse.cdt.make.core.buildLocation</key>
							<value>${workspace_loc:/<xsl:value-of select="//VisualStudioProject/@Name"/>/Debug}</value>
						</dictionary>
						<dictionary>
							<key>org.eclipse.cdt.make.core.contents</key>
							<value>org.eclipse.cdt.make.core.activeConfigSettings</value>
						</dictionary>
						<dictionary>
							<key>org.eclipse.cdt.make.core.enableAutoBuild</key>
							<value>false</value>
						</dictionary>
						<dictionary>
							<key>org.eclipse.cdt.make.core.enableCleanBuild</key>
							<value>true</value>
						</dictionary>
						<dictionary>
							<key>org.eclipse.cdt.make.core.enableFullBuild</key>
							<value>true</value>
						</dictionary>
						<dictionary>
							<key>org.eclipse.cdt.make.core.stopOnError</key>
							<value>true</value>
						</dictionary>
						<dictionary>
							<key>org.eclipse.cdt.make.core.useDefaultBuildCmd</key>
							<value>true</value>
						</dictionary>
					</arguments>
				</buildCommand>
				<buildCommand>
					<name>org.eclipse.cdt.managedbuilder.core.ScannerConfigBuilder</name>
					<arguments>
					</arguments>
				</buildCommand>
			</buildSpec>
			<natures>
				<nature>org.eclipse.cdt.core.ccnature</nature>
				<nature>org.eclipse.cdt.managedbuilder.core.ScannerConfigNature</nature>
				<nature>org.eclipse.cdt.managedbuilder.core.managedBuildNature</nature>
				<nature>org.eclipse.cdt.core.cnature</nature>
			</natures>
			<linkedResources>
				<xsl:apply-templates mode="Sources"/>
			</linkedResources>
		</projectDescription>
	</xsl:template>

	<!--Implementation of imported helper stylesheet-->

	<xsl:template name = "InsertSourceFile">
		<xsl:param name="Name"/>
		<xsl:param name="FilePath"/>
		<xsl:param name="GroupName" select="Sources"/>

		<xsl:element name = "link">
			<xsl:element name = "name">
				<xsl:value-of select = "concat($GroupName, '/', $Name)"/>
			</xsl:element>
			<xsl:element name = "type">1</xsl:element>
			<xsl:element name = "locationURI">
				<xsl:value-of select = "translate($FilePath, '\', '/')"/>
			</xsl:element>
		</xsl:element>
	</xsl:template>

	<xsl:template name = "InsertValue">
	</xsl:template>
</xsl:stylesheet> 
