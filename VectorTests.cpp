// =================================================================================
// Filename:     VectorTests.cpp
// Description:  unit-tests for DoorsEngine custom cvector
// 
// Created:      17.12.24
// =================================================================================
#pragma once

#include "VectorTests.h"
#include <string>
#include <format>
#include <iostream>


// some flags to control console text attributes
#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

const cvector<int>         v1234{ 1,2,3,4 };
const cvector<int>         v1to10{ 1,2,3,4,5,6,7,8,9,10 };
const cvector<std::string> vABCD{ "a","b","c","d" };
const cvector<std::string> vAtoH{ "a","b","c","d","e","f","g","h" };


void VectorTests::Run()
{
    // test constructors
    PrintTestBlockHeader("TEST CONSTRUCTORS:");
    TestEmptyVector();
    TestFillSameVal();
    TestConstructorTakesInitList();
    TestCopyConstructor();
    TestMoveConstructor();
    TestConstructorTakesIterators();
    

    // test methods
    PrintTestBlockHeader("TEST methods:");
    TestPushBack();
    TestPopBack();
    TestErase();
    TestClear();
    TestReserve();
    TestResize();
    TestResizeWithVal();

    std::cout << std::endl;

    PrintTestBlockHeader("TEST operators:");
    TestOperatorEqual();
    TestOperatorCopyAssignment();
    TestOperatorMoveAssignment();
    TestOperatorAssignInitList();

    std::cout << std::endl;

    PrintTestBlockHeader("TEST shift/insert/append:");
    TestShiftRight();
    TestShiftLeft();
    TestInsertBefore();
    TestGetInsertIdx();
    TestGetInsertIdxs();
    TestSortedInsertionMultiple();
    TestAppendCopyVector();
    TestAppendMoveVector();

    std::cout << std::endl;

    PrintTestBlockHeader("TEST search/find/get:");
    TestFind();
    TestGetIdx();
    TestGetIdxsFromRawArr();
    TestGetIdxsFromVector();
    TestGetDataByIdxs1();
    TestGetDataByIdxs2();

    std::cout << std::endl;

    PrintTestBlockHeader("TEST has value(s):");
    TestHasValue();
    TestBinarySearchForSingle();
    TestBinarySearchForMultiple();
    TestBinarySearchForRawMultiple();
    TestBinarySearchForRawMultipleOutFlags();

    std::cout << std::endl;

    // test memory deallocation
    TestShrinkToFit();
    TestPurge();
    

    printf("\n\n%sALL THE TEST ARE PASSED%s\n", KCYN, KNRM);
}


// =================================================================================
//                             test constructors
// =================================================================================

void VectorTests::TestEmptyVector()
{
    PrintTestName("Test cvector():");

    cvector<int> v1;
    cvector<std::string> v2;

    Assert(v1.size() == 0, "size is wrong");
    Assert(v1.empty() == true, "arr isn't empty");
    Assert(v1.capacity() == 0, "capacity is wrong");

    Assert(v2.size() == 0, "size is wrong");
    Assert(v2.empty() == true, "arr isn't empty");
    Assert(v2.capacity() == 0, "capacity is wrong");

    PrintPassed();
}

///////////////////////////////////////////////////////////

void VectorTests::TestFillSameVal()
{
    // TEST constructor: cvector is filled in n times with the same value
    PrintTestName("Test cvector(const vsize n, const T&): ");

    const vsize      size = 10;
    const int         rawArr1[size] = { 10,10,10,10,10,10,10,10,10,10 };
    const std::string rawArr2[size] = { "a","a","a","a","a","a","a","a","a","a" };

    // check these vectors
    const cvector<int>         v1(size, 10);        
    const cvector<std::string> v2(size, "a");

    // case_1: cvector of integers
    Assert(v1.size() == size, "wrong size");
    AssertVectorsEqual(v1.data(), rawArr1, v1.size());

    // case_2: cvector of std::string
    Assert(v2.size() == size, "wrong size");
    AssertVectorsEqual(v2.data(), rawArr2, v2.size());

    PrintPassed();
}

///////////////////////////////////////////////////////////

void VectorTests::TestConstructorTakesInitList()
{
    // TEST: constructor takes an initializer list
    PrintTestName("Test cvector(init_list<T>&)");

    const int         numElems = 5;
    const int         rawArr1[numElems] = { 1,2,3,4,5 };
    const std::string rawArr2[numElems] = { "a","b","c","d","e" };

    const cvector<int>         v1{ 1,2,3,4,5 };
    const cvector<std::string> v2({ "a","b","c","d","e" });

    AssertVectorsEqual(v1.data(), rawArr1, v1.size());
    AssertVectorsEqual(v2.data(), rawArr2, v2.size());
    PrintPassed();
}

///////////////////////////////////////////////////////////

void VectorTests::TestCopyConstructor()
{
    PrintTestName("Test cvector(const cvector&):");

    const cvector<int> vIntSrc{ 1,2,3,4,5 };
    const cvector<int> vIntDst(vIntSrc);

    const cvector<std::string> vStrSrc = { "copy0", "copy1", "copy2" };
    const cvector<std::string> vStrDst(vStrSrc);

    AssertVectorsEqual(vIntDst, vIntSrc);
    AssertVectorsEqual(vStrDst, vStrSrc);

    PrintPassed();
}

