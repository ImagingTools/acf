# Migration Guide: Consolidating ACF Repositories

## Overview

This guide provides step-by-step instructions for migrating from the four-repository structure (Acf, AcfSln, IAcf, ImtCore) to the unified domain-oriented repository structure.

## Prerequisites

### Required Tools
- Git 2.20+
- CMake 3.16+
- Qt 5.15+ or Qt 6.x
- Python 3.8+ (for migration scripts)
- Bash or PowerShell

### Required Access
- Read access to all four source repositories
- Write access to the unified Acf repository
- Administrative access (for repository settings)

## Migration Timeline

**Estimated Duration**: 6-8 weeks

- Week 1: Repository setup and Core domain
- Week 2: Foundation domains (Math, Geometry, Platform, Data)
- Week 3-4: Application domains and ImtCore (ImageProcessing, Calibration, Acquisition)
- Week 5: UI domain and services
- Week 6: Testing and validation
- Week 7-8: Documentation and cleanup

## Phase 1: Preparation (Week 1, Days 1-2)

### 1.1 Backup Existing Repositories

```bash
# Clone all four repositories
git clone https://github.com/ImagingTools/Acf.git Acf-backup
git clone https://github.com/ImagingTools/AcfSln.git AcfSln-backup
git clone https://github.com/ImagingTools/IAcf.git IAcf-backup
git clone https://github.com/ImagingTools/ImtCore.git ImtCore-backup

# Create archive
tar -czf acf-repositories-backup-$(date +%Y%m%d).tar.gz Acf-backup AcfSln-backup IAcf-backup ImtCore-backup
```

### 1.2 Create Migration Branch

```bash
cd Acf
git checkout -b migration/unified-domain-structure
```

### 1.3 Create Domain Directory Structure

```bash
# Create domain directories
mkdir -p Core Math Geometry ImageProcessing Calibration Acquisition
mkdir -p Production DocumentView UI/Core UI/Domain UI/Integration
mkdir -p Platform Services Examples

# Preserve existing directories
# Build/, Config/, Docs/, Install/, Tests/, Partitura/ remain unchanged
```

## Phase 2: Migrate Core Domain (Week 1, Days 3-5)

### 2.1 Identify Core Libraries

Core libraries from Acf repository:
- istd, icomp, iser, iprm, iattr, ibase, ipackage, ilog, itest

### 2.2 Move Core Libraries

```bash
# Move from Include/ to Core/
for lib in istd icomp iser iprm iattr ibase ipackage ilog itest; do
    git mv Include/$lib Core/$lib
done

# Update Impl packages
# These remain in Impl/ but update CMakeLists.txt to reference Core/
```

### 2.3 Update CMake Configuration

Create `Core/CMakeLists.txt`:

```cmake
# Core Domain Libraries
add_subdirectory(istd)
add_subdirectory(icomp)
add_subdirectory(iser)
add_subdirectory(iprm)
add_subdirectory(iattr)
add_subdirectory(ibase)
add_subdirectory(ipackage)
add_subdirectory(ilog)
add_subdirectory(itest)
```

Update root `CMakeLists.txt`:

```cmake
# Add domain directories
add_subdirectory(Core)
add_subdirectory(Math)
add_subdirectory(Geometry)
# ... etc
```

### 2.4 Update Include Paths

Maintain backward compatibility by creating symlinks:

```bash
# In Include/ directory
cd Include
for lib in istd icomp iser iprm iattr ibase ipackage ilog itest; do
    ln -s ../Core/$lib $lib
done
```

### 2.5 Test Core Domain Build

```bash
mkdir build-core
cd build-core
cmake .. -DACF_BUILD_DOMAINS="Core"
cmake --build .
ctest --output-on-failure
```

## Phase 3: Migrate Foundation Domains (Week 2)

### 3.1 Math Domain

```bash
# Libraries: imath, ialgo (from AcfSln), icmpstr (from AcfSln), isig (from AcfSln)

# From Acf
git mv Include/imath Math/imath

# From AcfSln clone
cd ../AcfSln-backup
git format-patch --root Include/ialgo Include/icmpstr Include/isig
cd ../Acf
git am ../AcfSln-backup/*.patch

# Move to Math domain
git mv Include/ialgo Math/ialgo
git mv Include/icmpstr Math/icmpstr  
git mv Include/isig Math/isig
```

### 3.2 Geometry Domain

```bash
# Libraries: i2d, i3d, icmm, iimg

for lib in i2d i3d icmm iimg; do
    git mv Include/$lib Geometry/$lib
done
```

### 3.3 Platform Domain

```bash
# From IAcf: iwin, inat

cd ../IAcf-backup
git format-patch --root Include/iwin Include/inat
cd ../Acf
git am ../IAcf-backup/*.patch

git mv Include/iwin Platform/iwin
git mv Include/inat Platform/inat
```

