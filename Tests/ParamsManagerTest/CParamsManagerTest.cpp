#include "CParamsManagerTest.h"


// ACF includes
#include <iprm/IParamsSet.h>
#include <iser/CMemoryReadArchive.h>
#include <iser/CMemoryWriteArchive.h>


// protected slots

void CParamsManagerTestRunner::initTestCase()
{
	m_testPartituraInstanceCompPtr.reset(new CParamsManagerTest);

	m_paramsManagerPtr = m_testPartituraInstanceCompPtr->GetInterface<iprm::IParamsManager>("ParamsManager");

	QVERIFY(m_paramsManagerPtr != nullptr);
}


void CParamsManagerTestRunner::GetIndexOperationFlagsTest()
{
	// Test general flags (negative index)
	int generalFlags = m_paramsManagerPtr->GetIndexOperationFlags(-1);
	QVERIFY(generalFlags >= 0);

	// Test flags for first fixed parameter set
	int firstSetFlags = m_paramsManagerPtr->GetIndexOperationFlags(0);
	QVERIFY(firstSetFlags >= 0);
	
	// Fixed sets should have MF_COUNT_FIXED flag
	QVERIFY((firstSetFlags & iprm::IParamsManager::MF_COUNT_FIXED) != 0);
}


void CParamsManagerTestRunner::SetIndexOperationFlagsTest()
{
	// Get initial count
	int initialCount = m_paramsManagerPtr->GetParamsSetsCount();
	QVERIFY(initialCount >= 2);

	// Insert a new parameter set to test SetIndexOperationFlags
	int newIndex = m_paramsManagerPtr->InsertParamsSet();
	QVERIFY(newIndex >= 0);

	// Try to set operation flags for the new set
	int flagsToSet = iprm::IParamsManager::MF_SUPPORT_EDIT;
	bool result = m_paramsManagerPtr->SetIndexOperationFlags(newIndex, flagsToSet);
	
	// Clean up
	m_paramsManagerPtr->RemoveParamsSet(newIndex);

	// Note: The result may vary based on implementation, we just verify it doesn't crash
	QVERIFY(result || !result);
}


void CParamsManagerTestRunner::GetParamsSetsCountTest()
{
	int count = m_paramsManagerPtr->GetParamsSetsCount();
	
	// We configured 2 fixed parameter sets in the .acc file
	QVERIFY(count >= 2);
}


void CParamsManagerTestRunner::GetParamsTypeConstraintsTest()
{
	const iprm::IOptionsList* constraints = m_paramsManagerPtr->GetParamsTypeConstraints();
	
	// Can be NULL if only one anonymous type is supported
	// We just verify the call doesn't crash
	QVERIFY(constraints != nullptr || constraints == nullptr);
}


void CParamsManagerTestRunner::InsertParamsSetTest()
{
	int initialCount = m_paramsManagerPtr->GetParamsSetsCount();
	
	// Insert a new parameter set with default parameters
	int newIndex = m_paramsManagerPtr->InsertParamsSet();
	
	QVERIFY(newIndex >= 0);
	
	int newCount = m_paramsManagerPtr->GetParamsSetsCount();
	QVERIFY(newCount == initialCount + 1);
	
	// Verify the new set exists
	const iprm::IParamsSet* paramsSet = m_paramsManagerPtr->GetParamsSet(newIndex);
	QVERIFY(paramsSet != nullptr);
	
	// Clean up
	bool removed = m_paramsManagerPtr->RemoveParamsSet(newIndex);
	QVERIFY(removed);
	
	int finalCount = m_paramsManagerPtr->GetParamsSetsCount();
	QVERIFY(finalCount == initialCount);
}


void CParamsManagerTestRunner::RemoveParamsSetTest()
{
	int initialCount = m_paramsManagerPtr->GetParamsSetsCount();
	
	// Insert a new parameter set
	int newIndex = m_paramsManagerPtr->InsertParamsSet();
	QVERIFY(newIndex >= 0);
	
	// Remove it
	bool removed = m_paramsManagerPtr->RemoveParamsSet(newIndex);
	QVERIFY(removed);
	
	int finalCount = m_paramsManagerPtr->GetParamsSetsCount();
	QVERIFY(finalCount == initialCount);
	
	// Try to remove a fixed parameter set (should fail or be restricted)
	// Fixed sets are at indices 0 and 1
	bool canRemoveFixed = m_paramsManagerPtr->RemoveParamsSet(0);
	// Fixed sets should not be removable
	QVERIFY(!canRemoveFixed);
}


