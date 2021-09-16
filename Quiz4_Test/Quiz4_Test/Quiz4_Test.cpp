#include "pch.h"
#include "CppUnitTest.h"
#include "C:\Users\kjones\source\school\Foothill-CS2C\Quiz4\Quiz4\quiz4_include.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Quiz4Test
{
	TEST_CLASS(Quiz4Test)
	{
	public:
		
		TEST_METHOD(numhilbertcurvecorners)
		{
			std::map<int, long> values;
			for (int i = 0; i < 10; ++i) {
				auto val = num_hilbert_curve_corners(i, values);
			}
			Assert::AreEqual(long(0), num_hilbert_curve_corners(0, values));
			Assert::AreEqual(long(2), num_hilbert_curve_corners(1, values));
			Assert::AreEqual(long(12), num_hilbert_curve_corners(2, values));
			Assert::AreEqual(long(50), num_hilbert_curve_corners(3, values));
			Assert::AreEqual(long(204), num_hilbert_curve_corners(4, values));
		}
		TEST_METHOD(removeuppercase)
		{
			std::set<std::string> question4 = { "First","sEcond","lasT","ALL","none","SoMe","sjkdfhsksdjkf","the" };
			remove_upper_case(question4);
			for (auto it : question4) {
				for (auto c : it) {
					Assert::IsTrue(islower(c));
				}
			}
		}
	};
}