///////////////////////////////////////////////////////////

void VectorTests::TestMoveConstructor()
{
    PrintTestName("Test cvector(cvector<string>&&):");

    const cvector<int>         vIntSrc{ 1,2,3,4,5 };
    const cvector<std::string> vStrSrc = { "mov0", "mov1", "mov2" };

    cvector<int>               vIntTemp(vIntSrc);
    cvector<std::string>       vStrTemp(vStrSrc);

    // move data from temp cvector into the destination cvector
    const cvector<int>         vIntDst(std::move(vIntTemp));
    const cvector<std::string> vStrDst(std::move(vStrTemp));

    AssertVectorsEqual(vIntDst, vIntSrc);
    AssertVectorsEqual(vStrDst, vStrSrc);

    Assert(vIntTemp.empty() == true, "moved cvector isn't empty");
    Assert(vStrTemp.empty() == true, "moved cvector isn't empty");

    PrintPassed();
}

///////////////////////////////////////////////////////////

void VectorTests::TestConstructorTakesIterators()
{
    // TEST constructor takes iterators (range of elements: first/last)
    PrintTestName("Test cvector(const Iter* first, const Iter* last):");
     
    constexpr vsize           numElems = 5;
    const int                 arrSrc[numElems] = { 1,2,3,4,5 };

    const cvector<int>         vSrc1 = { 1,2,3,4,5 };
    const cvector<std::string> vSrc2 = { "str1", "str2", "str3" };


    const cvector<int> vDst0(arrSrc, arrSrc + numElems);          // takes raw arr
    const cvector<int> vDst1(vSrc1.begin(), vSrc1.end());         // takes iterators from cvector (POD-types)
    const cvector<std::string> vDst2(vSrc2.begin(), vSrc2.end()); // takes iterators from cvector (std::string)

    AssertVectorsEqual(vDst0.data(), arrSrc, numElems);
    AssertVectorsEqual(vDst1, vSrc1);
    AssertVectorsEqual(vDst2, vSrc2);

    PrintPassed();
}



// =================================================================================
//                               test methods
// =================================================================================

void VectorTests::TestPushBack()
{
    // test push_back() for integers and std::strings
    PrintTestName("Test push_back():");

    constexpr int size = 129;
    cvector<int> vInt;
    cvector<std::string> vStr;

    for (int i = 0; i < size; ++i)	vInt.push_back(i);
    for (int i = 0; i < size; ++i)	vStr.push_back("a");

    Assert(vInt.size() == size, "wrong size");
    Assert(vStr.size() == size, "wrong size");

    for (vsize i = 0; i < vInt.size(); ++i)
        Assert(vInt[i] == i, std::format("vInt[{}] != {}", i, i));

    for (vsize i = 0; i < vStr.size(); ++i)
        Assert(vStr[i] == "a", std::format("vStr[{}] != {}", i, "a"));

    PrintPassed();
}

///////////////////////////////////////////////////////////

void VectorTests::TestPopBack()
{
    PrintTestName("Test pop_back():");

    const cvector<int>         vIntToCmp({ 10, 20, 30, 40, 50, 60 });
    const cvector<std::string> vStrToCmp({ "a","b","c","d","e","f" });

    cvector<int>         v0;
    cvector<int>         v1(vIntToCmp);
    cvector<std::string> v2(vStrToCmp);
    

    // pop back two elements
    v0.pop_back();

    v1.pop_back();
    v1.pop_back();

    v2.pop_back();
    v2.pop_back();

    AssertVectorsEqual(v1, { 10, 20, 30, 40 });
    AssertVectorsEqual(v2, { "a","b","c","d" });
    PrintPassed();
}

///////////////////////////////////////////////////////////

void VectorTests::TestErase()
{
    PrintTestName("Test erase():");

    const cvector<int> v1ToCmp{ 20,30,40,50 };
    const cvector<int> v2ToCmp{ 20,40,50 };
    const cvector<std::string> v3ToCmp{ "a","b","c","c" };
    const cvector<std::string> v4ToCmp{ "a","b","c" };

    cvector<int> v1{ 10,20,30,40,50 };
    cvector<std::string> v2{ "a","a","b","c","c" };

    v1.erase(0);                      // remove 10
    AssertVectorsEqual(v1, v1ToCmp);
    v1.erase(1);                      // remove 30
    AssertVectorsEqual(v1, v2ToCmp);

    v2.erase(0);                      // remove first "a"
    AssertVectorsEqual(v2, v3ToCmp);
    vsize lastIdx = v2.size() - 1;
    v2.erase(lastIdx);                // remove last "c"
    AssertVectorsEqual(v2, v4ToCmp);

    PrintPassed();
}

///////////////////////////////////////////////////////////

void VectorTests::TestClear()
{
    PrintTestName("Test clear():");

    cvector<int> vInt(10, 10);
    cvector<std::string> vStr(10, "a");

    vInt.clear();
    vStr.clear();

    Assert(vInt.size() == 0, "size must == 0");
    Assert(vStr.size() == 0, "size must == 0");

    PrintPassed();
}

