#include "iqt3d/CSurfaceViewComp.h"


// Qwt3d includes
#include "qwt3d_surfaceplot.h"


// ACF includes
#include "istd/TChangeNotifier.h"

#include "iqt/CSignalBlocker.h"



namespace iqt3d
{


// reimplemented (imod::IModelEditor)

void CSurfaceViewComp::UpdateModel() const
{
}


void CSurfaceViewComp::UpdateEditor(int /*updateFlags*/)
{
	imath::ISampledFunction2d* objectPtr = GetObjectPtr();
	if (objectPtr != NULL && IsGuiCreated()){
		I_ASSERT(objectPtr->GetArgumentDimensionality() == 2);
		I_ASSERT(objectPtr->GetResultDimensionality() == 1);

		istd::CRange xAxisRange = objectPtr->GetIntervalRange(0);
		istd::CRange yAxisRange = objectPtr->GetIntervalRange(1);

		int width = xAxisRange.GetLength();
		int height = yAxisRange.GetLength();

		if ((width * height) <= 0){
			return;
		}

		double** dataPtr = new double*[height];
		for (int y = 0; y < height; y++){
			dataPtr[y] = new double[width];

			for (int x = 0; x < width; x++){
				imath::ISampledFunction2d::ElementIndex index;
				index.SetAt(0, x);
				index.SetAt(1, y);

				dataPtr[y][x] = objectPtr->GetValueAt(index).GetElement(0);
			}
		}

		m_surfacePlotPtr->loadFromData(
				dataPtr,
				height, 
				width,
				yAxisRange.GetMinValue(), 
				yAxisRange.GetMaxValue(),
				xAxisRange.GetMinValue(), 
				xAxisRange.GetMaxValue());

		for (int y = 0; y < height; y++){
			delete [] dataPtr[y];
		}
	
		delete [] dataPtr;

		m_surfacePlotPtr->updateData();
		m_surfacePlotPtr->updateGL();
	}
}


// protected methods

// reimplemented (iqtgui::CGuiComponentBase)

void CSurfaceViewComp::OnGuiCreated()
{
	m_surfacePlotPtr = new Qwt3D::SurfacePlot(SurfacePlotFrame);
	QLayout* layoutPtr = SurfacePlotFrame->layout();
	if (layoutPtr == NULL){
		layoutPtr = new QVBoxLayout(SurfacePlotFrame);
	}

	m_surfacePlotPtr->setOrtho(false);
	m_surfacePlotPtr->setSmoothMesh(true);
	QPalette palette = GetWidget()->palette();

	QColor backgroundColor = palette.color(QPalette::Window);

	m_surfacePlotPtr->setBackgroundColor(Qwt3D::RGBA(backgroundColor.red()/255.0, backgroundColor.green()/255.0, backgroundColor.blue()/255.0));
	m_surfacePlotPtr->setShininess(1);
	m_surfacePlotPtr->setMaterialComponent(GL_EMISSION, 0.1);
	m_surfacePlotPtr->setMaterialComponent(GL_SPECULAR, 0.1);
	m_surfacePlotPtr->setMaterialComponent(GL_AMBIENT_AND_DIFFUSE, 0.1);
	m_surfacePlotPtr->setMeshColor(Qwt3D::RGBA(0, 255, 0, 0));
    m_surfacePlotPtr->setCoordinateStyle(Qwt3D::BOX);

	m_surfacePlotPtr->setLightShift(200, 0, 0, 0);
	m_surfacePlotPtr->setLightShift(0, 200, 0, 1);
	m_surfacePlotPtr->setLightShift(0, 0, 200, 2);
	m_surfacePlotPtr->setLightShift(-200, 0, 0, 3);
	m_surfacePlotPtr->setLightShift(0, -200, 0, 4);
	m_surfacePlotPtr->setLightShift(0, 0, -200, 5);

	m_surfacePlotPtr->setLightComponent(GL_LINEAR_ATTENUATION, 0, 0.5, 0.5, 0.2, 0);
	m_surfacePlotPtr->setLightComponent(GL_LINEAR_ATTENUATION, 0.5, 0, 0.5, 0.2, 1);
	m_surfacePlotPtr->setLightComponent(GL_LINEAR_ATTENUATION, 0, 0.5, 0.5, 0.2, 2);
	m_surfacePlotPtr->setLightComponent(GL_LINEAR_ATTENUATION, 0, 0.5, 0.5, 0.2, 3);
	m_surfacePlotPtr->setLightComponent(GL_LINEAR_ATTENUATION, 0.5, 0, 0.5, 0.2, 4);
	m_surfacePlotPtr->setLightComponent(GL_LINEAR_ATTENUATION, 0, 0.5, 0.5, 0.2, 5);

	m_surfacePlotPtr->illuminate(0);
	m_surfacePlotPtr->illuminate(1);
	m_surfacePlotPtr->illuminate(2);
	m_surfacePlotPtr->illuminate(3);
	m_surfacePlotPtr->illuminate(4);
	m_surfacePlotPtr->illuminate(5);

	QString axisXName = "X"; //m_componentNames[componentAxisXIndex];
	QString axisYName = "Y"; //m_componentNames[componentAxisYIndex];
	QString axisZName = "Z"; //tr("Converted ") + m_componentNames[displayIndex];

	m_surfacePlotPtr->coordinates()->axes[Qwt3D::X1].setLabelString(axisXName);
	m_surfacePlotPtr->coordinates()->axes[Qwt3D::X2].setLabelString(axisXName);
	m_surfacePlotPtr->coordinates()->axes[Qwt3D::X3].setLabelString(axisXName);
	m_surfacePlotPtr->coordinates()->axes[Qwt3D::X4].setLabelString(axisXName);
	m_surfacePlotPtr->coordinates()->axes[Qwt3D::Y1].setLabelString(axisYName);
	m_surfacePlotPtr->coordinates()->axes[Qwt3D::Y2].setLabelString(axisYName);
	m_surfacePlotPtr->coordinates()->axes[Qwt3D::Y3].setLabelString(axisYName);
	m_surfacePlotPtr->coordinates()->axes[Qwt3D::Y4].setLabelString(axisYName);
	m_surfacePlotPtr->coordinates()->axes[Qwt3D::Z1].setLabelString(axisZName);
	m_surfacePlotPtr->coordinates()->axes[Qwt3D::Z2].setLabelString(axisZName);
	m_surfacePlotPtr->coordinates()->axes[Qwt3D::Z3].setLabelString(axisZName);
	m_surfacePlotPtr->coordinates()->axes[Qwt3D::Z4].setLabelString(axisZName);

	m_surfacePlotPtr->coordinates()->setAutoScale(true);
	m_surfacePlotPtr->showColorLegend(true);


	connect(OrthoCheck, SIGNAL(toggled(bool)), m_surfacePlotPtr, SLOT(setOrtho(bool)));
	connect(AutoScaleCheck, SIGNAL(toggled(bool)), this, SLOT(OnToggleAutoScale(bool)));
	connect(colorlegend, SIGNAL(toggled(bool)), m_surfacePlotPtr, SLOT(showColorLegend(bool)));
	connect(shader, SIGNAL(toggled(bool)), this, SLOT( OnToggleShader(bool)));
	connect(mouseinput, SIGNAL(toggled(bool)), m_surfacePlotPtr, SLOT( enableMouse(bool)));
	connect(lightingswitch, SIGNAL( toggled(bool) ), m_surfacePlotPtr, SLOT( enableLighting(bool)));
	connect(normalsquality,  SIGNAL(valueChanged(int)), this, SLOT(OnNormalsQuality(int)) );
	connect(normalslength,  SIGNAL(valueChanged(int)), this, SLOT(setNormalLength(int)) );
	
	ResolutionSlider->setRange(1,10);
	connect(ResolutionSlider, SIGNAL(valueChanged(int)), m_surfacePlotPtr, SLOT(setResolution(int)));
	connect(m_surfacePlotPtr, SIGNAL(resolutionChanged(int)), ResolutionSlider, SLOT(setValue(int)));
	ResolutionSlider->setValue(1);             

	layoutPtr->addWidget(m_surfacePlotPtr);

	BaseClass::OnGuiCreated();
}


void CSurfaceViewComp::OnGuiDestroyed()
{
	BaseClass::OnGuiDestroyed();
}


// protected slots

void CSurfaceViewComp::OnParamsChanged(double /*value*/)
{
	UpdateModel();
}


void CSurfaceViewComp::OnToggleAutoScale(bool val)
{
	m_surfacePlotPtr->coordinates()->setAutoScale(val);
	m_surfacePlotPtr->updateGL();
}


void CSurfaceViewComp::OnToggleShader(bool val)
{
	if (val)
		m_surfacePlotPtr->setShading(Qwt3D::GOURAUD);
	else
		m_surfacePlotPtr->setShading(Qwt3D::FLAT);
}


void CSurfaceViewComp::OnPolygonOffset(int val)
{
	m_surfacePlotPtr->setPolygonOffset(val / 10.0);
	m_surfacePlotPtr->updateData();
	m_surfacePlotPtr->updateGL();
}


void CSurfaceViewComp::OnNormalsQuality(int val)
{
	m_surfacePlotPtr->setNormalQuality(val);
	m_surfacePlotPtr->updateNormals();
	m_surfacePlotPtr->updateGL();
}


} // namespace iqt3d


