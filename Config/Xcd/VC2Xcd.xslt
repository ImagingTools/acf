<?xml version="1.0"?>
<xsl:stylesheet xmlns:xsl = "http://www.w3.org/1999/XSL/Transform" version = "1.0" >
	<xsl:import href="C:/Work/Projects/Development/ACF/Config/VC8/VCHelper.xslt"/>

	<xsl:output method = "text" indent="yes" encoding="utf-8"/>
	<xsl:strip-space elements="*"/>

	<xsl:template match = "*">
		<xsl:param name = "UserParam" select="''"/>
		
		Bla
		<xsl:apply-templates>
			<xsl:with-param name="UserParam" select="$UserParam"/>
		</xsl:apply-templates>
	</xsl:template>

	<xsl:template match = "VisualStudioProject">
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
		D2AAC046055464E500DB518D /* lib<xsl:value-of select="//VisualStudioProject/@Name"/>.a */ = {isa = PBXFileReference; explicitFileType = archive.ar; includeInIndex = 0; path = lib<xsl:value-of select="//VisualStudioProject/@Name"/>.a; sourceTree = BUILT_PRODUCTS_DIR; };
<xsl:apply-templates mode="Sources" select="Files/Filter/*">
	<xsl:with-param name="UserParam" select="'PBXFileReference'"/>
</xsl:apply-templates>
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
	08FB7794FE84155DC02AAC07 /* istd */ = {
	isa = PBXGroup;
	children = (
	EAB5AC950ED2E587006EC826 /* Header */,
	08FB7795FE84155DC02AAC07 /* Source */,
	C6A0FF2B0290797F04C91782 /* Documentation */,
	1AB674ADFE9D54B511CA2CBB /* Products */,
	);
	name = istd;
	sourceTree = "&lt;group&gt;";
		};
		08FB7795FE84155DC02AAC07 /* Source */ = {
			isa = PBXGroup;
			children = (
<xsl:apply-templates mode="Sources" select="Files/Filter/*">
	<xsl:with-param name="UserParam" select="'.Sources'"/>
</xsl:apply-templates>
	);
	name = Source;
	sourceTree = "&lt;group&gt;";
		};
		1AB674ADFE9D54B511CA2CBB /* Products */ = {
			isa = PBXGroup;
			children = (
				D2AAC046055464E500DB518D /* lib<xsl:value-of select="//VisualStudioProject/@Name"/>.a */,
			);
			name = Products;
			sourceTree = "&lt;group&gt;";
		};
		C6A0FF2B0290797F04C91782 /* Documentation */ = {
			isa = PBXGroup;
			children = (
			);
			name = Documentation;
			sourceTree = "&lt;group&gt;";
		};
		EAB5AC950ED2E587006EC826 /* Header */ = {
			isa = PBXGroup;
			children = (
<xsl:apply-templates mode="Sources" select="Files/Filter/*">
	<xsl:with-param name="UserParam" select="'.Headers'"/>
</xsl:apply-templates>
	);
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
</xsl:apply-templates>
			);
			runOnlyForDeploymentPostprocessing = 0;
		};
/* End PBXHeadersBuildPhase section */

/* Begin PBXNativeTarget section */
		D2AAC045055464E500DB518D /* istd */ = {
			isa = PBXNativeTarget;
			buildConfigurationList = 1DEB91EB08733DB70010E9CD /* Build configuration list for PBXNativeTarget "istd" */;
			buildPhases = (
				D2AAC043055464E500DB518D /* Headers */,
				D2AAC044055464E500DB518D /* Sources */,
				D289987405E68DCB004EDB86 /* Frameworks */,
			);
			buildRules = (
			);
			dependencies = (
			);
			name = istd;
			productName = istd;
			productReference = D2AAC046055464E500DB518D /* lib<xsl:value-of select="//VisualStudioProject/@Name"/>.a */;
			productType = "com.apple.product-type.library.static";
		};
/* End PBXNativeTarget section */

/* Begin PBXProject section */
		08FB7793FE84155DC02AAC07 /* Project object */ = {
			isa = PBXProject;
			buildConfigurationList = 1DEB91EF08733DB70010E9CD /* Build configuration list for PBXProject "istd" */;
			compatibilityVersion = "Xcode 3.0";
			hasScannedForEncodings = 1;
			mainGroup = 08FB7794FE84155DC02AAC07 /* istd */;
			projectDirPath = "";
			projectRoot = "";
			targets = (
				D2AAC045055464E500DB518D /* istd */,
			);
		};
/* End PBXProject section */

/* Begin PBXSourcesBuildPhase section */
		D2AAC044055464E500DB518D /* Sources */ = {
			isa = PBXSourcesBuildPhase;
			buildActionMask = 2147483647;
			files = (
<xsl:apply-templates mode="Sources" select="Files/Filter/*">
	<xsl:with-param name="UserParam" select="'Group.Sources'"/>
</xsl:apply-templates>
			);
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
				PRODUCT_NAME = istd;
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
				PRODUCT_NAME = istd;
			};
			name = Release;
		};
		1DEB91F008733DB70010E9CD /* Debug */ = {
			isa = XCBuildConfiguration;
			buildSettings = {
				GCC_WARN_ABOUT_RETURN_TYPE = YES;
				GCC_WARN_UNUSED_VARIABLE = YES;
				HEADER_SEARCH_PATHS = "/Users/Witold/Work/Projects/Development/ACF/**";
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
				HEADER_SEARCH_PATHS = "/Users/Witold/Work/Projects/Development/ACF/**";
				PREBINDING = NO;
				SDKROOT = "$(DEVELOPER_SDK_DIR)/MacOSX10.5.sdk";
			};
			name = Release;
		};
/* End XCBuildConfiguration section */

/* Begin XCConfigurationList section */
		1DEB91EB08733DB70010E9CD /* Build configuration list for PBXNativeTarget "istd" */ = {
			isa = XCConfigurationList;
			buildConfigurations = (
				1DEB91EC08733DB70010E9CD /* Debug */,
				1DEB91ED08733DB70010E9CD /* Release */,
			);
			defaultConfigurationIsVisible = 0;
			defaultConfigurationName = Release;
		};
		1DEB91EF08733DB70010E9CD /* Build configuration list for PBXProject "istd" */ = {
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

		<xsl:choose>
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
				<xsl:value-of select="$FilePath"/>
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
			<xsl:when test="$UserParam = concat('Group.', $GroupName)">
				<xsl:text>				</xsl:text>
				<xsl:value-of select="$BuildId"/>
				<xsl:text>0ED2E572006EC826 /* </xsl:text>
				<xsl:value-of select="$Name"/>
				<xsl:text> in </xsl:text>
				<xsl:value-of select="$GroupName"/>
				<xsl:text> */,
</xsl:text>
			</xsl:when>
		</xsl:choose>
	</xsl:template>

	<xsl:template name = "InsertValue">
	</xsl:template>
</xsl:stylesheet>
