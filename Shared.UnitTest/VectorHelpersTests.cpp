#include "stdafx.h"
#include "CppUnitTest.h"
#include "VectorHelpers.h"
#include "DirectXMath.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace DirectX;

namespace SharedUnitTest
{
    TEST_CLASS(VectorHelpersTests)
    {
    public:
        
        TEST_METHOD(IsDirectlyBehind)
        {
            XMFLOAT3 playerPos = XMFLOAT3(4.0f, 0.0f, 5.0f);
            XMFLOAT3 playerDir = XMFLOAT3(0.0f, 0.0f, 1.0f);
            XMFLOAT3 targetPos = XMFLOAT3(4.0f, 0.0f, -1.0f);
            Assert::IsTrue(VectorHelpers::IsBehind(playerPos, playerDir, targetPos));
            Assert::IsFalse(VectorHelpers::IsInFront(playerPos, playerDir, targetPos));
        }

        TEST_METHOD(IsDirectlyInFront)
        {
            XMFLOAT3 playerPos = XMFLOAT3(4.0f, 0.0f, 5.0f);
            XMFLOAT3 playerDir = XMFLOAT3(0.0f, 0.0f, 1.0f);
            XMFLOAT3 targetPos = XMFLOAT3(4.0f, 0.0f, 8.0f);
            Assert::IsTrue(VectorHelpers::IsInFront(playerPos, playerDir, targetPos));
            Assert::IsFalse(VectorHelpers::IsBehind(playerPos, playerDir, targetPos));
        }

        TEST_METHOD(IsDirectlyToLeft)
        {
            XMFLOAT3 playerPos = XMFLOAT3(4.0f, 0.0f, 5.0f);
            XMFLOAT3 playerDir = XMFLOAT3(0.0f, 0.0f, 1.0f);
            XMFLOAT3 targetPos = XMFLOAT3(-1.0f, 0.0f, 5.0f);
            Assert::IsTrue(VectorHelpers::IsToLeft(playerPos, playerDir, targetPos));
            Assert::IsFalse(VectorHelpers::IsToRight(playerPos, playerDir, targetPos));
        }

        TEST_METHOD(IsDirectlyToRight)
        {
            XMFLOAT3 playerPos = XMFLOAT3(4.0f, 0.0f, 5.0f);
            XMFLOAT3 playerDir = XMFLOAT3(0.0f, 0.0f, 1.0f);
            XMFLOAT3 targetPos = XMFLOAT3(6.0f, 0.0f, 5.0f);
            Assert::IsTrue(VectorHelpers::IsToRight(playerPos, playerDir, targetPos));
            Assert::IsFalse(VectorHelpers::IsToLeft(playerPos, playerDir, targetPos));
        }

        TEST_METHOD(IsInFrontAndRight)
        {
            XMFLOAT3 playerPos = XMFLOAT3(4.0f, 0.0f, 5.0f);
            XMFLOAT3 playerDir = XMFLOAT3(0.0f, 0.0f, 1.0f);
            XMFLOAT3 targetPos = XMFLOAT3(4.5f, 0.0f, 11.0f);
            Assert::IsTrue(VectorHelpers::IsInFront(playerPos, playerDir, targetPos));
            Assert::IsFalse(VectorHelpers::IsBehind(playerPos, playerDir, targetPos));
            Assert::IsTrue(VectorHelpers::IsToRight(playerPos, playerDir, targetPos));
            Assert::IsFalse(VectorHelpers::IsToLeft(playerPos, playerDir, targetPos));
        }

        TEST_METHOD(IsInFrontAndLeft)
        {
            XMFLOAT3 playerPos = XMFLOAT3(4.0f, 0.0f, 5.0f);
            XMFLOAT3 playerDir = XMFLOAT3(0.0f, 0.0f, 1.0f);
            XMFLOAT3 targetPos = XMFLOAT3(0.0f, 0.0f, 10.0f);
            Assert::IsTrue(VectorHelpers::IsInFront(playerPos, playerDir, targetPos));
            Assert::IsFalse(VectorHelpers::IsBehind(playerPos, playerDir, targetPos));
            Assert::IsTrue(VectorHelpers::IsToLeft(playerPos, playerDir, targetPos));
            Assert::IsFalse(VectorHelpers::IsToRight(playerPos, playerDir, targetPos));
        }

        TEST_METHOD(IsInBehindAndRight)
        {
            XMFLOAT3 playerPos = XMFLOAT3(4.0f, 0.0f, 5.0f);
            XMFLOAT3 playerDir = XMFLOAT3(0.0f, 0.0f, 1.0f);
            XMFLOAT3 targetPos = XMFLOAT3(4.1f, 0.0f, 4.9f);
            Assert::IsTrue(VectorHelpers::IsBehind(playerPos, playerDir, targetPos));
            Assert::IsFalse(VectorHelpers::IsInFront(playerPos, playerDir, targetPos));
            Assert::IsTrue(VectorHelpers::IsToRight(playerPos, playerDir, targetPos));
            Assert::IsFalse(VectorHelpers::IsToLeft(playerPos, playerDir, targetPos));
        }

        TEST_METHOD(IsInBehindAndLeft)
        {
            XMFLOAT3 playerPos = XMFLOAT3(4.0f, 0.0f, 5.0f);
            XMFLOAT3 playerDir = XMFLOAT3(0.0f, 0.0f, 1.0f);
            XMFLOAT3 targetPos = XMFLOAT3(2.0f, 0.0f, 2.5f);
            Assert::IsTrue(VectorHelpers::IsBehind(playerPos, playerDir, targetPos));
            Assert::IsFalse(VectorHelpers::IsInFront(playerPos, playerDir, targetPos));
            Assert::IsTrue(VectorHelpers::IsToLeft(playerPos, playerDir, targetPos));
            Assert::IsFalse(VectorHelpers::IsToRight(playerPos, playerDir, targetPos));
        }

        TEST_METHOD(NotCollided_Sphere)
        {
            XMFLOAT3 pos = XMFLOAT3(2.5f, 0.0f, 0.0f);
            XMFLOAT3 pos2 = XMFLOAT3(0.0f, 0.0f, 0.0f);
            float r = 1.0f;
            Assert::IsFalse(VectorHelpers::SphereSphereCollision(pos, r, pos2, r));
        }

        TEST_METHOD(Collided_Sphere)
        {
            XMFLOAT3 pos = XMFLOAT3(1.5f, 0.0f, 0.0f);
            XMFLOAT3 pos2 = XMFLOAT3(0.0f, 0.0f, 0.0f);
            float r = 1.0f;
            Assert::IsTrue(VectorHelpers::SphereSphereCollision(pos, r, pos2, r));
        }
    };
}