### 3.4 Update and Test

```bash
# Update CMakeLists.txt for each domain
# Test build
cmake --build build-core --target Math Geometry Platform
ctest -R "Math|Geometry|Platform"
```

## Phase 4: Migrate Application Domains (Week 3)

### 4.1 ImageProcessing Domain

```bash
# From Acf: iproc (if exists)
# From AcfSln: iproc, iipr, iblob, iedge, ibarcode
# From IAcf: iocv

# Process patches from AcfSln and IAcf
cd ../AcfSln-backup
git format-patch --root Include/iproc Include/iipr Include/iblob Include/iedge Include/ibarcode
cd ../Acf
git am ../AcfSln-backup/*.patch

cd ../IAcf-backup
git format-patch --root Include/iocv
cd ../Acf
git am ../IAcf-backup/*.patch

# Move to ImageProcessing domain
for lib in iproc iipr iblob iedge ibarcode iocv; do
    git mv Include/$lib ImageProcessing/$lib
done
```

### 4.2 Calibration Domain

```bash
# From AcfSln: icalib, imeas
# From IAcf: iqwt, iqwt3d

# Apply patches and move
# ... (similar process as ImageProcessing)

for lib in icalib imeas iqwt iqwt3d; do
    git mv Include/$lib Calibration/$lib
done
```

### 4.3 Acquisition Domain

```bash
# From AcfSln: icam, icomm
# From IAcf: idc1394, icbox, ilibav

# Apply patches and move
for lib in icam icomm idc1394 icbox ilibav; do
    git mv Include/$lib Acquisition/$lib
done
```

### 4.4 Test Application Domains

```bash
cmake --build build-core --target ImageProcessing Calibration Acquisition
ctest -R "ImageProcessing|Calibration|Acquisition"
```

## Phase 5: Migrate Production Domain (Week 4, Days 1-2)

### 5.1 Production Libraries

```bash
# From AcfSln: iprod, iinsp, ihotf, ifileproc, idocproc

cd ../AcfSln-backup
git format-patch --root Include/iprod Include/iinsp Include/ihotf Include/ifileproc Include/idocproc
cd ../Acf
git am ../AcfSln-backup/*.patch

for lib in iprod iinsp ihotf ifileproc idocproc; do
    git mv Include/$lib Production/$lib
done
```

## Phase 6: Migrate DocumentView Domain (Week 4, Day 3)

### 6.1 DocumentView Libraries

```bash
# From Acf: idoc, iview, imod, ifile
# From AcfSln: imm
# From IAcf: iphonon

for lib in idoc iview imod ifile; do
    git mv Include/$lib DocumentView/$lib
done

# Apply patches for imm and iphonon
# Move to DocumentView
```

## Phase 7: Migrate UI Domain (Week 4, Days 4-5)

### 7.1 Core UI Libraries

```bash
# From Acf: iqt, iqtgui, iqtprm, iqt2d, iqtdoc, iwidgets, ifilegui, iloggui
# From AcfSln: iwiz
# From IAcf: iqsci

for lib in iqt iqtgui iqtprm iqt2d iqtdoc iwidgets ifilegui iloggui; do
    git mv Include/$lib UI/Core/$lib
done

# Apply patches and move iwiz, iqsci
```

### 7.2 Domain UI Libraries

```bash
# From AcfSln: iprocgui, iqtipr, icalibgui, iqtmeas, iqtcam, iqtinsp,
#              iblobgui, iedgegui, ihotfgui, iwizgui, iqtmm, iqtsig

cd ../AcfSln-backup
git format-patch --root Include/iprocgui Include/iqtipr \
    Include/icalibgui Include/iqtmeas Include/iqtcam Include/iqtinsp \
    Include/iblobgui Include/iedgegui Include/ihotfgui Include/iwizgui \
    Include/iqtmm Include/iqtsig

cd ../Acf
git am ../AcfSln-backup/*.patch

for lib in iprocgui iqtipr icalibgui iqtmeas iqtcam iqtinsp \
           iblobgui iedgegui ihotfgui iwizgui iqtmm iqtsig; do
    git mv Include/$lib UI/Domain/$lib
done
```

### 7.3 UI Integration Libraries

```bash
# From IAcf: iqaxmm
# From AcfSln: iqtex (if exists)

# Apply patches and move
```

## Phase 8: Migrate Services Domain (Week 5, Day 1)

### 8.1 Services Libraries

```bash
# From AcfSln: iauth, iqtauth, iweb, iservice

cd ../AcfSln-backup
git format-patch --root Include/iauth Include/iqtauth Include/iweb Include/iservice
cd ../Acf
git am ../AcfSln-backup/*.patch

for lib in iauth iqtauth iweb iservice; do
    git mv Include/$lib Services/$lib
done
```