///////////////////////////////////////////////////////////

void VectorTests::TestReserve()
{
    PrintTestName("Test reserve(const vsize newCapacity):");

    cvector<int> v1{ 1,2,3 };                     // currently capacity == size
    Assert(v1.size() == 3, "wrong size");
    Assert(v1.capacity() == 3, "wrong capacity");

    v1.reserve(10);                              // we have the same size but the capacity has grown
    Assert(v1.size() == 3, "wrong size");
    Assert(v1.capacity() == 10, "wrong capacity");

    v1.reserve(2);                               // we can't reduce the capacity using reserve() so we just do nothing 
    Assert(v1.size() == 3, "wrong size");
    Assert(v1.capacity() == 10, "wrong capacity");

    PrintPassed();
}

///////////////////////////////////////////////////////////

void VectorTests::TestResize()
{
    PrintTestName("Test resize(const vsize sz):");

    // case_1: test with POD-type
    const cvector<int> vSrc1{ 1,2,0,0,0,0 };
    cvector<int> vDst1{ 1,2 };

    // resize to 6 elems and fill in the new elems with 0 by default
    vDst1.resize(6);
    AssertVectorsEqual(vDst1, vSrc1);

    // case_2: test with std::string
    const cvector<std::string> vSrc2{ "a", "b", "", "", "" };
    cvector<std::string> vDst2{ "a", "b" };

    // resize to 5 elems and fill in the new elems with "" by default
    vDst2.resize(5);
    AssertVectorsEqual(vDst2, vSrc2);

    // test if size is getting smaller and capacity remains the same
    cvector<std::string> vStr(vAtoH);
    const vsize capacity = vStr.capacity();

    vStr.resize(3);
    Assert((vStr.size() == 3) && vStr.capacity() == capacity, "wrong data");

    vStr.resize(1);
    Assert((vStr.size() == 1) && vStr.capacity() == capacity, "wrong data");

    vStr.resize(0);
    Assert((vStr.size() == 0) && vStr.capacity() == capacity, "wrong data");

    PrintPassed();
}

///////////////////////////////////////////////////////////

void VectorTests::TestResizeWithVal()
{
    PrintTestName("Test resize(const vsize sz, const T& val):");

    // case_1: test with POD-type
    const cvector<int> vSrc1{ 1,2,3,3,3,3 };
    cvector<int> vDst1{ 1,2 };

    // resize to 6 elems and fill in the new elems with 3
    vDst1.resize(6, 3);
    AssertVectorsEqual(vDst1, vSrc1);

    // case_2: test with std::string
    const cvector<std::string> vSrc2{ "a", "b", "c", "c", "c" };
    cvector<std::string> vDst2{ "a", "b" };

    // resize to 5 elems and fill in the new elems with "c"
    vDst2.resize(5, "c");
    AssertVectorsEqual(vDst2, vSrc2);

    PrintPassed();
}

///////////////////////////////////////////////////////////

void VectorTests::TestOperatorEqual()
{
    PrintTestName("Test operator==(const cvector<T>&) const:");

    const cvector<int>         v1Int{ 1,2,3,4,5 };
    const cvector<int>         v2Int(v1Int);

    const cvector<std::string> v1Str{ "a","b","c","d" };
    const cvector<std::string> v2Str(v1Str);

    // test operator==
    Assert(v1Int == v2Int, "vectors !equal");
    Assert(v1Str == v2Str, "vectors !equal");

    AssertVectorsEqual(v1Int, v2Int);
    AssertVectorsEqual(v1Str, v2Str);	

    PrintPassed();
}

///////////////////////////////////////////////////////////

void VectorTests::TestOperatorCopyAssignment()
{
    PrintTestName("Test operator=(const cvector<T>&):");

    cvector<std::string> v1;
    cvector<std::string> v2{ "a","b","c","d" };
    const cvector<std::string> v1Str{ "a","b","c" };
    const cvector<std::string> v2Str{ "a","b","c","d","e","f","g","h","x","y","z" };

    // case_1: copy to an empty cvector
    v1 = v2Str;
    AssertVectorsEqual(v1, v2Str);

    // case_2: copy a lower cvector to a cvector which already has values
    v2 = v1Str;
    AssertVectorsEqual(v2, v1Str);

    // case_3: copy a bigger cvector to a cvector which already has values
    v2 = v2Str;
    AssertVectorsEqual(v2, v2Str);

    PrintPassed();
}

///////////////////////////////////////////////////////////

