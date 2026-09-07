# ACF Repository Consolidation Plan

> **Last Updated:** March 2026  
> **Status:** ✅ Validated - All 4 repositories analyzed and ready for consolidation

## Executive Summary

This document proposes consolidating four separate ACF repositories (Acf, AcfSln, IAcf, ImtCore) into a single unified repository with a domain-oriented structure. This consolidation will improve maintainability, discoverability, and developer experience while preserving all existing functionality.

## Current State

### Four Separate Repositories

1. **Acf** (Core Framework)
   - **Purpose**: Foundation framework and infrastructure
   - **Libraries**: 27 libraries
   - **Focus**: Core components, Qt integration, basic utilities
   - **Status**: ✅ Active, well-documented, conanfile.py infrastructure added

2. **AcfSln** (Application Packages)
   - **Purpose**: Domain-specific business logic
   - **Libraries**: 34 libraries (iqtex added since initial analysis)
   - **Focus**: Imaging, calibration, measurement, production
   - **Status**: ✅ Active, application-focused

3. **IAcf** (Integration Packages)
   - **Purpose**: Third-party integrations
   - **Libraries**: 11 libraries
   - **Focus**: OpenCV, Qwt, QScintilla, Phonon, platform-specific
   - **Status**: ✅ Active, integration-focused

4. **ImtCore** (Enterprise Application Core)
   - **Purpose**: Enterprise application infrastructure
   - **Libraries**: 61+ libraries
   - **Focus**: Enterprise features (auth, licensing, GraphQL APIs, 3D visualization, QML, code generation)
   - **Status**: ✅ Active, enterprise-focused

**Total**: 140 libraries across 4 repositories (27 + 34 + 11 + 61 + iqtex = 140, verified March 2026)

## Problems with Current Structure

### 1. Repository Fragmentation
- Related functionality scattered across four repositories
- Difficult to understand overall framework capabilities
- Complex dependency management between repositories
- Four separate build systems to maintain

### 2. Poor Discoverability
- Users don't know which repository contains needed functionality
- No clear organization by functional domain
- Technical layering doesn't match user mental models
- Documentation fragmented across repositories

### 3. Maintenance Overhead
- Four separate release cycles
- Duplicate infrastructure (CI/CD, docs, issues)
- Inconsistent versioning across repos
- Complex cross-repository changes

### 4. Dependency Challenges
- Cross-repository dependencies complicate builds
- Circular dependency risks
- Difficult to extract subsets of functionality
- Version synchronization issues

## Proposed Solution: Unified Domain-Oriented Repository

### Key Principles

1. **Single Source of Truth**: One repository for all ACF code
2. **Domain Organization**: Group by what libraries do, not how they're implemented
3. **Layered Architecture**: Clear dependency hierarchy
4. **Backward Compatibility**: Existing code continues to work
5. **Flexible Building**: Build full framework or domain subsets

### Proposed Domain Structure

```
Acf/ (Unified Repository)
├── Core/              # Foundation (10 libs) - Layer 0
├── Math/              # Mathematics (3 libs) - Layer 1
├── Geometry/          # Spatial data (4 libs) - Layer 1
├── Platform/          # Platform-specific (2 libs) - Layer 1
├── Data/              # Data management (2 libs) - Layer 1/2
├── ImageProcessing/   # Core image processing (5 libs) - Layer 2
├── ComputerVision/    # CV, calibration, cameras (11 libs) - Layer 2
├── Measurement/       # Measurement and viz (4 libs) - Layer 2
├── Acquisition/       # Hardware I/O (4 libs) - Layer 2
├── ProcessingEngine/  # Processing infrastructure (1 lib) - Layer 2
├── DocumentView/      # Document framework (8 libs) - Layer 2
├── Multimedia/        # Media management (3 libs) - Layer 2
├── DevelopmentTools/  # Design and scripting (3 libs) - Layer 2
├── DeviceManagement/  # Device management (2 libs) - Layer 2
├── UI/                # Core UI infrastructure (19+ libs) - Layer 2
├── API/               # APIs and communication (12 libs) - Layer 2
├── Production/        # Manufacturing (5 libs) - Layer 3
└── Services/          # Application services (6 libs) - Layer 3
```

### Domain Descriptions

**Layer 0 - Core (10 libraries)**
- istd, icomp, iser, iprm, iattr, ibase, ipackage, ilog, itest, imtbase
- No dependencies on other ACF libraries
- Foundation for everything else

