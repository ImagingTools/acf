#ifndef ACF_included
#define ACF_included


/**
\defgroup AcfCore ACF Core Libraries

ACF Core Libraries is a set of implementations of most important concepts and design patterns.

\mainpage
\section Introduction
ACF is a C++ based, cross-plattform component framework, that was especially designed for creation of complex, customizable and highly scalable applications.
ACF follows the component-oriented programming paradigm and provides instruments to build software from reusable components and serves interfaces and concepts to create those components.
ACF contains a set of libraries with implementations of most usefull design patterns, such Serialization, Model/Observer, Factory, Dependency Injection etc..
ACF implements a powerfull component concept and includes many well designed components, which you can directly use in your application.
The UI-part of the framework is based on Qt library provided by Digia.
ACF is open source and released under the terms of the GNU Lesser General Public License (LGPL).
See http://www.ilena.org or write info@imagingtools.de for further information about the ACF.

More theoretical considerations about the component-based development can be found at the following location:
http://en.wikipedia.org/wiki/Component-based_software_engineering

\section HowItWorks How it works
The main idea behind ACF is to see each software product as a composition of components with clearly defined interfaces.
Interface is also the only way for communication between components.
The usual steps to implement a component are:

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

\section BasicTutorials Tutorials
Hier you will find beginner tutorials:
- Say hello to ACF world:
http://ilena.org/redmine/projects/acf/wiki/HelloWorld
- Big brother is watching you (Model/Observer):
http://ilena.org/redmine/projects/acf/wiki/ModelObserver-Tutorial
*/


/**
	\defgroup DataModel Data model
	Basic interfaces and implementations for abstract definition of the data model.

	\section BasicMotivation Motivation
	A fundamental problem in the implementation of complex software applications is ensuring a clean separation between the data model, business logic (controller) and data presentation (GUI).
	Such a separation allows a high degree of reusability of source code. The following aspects have to be represented by interfaces:
	- Data changes. Transaction concept.
	- Notification of data changes.
	- Forwarding of data changes.
	- Subscribing/Unsubscribing for the entire data object or for concrete data change notifications.
	- General concept of the data visualization and editing.
	- Undo/Redo mechanism to roll back the data changes.

	\section DataModelOverview Overview
	The most important interface for a general data model definition is istd::IChangeable.
	This is a common interface for describing of objects which change their state during the run time of the application.
	The interface provides methods for managing data change transaction (\c istd::IChangeable::BeginChanges and \c istd::IChangeable::EndChanges), methods for coping, cloning, reseting and comparison of objects. The realization of change notification mechanism is also based on this interface.
	Following example demonstrates implementation of a simple data object:
	\code
	class CPerson: virtual public istd::IChangeable
	{
	public:
		enum ChangeFlags
		{
			CF_NAME_CHANGED = 0x74b520	// Some random, unique number
		}

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
			static ChangeSet changeSet(CF_NAME_CHANGED, "Change first name");
			BeginChanges(changeSet);

			m_firstName = firstName;

			EndChanges(changeSet);
		}
	}

	QString CPerson::GetLastName() const
	{
		return m_lastName;
	}

	QString CPerson::SetLastName(const QString& lastName)
	{
		if (m_lastName != lastName){
			static ChangeSet changeSet(CF_NAME_CHANGED, "Change last name");
			BeginChanges(changeSet);

			m_lastName = lastName;

			EndChanges(changeSet);
		}
	}
	\endcode

	To ensure that the transaction block is always consistent, you could also use a helper class - istd::CChangeNotifier:

	\code
	QString CPerson::SetFirstName(const QString& firstName)
	{
		if (m_firstName != firstName){
			static ChangeSet changeSet(CF_NAME_CHANGED, "Change first name");
			istd::CChangeNotifier notifier(this, &changeSet);
			Q_UNUSED(notifier);

			m_firstName = firstName;
		}
	}
	\endcode

	istd::CChangeNotifier calls BeginChanges in its constructor and EndChanges in the destructor.

	\subsection WhyTransaction Why do we need begin-end notification of the changes of data?
	Often you want to be informed about the upcoming changes. A simple example you want to save the existing data before it is overwritten with the changed data.
	This is responsibility of the istd::IChangeable::BeginChanges method. 
	The end change notification you will need, if you want to know when the data changes are complete, for example to update a GUI.
	
	\section DelegatingOfChanges Delegating of changes
	An important aspect in the management of data change notifications is the delegating of changes from a part of data to another.
	Let us consider the following situation - the data object of class CPerson could "live" in any container class (eg. in a database).
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
		virtual void OnEndChanges(const ChangeSet& changeSet)

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

	void CPersonDatabase::OnEndChanges(const ChangeSet& changeSet)
	{
		// Use CF_DELEGATED masking to filter out the delegated changes:
		if (changeSet.Contains(CF_DELEGATED)){
			// We will end up here, every time when CPerson::SetFirstName or CPerson::SetLastName were called:
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

	\section ModelObserverExternalLinks External Links
	http://en.wikipedia.org/wiki/Observer_pattern

	http://en.wikipedia.org/wiki/Model%E2%80%93view%E2%80%93controller

	\sa DataModel

	\ingroup AcfCore
*/

