#ifndef ACF_included
#define ACF_included


/**
\defgroup AcfCore ACF Core Libraries

\mainpage
\section Introduction
ACF is a C++ based, cross-plattform component framework, that was especially designed for creation of very complex, customizable and highly scalable applications.
ACF follows the component-oriented programming paradigm and provides instruments to build software from reusable components and serves interfaces and concepts to create those components.
ACF contains a set of libraries with implementations of most usefull design patterns, such Serialization, Model/Observer, Factory, Dependency Injection etc..
ACF implements a powerfull component concept and includes many well designed components, which you can directly use in your application.
The UI-part of the framework is based on Qt library provided by Digia.
ACF is open source and released under the terms of the GNU Lesser General Public License (LGPL).
See http://www.ilena.org or write info@imagingtools.de or contact
by Skype to ACF_infoline for further information about the ACF.

More theoretical considerations about the component-based development can be found at the following location:
http://en.wikipedia.org/wiki/Component-based_software_engineering

\section HowItWorks How it works
The main idea behind ACF is to see each software product as a composition of components with clearly defined interfaces. Interface is also the only way for communication between components. The usual steps to implement a component are:

- A C++ interface must be defined.
- A class that implements this interface must be created.There are several ways to make a "normal" C++ class to a ACF-Component, the simplest way is to derive from a corresponding basic implementation or use a generic macro. A component can have dependencies from other components (e.g. component <b>A</b> needs component <b>B</b> to realize some functionality) and can have attributes. Via attributes it is possible, to parameterize some behaviour aspects of the component. Another way to create a component is to combine more existing components into a composition. This composition is also perceived by the framework as a component. A component composition is created with the framework tool called <b>ACF Compositor</b>. ACF Compositor provides graphical editing for the component dependencies and attributes. The compositions are stored in so-called ARX files (we also call them registries).
- After a component is implemented, it must be registered in a so called Package. Component packages are logical containers of components. There are two kinds of packages:
	- For C++ based component the package is a DLL with special export interface.
	- For component compositions the package is a special folder in the file system.

To keep the management component packages consistent, all kind of packages have the same file extension - ARP (ACF Registry Package).

If all the necessary components have been implemented, the application can be executed. There are two basic ways. A composition of components can be run directly by Framework-Runtime.
However, this mechanism is usefull only in the prototyping stage. For the final application a different approach can be used. ACF provides a special source-code generator, which makes it possible to produce source code back from component compositions. The resulting source code is then compiled by the usual way.
This has the advantage that during the runtime of the application no modules (DLLs) must be loaded.

\section Features
Below are some of the main features of ACF:
- Graphical overview and editing of component connections and properties in UML-like manner
- Definition of composite components
- C++ Code Generator (ARX Compiler)
- C++ library containing several implementatons of common business logik components, which can be used in every application
- Component library for abstract data model definition, visualization and processing
- High level components for data binding and persistence
- Multi-Document Interface application framework
- Component library for industrial and scientic image processing
- Component library for 2D and 3D visualization
- Component wrappers for popular 3rd-party libraries such:
	- Qt/Qt Solutions
	- Qwt
	- Qwt3d
	- OpenCV
	- phonon
	- FFMpeg
	- zlib
	- QScintilla
*/


