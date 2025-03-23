// =================================================================================
// Filename:     VectorTests.h
// Description:  unit-tests for DoorsEngine custom cvector
// 
// Created:      17.12.24
// =================================================================================
#pragma once

#include "cvector.h"
#include <string>

class VectorTests
{


public:
    void Run();

    // test constructors
    void TestEmptyVector();
    void TestFillSameVal();
    void TestConstructorTakesInitList();
    void TestCopyConstructor();
    void TestMoveConstructor();
    void TestConstructorTakesIterators();

    // test methods
    void TestPushBack();
    void TestPopBack();
    void TestErase();
    void TestClear();

    void TestReserve();
    void TestResize();
    void TestResizeWithVal();

    void TestOperatorEqual();
    void TestOperatorCopyAssignment();
    void TestOperatorMoveAssignment();
    void TestOperatorAssignInitList();

    // test shift/insert methods
    void TestShiftRight();
    void TestShiftLeft();
    void TestInsertBefore();

    void TestAppendCopyVector();
    void TestAppendMoveVector();

    // test seach methods
    void TestFind();
    void TestGetIdx();
    void TestGetIdxsFromRawArr();
    void TestGetIdxsFromVector();
    void TestGetDataByIdxs1();
    void TestGetDataByIdxs2();
    void TestGetInsertIdx();
    void TestGetInsertIdxs();
    void TestSortedInsertionMultiple();

    void TestHasValue();
    void TestBinarySearchForSingle();
    void TestBinarySearchForMultiple();
    void TestBinarySearchForRawMultiple();
    void TestBinarySearchForRawMultipleOutFlags();

    // test memory deallocation
    void TestShrinkToFit();
    void TestPurge();

private:

    void Assert(bool condition, const char* msg);
    void Assert(bool condition, const std::string& msg);
    void PrintTestBlockHeader(const char* str);
    void PrintTestName(const char* str);
    void PrintPassed();

    template <typename T>
    void AssertVectorsEqual(const cvector<T>& vDst, const cvector<T>& vSrc);

    template <typename T>
    void AssertVectorsEqual(const T* vDst, const T* vSrc, const vsize numElems);

    template <typename T>
    void PrintVector(const cvector<T>& v, const std::string& msg = "");

    // helpers: test insert_before() for integers and std::strings
    void TestInsertBeforeForInt();
    void TestInsertBeforeForStr();
};