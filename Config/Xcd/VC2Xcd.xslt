<?xml version="1.0"?>
<xsl:stylesheet xmlns:xsl = "http://www.w3.org/1999/XSL/Transform" version = "1.0" >
	<xsl:import href="C:/Work/Projects/Development/ACF/Config/VC8/VCHelper.xslt"/>

	<xsl:output method = "text" indent="yes" encoding="utf-8"/>
	<xsl:strip-space elements="*"/>

	<xsl:template match = "*">
		<xsl:param name = "UserParam" select="''"/>
		<xsl:apply-templates>
			<xsl:with-param name="UserParam" select="$UserParam"/>
		</xsl:apply-templates>
	</xsl:template>

	<xsl:template match = "VisualStudioProject">
		<xsl:variable name="LowercaseName" select="translate(//VisualStudioProject/@Name, 'ABCDEFGHIJKLMNOPQRSTUVWXYZ.', 'abcdefghijklmnopqrstuvwxyz_')"/>
// !$*UTF8*$!
{
	archiveVersion = 1;
	classes = {
	};
	objectVersion = 44;
	objects = {

/* Begin PBXBuildFile section */
<xsl:apply-templates mode="Sources" select="Files/Filter/*">
	<xsl:with-param name="UserParam" select="'PBXBuildFile'"/>
</xsl:apply-templates>
/* End PBXBuildFile section */

/* Begin PBXFileReference section */
<xsl:apply-templates mode="Sources" select="Files/Filter/*">
	<xsl:with-param name="UserParam" select="'PBXFileReference'"/>
</xsl:apply-templates>		D2AAC046055464E500DB518D /* lib<xsl:value-of select="$LowercaseName"/>.a */ = {isa = PBXFileReference; explicitFileType = archive.ar; includeInIndex = 0; path = lib<xsl:value-of select="$LowercaseName"/>.a; sourceTree = BUILT_PRODUCTS_DIR; };
/* End PBXFileReference section */

/* Begin PBXFrameworksBuildPhase section */
		D289987405E68DCB004EDB86 /* Frameworks */ = {
			isa = PBXFrameworksBuildPhase;
			buildActionMask = 2147483647;
			files = (
			);
			runOnlyForDeploymentPostprocessing = 0;
		};
/* End PBXFrameworksBuildPhase section */

/* Begin PBXGroup section */
		08FB7794FE84155DC02AAC07 /* <xsl:value-of select="$LowercaseName"/> */ = {
			isa = PBXGroup;
			children = (
				EAB5AC950ED2E587006EC826 /* Header */,
				08FB7795FE84155DC02AAC07 /* Source */,
				EA9856A50EEABE3D00C738EF /* Generated */,
				EA9856AA0EEABE3D00C738EF /* Resources */,
				EA9856A60EEABE5300C738EF /* Garbages */,
				C6A0FF2B0290797F04C91782 /* Forms */,
				1AB674ADFE9D54B511CA2CBB /* Products */,
			);
			name = <xsl:value-of select="$LowercaseName"/>;
			sourceTree = "&lt;group&gt;";
		};
		08FB7795FE84155DC02AAC07 /* Source */ = {
			isa = PBXGroup;
			children = (
<xsl:apply-templates mode="Sources" select="Files/Filter/*">
	<xsl:with-param name="UserParam" select="'.Sources'"/>
</xsl:apply-templates>			);
			name = Source;
			sourceTree = "&lt;group&gt;";
		};
		1AB674ADFE9D54B511CA2CBB /* Products */ = {
			isa = PBXGroup;
			children = (
				D2AAC046055464E500DB518D /* lib<xsl:value-of select="$LowercaseName"/>.a */,
			);
			name = Products;
			sourceTree = "&lt;group&gt;";
		};
		C6A0FF2B0290797F04C91782 /* Forms */ = {
			isa = PBXGroup;
			children = (
<xsl:apply-templates mode="Sources" select="Files/Filter/*">
	<xsl:with-param name="UserParam" select="'.Forms'"/>
</xsl:apply-templates>			);
			name = Forms;
			sourceTree = "&lt;group&gt;";
		};
		EA9856A50EEABE3D00C738EF /* Generated */ = {
			isa = PBXGroup;
			children = (
<xsl:apply-templates mode="Sources" select="Files/Filter/*">
	<xsl:with-param name="UserParam" select="'.Generated'"/>
</xsl:apply-templates>			);
			name = Generated;
			sourceTree = "&lt;group&gt;";
		};
		EA9856AA0EEABE3D00C738EF /* Resources */ = {
			isa = PBXGroup;
			children = (
<xsl:apply-templates mode="Sources" select="Files/Filter/*">
	<xsl:with-param name="UserParam" select="'.Resources'"/>
</xsl:apply-templates>			);
			name = Resources;
			sourceTree = "&lt;group&gt;";
		};
		EA9856A60EEABE5300C738EF /* Garbages */ = {
			isa = PBXGroup;
			children = (
<xsl:apply-templates mode="Sources" select="Files/Filter/*">
	<xsl:with-param name="UserParam" select="'.Garbages'"/>
</xsl:apply-templates>			);
			name = Garbages;
			sourceTree = "&lt;group&gt;";
		};
		EAB5AC950ED2E587006EC826 /* Header */ = {
			isa = PBXGroup;
			children = (
<xsl:apply-templates mode="Sources" select="Files/Filter/*">
	<xsl:with-param name="UserParam" select="'.Headers'"/>
</xsl:apply-templates>			);
			name = Header;
			sourceTree = "&lt;group&gt;";
		};
/* End PBXGroup section */

/* Begin PBXHeadersBuildPhase section */
		D2AAC043055464E500DB518D /* Headers */ = {
			isa = PBXHeadersBuildPhase;
			buildActionMask = 2147483647;
			files = (
<xsl:apply-templates mode="Sources" select="Files/Filter/*">
	<xsl:with-param name="UserParam" select="'Group.Headers'"/>
</xsl:apply-templates>			);
			runOnlyForDeploymentPostprocessing = 0;
		};
/* End PBXHeadersBuildPhase section */

/* Begin PBXNativeTarget section */
		D2AAC045055464E500DB518D /* <xsl:value-of select="$LowercaseName"/> */ = {
			isa = PBXNativeTarget;
			buildConfigurationList = 1DEB91EB08733DB70010E9CD /* Build configuration list for PBXNativeTarget "<xsl:value-of select="$LowercaseName"/>" */;
			buildPhases = (
				EA6981130EEBF38E00FE4110 /* ShellScript */,
				D2AAC043055464E500DB518D /* Headers */,
				D2AAC044055464E500DB518D /* Sources */,
				D289987405E68DCB004EDB86 /* Frameworks */,
			);
			buildRules = (
			);
			dependencies = (
			);
			name = <xsl:value-of select="$LowercaseName"/>;
			productName = <xsl:value-of select="$LowercaseName"/>;
			productReference = D2AAC046055464E500DB518D /* lib<xsl:value-of select="$LowercaseName"/>.a */;
			productType = "com.apple.product-type.library.static";
		};
/* End PBXNativeTarget section */

/* Begin PBXProject section */
		08FB7793FE84155DC02AAC07 /* Project object */ = {
			isa = PBXProject;
			buildConfigurationList = 1DEB91EF08733DB70010E9CD /* Build configuration list for PBXProject "<xsl:value-of select="$LowercaseName"/>" */;
			compatibilityVersion = "Xcode 3.0";
			hasScannedForEncodings = 1;
			mainGroup = 08FB7794FE84155DC02AAC07 /* <xsl:value-of select="$LowercaseName"/> */;
			projectDirPath = "";
			projectRoot = "";
			targets = (
				D2AAC045055464E500DB518D /* <xsl:value-of select="$LowercaseName"/> */,
			);
		};
/* End PBXProject section */

/* Begin PBXShellScriptBuildPhase section */
		EA6981130EEBF38E00FE4110 /* ShellScript */ = {
			isa = PBXShellScriptBuildPhase;
			buildActionMask = 2147483647;
			comments = "UIC'ing...";
			files = (
			);
			inputPaths = (
<xsl:apply-templates mode="Sources" select="Files/Filter/*">
	<xsl:with-param name="UserParam" select="'PreprocessInput'"/>
</xsl:apply-templates>			);
			outputPaths = (
<xsl:apply-templates mode="Sources" select="Files/Filter/*">
	<xsl:with-param name="UserParam" select="'PreprocessOutput'"/>
</xsl:apply-templates>			);
			runOnlyForDeploymentPostprocessing = 0;
			shellPath = /bin/sh;
			shellScript = "<xsl:apply-templates mode="Sources" select="Files/Filter/*">
	<xsl:with-param name="UserParam" select="'PreprocessCommand'"/>
</xsl:apply-templates>";
		};
/* End PBXShellScriptBuildPhase section */

/* Begin PBXSourcesBuildPhase section */
		D2AAC044055464E500DB518D /* Sources */ = {
			isa = PBXSourcesBuildPhase;
			buildActionMask = 2147483647;
			files = (
<xsl:apply-templates mode="Sources" select="Files/Filter/*">
	<xsl:with-param name="UserParam" select="'Group.Sources'"/>
</xsl:apply-templates>			);
			runOnlyForDeploymentPostprocessing = 0;
		};
/* End PBXSourcesBuildPhase section */

/* Begin XCBuildConfiguration section */
		1DEB91EC08733DB70010E9CD /* Debug */ = {
			isa = XCBuildConfiguration;
			buildSettings = {
				COPY_PHASE_STRIP = NO;
				GCC_DYNAMIC_NO_PIC = NO;
				GCC_ENABLE_FIX_AND_CONTINUE = YES;
				GCC_MODEL_TUNING = G5;
				GCC_OPTIMIZATION_LEVEL = 0;
				INSTALL_PATH = /usr/local/lib;
				PRODUCT_NAME = <xsl:value-of select="$LowercaseName"/>;
				ZERO_LINK = YES;
			};
			name = Debug;
		};
		1DEB91ED08733DB70010E9CD /* Release */ = {
			isa = XCBuildConfiguration;
			buildSettings = {
				DEBUG_INFORMATION_FORMAT = "dwarf-with-dsym";
				GCC_MODEL_TUNING = G5;
				INSTALL_PATH = /usr/local/lib;
				PRODUCT_NAME = <xsl:value-of select="$LowercaseName"/>;
			};
			name = Release;
		};
		1DEB91F008733DB70010E9CD /* Debug */ = {
			isa = XCBuildConfiguration;
			buildSettings = {
				GCC_WARN_ABOUT_RETURN_TYPE = YES;
				GCC_WARN_UNUSED_VARIABLE = YES;
				HEADER_SEARCH_PATHS = (
					"../..",
					/Library/Frameworks/QtCore.framework/Headers,
					/Library/Frameworks/QtGui.framework/Headers,
					/Library/Frameworks/QtNetwork.framework/Headers,
					/Library/Frameworks/QtScript.framework/Headers,
					/Library/Frameworks/QtXml.framework/Headers,
				);
				PREBINDING = NO;
				SDKROOT = "$(DEVELOPER_SDK_DIR)/MacOSX10.5.sdk";
			};
			name = Debug;
		};
		1DEB91F108733DB70010E9CD /* Release */ = {
			isa = XCBuildConfiguration;
			buildSettings = {
				ARCHS = (
					ppc,
					i386,
				);
				GCC_WARN_ABOUT_RETURN_TYPE = YES;
				GCC_WARN_UNUSED_VARIABLE = YES;
				HEADER_SEARCH_PATHS = (
					"../..",
					/Library/Frameworks/QtCore.framework/Headers,
					/Library/Frameworks/QtGui.framework/Headers,
					/Library/Frameworks/QtNetwork.framework/Headers,
					/Library/Frameworks/QtScript.framework/Headers,
					/Library/Frameworks/QtXml.framework/Headers,
				);
				PREBINDING = NO;
				SDKROOT = "$(DEVELOPER_SDK_DIR)/MacOSX10.5.sdk";
			};
			name = Release;
		};
/* End XCBuildConfiguration section */

/* Begin XCConfigurationList section */
		1DEB91EB08733DB70010E9CD /* Build configuration list for PBXNativeTarget "<xsl:value-of select="$LowercaseName"/>" */ = {
			isa = XCConfigurationList;
			buildConfigurations = (
				1DEB91EC08733DB70010E9CD /* Debug */,
				1DEB91ED08733DB70010E9CD /* Release */,
			);
			defaultConfigurationIsVisible = 0;
			defaultConfigurationName = Release;
		};
		1DEB91EF08733DB70010E9CD /* Build configuration list for PBXProject "<xsl:value-of select="$LowercaseName"/>" */ = {
			isa = XCConfigurationList;
			buildConfigurations = (
				1DEB91F008733DB70010E9CD /* Debug */,
				1DEB91F108733DB70010E9CD /* Release */,
			);
			defaultConfigurationIsVisible = 0;
			defaultConfigurationName = Release;
		};
/* End XCConfigurationList section */
	};
	rootObject = 08FB7793FE84155DC02AAC07 /* Project object */;
}
	</xsl:template>

	<!--Implementation of imported helper stylesheet-->

	<xsl:template name = "InsertSourceFile">
		<xsl:param name = "Name"/>
		<xsl:param name = "FilePath"/>
		<xsl:param name = "GroupName" select="Sources"/>
		<xsl:param name = "UserParam"/>

		<xsl:variable name="BuildId" select="position() + 22345678"/>
		<xsl:variable name="FileId" select="position() + 12345678"/>

		<xsl:if test = "($GroupName='Sources') or ($GroupName='Headers') or ($GroupName='Generated') or ($GroupName='Garbages') or ($GroupName='Forms') or ($GroupName='Resources')">
			<xsl:choose>
				<xsl:when test="$UserParam = 'PreprocessInput'">
					<xsl:choose>
						<xsl:when test="contains($Name, '.ui') or contains($Name, '.qrc')">
							<xsl:text>				</xsl:text>
							<xsl:value-of select="$FilePath"/>
							<xsl:text>,
</xsl:text>
						</xsl:when>
						<xsl:when test="contains($Name, '.moc')">
							<xsl:text>				</xsl:text>
							<xsl:value-of select="concat(substring-before($FilePath, '.moc'), '.h')"/>
							<xsl:text>,
</xsl:text>
						</xsl:when>
					</xsl:choose>
				</xsl:when>
				<xsl:when test="$UserParam = 'PreprocessOutput'">
					<xsl:choose>
						<xsl:when test="contains($Name, '.ui')">
							<xsl:text>				../Generated/</xsl:text>
							<xsl:value-of select="concat('ui_', substring-before($Name, '.'), '.h')"/>
							<xsl:text>,
</xsl:text>
						</xsl:when>
						<xsl:when test="contains($Name, '.moc')">
							<xsl:text>				../Generated/</xsl:text>
							<xsl:value-of select="concat('moc_', substring-before($Name, '.'), '.cpp')"/>
							<xsl:text>,
</xsl:text>
						</xsl:when>
						<xsl:when test="contains($Name, '.qrc')">
							<xsl:text>				../Generated/</xsl:text>
							<xsl:value-of select="concat('qrc_', substring-before($Name, '.'), '.cpp')"/>
							<xsl:text>,
</xsl:text>
						</xsl:when>
					</xsl:choose>
				</xsl:when>
				<xsl:when test="$UserParam = 'PreprocessCommand'">
					<xsl:choose>
						<xsl:when test="contains($Name, '.ui')">
							<xsl:text>uic -o ../Generated/</xsl:text>
							<xsl:value-of select="concat('ui_', substring-before($Name, '.'), '.h')"/>
							<xsl:text> </xsl:text>
							<xsl:value-of select="$FilePath"/>
							<xsl:text>\n</xsl:text>
						</xsl:when>
						<xsl:when test="contains($Name, '.moc')">
							<xsl:text>moc -o ../Generated/</xsl:text>
							<xsl:value-of select="concat('moc_', substring-before($Name, '.'), '.cpp')"/>
							<xsl:text> </xsl:text>
							<xsl:value-of select="concat(substring-before($FilePath, '.moc'), '.h')"/>
							<xsl:text>\n</xsl:text>
						</xsl:when>
						<xsl:when test="contains($Name, '.qrc')">
							<xsl:text>rcc</xsl:text>
							<xsl:text> -name </xsl:text>
							<xsl:value-of select="substring-before($Name, '.')"/>
							<xsl:text> -o ../Generated/</xsl:text>
							<xsl:value-of select="concat('qrc_', substring-before($Name, '.'), '.cpp')"/>
							<xsl:text> </xsl:text>
							<xsl:value-of select="$FilePath"/>
							<xsl:text>\n</xsl:text>
						</xsl:when>
					</xsl:choose>
				</xsl:when>
				<xsl:when test="$UserParam = 'PBXBuildFile'">
					<xsl:text>		</xsl:text>
					<xsl:value-of select="$BuildId"/>
					<xsl:text>0ED2E572006EC826 /* </xsl:text>
					<xsl:value-of select="$Name"/>
					<xsl:text> in </xsl:text>
					<xsl:value-of select="$GroupName"/>
					<xsl:text> */ = {isa = PBXBuildFile; fileRef = </xsl:text>
					<xsl:value-of select="$FileId"/>
					<xsl:text>0ED2E572006EC826 /* </xsl:text>
					<xsl:value-of select="$Name"/>
					<xsl:text> */; };
</xsl:text>
				</xsl:when>
				<xsl:when test="$UserParam = 'PBXFileReference'">
					<xsl:text>		</xsl:text>
					<xsl:value-of select="$FileId"/>
					<xsl:text>0ED2E572006EC826 /* </xsl:text>
					<xsl:value-of select="$Name"/>
					<xsl:text> */ = {isa = PBXFileReference; fileEncoding = 4; lastKnownFileType = sourcecode.cpp.cpp; name = </xsl:text>
					<xsl:value-of select="$Name"/>
					<xsl:text>; path = </xsl:text>
					<xsl:value-of select="translate($FilePath, '\', '/')"/>
					<xsl:text>; sourceTree = SOURCE_ROOT; };
</xsl:text>
				</xsl:when>
				<xsl:when test="$UserParam = concat('.', $GroupName)">
					<xsl:text>				</xsl:text>
					<xsl:value-of select="$FileId"/>
					<xsl:text>0ED2E572006EC826 /* </xsl:text>
					<xsl:value-of select="$Name"/>
					<xsl:text> */,
</xsl:text>
				</xsl:when>
				<xsl:when test="$UserParam = 'Group.Headers'">
					<xsl:if test="contains($Name, '.h') or contains($Name, '.H')">
						<xsl:text>				</xsl:text>
						<xsl:value-of select="$BuildId"/>
						<xsl:text>0ED2E572006EC826 /* </xsl:text>
						<xsl:value-of select="$Name"/>
						<xsl:text> in Headers */,
</xsl:text>
					</xsl:if>
				</xsl:when>
				<xsl:when test="$UserParam = 'Group.Sources'">
					<xsl:if test="contains($Name, '.c') or contains($Name, '.C')">
						<xsl:text>				</xsl:text>
						<xsl:value-of select="$BuildId"/>
						<xsl:text>0ED2E572006EC826 /* </xsl:text>
						<xsl:value-of select="$Name"/>
						<xsl:text> in Sources */,
</xsl:text>
					</xsl:if>
				</xsl:when>
			</xsl:choose>
		</xsl:if>
	</xsl:template>

	<xsl:template name = "InsertValue">
	</xsl:template>
</xsl:stylesheet>
