#pragma once


// Qt includes
#include <QtCore/QCoreApplication>
#include <QtCore/QStringList>

// ACF includes
#include <istd/TOptDelPtr.h>
#include <imod/TModelWrap.h>
#include <ilog/TLoggerCompWrap.h>
#include <iqt/CConsoleReader.h>
#include <ibase/IApplication.h>
#include <ibase/IApplicationInfo.h>
#include <ibase/IRuntimeStatusProvider.h>


namespace ibase
{


/**
	Console (non-gui) application.
*/
class CConsoleApplicationComp:
			public QObject,
			public ilog::CLoggerComponentBase,
			virtual public ibase::IApplication
{
	Q_OBJECT

public:
	typedef ilog::CLoggerComponentBase BaseClass;

	I_BEGIN_COMPONENT(CConsoleApplicationComp);
		I_REGISTER_INTERFACE(ibase::IApplication);
		I_REGISTER_SUBELEMENT(RuntimeStatus);
		I_REGISTER_SUBELEMENT_INTERFACE(RuntimeStatus, ibase::IRuntimeStatusProvider, ExtractRuntimeStatus);
		I_REGISTER_SUBELEMENT_INTERFACE(RuntimeStatus, imod::IModel, ExtractRuntimeStatus);
		I_REGISTER_SUBELEMENT_INTERFACE(RuntimeStatus, istd::IChangeable, ExtractRuntimeStatus);
		I_ASSIGN(m_applicationInfoCompPtr, "ApplicationInfo", "Info about the worker application", true, "ApplicationInfo");
		I_ASSIGN_MULTI_0(m_componentsToInitializeCompPtr, "ComponentsToInitialize", "List of components to be initialized after creation of the application instance (QCoreApplication)", false);
	I_END_COMPONENT;

	// reimplemented (ibase::IApplication)
	virtual bool InitializeApplication(int argc, char** argv);
	virtual int Execute(int argc, char** argv);
	virtual QString GetHelpText() const;
	virtual QStringList GetApplicationArguments() const;

protected:
	/**
		Callback for the application's shutdown.
		Default implementation does nothing.
	*/
	virtual void OnApplicationShutdown();

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentDestroyed();

private slots:
	void OnKeyPressed(char ch);

private:
	class RuntimeStatus: public ibase::IRuntimeStatusProvider
	{
	public:
		RuntimeStatus();

		void SetRuntimeStatus(IRuntimeStatusProvider::RuntimeStatus runtimeStatus);

		// reimplemented (ibase::IRuntimeStatusProvider)
		virtual IRuntimeStatusProvider::RuntimeStatus GetRuntimeStatus() const;

	private:
		IRuntimeStatusProvider::RuntimeStatus m_status;
	};

	imod::TModelWrap<RuntimeStatus> m_runtimeStatus;

	// static template methods for sub element access
	template <class InterfaceType>
	static InterfaceType* ExtractRuntimeStatus(CConsoleApplicationComp& component)
	{
		return &component.m_runtimeStatus;
	}

private:
	I_REF(ibase::IApplicationInfo, m_applicationInfoCompPtr);
	I_MULTIREF(istd::IPolymorphic, m_componentsToInitializeCompPtr);

	QStringList m_applicationArguments;
	istd::TOptDelPtr<QCoreApplication> m_applicationPtr;

	iqt::CConsoleReader m_consoleReader;
};


} // namespace ibase


