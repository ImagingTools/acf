# ACF Unit Tests

This directory contains unit tests for the ACF (Application Component Framework) library.

## Test Structure

The ACF project uses Qt Test framework with a custom test executor (`itest::CStandardTestExecutor`). Each test is structured as follows:

### Test Organization

Tests are organized by location:

- **Tests/** - Integration and component tests
  - **ParamsManagerTest/** - Tests for IParamsManager interface
  - **MultiThreadingComponentTest/** - Tests for multi-threading components
  - **SelectionParamIntegrationTest/** - Integration tests for selection parameters

- **Include/[library]/Test/** - Unit tests for each library
  - **Include/istd/Test/** - Tests for standard utilities (CRandomNumber, CIndex2d, CBitManip, CCrcCalculator, etc.)
  - **Include/imath/Test/** - Tests for mathematics library (CDouble, CLinearInterpolator, etc.)
  - **Include/i2d/Test/** - Tests for 2D graphics library (CVector2d, etc.)
  - **Include/i3d/Test/** - Tests for 3D graphics library (CVector3d, CMatrix3d, etc.)
  - **Include/icmm/Test/** - Tests for color management library (CHsv, etc.)
  - **Include/iser/Test/** - Tests for serialization library (CMemoryArchive, etc.)

### Test File Structure

Each test directory contains:
- `main.cpp` - Entry point that runs all registered tests
- `C*Test.h` - Test class header files
- `C*Test.cpp` - Test class implementation files
- `CMake/CMakeLists.txt` - CMake build configuration
- Optional: `.acc` and `.accl` files for component configuration (for component tests)

### Writing Tests

1. **Create test class** inheriting from `QObject`:
```cpp
class CMyClassTest: public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void myTest();
    void cleanupTestCase();
};
```

2. **Implement test methods** using QVERIFY/QCOMPARE macros:
```cpp
void CMyClassTest::myTest()
{
    MyClass obj;
    QVERIFY(obj.doSomething());
    QCOMPARE(obj.getValue(), 42);
}
```

3. **Register test** at the end of the .cpp file:
```cpp
I_ADD_TEST(CMyClassTest);
```

## Library Unit Tests

Unit tests for each library are located in `Include/[library]/Test/` directories.

### Tested Classes by Library

#### istd (Standard Utilities) - Include/istd/Test/
- **CRandomNumber** - Random number generation for integers and doubles
- **CIndex2d** - 2D indexing with Qt type conversions
- **CBitManip** - Bit manipulation operations
- **CCrcCalculator** - CRC-32 checksum calculations
- **CInterfacePtr** - Smart pointer for interface management

#### imath (Mathematics) - Include/imath/Test/
- **CDouble** - Double wrapper with rounding and comparison operations
- **CLinearInterpolator** - Linear interpolation with optional extrapolation

#### i2d (2D Graphics) - Include/i2d/Test/
- **CVector2d** - 2D vector operations including angles, orthogonals, and transformations
- **CI2d** - Comprehensive 2D geometry tests (Vector2d, Circle, Rectangle, Line, Matrix, etc.)

#### i3d (3D Graphics) - Include/i3d/Test/
- **CVector3d** - 3D vector operations including cross products and normalization
- **CMatrix3d** - 3D matrix operations including transformations and determinants

#### icmm (Color Management) - Include/icmm/Test/
- **CHsv** - HSV color model with component-wise operations

#### iser (Serialization) - Include/iser/Test/
- **CMemoryArchive** - Binary serialization to/from memory buffers
- **CBitMemoryArchive** - Bit-level serialization
- **CCompactXmlArchive** - Compact XML serialization
- **CJsonArchive** - JSON serialization

## Building and Running Tests

### Prerequisites
- Qt 5 or Qt 6
- CMake
- C++ compiler with C++11 support
- ACF library built and installed

### Build Instructions

Tests can be built individually for each library.

Using CMake for a specific library:
```bash
# For istd tests
cd Include/istd/Test/CMake
mkdir build && cd build
cmake ..
make

# For imath tests
cd Include/imath/Test/CMake
mkdir build && cd build
cmake ..
make

# Similarly for other libraries: i2d, i3d, icmm, iser
```

### Running Tests

Execute the test binary for each library:
```bash
./istdTest
./imathTest
./i2dTest
./i3dTest
```

Run specific test:
```bash
./istdTest CRandomNumberTest
```

## Test Coverage

Each library's test suite provides comprehensive coverage for:
- Constructor variations
- Getter/setter methods
- Arithmetic operations
- Comparison operations
- Type conversions
- Edge cases and boundary conditions
- Serialization (where applicable)

Tests are organized by library to maintain clear separation of concerns and enable independent testing of each component.

## Contributing

When adding new tests:
1. Place tests in the appropriate library's Test directory: `Include/[library]/Test/`
2. Follow the existing naming conventions (C*Test for class tests)
3. Include `initTestCase()` and `cleanupTestCase()` methods
4. Use descriptive test method names ending in "Test"
5. Update the library's CMakeLists.txt if needed
6. Register your test using `I_ADD_TEST` macro
7. Update this README with the new test information

## Notes

- Tests are independent and can run in any order
- Each test class should clean up after itself in `cleanupTestCase()`
- Use Qt Test macros: QVERIFY, QCOMPARE, QVERIFY2, etc.
- Floating point comparisons should use tolerance checks (qAbs(a - b) < epsilon)
