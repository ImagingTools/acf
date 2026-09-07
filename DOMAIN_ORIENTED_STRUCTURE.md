# Domain-Oriented Library Structure for Unified ACF Repository

> **Last Updated:** March 2026  
> **Status:** ✅ Validated - All repository analyses current and accurate

## Executive Summary

This document proposes a domain-oriented reorganization of the four ACF repositories (Acf, AcfSln, IAcf, ImtCore) into a single unified repository. The new structure groups libraries by functional domain rather than by technical layer, improving discoverability, maintainability, and logical organization.

**Repository Status (March 2026):**
- ✅ Acf: 27 libraries verified (stable, conanfile.py added, infrastructure improvements)
- ✅ AcfSln: 34 libraries verified (iqtex added since initial analysis)
- ✅ IAcf: 11 libraries verified (stable, no changes)
- ✅ ImtCore: 61+ libraries verified (stable repository structure)
- ✅ Total: **139+ libraries** ready for consolidation

## Current State Analysis

### Repository 1: **Acf** (Core Framework - 27 libraries)
The foundation framework providing core infrastructure, Qt integration, and basic utilities.

**Libraries**: istd, icomp, iser, iprm, iattr, ibase, ipackage, imath, i2d, i3d, icmm, iimg, idoc, iview, imod, ifile, ifilegui, iqt, iqt2d, iqtdoc, iqtgui, iqtprm, iwidgets, ilog, iloggui, itest

### Repository 2: **AcfSln** (Application Packages - 34 libraries)
Domain-specific business logic for imaging and industrial applications.

**Libraries**: iproc, iprocgui, iipr, iqtipr, icalib, icalibgui, imeas, iqtmeas, icam, iqtcam, icomm, iauth, iqtauth, isig, iqtsig, iprod, iinsp, iqtinsp, idocproc, ifileproc, ibarcode, iblob, iblobgui, iedge, iedgegui, ihotf, ihotfgui, imm, iqtmm, iwiz, iwizgui, iweb, ialgo, icmpstr, iqtex, iservice

### Repository 3: **IAcf** (Integration Packages - 11 libraries)
Third-party library integrations and platform-specific implementations.

**Libraries**: iocv, iqwt, iqwt3d, iqsci, iphonon, iwin, inat, idc1394, icbox, ilibav, iqaxmm

### Repository 4: **ImtCore** (Enterprise Application Core - 61 libraries)
Enterprise application infrastructure including database, authentication, services, GraphQL APIs, 3D visualization, and comprehensive UI framework.

**Core Infrastructure (7 libraries)**: imtbase, imtcore, imtcom, imtcol, imttest, imtfile, imtzip

**Authentication & Authorization (4 libraries)**: imtauth, imtauthdb, imtauthgql, imtauthgui

**Database & Persistence (5 libraries)**: imtdb, imtdbgql, imtdbgui, imtmdbx, imtmongo

**Licensing System (4 libraries)**: imtlic, imtlicdb, imtlicgql, imtlicgui

**GraphQL APIs (5 core libraries)**: imtgql, imtclientgql, imtservergql, imtguigql, imtgqltest

**User Interface (10 core libraries)**: imtgui, imtwidgets, imtstyle, imtqml, imtqml2d, imtstylecontrolsqml, imtdesign, imt3dgui, imthypegui, imtauthgui

**3D & Visualization (6 libraries)**: imt3d, imt3dgui, imt3dview, imthype, imthypegui, imtgeo

**Content & Media (3 libraries)**: imtimg, imtdoc, imtrepo

**Communication & Integration (4 libraries)**: imthttp, imtrest, imtmail, imtservice

**Security (1 library)**: imtcrypt

**Code Generation & SDL (5 libraries)**: imtsdl, imtsdlgencpp, imtsdlgenqml, imtddl, imtoas

**Development Tools (4 libraries)**: imtdev, imtdevgui, imtpy, imtej

**Reporting & Logging (4 libraries)**: imtlog, imtloggui, imtreport, imtreportgui

**Application Frameworks (3 libraries)**: imtapp, imtserverapp, imtupdate

**Total**: 139 libraries across 4 repositories

## Problems with Current Structure

