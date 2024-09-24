#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>

// ACF includes
#include <itest/CStandardTestExecutor.h>
#include <ipackage/CComponentAccessor.h>


class CPartituraBasedTestBase : public QObject
{
	Q_OBJECT
protected:
	ipackage::CComponentAccessor* GetComponentAccessor();

protected:
	QString m_acfPath;
	QString m_registryFile;
	QString m_configFile;

	std::unique_ptr<ipackage::CComponentAccessor> m_accessorPtr;
};


template <const char* partituraName>
class CPartituraBasedTest: public CPartituraBasedTestBase
{
protected:
	virtual void initTestCase()
	{
		m_acfPath = qEnvironmentVariable("ACFDIR");
		m_registryFile = m_acfPath + QString("/IntegrationTest/Partitura/%1.acc").arg(partituraName);
		m_configFile = m_acfPath + QString("/IntegrationTest/Config/AcfIntegrationTest.awc");

		if (m_accessorPtr == nullptr){
			m_accessorPtr.reset(new ipackage::CComponentAccessor(m_registryFile, m_configFile));
		}
	}

	virtual void cleanupTestCase()
	{
	}
};