void VectorTests::TestOperatorMoveAssignment()
{
    PrintTestName("Test operator=(cvector<T>&&):");

    // src data
    const std::string arr1[3] = { "a","b","c" };
    const std::string arr2[3] = { "a","b","c" };
    const std::string arr3[11] = { "a","b","c","d","e","f","g","h","x","y","z" };

    // vectors to test
    cvector<std::string> vDst1;
    cvector<std::string> vDst2{ "a","b","c","d" };

    // vectors to move
    cvector<std::string> vSrc1(arr1, arr1 + 3);
    cvector<std::string> vSrc2(arr2, arr2 + 3);
    cvector<std::string> vSrc3(arr3, arr3 + 11);


    // case_1: move to an empty cvector
    vDst1 = std::move(vSrc1);
    AssertVectorsEqual(vDst1.data(), arr1, vDst1.size());
    Assert(vSrc1.empty() == true, "cvector must be empty (1)");


    // case_2: move a smaller cvector to a cvector which already has values
    vDst2 = std::move(vSrc2);
    AssertVectorsEqual(vDst2.data(), arr2, vDst2.size());
    Assert(vSrc2.empty() == true, "cvector must be empty (2)");


    // case_3: move a bigger cvector to a cvector which already has values
    vDst2 = std::move(vSrc3);
    AssertVectorsEqual(vDst2.data(), arr3, vDst2.size());
    Assert(vSrc3.empty() == true, "cvector must be empty (3)");

    PrintPassed();
}


///////////////////////////////////////////////////////////

void VectorTests::TestOperatorAssignInitList()
{
    PrintTestName("Test operator=(std::initializer_list<T>):");

    cvector<int>         vInt1;
    cvector<int>         vInt2({ 1,2,3,4,5 });

    cvector<std::string> vStr1;
    cvector<std::string> vStr2{ "a","b","c" };

    vInt1 = { 1,2,3,4,5 };
    vStr1 = { "a","b","c" };

    AssertVectorsEqual(vInt1, vInt2);
    AssertVectorsEqual(vStr1, vStr2);

    PrintPassed();
}

///////////////////////////////////////////////////////////

void VectorTests::TestShiftRight()
{
    // test shift right elements of range [idx, end) by num positions
    PrintTestName("Test shift_right(const index idx, const int num):");

    // a flag to print cvector content into the console
    constexpr bool printDbgVector = false;  

    // case_1: shift right elems in cvector of integers
    cvector<int>               v1{ 1,2,3,4,5,6,7,8,9,10 };
    cvector<std::string>       v2{ "a","b","c","d","e","f","g" };

    const cvector<int>         v1AfterShift{ 1,2,3,4,3,4,5,6,7,8 };
#if DEBUG || _DEBUG
    const cvector<std::string> v2AfterShift{ "a","b","","","c","d","e" };
#else
    const cvector<std::string> v2AfterShift{ "a","b","c","d","c","d","e" };
#endif

    if constexpr (printDbgVector)
        PrintVector(v1, "\nv1 before shift: ");

    v1.shift_right(2, 2);
    AssertVectorsEqual(v1, v1AfterShift);

    if constexpr (printDbgVector)
        PrintVector(v1, "v1 after shift:  ");

    // ---------------------------------------------

    // case_2: shift right elems in cvector of strings
    if constexpr (printDbgVector)
        PrintVector(v2, "v2 before shift: ");

    v2.shift_right(2, 2);

    if constexpr (printDbgVector)
        PrintVector(v2, "v2 after shift:  ");

    AssertVectorsEqual(v2, v2AfterShift);

    PrintPassed();
}

///////////////////////////////////////////////////////////

void VectorTests::TestShiftLeft()
{
    // test shift left elements of range [idx, end) by num positions
    PrintTestName("Test shift_left(const index idx, const int num):");

    // a flag to print cvector content into the console
    constexpr bool printDbgVector = false;

    // case_1: shift elems in cvector of integers
    cvector<int>               v1(v1to10);
    cvector<std::string>       v2(vAtoH);

    const cvector<int>         v1AfterShift{ 1,2,5,6,7,8,9,10,9,10 };
    const cvector<std::string> v2AfterShift{ "a","b","e","f","g","h","",""};


    // ---------------------------------------------

    if constexpr (printDbgVector) 
        PrintVector(v1, "\nv1 before shift: ");

    // shift left starting from idx 2 by 2 positions
    v1.shift_left(2, 2);
    AssertVectorsEqual(v1, v1AfterShift);

    if constexpr (printDbgVector)
        PrintVector(v1, "v1 after shift:  ");

    // ---------------------------------------------

    // case_2: shift elems in cvector of strings
    if constexpr (printDbgVector)
        PrintVector(v2, "v2 before shift: ");

    // shift left starting from idx 2 by 2 positions
    v2.shift_left(2, 2);

    if constexpr (printDbgVector)
        PrintVector(v2, "v2 after shift:  ");

    AssertVectorsEqual(v2, v2AfterShift);

    PrintPassed();
}

///////////////////////////////////////////////////////////

void VectorTests::TestInsertBefore()
{
    PrintTestName("Test insert_before(const vsize idx, const T& val):");

    TestInsertBeforeForInt();
    TestInsertBeforeForStr();
    PrintPassed();
}

///////////////////////////////////////////////////////////