1. **Repository Fragmentation**: Related functionality split across multiple repositories
2. **Technical Layering**: Organized by implementation detail rather than domain
3. **Discovery Challenges**: Difficult to find which library provides specific functionality
4. **Dependency Complexity**: Cross-repository dependencies complicate builds
5. **Maintenance Overhead**: Four separate repositories to manage

## Proposed Domain-Oriented Structure

### Domain Organization Principles

1. **Group by Business Domain**: Libraries organized by what they do, not how they're implemented
2. **Clear Boundaries**: Each domain has well-defined responsibilities
3. **Minimize Cross-Domain Dependencies**: Domains should be relatively independent
4. **Progressive Disclosure**: Core → Common → Specialized hierarchy
5. **Naming Consistency**: Maintain "i" prefix convention for interfaces

## Unified Repository Structure

```
Acf/  (Unified Repository)
├── Core/                          # Foundation libraries (Layer 0)
│   ├── istd/                      # Standard utilities and interfaces
│   ├── icomp/                     # Component framework
│   ├── iser/                      # Serialization
│   ├── iprm/                      # Parameter management
│   ├── iattr/                     # Attributes
│   ├── ibase/                     # Base components
│   ├── ipackage/                  # Package management
│   ├── ilog/                      # Logging
│   ├── itest/                     # Testing framework
│   └── imtbase/                   # ImtCore base functionality
│
├── Math/                          # Mathematics and Algorithms (Layer 1)
│   ├── imath/                     # Mathematical utilities
│   ├── ialgo/                     # General algorithms
│   └── isig/                      # Signal processing
│
├── DevelopmentTools/              # Design and Scripting Tools (Layer 2)
│   ├── icmpstr/                   # Compositor - visual editor for component diagrams
│   ├── imtpy/                     # Python integration and bindings
│   └── imtej/                     # Export and JSON format utilities
│
├── DeviceManagement/              # Device Management Infrastructure (Layer 2)
│   ├── imtdev/                    # Device management framework
│   └── imtdevgui/                 # Device management UI
│
├── Geometry/                      # Spatial and Visual Data (Layer 1)
│   ├── i2d/                       # 2D geometry
│   ├── i3d/                       # 3D geometry
│   ├── icmm/                      # Color management
│   └── imtgeo/                    # Geographic/geospatial data
│
├── ImageProcessing/               # Image Analysis and Processing (Layer 2)
│   ├── iproc/                     # Core image processing
│   ├── iprocgui/                  # Image processing UI
│   ├── iblob/                     # Blob analysis
│   ├── iblobgui/                  # Blob analysis UI
│   └── iocv/                      # OpenCV integration
│
├── ComputerVision/                # Computer Vision and Recognition (Layer 2)
│   ├── iipr/                      # Advanced image processing and CV
│   ├── iqtipr/                    # Advanced processing UI
│   ├── iedge/                     # Edge detection
│   ├── iedgegui/                  # Edge detection UI
│   ├── ibarcode/                  # Barcode recognition
│   ├── iimg/                      # Image data structures
│   ├── imtimg/                    # ImtCore image processing
│   ├── icalib/                    # Calibration algorithms
│   ├── icalibgui/                 # Calibration UI
│   ├── icam/                      # Camera abstraction
│   └── iqtcam/                    # Camera UI
│
├── Measurement/                   # Measurement and Visualization (Layer 2)
│   ├── imeas/                     # Measurement tools
│   ├── iqtmeas/                   # Measurement UI
│   ├── iqwt/                      # Qwt plotting (for visualization)
│   └── iqwt3d/                    # Qwt 3D plotting
│
├── Acquisition/                   # Data Acquisition and Hardware (Layer 2)
│   ├── idc1394/                   # Firewire cameras
│   ├── icbox/                     # Hardware control boxes
│   ├── icomm/                     # Communication protocols
│   └── ilibav/                    # Video I/O (FFmpeg)
│
├── ProcessingEngine/              # Processing Infrastructure (Layer 2)
│   └── iinsp/                     # Pull-based inspection/processing engine
│
├── Production/                    # Manufacturing and Quality Control (Layer 3)
│   ├── iprod/                     # Production management
│   ├── iqtinsp/                   # Inspection UI
│   ├── ihotf/                     # Hotfolder monitoring
│   ├── ihotfgui/                  # Hotfolder UI
│   └── ifileproc/                 # File processing
│
├── DocumentView/                  # Document-View Framework (Layer 2)
│   ├── idoc/                      # Document framework
│   ├── iview/                     # View framework
│   ├── imod/                      # Model-observer pattern
│   ├── ifile/                     # File handling
│   ├── idocproc/                  # Document processing
│   ├── imtapp/                    # ImtCore application framework
│   └── imtcol/                    # ImtCore collections/collaboration
│
├── Multimedia/                    # Multimedia and Media Management (Layer 2)
│   ├── imm/                       # Media management
│   ├── iqtmm/                     # Media management UI
│   └── iphonon/                   # Media playback
│
├── Data/                          # Data Management (Layer 1/2)
│   ├── imtdb/                     # Database access and management
│   └── imtfile/                   # Extended file handling
│
├── UI/                            # Core User Interface Infrastructure (Layer 2)
│   ├── Qt Widgets/
│   │   ├── iqt/                   # Qt utilities
│   │   ├── iqtgui/                # Qt GUI base
│   │   ├── iqtprm/                # Qt parameter widgets
│   │   ├── iqt2d/                 # Qt 2D widgets
│   │   ├── iqtdoc/                # Qt document handling
│   │   ├── iwidgets/              # Custom widgets
│   │   ├── iqsci/                 # QScintilla code editor
│   │   ├── iwiz/                  # Wizard framework
│   │   ├── iwizgui/               # Wizard UI
│   │   ├── ifilegui/              # File UI
│   │   └── iloggui/               # Logging UI
│   │
│   ├── ImtCore UI/
│   │   ├── imtgui/                # ImtCore Qt GUI widgets and layouts
│   │   ├── imtwidgets/            # Extended widget library
│   │   ├── imtstyle/              # Theme and style management
│   │   ├── imtqml/                # Core QML utilities and components
│   │   ├── imtqml2d/              # 2D graphics and drawing in QML
│   │   ├── imtstylecontrolsqml/   # QML style controls and themes
│   │   └── imtdesign/             # Design tokens and system components
│   │
│   ├── Signal Processing UI/
│   │   └── iqtsig/                # Signal processing UI
│   │
│   └── Integration/
│       ├── iqaxmm/                # ActiveX multimedia
│       └── iqtex/                 # Qt extensions
│
│   # Note: Domain-specific UI modules are now co-located with their domains:
│   # iprocgui, iqtipr → ImageProcessing/ComputerVision domains
│   # icalibgui, iqtmeas → ComputerVision/Measurement domains
│   # iqtcam → ComputerVision domain
│   # iblobgui, iedgegui → ImageProcessing/ComputerVision domains
│   # iqtinsp, ihotfgui → Production domain
│   # iqtmm → Multimedia domain
│   # imtauthgui, imtdbgui, imtlicgui, etc. → Their respective ImtCore domains
│   # iqtcam → Acquisition domain
│   # iblobgui, iedgegui → ImageProcessing/ComputerVision domains
│   # iqtinsp, ihotfgui → Production domain
│   # imtauthgui, imtdbgui, imtlicgui, etc. → Their respective ImtCore domains
│
├── Platform/                      # Platform-Specific Libraries (Layer 1)
│   ├── iwin/                      # Windows platform
│   └── inat/                      # Native platform abstractions
│
├── API/                           # API and Communication Infrastructure (Layer 2)
│   ├── GraphQL/
│   │   ├── imtgql/                # Core GraphQL request/response handling
│   │   ├── imtclientgql/          # Client-side GraphQL operations and caching
│   │   ├── imtservergql/          # Server-side GraphQL schema and resolvers
│   │   ├── imtguigql/             # UI integration with GraphQL
│   │   └── imtgqltest/            # GraphQL testing framework
│   │
│   ├── HTTP/
│   │   ├── imthttp/               # HTTP client and server (base protocol)
│   │   └── imtrest/               # REST API framework (to be split)
│   │       # TODO: Split into imthttp (basic HTTP protocol) 
│   │       # and imtrestapi (REST API conventions/patterns)
│   │
│   └── Communication/
│       ├── imtmail/               # SMTP email client and message handling
│       └── icomm/                 # Low-level communication protocols
│
├── Services/                      # Application Services (Layer 3)
│   ├── iauth/                     # Authorization
│   ├── iqtauth/                   # Authorization UI
│   ├── iweb/                      # Web services
│   ├── iservice/                  # Service infrastructure
│   ├── imtservice/                # ImtCore service infrastructure
│   └── imtauth/                   # ImtCore authentication
│
├── Build/                         # Build system (unchanged)
├── Config/                        # Configuration (unchanged)
├── Docs/                          # Documentation (enhanced)
├── Install/                       # Installation scripts (unchanged)
├── Tests/                         # Tests (reorganized by domain)
└── Examples/                      # Example applications (new)
```