**Layer 1 - Foundation (11 libraries)**
- Math: imath, ialgo, isig
- Geometry: i2d, i3d, icmm, imtgeo
- Platform: iwin, inat
- Data: imtdb, imtfile
- Depends only on Core

**Layer 2 - Domain Libraries (70+ libraries)**
- ImageProcessing: iproc, iprocgui, iblob, iblobgui, iocv (with integrated UI)
- ComputerVision: iipr, iqtipr, iedge, iedgegui, ibarcode, iimg, imtimg, icalib, icalibgui, icam, iqtcam (CV, calibration, cameras with integrated UI)
- Measurement: imeas, iqtmeas, iqwt, iqwt3d (measurement and visualization with integrated UI)
- Acquisition: idc1394, icbox, icomm, ilibav (hardware I/O)
- ProcessingEngine: iinsp (pull-based processing engine)
- DocumentView: idoc, iview, imod, ifile, idocproc, imtapp, imtcol
- Multimedia: imm, iqtmm, iphonon (media management and playback with integrated UI)
- DevelopmentTools: icmpstr (Compositor visual editor), imtpy, imtej
- DeviceManagement: imtdev, imtdevgui (device management framework and UI)
- UI: 19+ core UI libraries (Qt widgets, QML, theming, wizards, Qt extensions)
- API: GraphQL APIs, HTTP/REST (imtrest to be split), email, communication
- Depends on Core and Foundation

**Layer 3 - Applications (11 libraries)**
- Production: iprod, iqtinsp, ihotf, ihotfgui, ifileproc (manufacturing with integrated UI)
- Services: iauth, iqtauth, iweb, iservice, imtservice, imtauth
- Depends on Core, Foundation, and Domain layers

## Benefits of Unified Structure

### 1. Improved Discoverability
✅ Developers quickly locate functionality by domain
✅ Logical grouping reduces cognitive load
✅ Clear separation of concerns
✅ Single point of entry for new users

### 2. Better Dependency Management
✅ Layered architecture prevents circular dependencies
✅ Clear dependency boundaries
✅ Easier to extract subsets
✅ Simplified dependency resolution

### 3. Simplified Maintenance
✅ Single release cycle
✅ One CI/CD pipeline
✅ Unified documentation
✅ Consistent versioning

### 4. Enhanced Developer Experience
✅ Clone once, build everything
✅ Related code co-located
✅ Domain experts can focus
✅ Clearer ownership

### 5. Flexible Deployment
✅ Deploy full framework or domain subsets
✅ Create specialized distributions
✅ Support modular applications
✅ Easier plugin architecture

## Migration Strategy

### Timeline: 6 Weeks

**Week 1: Preparation & Core**
- Setup unified repository structure
- Migrate Core domain libraries
- Establish build patterns

**Week 2: Foundation Domains**
- Migrate Math, Geometry, Platform
- Update CMake configuration
- Test foundation builds

**Week 3: Application Domains & ImtCore**
- Migrate ImageProcessing, Calibration, Acquisition
- Migrate ImtCore libraries (imtbase, imtapp, imtdb, imtfile, imtservice, imtauth, imtcol)
- Test domain interactions
- Validate dependencies

**Week 4: UI & Services**
- Migrate UI domain (largest)
- Migrate Production and Services
- Complete library migration

**Week 5: Testing & Validation**
- Comprehensive testing
- Performance validation
- Backward compatibility verification
- Documentation updates

**Week 6: Finalization**
- Final review and polish
- Release preparation
- Archive old repositories
- Communication and rollout

### Backward Compatibility

**Include Paths Preserved**
```cpp
#include <iproc/ImageProcessor.h>  // Still works
#include <icalib/Calibration.h>    // Still works
#include <imtbase/ImtBase.h>       // Still works
```

**CMake Targets Unchanged**
```cmake
target_link_libraries(MyApp PRIVATE iproc icalib icam imtbase)  # Still works
```

**API Compatibility**
- No API changes
- No breaking changes
- Existing projects work without modification

### Risk Mitigation

1. **Comprehensive Backups**: Full backup of all three repositories
2. **Parallel Testing**: Test new structure while keeping old repos
3. **Gradual Migration**: Domain-by-domain approach
4. **Rollback Plan**: Clear rollback procedure documented
5. **Compatibility Layer**: Symlinks ensure old paths work

## Implementation Details

### CMake Build Options