## Phase 9: Update Build System (Week 5, Days 2-3)

### 9.1 Update Root CMakeLists.txt

```cmake
cmake_minimum_required(VERSION 3.16)
project(ACF VERSION 2.1.0 LANGUAGES CXX)

# Options for domain selection
option(ACF_BUILD_ALL "Build all domains" ON)
option(ACF_BUILD_CORE "Build Core domain" ON)
option(ACF_BUILD_MATH "Build Math domain" OFF)
option(ACF_BUILD_GEOMETRY "Build Geometry domain" OFF)
option(ACF_BUILD_IMAGE_PROCESSING "Build ImageProcessing domain" OFF)
option(ACF_BUILD_CALIBRATION "Build Calibration domain" OFF)
option(ACF_BUILD_ACQUISITION "Build Acquisition domain" OFF)
option(ACF_BUILD_PRODUCTION "Build Production domain" OFF)
option(ACF_BUILD_DOCUMENT_VIEW "Build DocumentView domain" OFF)
option(ACF_BUILD_UI "Build UI domain" OFF)
option(ACF_BUILD_PLATFORM "Build Platform domain" OFF)
option(ACF_BUILD_SERVICES "Build Services domain" OFF)

# Add domains
if(ACF_BUILD_ALL OR ACF_BUILD_CORE)
    add_subdirectory(Core)
endif()

if(ACF_BUILD_ALL OR ACF_BUILD_MATH)
    add_subdirectory(Math)
endif()

# ... etc for all domains
```

### 9.2 Create Domain-Specific CMake Files

Each domain gets its own CMakeLists.txt listing its libraries.

### 9.3 Update Library CMakeLists.txt

Update include paths in each library's CMakeLists.txt:

```cmake
# Old
target_include_directories(iproc PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../Include>
)

# New
target_include_directories(iproc PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../..>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)
```

### 9.4 Backward Compatibility Layer

Create compatibility include directory:

```bash
mkdir -p Include-compat

# Script to create symlinks
cat > Include-compat/create_symlinks.sh << 'EOF'
#!/bin/bash
# Create backward-compatible symlinks

for domain in Core Math Geometry ImageProcessing Calibration Acquisition \
              Production DocumentView Platform Services; do
    find ../$domain -maxdepth 1 -type d ! -name $domain -exec \
        ln -sf {} . \;
done

find ../UI/Core ../UI/Domain ../UI/Integration -maxdepth 1 -type d \
    ! -name Core ! -name Domain ! -name Integration -exec ln -sf {} . \;
EOF

chmod +x Include-compat/create_symlinks.sh
./Include-compat/create_symlinks.sh
```

## Phase 10: Migrate Implementation Packages (Week 5, Day 4)

### 10.1 Update Impl Directory

Implementation packages remain in `Impl/` but update their CMakeLists.txt:

```bash
# For each package in Impl/
cd Impl/BasePck/CMake
# Update CMakeLists.txt to reference new domain paths
```

### 10.2 Merge Implementation Packages

```bash
# From AcfSln Impl/
cd ../AcfSln-backup/Impl
for dir in *; do
    git format-patch --root $dir
done
cd ../../Acf
git am ../AcfSln-backup/Impl/*.patch

# From IAcf Impl/
cd ../IAcf-backup/Impl
for dir in *; do
    git format-patch --root $dir
done
cd ../../Acf
git am ../IAcf-backup/Impl/*.patch
```

## Phase 11: Update Documentation (Week 5, Day 5)

### 11.1 Update README.md

Update the main README to reflect the new structure:

```markdown
## Libraries by Domain

### Core Domain
- **istd**: Standard utilities and interfaces
- **icomp**: Component framework
...

### Math Domain
- **imath**: Mathematical utilities
...
```

### 11.2 Create Domain README Files

```bash
# Create README for each domain
cat > Core/README.md << 'EOF'
# Core Domain

Foundation libraries providing basic infrastructure for all other domains.

## Libraries
- istd - Standard utilities
- icomp - Component framework
...
EOF
```

### 11.3 Update API Documentation

Update Doxygen configuration:

```doxyfile
# Update INPUT paths
INPUT = Core/ \
        Math/ \
        Geometry/ \
        ImageProcessing/ \
        Calibration/ \
        Acquisition/ \
        Production/ \
        DocumentView/ \
        UI/ \
        Platform/ \
        Services/
```

## Phase 12: Testing and Validation (Week 6, Days 1-3)

### 12.1 Build All Configurations

