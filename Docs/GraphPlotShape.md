# 2D Graph Plot Shape

This document describes the 2D graph plot shape implementation for the iview library.

## Overview

The graph plot shape allows displaying multiple data curves with automatic axis scaling, tick marks, labels, legend, and grid. It follows the standard ACF observer pattern where a shape observes a data model.

## Components

### Data Model: `i2d::CGraphData2d`

The data model stores all graph information:
- Multiple curves (each with name, color, and data points)
- Axis labels (X and Y)
- Graph title
- Axis ranges (automatic or manual)
- Display options (legend visibility, grid visibility)

### Shape: `iview::CGraphPlotShape`

The shape observes the data model and handles rendering:
- Automatic axis scaling with smart tick positioning
- Configurable margins, fonts, and line widths
- Legend display (top-right corner)
- Optional grid lines
- Title rendering

### GUI Component: `iqt2d::CGraphPlotParamsGuiComp`

Basic GUI component for integrating the graph into the View framework.

## Usage Example

```cpp
// Create the data model
i2d::CGraphData2d* graphData = new i2d::CGraphData2d();

// Set graph properties
graphData->SetTitle("Sensor Measurements");
graphData->SetXAxisLabel("Time (s)");
graphData->SetYAxisLabel("Temperature (°C)");

// Create a curve
i2d::CGraphData2d::Curve curve1;
curve1.name = "Sensor A";
curve1.color = Qt::red;

// Add data points
for (int i = 0; i < 100; ++i) {
    double x = i * 0.1;  // Time in seconds
    double y = 20.0 + 5.0 * qSin(x);  // Temperature
    curve1.points.append(i2d::CVector2d(x, y));
}

// Add the curve to the graph
graphData->AddCurve(curve1);

// Create another curve
i2d::CGraphData2d::Curve curve2;
curve2.name = "Sensor B";
curve2.color = Qt::blue;
for (int i = 0; i < 100; ++i) {
    double x = i * 0.1;
    double y = 22.0 + 3.0 * qCos(x);
    curve2.points.append(i2d::CVector2d(x, y));
}
graphData->AddCurve(curve2);

// Create the shape
iview::CGraphPlotShape* shape = new iview::CGraphPlotShape();

// Attach the shape as observer to the model
graphData->AttachObserver(shape);

// The shape will automatically update when the model changes
```

## Features

### Automatic Axis Scaling
When axis ranges are not explicitly set, they are automatically calculated from the data with 5% padding added for visual clarity.

### Manual Axis Ranges
You can override automatic scaling:
```cpp
graphData->SetXAxisRange(istd::CRange(0.0, 10.0));
graphData->SetYAxisRange(istd::CRange(0.0, 100.0));
```

To return to automatic scaling:
```cpp
graphData->SetXAxisRange(istd::CRange());  // Invalid range enables auto
```

### Smart Tick Positioning
The shape automatically calculates "nice" tick positions using multiples of 1, 2, 5, and 10 to ensure readable axis labels.

### Customization
The shape provides several configurable parameters:
- `SetPlotMargin(int)` - Margin around the plot area (default: 60 pixels)
- `SetAxisLabelFontSize(int)` - Font size for axis labels (default: 12)
- `SetTickLabelFontSize(int)` - Font size for tick labels (default: 10)
- `SetCurveLineWidth(int)` - Width of curve lines (default: 2 pixels)

### Display Options
Control what's displayed:
```cpp
graphData->SetLegendVisible(true);   // Show/hide legend
graphData->SetGridVisible(false);    // Show/hide grid lines
```

## Model Updates

The shape automatically updates when the model changes due to the observer pattern. After modifying the data:
- Add/remove curves
- Change axis labels
- Modify data points
- Adjust display options

The shape will automatically redraw to reflect the changes.

## Thread Safety

Like other ACF components, modifications to the data model should be done from the GUI thread or properly synchronized.

## Testing

Comprehensive unit tests are provided in `Include/i2d/Test/CGraphData2dTest.cpp` covering:
- Curve management (add, remove, clear)
- Axis labels and title
- Axis ranges (automatic and manual)
- Display options
- Bounding box calculation

Run the tests with:
```bash
cd Include/i2d/Test/CMake
mkdir build && cd build
cmake ..
make
./i2dTest CGraphData2dTest
```

## Integration with View Framework

To integrate with the view framework, use the `CGraphPlotParamsGuiComp` component:

1. Create the GUI component in your scene
2. Assign the graph data model to it
3. The component will automatically create and manage the shape
4. The shape will be displayed in the associated view layer

## Serialization

The `CGraphData2d` model supports full serialization through the ACF archive system, allowing graphs to be saved and loaded from:
- XML files
- JSON files
- Binary archives

All graph properties, curves, and data points are preserved during serialization.