```cmake
# Build everything
cmake -DACF_BUILD_ALL=ON

# Build specific domains
cmake -DACF_BUILD_CORE=ON \
      -DACF_BUILD_IMAGE_PROCESSING=ON \
      -DACF_BUILD_UI=ON

# Build layers
cmake -DACF_BUILD_LAYER_0=ON  # Core only
cmake -DACF_BUILD_LAYER_0_1=ON  # Core + Foundation
```

### Directory Structure

```
Acf/
├── Core/
│   ├── istd/
│   │   ├── CMakeLists.txt
│   │   ├── *.h
│   │   └── src/
│   ├── icomp/
│   └── ...
├── ImageProcessing/
│   ├── CMakeLists.txt
│   ├── iproc/
│   ├── iblob/
│   └── ...
├── Impl/               # Implementation packages
│   ├── BasePck/
│   ├── ProcessingPck/
│   └── ...
├── Build/              # Build scripts
├── Config/             # Configuration
├── Docs/               # Documentation
├── Tests/              # Tests (organized by domain)
├── Examples/           # Example applications (NEW)
└── Include-compat/     # Backward compatibility (symlinks)
```

### Documentation Updates

1. **Domain READMEs**: Each domain gets its own README
2. **Migration Guide**: Step-by-step migration instructions
3. **Architecture Guide**: Domain relationships and dependencies
4. **API Documentation**: Enhanced with domain organization
5. **Getting Started**: Updated for unified structure

## Success Criteria

### Functional Requirements
✅ All 71 libraries successfully migrated
✅ All tests pass in new structure
✅ Build time comparable or better
✅ Backward compatibility verified
✅ Documentation complete

### Quality Requirements
✅ No API breaking changes
✅ Clear domain boundaries
✅ No circular dependencies
✅ Consistent naming conventions
✅ Complete test coverage maintained

### Process Requirements
✅ Migration completed in 6 weeks
✅ Zero downtime for users
✅ Old repositories archived
✅ Communication plan executed
✅ Support documentation available

## Post-Migration Roadmap

### Phase 1: Stabilization (Month 1-2)
- Monitor for issues
- Gather user feedback
- Fix any compatibility problems
- Optimize build performance

### Phase 2: Enhancement (Month 3-6)
- Create domain-specific examples
- Enhance domain documentation
- Implement plugin architecture
- Create specialized distributions

### Phase 3: Evolution (Month 6+)
- Package manager for domains
- Improved modularity
- Performance optimizations
- New domain additions

## Risks and Mitigation

### Risk: Migration Complexity
**Mitigation**: Phased approach, comprehensive testing, clear rollback plan

### Risk: Backward Compatibility Issues
**Mitigation**: Compatibility layer, extensive testing, gradual migration

### Risk: Build System Problems
**Mitigation**: Preserve existing build patterns, test incrementally

### Risk: User Confusion
**Mitigation**: Clear documentation, migration guide, announcement plan

### Risk: Lost Git History
**Mitigation**: Use git format-patch/am, preserve all commits, tag milestones

## Stakeholder Communication

### Announcement Plan

**Week -1**: Pre-announcement
- Notify maintainers and key users
- Share consolidation plan
- Request feedback

**Week 1**: Migration Start
- Public announcement on GitHub
- Blog post explaining benefits
- Documentation preview

**Week 6**: Migration Complete
- Completion announcement
- Release notes for v2.1.0
- Migration guide available
- Old repos archived with redirects

### Support Plan

- GitHub Discussions for questions
- Migration support issues template
- Email support for enterprise users
- Video walkthrough of new structure

## Conclusion

Consolidating the four ACF repositories (Acf, AcfSln, IAcf, ImtCore) into a unified, domain-oriented structure will significantly improve the framework's usability, maintainability, and scalability. The benefits far outweigh the migration effort, and the comprehensive migration plan ensures a smooth transition with minimal risk.

The new structure positions ACF for future growth while maintaining full backward compatibility with existing code.

## Related Documents

- **[Domain-Oriented Structure](DOMAIN_ORIENTED_STRUCTURE.md)**: Detailed domain design
- **[Migration Guide](MIGRATION_GUIDE.md)**: Step-by-step migration instructions
- **[Current README](README.md)**: Existing framework documentation

## Approval and Sign-off

| Role | Name | Status | Date |
|------|------|--------|------|
| Project Lead | | Pending | |
| Technical Architect | | Pending | |
| DevOps Lead | | Pending | |
| Documentation Lead | | Pending | |

---

**Version**: 1.0  
**Date**: 2026-01-22  
**Author**: ACF Migration Team  
**Status**: Proposed  