```bash
# Full build
cmake -S . -B build-full -DACF_BUILD_ALL=ON
cmake --build build-full
ctest --test-dir build-full

# Core only
cmake -S . -B build-core -DACF_BUILD_CORE=ON -DACF_BUILD_ALL=OFF
cmake --build build-core

# Specific domains
cmake -S . -B build-domains \
    -DACF_BUILD_CORE=ON \
    -DACF_BUILD_IMAGE_PROCESSING=ON \
    -DACF_BUILD_CALIBRATION=ON \
    -DACF_BUILD_UI=ON
cmake --build build-domains
```

### 12.2 Run Test Suite

```bash
# Run all tests
cd build-full
ctest --output-on-failure

# Run domain-specific tests
ctest -R "Core"
ctest -R "ImageProcessing"
ctest -R "Calibration"
```

### 12.3 Verify Backward Compatibility

Create test project using old include paths:

```cpp
#include <iproc/ImageProcessor.h>  // Should still work
#include <icalib/Calibration.h>    // Should still work
```

### 12.4 Performance Validation

```bash
# Compare build times
time cmake --build build-full
# Should be similar or better than combined build of 3 repos
```

## Phase 13: Documentation and Communication (Week 6, Days 4-5)

### 13.1 Create Migration Announcement

```markdown
# ACF Repository Consolidation Complete

We've successfully merged Acf, AcfSln, and IAcf into a unified repository
with a domain-oriented structure.

## What Changed
- Three repositories → One repository
- Technical layering → Domain organization
- 71 libraries organized into 11 domains

## What Stayed the Same
- All library names and APIs
- Include paths (backward compatible)
- CMake target names
```

### 13.2 Update Repository Settings

```bash
# Update repository description
gh repo edit --description "ACF - Application Component Framework: \
    Unified domain-oriented framework for imaging and industrial applications"

# Update topics
gh repo edit --add-topic imaging,qt,cpp,framework,computer-vision,\
    calibration,image-processing
```

### 13.3 Archive Old Repositories

```bash
# Archive AcfSln
gh repo archive ImagingTools/AcfSln --yes

# Archive IAcf
gh repo archive ImagingTools/IAcf --yes

# Update READMEs with redirect
echo "# This repository has been merged into Acf
Please visit https://github.com/ImagingTools/Acf" > README.md
```

## Phase 14: Finalization (Week 6, Day 5)

### 14.1 Merge Migration Branch

```bash
git checkout main
git merge migration/unified-domain-structure
git push origin main
```

### 14.2 Tag Release

```bash
git tag -a v2.1.0 -m "Unified domain-oriented structure"
git push origin v2.1.0
```

### 14.3 Create Release Notes

```markdown
# ACF v2.1.0 - Unified Domain-Oriented Structure

## Major Changes
- Merged three repositories (Acf, AcfSln, IAcf) into unified structure
- Reorganized 71 libraries into 11 logical domains
- Improved discoverability and maintainability

## Domains
- Core, Math, Geometry
- ImageProcessing, Calibration, Acquisition
- Production, DocumentView, UI
- Platform, Services

## Backward Compatibility
- All existing include paths work via compatibility layer
- No API changes
- CMake targets unchanged
```

## Rollback Procedure

If issues arise during migration:

```bash
# Restore from backup
git checkout main
git reset --hard <pre-migration-commit>

# Or restore individual domain
git checkout main -- Core/
```

## Post-Migration Tasks

### Ongoing Maintenance

1. **Update CI/CD**: Configure builds for domain subsets
2. **Documentation**: Enhance domain-specific guides
3. **Examples**: Create example apps per domain
4. **Performance**: Profile and optimize domain builds
5. **Dependencies**: Document inter-domain dependencies

### Future Enhancements

1. **Plugin System**: Make domains loadable as plugins
2. **Package Manager**: Create ACF package manager
3. **Modular Installation**: Install only needed domains
4. **Cross-Platform**: Enhance Windows/Linux/macOS support

## Troubleshooting

### Common Issues

**Issue**: Include paths not found
**Solution**: Verify Include-compat symlinks created correctly

**Issue**: CMake can't find libraries
**Solution**: Check domain CMakeLists.txt paths, ensure compatibility layer active

**Issue**: Circular dependencies
**Solution**: Review dependency layers, ensure higher layers don't depend on lower

**Issue**: Build time increased
**Solution**: Enable parallel builds (`cmake --build . -j8`), use ccache

## Support and Contact

For migration support:
- GitHub Issues: https://github.com/ImagingTools/Acf/issues
- Documentation: https://imagingtools.github.io/Acf/
- Email: support@imagingtools.com

## Conclusion

This migration consolidates three repositories into a unified, domain-oriented structure that improves maintainability, discoverability, and developer experience while maintaining full backward compatibility.

The new structure positions ACF for future growth and makes it easier for developers to understand, use, and contribute to the framework.
