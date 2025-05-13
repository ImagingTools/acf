#include <ibase/Test/CCumulatedProgressManagerBaseTest.h>


// ACF includes
#include <ibase/CCumulatedProgressManagerBase.h>


void CCumulatedProgressManagerBaseTest::DoOverflowTest1()
{
	ibase::CCumulatedProgressManagerBase progress;

	std::unique_ptr<ibase::IProgressLogger> loggerPtr = progress.StartProgressLogger();
	loggerPtr->OnProgress(1);

	std::unique_ptr<ibase::IProgressManager> subtaskPtr = progress.CreateSubtaskManager("subtask1", "", 0.5);
	std::unique_ptr<ibase::IProgressLogger> subtaskLoggerPtr = subtaskPtr->StartProgressLogger();
	subtaskLoggerPtr->OnProgress(1);

	double cumulatedProgress = progress.GetCumulatedProgress();

	QVERIFY(cumulatedProgress < 1 || qFuzzyCompare(cumulatedProgress, 1.0));
}


I_ADD_TEST(CCumulatedProgressManagerBaseTest);