/**
	\defgroup DataModel Data model
	Basic interfaces and implementations for abstract definition of the data model.
	
	The most important interface for a general data model definition is istd::IChangeable.
	This is a common interface for describing of objects which change their state during the run time of the application.
	The interface provides methods for managing data change transaction (istd::IChangeable::BeginChanges() and istd::IChangeable::EndChanges()), methods for coping, cloning, reseting and comparison of objects. The realization of change notification mechanism is also based on this interface.
	Following example demonstrates implementation of a simple data object:
	\code
	class CPerson: virtual public istd::IChangeable
	{
	public:
		CPerson(const QString& firstName = QString(), const QString& lastName = QString());
		CPerson(const CPerson& person);

		QString GetFirstName() const;
		void SetFirstName(const QString& firstName);
		QString GetLastName() const;
		void SetLastName(const QString& name);

	private:
		QString m_firstName;
		QString m_lastName;
	};

	QString CPerson::GetFirstName() const
	{
		return m_firstName;
	}

	QString CPerson::SetFirstName(const QString& firstName)
	{
		if (m_firstName != firstName){
			BeginChanges();

			m_firstName = firstName;

			EndChanges();
		}
	}

	QString CPerson::GetLastName() const
	{
		return m_lastName;
	}

	QString CPerson::SetLastName(const QString& lastName)
	{
		if (m_lastName != lastName){
			BeginChanges();

			m_lastName = lastName;

			EndChanges();
		}
	}
	\endcode

	To ensure that the transaction block is always consistent, you could also use a helper class - istd::CChangeNotifier:

	\code
	QString CPerson::SetFirstName(const QString& firstName)
	{
		if (m_firstName != firstName){
			istd::CChangeNotifier changeNotifier(this);

			m_firstName = firstName;
		}
	}
	\endcode

	istd::CChangeNotifier calls BeginChanges in its constructor and EndChanges in the destructor.

	\section DelegatingOfChanges Delegating of changes
	Let us consider also the following situation - the data object of class CPerson could "live" in any container class (eg. in a database).
	In this case we want the container implentation will also notice about the changes of the CPerson instance.
	What we have to do is to extend our CPerson implementation as following:

	\code
	class CPerson: public istd::TChangeDelegator<istd::IChangeable>
	\endcode

	Then our container class can be defined as:
	\code
	class CPersonDatabase: virtual public istd::IChangeable
	{
	public:
		int GetPersonsCount() const
		{
			return m_persons.count();
		}

		const CPerson& GetPerson(int personIndex) const
		{
			return m_persons[personIndex];
		}

		CPerson& AddNewPerson(const QString& firstName, const QString& lastName);

	protected:
		// reimplemented (istd::IChangeable)
		virtual void OnEndChanges(int changeFlags, istd::IPolymorphic* changeParamsPtr)

	private:
		QVector<CPerson> m_persons;
	};

	CPerson& CPersonDatabase::AddNewPerson(const QString& firstName, const QString& lastName)
	{
		CPerson person(firstName, lastName);

		m_persons.push_back(person);

		// Register the container as change notification target:
		m_persons.back().SetSlavePtr(this);

		return m_persons.back();
	}
	\endcode

	Now we can catch the changes of person instances in the implementation of the method OnEndChanges():

	\code
	// reimplemented (istd::IChangeable)

	void CPersonDatabase::OnEndChanges(int changeFlags, istd::IPolymorphic* changeParamsPtr)
	{
		if (changeFlags & CF_DELEGATED){
			qDebug("Some person data have been changed");
		}
	}
	\endcode
	
	In this section we have considered situations in which a decision that a data model would delegate its changes has been made in the design phase.
	For situations where this is not the case, we must rely on other mechanisms. These are described in the \ref ModelObserver Section.

	\ingroup AcfCore
*/

/**
	\defgroup ModelObserver Model/Observer concept
	The main use-case of this concept is to get information on the observer side about changes of the data.
	It can be used e.g. to realize GUI update if related data model changes.

	\sa DataModel

	\ingroup AcfCore
*/

/**
	\defgroup Persistence Persistence concept
	Common interfaces and implementations for file based persistence and general object serialization.
	
	\section Archive-based object serialization.
	\section File-based persistence.

	\sa DataModel

	\ingroup AcfCore
*/

/**
	\defgroup Main Main concepts and implementations
	If you want to learn how to use ACF, you should begin with elements defined in this module.

	\ingroup AcfCore
*/

/**
	\defgroup SystemDefines Operation system services
	Access to operating system related functionality.

	\ingroup AcfCore
*/

/**
	\defgroup ComponentConcept Component concept
	This concept allows to divide application into smaller modules.
	At the implementation level you don't need to care about the application topology.
	The topology will be described separately using \em .arx files and can be started using
	runtime environment over ACF tool or compiled to C++ code using ARXC compiler.
	Additionally you can use component simulation to use components as "normal" classes.

	\ingroup AcfCore
*/


#endif // !ACF_included