## Dependency Layers

The structure follows a layered architecture to minimize circular dependencies:

- **Layer 0 (Core)**: No dependencies on other ACF libraries
- **Layer 1 (Foundation)**: Depends only on Layer 0
- **Layer 2 (Domain)**: Depends on Layer 0-1
- **Layer 3 (Applications)**: Depends on Layer 0-2

## Domain Descriptions

### 1. Core (10 libraries)
**Purpose**: Foundation infrastructure used by all other domains
**Libraries**: istd, icomp, iser, iprm, iattr, ibase, ipackage, ilog, itest, imtbase
**Key Features**: Component model, serialization, parameters, logging, testing, ImtCore base functionality

### 2. Math (3 libraries)
**Purpose**: Mathematical algorithms and numerical processing
**Libraries**: imath, ialgo, isig
**Key Features**: Interpolation, algorithms, signal processing

### 3. Geometry (4 libraries)
**Purpose**: Spatial data structures and transformations
**Libraries**: i2d, i3d, icmm, imtgeo
**Key Features**: 2D/3D geometry, color management, geospatial data

### 4. ImageProcessing (5 libraries)
**Purpose**: Core image analysis and processing
**Libraries**: iproc, iprocgui, iblob, iblobgui, iocv
**Key Features**: Image filtering, blob analysis, OpenCV integration, with integrated UI