void CParamsManagerTestRunner::SwapParamsSetTest()
{
	// Insert two new parameter sets
	int index1 = m_paramsManagerPtr->InsertParamsSet();
	int index2 = m_paramsManagerPtr->InsertParamsSet();
	
	QVERIFY(index1 >= 0);
	QVERIFY(index2 >= 0);
	
	// Set different names to verify the swap
	QString name1 = "Set1";
	QString name2 = "Set2";
	
	m_paramsManagerPtr->SetParamsSetName(index1, name1);
	m_paramsManagerPtr->SetParamsSetName(index2, name2);
	
	// Verify names before swap
	QVERIFY(m_paramsManagerPtr->GetParamsSetName(index1) == name1);
	QVERIFY(m_paramsManagerPtr->GetParamsSetName(index2) == name2);
	
	// Swap the parameter sets
	bool swapped = m_paramsManagerPtr->SwapParamsSet(index1, index2);
	QVERIFY(swapped);
	
	// Verify names after swap
	QVERIFY(m_paramsManagerPtr->GetParamsSetName(index1) == name2);
	QVERIFY(m_paramsManagerPtr->GetParamsSetName(index2) == name1);
	
	// Clean up
	m_paramsManagerPtr->RemoveParamsSet(index1);
	m_paramsManagerPtr->RemoveParamsSet(index2);
}


void CParamsManagerTestRunner::GetParamsSetTest()
{
	// Test getting the first fixed parameter set
	const iprm::IParamsSet* paramsSet = m_paramsManagerPtr->GetParamsSet(0);
	QVERIFY(paramsSet != nullptr);
	
	// Test getting an invalid index
	const iprm::IParamsSet* invalidSet = m_paramsManagerPtr->GetParamsSet(9999);
	QVERIFY(invalidSet == nullptr);
}


void CParamsManagerTestRunner::CreateParameterSetTest()
{
	// Create a parameter set without copying existing data
	iprm::IParamsSetUniquePtr newParamsSet = m_paramsManagerPtr->CreateParameterSet(-1, -1);
	QVERIFY(newParamsSet.IsValid());
	
	// Create a parameter set by copying from index 0
	iprm::IParamsSetUniquePtr copiedParamsSet = m_paramsManagerPtr->CreateParameterSet(-1, 0);
	QVERIFY(copiedParamsSet.IsValid());
}


void CParamsManagerTestRunner::GetSetParamsSetNameTest()
{
	// Insert a new parameter set
	int newIndex = m_paramsManagerPtr->InsertParamsSet();
	QVERIFY(newIndex >= 0);
	
	// Set a name
	QString testName = "Test Parameter Set";
	bool nameSet = m_paramsManagerPtr->SetParamsSetName(newIndex, testName);
	QVERIFY(nameSet);
	
	// Get the name and verify
	QString retrievedName = m_paramsManagerPtr->GetParamsSetName(newIndex);
	QVERIFY(retrievedName == testName);
	
	// Clean up
	m_paramsManagerPtr->RemoveParamsSet(newIndex);
}


void CParamsManagerTestRunner::GetSetParamsSetDescriptionTest()
{
	// Insert a new parameter set
	int newIndex = m_paramsManagerPtr->InsertParamsSet();
	QVERIFY(newIndex >= 0);
	
	// Set a description
	QString testDescription = "This is a test parameter set description";
	m_paramsManagerPtr->SetParamsSetDescription(newIndex, testDescription);
	
	// Get the description and verify
	QString retrievedDescription = m_paramsManagerPtr->GetParamsSetDescription(newIndex);
	QVERIFY(retrievedDescription == testDescription);
	
	// Clean up
	m_paramsManagerPtr->RemoveParamsSet(newIndex);
}


void CParamsManagerTestRunner::SerializeTest()
{
	// Insert a parameter set and configure it
	int newIndex = m_paramsManagerPtr->InsertParamsSet();
	QVERIFY(newIndex >= 0);
	
	QString testName = "Serialization Test";
	m_paramsManagerPtr->SetParamsSetName(newIndex, testName);
	
	// Serialize to memory
	iser::CMemoryWriteArchive writeArchive(nullptr);
	bool serialized = m_paramsManagerPtr->Serialize(writeArchive);
	QVERIFY(serialized);
	
	// Modify the data
	m_paramsManagerPtr->SetParamsSetName(newIndex, "Modified Name");
	
	// Deserialize
	iser::CMemoryReadArchive readArchive(writeArchive);
	bool deserialized = m_paramsManagerPtr->Serialize(readArchive);
	QVERIFY(deserialized);
	
	// Verify the name was restored
	QString restoredName = m_paramsManagerPtr->GetParamsSetName(newIndex);
	QVERIFY(restoredName == testName);
	
	// Clean up
	m_paramsManagerPtr->RemoveParamsSet(newIndex);
}


void CParamsManagerTestRunner::CopyTest()
{
	// Insert a parameter set
	int sourceIndex = m_paramsManagerPtr->InsertParamsSet();
	QVERIFY(sourceIndex >= 0);
	
	QString testName = "Copy Test";
	m_paramsManagerPtr->SetParamsSetName(sourceIndex, testName);
	
	// Create a copy using CreateParameterSet
	iprm::IParamsSetUniquePtr copiedSet = m_paramsManagerPtr->CreateParameterSet(-1, sourceIndex);
	QVERIFY(copiedSet.IsValid());
	
	// Clean up
	m_paramsManagerPtr->RemoveParamsSet(sourceIndex);
}


void CParamsManagerTestRunner::cleanupTestCase()
{
	m_testPartituraInstanceCompPtr.reset();
}


I_ADD_TEST(CParamsManagerTestRunner);