void VectorTests::TestGetInsertIdx()
{
    PrintTestName("Test get_insert_idx(const ptrdiff_t& value)");

    cvector<int> vInt({ 1, 3, 5 });

    // find insert position for value 0 and store it into the array right at this pos
    const index idx1 = vInt.get_insert_idx(0);
    vInt.insert_before(idx1, 0);

    const index idx2 = vInt.get_insert_idx(2);
    vInt.insert_before(idx2, 2);

    const index idx3 = vInt.get_insert_idx(4);
    vInt.insert_before(idx3, 4);

    const index idx4 = vInt.get_insert_idx(6);
    vInt.insert_before(idx4, 6);

    // check if we inserted all the values correctly
    AssertVectorsEqual(vInt, { 0,1,2,3,4,5,6 });

    PrintPassed();
}

///////////////////////////////////////////////////////////

void VectorTests::TestGetInsertIdxs()
{
    PrintTestName("Test get_insert_idxs(const cvector<ptrdiff_t>& values, cvector<index>& idxs:");

    constexpr int numElems = 7;
    cvector<int> vInt({ 1, 3, 5, 7, 10 });
    cvector<int> vIntsToInsert({ 0, 2, 4, 6, 9, 11, 15 });
    int arrIntsToInsert[numElems] = { 0, 2, 4, 6, 9, 11, 15 };

    cvector<index> idxs1;
    cvector<index> idxs2;
    cvector<index> expectedIdxs({ 0,1,2,3,4,5,5 });

    // case_1: using values from cvector
    vInt.get_insert_idxs(vIntsToInsert, idxs1);
    AssertVectorsEqual(idxs1, expectedIdxs);

    // case_2: using values from raw array
    vInt.get_insert_idxs(arrIntsToInsert, numElems, idxs2);
    AssertVectorsEqual(idxs2, expectedIdxs);

    PrintPassed();
}

///////////////////////////////////////////////////////////

void VectorTests::TestSortedInsertionMultiple()
{
    // test: get multiple insertion idxs and execute sorted insertion

    PrintTestName("Test sorted insertion multiple:");

    cvector<int>   vInt({ 1, 3, 5, 7, 10 });
    cvector<int>   vIntsToInsert({ 0, 2, 4, 6, 8, 9, 11, 15 });
    cvector<index> idxs;
    cvector<int>   vDst({ 0,1,2,3,4,5,6,7,8,9,10,11,15 });

    // get multiple idxs by multiple values
    vInt.get_insert_idxs(vIntsToInsert, idxs);

    // execute sorted insertion
    for (index i = 0; i < idxs.size(); ++i)
    {
        vInt.insert_before(idxs[i] + i, vIntsToInsert[i]);
    }

    // check if we still have sorted array after insertion process
    AssertVectorsEqual(vInt, vDst);

    PrintPassed();
}

///////////////////////////////////////////////////////////

void VectorTests::TestAppendCopyVector()
{
    PrintTestName("Test append_vector(const cvector<T>&):");

    cvector<int> v1Int{ 1,2,3 };
    cvector<int> v2Int{ 4,5,6 };
    cvector<std::string> v1Str{ "a","b","c" };
    cvector<std::string> v2Str{ "d","e","f" };

    // case_1: append (using copy-semantic) cvector of integers
    v1Int.append_vector(v2Int);
    AssertVectorsEqual(v1Int, { 1,2,3,4,5,6 });
    AssertVectorsEqual(v2Int, { 4,5,6 });

    // case_2: append (using copy-semantic) cvector of std::strings
    v1Str.append_vector(v2Str);
    AssertVectorsEqual(v1Str, { "a","b","c","d","e","f" });
    AssertVectorsEqual(v2Str, { "d","e","f" });

    PrintPassed();
}

///////////////////////////////////////////////////////////

void VectorTests::TestAppendMoveVector()
{
    PrintTestName("Test append_vector(cvector<T>&&):");

    cvector<int> v1Int{ 1,2,3 };
    cvector<int> v2Int{ 4,5,6 };
    cvector<std::string> v1Str{ "a","b","c" };
    cvector<std::string> v2Str{ "d","e","f" };

    // case_1: append (using move-semantic) cvector of integers
    v1Int.append_vector(std::move(v2Int));
    AssertVectorsEqual(v1Int, { 1,2,3,4,5,6 });
    Assert(v2Int.empty(), "cvector must be empty");

    // case_2: append (using move-semantic) cvector of std::strings
    v1Str.append_vector(std::move(v2Str));
    AssertVectorsEqual(v1Str, { "a","b","c","d","e","f" });
    Assert(v2Str.empty(), "cvector must be empty");

    PrintPassed();
}


// =================================================================================
//                            test seach methods
// =================================================================================

void VectorTests::TestFind()
{
    // test: find first matching val and return its index
    PrintTestName("Test find(const T& value) const:");

    const index               invalidIdx = -1;
    const cvector<int>         v1{ 1,2,3,4,5,6,7,8,9 };
    const cvector<std::string> v2{ "a","b","c","d" };
    
    index idx0 = v1.find(100);
    index idx1 = v1.find(2);
    index idx2 = v1.find(1);
    
    index idx3 = v2.find("kekw");
    index idx4 = v2.find("a");
    index idx5 = v2.find("d");

    Assert(idx0 == invalidIdx, "test_1");
    Assert(idx1 == 1, "test_2");
    Assert(idx2 == 0, "test_3");

    Assert(idx3 == invalidIdx, "test_4");
    Assert(idx4 == 0, "test_5");
    Assert(idx5 == v2.size() - 1, "test_6");     // check if we got the last idx

    PrintPassed();
}