/**
	\defgroup Persistence Persistence concept
	Common interfaces and implementations for file based persistence and general object serialization.
	
	\section ObjectSerialization Archive-based object serialization
	Objects that provide serialization must implement the iser::ISerializable interface. The most important method of this interface is Serialize().
	Serialize method becomes as input an so called archive (iser::IArchive). An archive provides an abstract low level read/write access to a data medium.
	By example an archive can represent a file, a memory block, a database or a network resource, but the concrete kind of the archive is completely hidden from data object's point of view.
	Thus, we create a complete separation between the data model and the medium on which it is to be made persistent. Following archive types are provided by ACF core libraries:
	- Read from a memory block - iser::CMemoryReadArchive
	- Write to a memory block - iser::CMemoryWriteArchive
	- Read from a binary file - ifile::CFileReadArchive
	- Write to a binary file - ifile::CFileWriteArchive
	- Read from a fast parsed XML document given as a string - iser::CXmlStringReadArchive
	- Write to a fast parsed XML-string - iser::CXmlStringWriteArchive
	- Read from a fast parsed XML file - ifile::CSimpleXmlFileReadArchive
	- Write to a fast parsed XML file - ifile::CSimpleXmlFileWriteArchive
	- Read from a memory block bitwise - iser::CBitMemoryReadArchive
	- Write to a memory block bitwise - iser::CBitMemoryWriteArchive
	- Write to a TCP/IP socket - iqt::CNetworkWriteArchive
	- Read from a compact XML document given as a string - iqt::CCompactXmlMemReadArchive
	- Write to a compact XML-string - iqt::CCompactXmlMemWriteArchive
	- Read from a compact XML file - iqt::CCompactXmlFileReadArchive
	- Write to a compact XML file - iqt::CCompactXmlFileWriteArchive

	We demonstrate the implementation of object serialization on example of CPerson class introduced in the \ref DataModel section. 
	First of all we have to change the CPerson class to be inherited from iser::ISerializable. Because iser::ISerializable is already derived from istd::IChangeable, we have nothing else to change in this aspect.
	Also we rewrite:

	\code
	class CPerson: virtual public iser::ISerializable
	{
	public:
		
		// reimplemented (iser::ISerializable)
		virtual bool Serialize(iser::IArchive& archive);

	private:
		QString m_firstName;
		QString m_lastName;
	};

	// reimplemented (iser::ISerializable)

	bool CPerson::Serialize(iser::IArchive& archive)
	{
		bool retVal = true;

		// Use data model change notification only if the object will be reading from archive:
		istd::CChangeNotifier notifier(archive.IsStoring()? NULL: this);

		// Serialize object using tags:

		// Serialize first name of the person:
		static iser::CArchiveTag firstNameTag("FirstName", "First name of the person");
		retVal = retVal && archive.BeginTag(firstNameTag); // Begin tag
		retVal = retVal && archive.Process(m_firstName); // Process first name by archive	
		retVal = retVal && archive.EndTag(firstNameTag); // Close tag

		// Serialize last name of the person:
		static iser::CArchiveTag lastNameTag("LastName", "Last name of the person");
		retVal = retVal && archive.BeginTag(lastNameTag);
		retVal = retVal && archive.Process(m_lastName);
		retVal = retVal && archive.EndTag(lastNameTag);

		return retVal;
	}

	\endcode

	\section FilePersistence File-based persistence

	Serialization is a low-level persistence mechanism, in which the structure of the archived data on the medium depends on Serialize method implementation inside of the data object.
	But often you will offer persistence functionality, that is not depending on concrete object implementation.
	A such situation is the supporting of known data formats (for example reading or writing of PNG images, OpenOffice documents and so on).
	For such persistence implementations ACF provides ifile::IFilePersistence interface.

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
	The topology will be described separately using \em .acc files and can be started using
	runtime environment over ACF tool or compiled to C++ code using ARXC compiler.
	Additionally you can use component simulation to use components as "normal" classes.

	\ingroup AcfCore
*/

/**
	\defgroup ApplicationFramework Application Framework

	Set of components and basic implementations for general definition of a software application.
*/

/*
	\defgroup Logging Logging
	Interfaces and implementations related to informations logging.


	\ingroup ApplicationFramework
*/

/**
	\defgroup BasicUi Basic UI
	Basic implementation for UI-based components.

	\ingroup ApplicationFramework
*/

/**
	\defgroup UiEditors UI-based Editors
	Basic implementation for UI-based data editors.

	\sa DataModel

	\ingroup ApplicationFramework
*/

/**
	\defgroup ParameterManagement Parameter Management
	Implementation of parameter management concept.

	\ingroup ApplicationFramework
*/

/**
	\defgroup DocumentBasedFramework Document-based Framework
	Framework for document-based applications (MDI, SDI)

	\ingroup ApplicationFramework
*/

/**
	\defgroup NumericalBase Numerical Basics & Algorithms
	Basic implementations of mathematical and algorithmical primitives.
*/

/**
	\defgroup Geometry
	Basic implementations of geometrical primitives in 2D- and 3D-space.

	\ingroup NumericalBase
*/

/**
	\defgroup Color Color
	Data primitives and algorithms related to color definition and management.
*/


#endif // !ACF_included


