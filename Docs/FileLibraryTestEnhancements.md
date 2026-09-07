# File Library Test Enhancement Documentation

## Overview

This document describes the complex unit test scenarios that have been added to the ACF file library (ifile) to improve test coverage and validate edge cases and advanced usage patterns.

## New Test Files

### 1. CAdvancedFileArchiveTest

**Location:** `Include/ifile/Test/CAdvancedFileArchiveTest.cpp`

This test suite covers advanced archive serialization scenarios:

#### Test Scenarios

1. **NestedSerializationTest**
   - Tests serialization of nested data structures with collections
   - Validates that parent-child relationships are preserved
   - Uses QVector to store child IDs within a model
   - Ensures proper deserialization of nested collections

2. **LargeDataSerializationTest**
   - Tests serialization of large datasets (10,000 doubles)
   - Validates archive performance with substantial data volumes
   - Ensures data integrity across large collections
   - Spot-checks values at different positions to verify correctness

3. **ConcurrentAccessTest**
   - Tests multiple archives accessing different files simultaneously
   - Validates that archives don't interfere with each other
   - Ensures thread-safety at the file level
   - Verifies independent read/write operations

4. **PartialReadTest**
   - Tests reading only a subset of serialized objects
   - Validates that archives support sequential reading
   - Ensures that partial reads don't corrupt remaining data
   - Tests reading 1, 2, and 3 objects from a file containing 3 objects

5. **ErrorRecoveryTest**
   - Tests graceful handling of invalid file paths
   - Validates that archives fail safely on non-existent files
   - Ensures that serialization failures are properly reported
   - Tests both read and write failure scenarios

6. **MultipleConcurrentWritesTest**
   - Tests writing to multiple files in sequence
   - Validates that multiple archives can be created and used
   - Ensures each file maintains its own independent data
   - Tests with 5 concurrent files containing different data

7. **ArchiveVersionCompatibilityTest**
   - Tests compatibility across archive operations
   - Ensures written data can be read back correctly
   - Validates archive format consistency
   - Provides foundation for future version migration tests

8. **NestedCollectionsTest**
   - Tests serialization of multiple collection types
   - Validates integer, string, and double collections
   - Tests different collection sizes (100, 50, 75 items)
   - Ensures proper handling of heterogeneous collections

### 2. CAdvancedFileComponentTest

**Location:** `Include/ifile/Test/CAdvancedFileComponentTest.cpp`

This test suite covers advanced component usage scenarios:

#### Test Scenarios

##### TempFileManager Tests

1. **testTempFileManagerNestedSessions**
   - Tests creating nested session hierarchies
   - Validates that parent/child session relationships work
   - Ensures files in different sessions have unique paths
   - Tests cleanup of nested sessions

2. **testTempFileManagerSessionCleanup**
   - Tests proper cleanup when sessions are finished
   - Validates that file paths become inaccessible after cleanup
   - Ensures no resource leaks from temporary files
   - Tests with 3 files in a single session

3. **testTempFileManagerMultipleFilesInSession**
   - Tests adding many files to a single session (20 files)
   - Validates that each file gets a unique path
   - Ensures scalability of session management
   - Tests path uniqueness across all files

4. **testTempFileManagerFileOperations**
   - Tests file addition and removal operations
   - Validates that removed files are properly cleaned up
   - Tests actual file I/O operations within temp sessions
   - Ensures file state consistency

##### FileNameParam Tests

5. **testFileNameParamComplexPaths**
   - Tests various path formats (absolute, relative, Windows, Unix)
   - Validates handling of paths with spaces
   - Tests special characters in paths
   - Ensures cross-platform path compatibility

6. **testFileNameParamRelativePaths**
   - Tests relative path handling (../, ./, subdirs)
   - Validates path normalization
   - Ensures relative paths are preserved correctly

7. **testFileNameParamSymbolicLinks**
   - Tests handling of file system symbolic links
   - Validates path type detection (file vs directory)
   - Note: Platform-dependent behavior

##### FileListProvider Tests

8. **testFileListProviderRecursiveSearch**
   - Tests recursive directory traversal
   - Creates nested directory structure for testing
   - Validates that files at different levels are found
   - Tests depth-limited recursive search

9. **testFileListProviderFiltering**
   - Tests file extension filtering (*.cpp, *.h vs *.txt)
   - Validates that only matching files are returned
   - Ensures filter patterns work correctly
   - Tests with mixed file types