///////////////////////////////////////////////////////////

void VectorTests::TestGetIdx()
{
    PrintTestName("Test get_idx(const T&) const:");

    // test getting of value's index for sorted (!) array
    const cvector<int>         v1{ 1,2,3,4,5 };
    const cvector<std::string> v2{ "a","b","c","d" };

    Assert(v1.get_idx(1) == 0, "test_1");
    Assert(v1.get_idx(3) == 2, "test_2");
    Assert(v1.get_idx(5) == v1.size()-1, "test_3");   // check for last idx
    
    Assert(v2.get_idx("a") == 0, "test_4");
    Assert(v2.get_idx("c") == 2, "test_5");
    Assert(v2.get_idx("d") == v2.size()-1, "test_6"); // check for last idx

    PrintPassed();
}

///////////////////////////////////////////////////////////

void VectorTests::TestGetIdxsFromRawArr()
{
    PrintTestName("Test get_idxs(const T* data, const int nElems, cvector<index>& idxs):");

    // test getting indices to values from the sorted (!) raw array
    const cvector<int>         vInt(v1to10);
    const cvector<std::string> vStr(vAtoH);

    const int ints[3]            = { 1, 5, 10 };
    const std::string strings[3] = { "a", "d", "h" };
    
    cvector<index> idxs1;
    cvector<index> idxs2;

    vInt.get_idxs(ints, 3, idxs1);
    vStr.get_idxs(strings, 3, idxs2);

    // check if idxs are proper
    AssertVectorsEqual(idxs1, { 0, 4, 9 });
    AssertVectorsEqual(idxs2, { 0, 3, 7 });

    PrintPassed();
}

///////////////////////////////////////////////////////////

void VectorTests::TestGetIdxsFromVector()
{
    PrintTestName("Test get_idxs(const cvector<T>& vals, cvector<index>& idxs):");

    // test getting indices to values from the sorted (!) cvector
    const cvector<int>         vInt(v1to10);
    const cvector<std::string> vStr(vAtoH);

    cvector<index> idxs1;
    cvector<index> idxs2;

    vInt.get_idxs({ 1,2,3,10 }, idxs1);
    vStr.get_idxs({ "a","d","g","h" }, idxs2);

    // check if idxs are proper
    AssertVectorsEqual(idxs1, { 0,1,2,9 });
    AssertVectorsEqual(idxs2, { 0,3,6,7 });

    PrintPassed();
}

///////////////////////////////////////////////////////////

void VectorTests::TestGetDataByIdxs1()
{
    PrintTestName("Test get_data_by_idxs(const cvector<index>& idxs, cvector<T>& outData):");

    // test getting data elements by input indices
    const cvector<int>         vInt(v1to10);
    const cvector<std::string> vStr(vAtoH);

    const cvector<index> idxs1({ 0,4,9 });
    const cvector<index> idxs2({ 0,3,7 });

    cvector<int>         data1;
    cvector<std::string> data2;

    vInt.get_data_by_idxs(idxs1, data1);
    vStr.get_data_by_idxs(idxs2, data2);

    // check if values by idxs are proper
    AssertVectorsEqual(data1, { 1,5,10 });
    AssertVectorsEqual(data2, { "a","d","h" });

    PrintPassed();
}

///////////////////////////////////////////////////////////

void VectorTests::TestGetDataByIdxs2()
{
    PrintTestName("Test get_data_by_idxs(const cvector<index>& idxs, T* outData):");

    // test getting data elements by input indices
    const cvector<int>         vInt(v1to10);
    const cvector<std::string> vStr(vAtoH);

    const cvector<index> idxs1({ 0,4,9 });
    const cvector<index> idxs2({ 0,3,7 });

    int         data1[3];
    std::string data2[3];

    vInt.get_data_by_idxs(idxs1, data1);
    vStr.get_data_by_idxs(idxs2, data2);

    // put data from raw arrays into vectors to make possible execute check
    const cvector<int>         vData1(data1, data1 + 3);
    const cvector<std::string> vData2(data2, data2 + 3);

    // check if values by idxs are proper
    AssertVectorsEqual(vData1, { 1,5,10 });
    AssertVectorsEqual(vData2, { "a","d","h" });

    PrintPassed();
}

///////////////////////////////////////////////////////////

void VectorTests::TestHasValue()
{
    PrintTestName("Test has_value(const T&) const:");

    const cvector<int>         vInt(v1to10);
    const cvector<std::string> vStr(vAtoH);

    Assert(vInt.has_value(1)   == true, "test_1");
    Assert(vInt.has_value(10)  == true, "test_2");
    Assert(vInt.has_value(0)   == false, "test_3");

    Assert(vStr.has_value("a") == true, "test_4");
    Assert(vStr.has_value("h") == true, "test_5");
    Assert(vStr.has_value("x") == false, "test_6");

    PrintPassed();
}