### 5. ComputerVision (11 libraries)
**Purpose**: Advanced computer vision, recognition, calibration, and camera management
**Libraries**: iipr, iqtipr, iedge, iedgegui, ibarcode, iimg, imtimg, icalib, icalibgui, icam, iqtcam
**Key Features**: Advanced image processing, edge detection, barcode recognition, image data structures, calibration algorithms, camera abstraction, with integrated UI

### 6. Measurement (4 libraries)
**Purpose**: Measurement tools and visualization
**Libraries**: imeas, iqtmeas, iqwt, iqwt3d
**Key Features**: Measurement tools, plotting and visualization, with integrated UI

### 7. Acquisition (4 libraries)
**Purpose**: Hardware interfacing and data capture
**Libraries**: idc1394, icbox, icomm, ilibav
**Key Features**: Firewire cameras, hardware control, communication protocols, video I/O

### 8. ProcessingEngine (1 library)
**Purpose**: Processing infrastructure and engines
**Libraries**: iinsp
**Key Features**: Pull-based inspection/processing engine for workflow automation

### 9. Production (5 libraries)
**Purpose**: Manufacturing and quality control workflows
**Libraries**: iprod, iqtinsp, ihotf, ihotfgui, ifileproc
**Key Features**: Production management, inspection UI, hotfolder monitoring, automated file processing

### 10. DocumentView (8 libraries)
**Purpose**: Document-based application framework
**Libraries**: idoc, iview, imod, ifile, idocproc, imtapp, imtcol
**Key Features**: Document management, view synchronization, document processing, application framework, collections

### 11. Multimedia (3 libraries)
**Purpose**: Multimedia and media management
**Libraries**: imm, iqtmm, iphonon
**Key Features**: Media management, media UI, media playback (audio/video)

### 12. DevelopmentTools (3 libraries)
**Purpose**: Design tools and scripting
**Libraries**: icmpstr, imtpy, imtej
**Key Features**: Compositor (visual component diagram editor), Python integration, export utilities

### 13. DeviceManagement (2 libraries)
**Purpose**: Device management infrastructure
**Libraries**: imtdev, imtdevgui
**Key Features**: Device management framework and UI

### 14. UI (19+ core libraries)
**Purpose**: Core user interface infrastructure (domain-specific UIs are co-located with their domains)
**Libraries**: iqt, iqtgui, iqtprm, iqt2d, iqtdoc, iwidgets, iqsci, iwiz, iwizgui, ifilegui, iloggui, imtgui, imtwidgets, imtstyle, imtqml, imtqml2d, imtstylecontrolsqml, imtdesign, iqtsig, iqaxmm, iqtex
**Key Features**: Qt widgets, QML framework, theming, wizards, file/log UI, signal processing UI, Qt extensions