10. **testFileListProviderSorting**
    - Tests file list sorting by name
    - Creates files with different sort orders
    - Validates that sorting is applied correctly
    - Tests with 4 files (alpha, beta, gamma, zebra)

##### Integration Tests

11. **testFileArchiveWithTempFileManager**
    - Tests integration of archive with temp file management
    - Validates that archives work with temp file paths
    - Tests full write/read cycle with temp files
    - Ensures proper cleanup of archive data

12. **testMultipleArchiveFormats**
    - Tests both normal and secure archives
    - Validates that different archive types produce different output
    - Ensures format-specific behavior is correct
    - Compares raw file data between formats

## Collection Serialization Pattern

All collection serialization uses the standard ACF pattern:

```cpp
// Writing collections
int count = collection.size();
archive.BeginMultiTag(containerTag, itemTag, count);
if (archive.IsStoring()) {
    for (const auto& item : collection) {
        archive.BeginTag(itemTag);
        archive.Process(item);
        archive.EndTag(itemTag);
    }
} else {
    collection.clear();
    for (int i = 0; i < count; ++i) {
        ItemType item;
        archive.BeginTag(itemTag);
        archive.Process(item);
        archive.EndTag(itemTag);
        collection.append(item);
    }
}
archive.EndTag(containerTag);
```

This pattern:
- Uses `BeginMultiTag` to declare collection size
- Iterates through items with `BeginTag`/`EndTag` pairs
- Concludes with `EndTag` on the container tag (NOT `EndMultiTag`)
- Handles both read and write operations

## Test Coverage Improvements

The new tests significantly improve coverage in the following areas:

### Previously Untested Scenarios
- Nested data structures with collections
- Large dataset serialization (> 1000 items)
- Concurrent archive access patterns
- Partial reads from multi-object archives
- Error recovery from invalid operations
- Nested temporary file sessions
- Complex path handling (spaces, special chars)
- Recursive directory operations
- File list filtering and sorting
- Archive format comparison

### Edge Cases
- Empty collections
- Non-existent file paths
- Invalid archive operations
- Session cleanup with multiple files
- Platform-specific path formats
- Depth-limited recursive searches

### Integration Points
- Archive + TempFileManager
- Multiple archive formats
- File operations within sessions

## Running the Tests

The tests are automatically included in the `ifileTest` executable when building the project:

```bash
cd Build/CMake
cmake -B build -DCMAKE_BUILD_TYPE=Release -DQT_VERSION_MAJOR=5
cmake --build build --target ifileTest
./build/bin/ifileTest
```

Or run specific tests:

```bash
./build/bin/ifileTest CAdvancedFileArchiveTest
./build/bin/ifileTest CAdvancedFileComponentTest
```

## Benefits

1. **Increased Confidence**: More comprehensive test coverage reduces the risk of regressions
2. **Better Documentation**: Tests serve as usage examples for advanced scenarios
3. **Early Bug Detection**: Edge cases and complex scenarios are validated automatically
4. **Performance Insights**: Large data tests help identify performance bottlenecks
5. **API Validation**: Integration tests ensure components work together correctly

## Future Enhancements

Potential areas for further test expansion:

1. **Performance Benchmarks**: Add timing measurements for large data tests
2. **Memory Leak Detection**: Validate proper resource cleanup
3. **Stress Testing**: Test with extremely large datasets (millions of items)
4. **Multi-threading**: Test concurrent access with actual threads
5. **Format Migration**: Test reading archives written by older versions
6. **Corruption Handling**: Test recovery from corrupted archive data
7. **Platform-Specific**: Expand tests for Windows/Linux/Mac differences
8. **Network Archives**: Add tests for network-based serialization

## Maintenance Notes

- Tests use Qt's QTemporaryFile and QTemporaryDir for automatic cleanup
- All tests are self-contained and don't depend on external files
- Test data is generated programmatically for reproducibility
- Tests follow the existing ACF test patterns and conventions
- All tests use the I_ADD_TEST macro for automatic registration

## Related Documentation

- Component Framework Documentation: `Docs/ComponentFramework.md`
- Serialization Guide: See `iser` library documentation
- File Library API: `Include/ifile/ifile.h`
- Test Framework: `Include/itest/` directory