///////////////////////////////////////////////////////////

void VectorTests::TestBinarySearchForSingle()
{
    // test for a single value (int / std::string)
    PrintTestName("Test binary_search(const T& val):");

    const cvector<int>         vInt(v1to10);
    const cvector<std::string> vStr{ "a","b","c","kek" };

    Assert(vInt.binary_search(1)   == true, "test_1");
    Assert(vInt.binary_search(10)  == true, "test_2");
    Assert(vInt.binary_search(99)  == false, "test_3");

    Assert(vStr.binary_search("a")   == true, "test_4");
    Assert(vStr.binary_search("kek") == true, "test_5");
    Assert(vStr.binary_search("lol") == false, "test_6");

    PrintPassed();
}

///////////////////////////////////////////////////////////

void VectorTests::TestBinarySearchForMultiple()
{
    // test for multiple values (multiple integers / std::strings)
    PrintTestName("Test binary_seach(const cvector<T>& values) const:");

    cvector<int>         vInt(v1to10);
    cvector<std::string> vStr(vAtoH);

    Assert(vInt.binary_search({ 1,4,10 })  == true, "test_1");
    Assert(vInt.binary_search({ 1,4,100 }) == false, "test_2");

    Assert(vStr.binary_search({ "a","b","h" }) == true, "test_3");
    Assert(vStr.binary_search({ "a","b","x" }) == false, "test_4");

    PrintPassed();
}

///////////////////////////////////////////////////////////

void VectorTests::TestBinarySearchForRawMultiple()
{
    // test for multiple values (multiple integers / std::strings)
    PrintTestName("Test binary_seach(const T* values, const vsize numElems) const:");

    cvector<int>         vInt(v1to10);
    cvector<std::string> vStr(vAtoH);

    constexpr int numElems = 3;
    const int intArrValid[numElems]           = { 1, 4, 10 };
    const int intArrInvalid[numElems]         = { 1, 4, 100 };
    const std::string strArrValid[numElems]   = { "a","b","h" };
    const std::string strArrInvalid[numElems] = { "a","b","x" };

    Assert(vInt.binary_search(intArrValid, numElems)   == true, "test_1");
    Assert(vInt.binary_search(intArrInvalid, numElems) == false, "test_2");

    Assert(vStr.binary_search(strArrValid, numElems)   == true, "test_3");
    Assert(vStr.binary_search(strArrInvalid, numElems) == false, "test_4");

    PrintPassed();
}

///////////////////////////////////////////////////////////

void VectorTests::TestBinarySearchForRawMultipleOutFlags()
{
    // test for multiple values (multiple integers / std::strings)
    PrintTestName("Test binary_seach(const T* values, size numElems, cvector<bool>& flags) const:");

    const cvector<int>         vInt(v1to10);
    const cvector<std::string> vStr(vAtoH);

    constexpr int numElems = 4;
    const int intArrValid[numElems]           = { 1, 4, 8, 10 };
    const int intArrInvalid[numElems]         = { 1, 20, 8, 100 };
    const std::string strArrValid[numElems]   = { "a","b","d","h" };
    const std::string strArrInvalid[numElems] = { "a","z","d","x" };

    cvector<bool> flags(numElems, false);

    const cvector<bool> expectFlags1({ true, true, true, true });
    const cvector<bool> expectFlags2({ true, false, true, false });
    const cvector<bool> expectFlags3({ true, true, true, true });
    const cvector<bool> expectFlags4({ true, false, true, false });

    // test for integers: valid and invalid cases
    vInt.binary_search(intArrValid, numElems, flags);
    Assert(flags == expectFlags1, "test_1");

    vInt.binary_search(intArrInvalid, numElems, flags);
    Assert(flags == expectFlags1, "test_2");

    // test for std::string: valid and invalid cases
    vStr.binary_search(strArrValid, numElems, flags);
    Assert(flags == expectFlags3, "test_3");

    vStr.binary_search(strArrInvalid, numElems, flags);
    Assert(flags == expectFlags4, "test_4");

    PrintPassed();
}


// =================================================================================
//                      test memory deallocation methods
// =================================================================================

void VectorTests::TestShrinkToFit()
{
    PrintTestName("Test shrink_to_fit():");

    cvector<int>         v1(v1to10);
    cvector<std::string> v2(vAtoH);

    v1.resize(2);
    v2.resize(2);

    v1.shrink_to_fit();
    v2.shrink_to_fit();

    // check if size and capacity are proper
    Assert((v1.size() == 2) && (v1.capacity() == 2), "test_1");
    Assert((v2.size() == 2) && (v2.capacity() == 2), "test_2");

    // check if remain data is proper
    AssertVectorsEqual(v1, { 1,2 });
    AssertVectorsEqual(v2, { "a","b" });

    PrintPassed();
}

///////////////////////////////////////////////////////////