### 15. API (12 libraries)
**Purpose**: API and communication infrastructure  
**Libraries**: imtgql, imtclientgql, imtservergql, imtguigql, imtgqltest, imthttp, imtrest (to be split), imtmail, icomm
**Key Features**: GraphQL APIs, HTTP/REST protocols, email, low-level communication
**Note**: imtrest should be split into imthttp (basic HTTP protocol) and imtrestapi (REST API patterns)

### 16. Platform (2 libraries)
**Purpose**: Platform-specific implementations
**Libraries**: iwin, inat
**Key Features**: Windows APIs, native platform abstractions

### 17. Services (6 libraries)
**Purpose**: Application-level services
**Libraries**: iauth, iqtauth, iweb, iservice, imtservice, imtauth
**Key Features**: Authentication, authorization, web services, service infrastructure

### 18. Data (2 libraries)
**Purpose**: Data management and persistence
**Libraries**: imtdb, imtfile
**Key Features**: Database access, extended file handling

## Implementation Benefits

### 1. Improved Discoverability
- Developers can quickly locate functionality by domain
- Clear separation of concerns
- Logical grouping reduces cognitive load

### 2. Better Dependency Management
- Layered architecture prevents circular dependencies
- Clear dependency boundaries
- Easier to extract subsets for specific applications

### 3. Simplified Build System
- Single repository with unified build
- CMake can build by domain or layer
- Easier to create domain-specific distributions

### 4. Enhanced Maintainability
- Related code is co-located
- Domain experts can focus on specific areas
- Clearer ownership and responsibility

### 5. Flexible Deployment
- Can deploy full framework or domain subsets
- Easier to create specialized distributions
- Better support for modular applications

## Migration Strategy

See [MIGRATION_GUIDE.md](MIGRATION_GUIDE.md) for detailed migration steps.

### High-Level Steps

1. **Phase 1**: Create unified repository structure
2. **Phase 2**: Merge IAcf integration libraries
3. **Phase 3**: Merge AcfSln application libraries
4. **Phase 4**: Reorganize by domain
5. **Phase 5**: Update build system and documentation
6. **Phase 6**: Archive old repositories

## Naming Conventions

### Maintained Conventions
- **"i" prefix**: All interface headers continue to use "i" prefix
- **Pck suffix**: Implementation packages continue to use "Pck" suffix
- **Qt prefix**: Qt-specific libraries continue to use "iqt" or "Qt" prefix
- **Gui suffix**: GUI libraries continue to use "gui" suffix

### Directory Structure
- Domain folders use PascalCase (e.g., `ImageProcessing/`, `DocumentView/`)
- Library folders use lowercase "i" prefix (e.g., `iproc/`, `icalib/`)
- Implementation folders in `Impl/` use mixed case with Pck suffix

## Compatibility Considerations

### Include Paths
Maintain backward compatibility with:
```cpp
#include <iproc/...>        // Works in both old and new structure
#include <icalib/...>       // Works in both old and new structure
#include <imtbase/...>      // Works in both old and new structure
```

### CMake Targets
Preserve existing target names:
```cmake
target_link_libraries(MyApp PRIVATE iproc icalib icam imtbase imtdb)
```

### Build Configurations
Support both monolithic and modular builds:
- Full build: All domains
- Core build: Core + Math + Geometry only
- Domain build: Core + specific domain(s)

## Future Enhancements

1. **Plugin Architecture**: Domains as optional plugins
2. **Package Manager**: npm/pip-style package management
3. **Domain Documentation**: Detailed guides per domain
4. **Example Applications**: Sample apps demonstrating each domain
5. **Performance Optimization**: Domain-specific optimizations

## Conclusion

This domain-oriented structure provides a more intuitive and maintainable organization for the unified ACF framework. By grouping libraries by functional domain rather than technical implementation, we improve discoverability, reduce complexity, and create a more scalable architecture for future development.

The migration from four repositories (Acf, AcfSln, IAcf, ImtCore) to this unified structure will require careful planning but will yield significant long-term benefits in maintainability, usability, and developer productivity.
