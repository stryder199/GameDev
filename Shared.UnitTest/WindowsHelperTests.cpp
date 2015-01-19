#include "stdafx.h"
#include "CppUnitTest.h"
#include "WindowsHelpers.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace SharedUnitTest
{		
	TEST_CLASS(WindowsHelperTests)
	{
	public:
		
        TEST_METHOD(FolderDoesExist)
		{
            Assert::IsTrue(WindowsHelpers::FolderExists("C:\\Windows"));
		}

        TEST_METHOD(FolderDoesNotExist)
        {
            Assert::IsFalse(WindowsHelpers::FolderExists("C:\\Wksdfjasnbka"));
        }

	};
}