void VectorTests::TestPurge()
{
    PrintTestName("Test purge():");

    cvector<int>         v1(v1to10);
    cvector<std::string> v2(vAtoH);

    // full delete of data, reset size and capacity
    v1.purge();
    v2.purge();

    Assert((v1.size() == 0) && (v1.capacity() == 0), "test_1");
    Assert((v2.size() == 0) && (v2.capacity() == 0), "test_2");

    Assert(v1.data() == nullptr, "test_3");
    Assert(v2.data() == nullptr, "test_4");

    PrintPassed();
}


// =================================================================================
//                              private helpers
// =================================================================================

void VectorTests::Assert(bool condition, const char* msg)
{
    if (!condition)
    {
        printf("ERROR: %s\n", msg);
        exit(-1);
    }
}

///////////////////////////////////////////////////////////

void VectorTests::Assert(bool condition, const std::string& msg)
{
    if (!condition)
    {
        printf("ERROR: %s\n", msg.c_str());
        exit(-1);
    }
}

///////////////////////////////////////////////////////////

void VectorTests::PrintTestBlockHeader(const char* str)
{
    printf("\n\n%s%s%s\n\n", KYEL, str, KNRM);
}

///////////////////////////////////////////////////////////

void VectorTests::PrintTestName(const char* str)
{
    const unsigned int reservedLength = 80;
    const unsigned int actualLength = (unsigned int)strlen(str);

    printf("%-*.*s", reservedLength, actualLength, str);
}

///////////////////////////////////////////////////////////

void VectorTests::PrintPassed()
{
    printf("%sPASSED%s\n", KGRN, KNRM);
}

///////////////////////////////////////////////////////////

template <typename T>
void VectorTests::AssertVectorsEqual(const cvector<T>& vDst, const cvector<T>& vSrc)
{
    // vDst -- array which we want to check
    // vSrc -- array for comparison

    Assert(vDst.size() == vSrc.size(), "size is wrong");

    for (vsize i = 0; i < vDst.size(); ++i)
        Assert(vDst[i] == vSrc[i], std::format("v1[{}] != {}, but == {}", i, vSrc[i], vDst[i]));
}

///////////////////////////////////////////////////////////

template <typename T>
void VectorTests::AssertVectorsEqual(const T* vDst, const T* vSrc, const vsize numElems)
{
    // vDst   -- array which we want to check
    // vSrc   -- data for comparison
    // numElems -- number of elements (must be the same for both)

    Assert(vDst != nullptr, "v1 ptr == nullptr");
    Assert(vSrc != nullptr, "v2 ptr == nullptr");
    Assert(numElems > 0, "size is wrong");

    for (vsize i = 0; i < numElems; ++i)
        Assert(vDst[i] == vSrc[i], std::format("v1[{}] != {}", i, vSrc[i]));
}

///////////////////////////////////////////////////////////

template <typename T>
void VectorTests::PrintVector(const cvector<T>& v, const std::string& msg)
{
    // print out a content of the input cvector

    std::cout << msg;

    for (vsize i = 0; i < v.size(); ++i)
        std::cout << v[i] << ' ';

    std::cout << std::endl;
}

///////////////////////////////////////////////////////////

void VectorTests::TestInsertBeforeForInt()
{
    // this function is called from TestInsertBefore();

    // case_1: test with integers
    cvector<int> v1;
    const cvector<int> v1Insert0{ 1,2,3 };
    const cvector<int> v1Insert1{ 1,10,2,3 };
    const cvector<int> v1Insert2{ 1,10,20,2,3 };
    const cvector<int> v1Insert3{ 30,1,10,20,2,3 };

    v1.insert_before(0, 3);
    v1.insert_before(0, 2);
    v1.insert_before(0, 1);
    AssertVectorsEqual(v1, v1Insert0);

    v1.insert_before(1, 10);
    AssertVectorsEqual(v1, v1Insert1);
    //PrintVector(v, "\nv = ");

    v1.insert_before(2, 20);
    AssertVectorsEqual(v1, v1Insert2);
    //PrintVector(v, "v = ");

    v1.insert_before(0, 30);
    AssertVectorsEqual(v1, v1Insert3);
    //PrintVector(v, "v = ");
}

///////////////////////////////////////////////////////////

void VectorTests::TestInsertBeforeForStr()
{
    // this function is called from TestInsertBefore();

    // case_2: test with std::string
    cvector<std::string> v2;
    const cvector<std::string> v2Insert0{ "a","b","c" };
    const cvector<std::string> v2Insert1{ "a","x","b","c" };
    const cvector<std::string> v2Insert2{ "a","x","y","b","c" };
    const cvector<std::string> v2Insert3{ "z","a","x","y","b","c" };

    v2.insert_before(0, "c");
    v2.insert_before(0, "b");
    v2.insert_before(0, "a");
    AssertVectorsEqual(v2, v2Insert0);
    //PrintVector(v2, "\nv2 = ");

    v2.insert_before(1, "x");
    AssertVectorsEqual(v2, v2Insert1);
    //PrintVector(v2, "v2 = ");

    v2.insert_before(2, "y");
    AssertVectorsEqual(v2, v2Insert2);
    //PrintVector(v2, "v2 = ");

    v2.insert_before(0, "z");
    AssertVectorsEqual(v2, v2Insert3);
    //PrintVector(v2, "v2 = ");
}