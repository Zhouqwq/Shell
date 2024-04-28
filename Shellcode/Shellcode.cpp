// Shellcode.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <Windows.h>
#include <winternl.h>

// ATTENTION:
// 1. NOT TESTED ON GCC/G++
// 2. /GS- /MT OPTION NEEDED
// 3. DON'T CALL OTHER FUNCTIONS EVEN INLINE FUNCTIONS
// 4. CHECK YOUR CODE IN IDA, MAKE SURE YOUR DATA ARE IN STACK
// 5. THE SHELLCODE FUNCTION MUST BE FASTCALL FUNCTION
// 6. GOOD LUCK

void _fastcall ShellCode() {
    // MAGIC CODE
    // DON'T CHANGE
    unsigned long long pMmGetSystemRoutineAddresst = 0x55EB6EB6D5AB6AB9; // MmGetSystemRoutineAddress()
    unsigned long long pRtlInitUnicodeStringt = 0x56EC6FB7D6AC6BBA; // RtlInitUnicodeString()
    
    PVOID pMmGetSystemRoutineAddress = (void*)(pMmGetSystemRoutineAddresst);
    PVOID pRtlInitUnicodeString = (void*)(pRtlInitUnicodeStringt);
    WCHAR tmp[9] = { L'D',L'b',L'g',L'P',L'r',L'i',L'n',L't',0 };
    UNICODE_STRING ustr;
    ((void(_stdcall*)(PUNICODE_STRING, PCWSTR))pRtlInitUnicodeString)(&ustr, tmp);
    // RtlInitUnicodeString(&ustr, tmp);
    PVOID pDbgPrint = ((PVOID(_stdcall*)(PUNICODE_STRING))pMmGetSystemRoutineAddress)(&ustr);
    // MmGetSystemRoutineAddress(&ustr);
    if (pDbgPrint != NULL) {
        char str[14] = { 'H','e','l','l','o',' ','W','o','r','l','d','!','\n',0 };
        ((ULONG(__cdecl*)(PCSTR Format, ...))pDbgPrint)(str);
        // DbgPrint("Hello World\n");
    }
    // Print Hello World in Kernel Debug Output
    // DebugView Needed
}

bool check(unsigned char* p) {
    // C function end
    // C3   ret         return;
    // CC   int 3       int 3 interrupt     avoid continue executing
    // CC   int 3       int 3 interrupt
    // CC   int 3       int 3 interrupt
    // CC   int 3       int 3 interrupt
    // CC   int 3       int 3 interrupt
    if (*p == 0xC3 && *(p + 1) == 0xCC && *(p + 2) == 0xCC && *(p + 3) == 0xCC && *(p + 4) == 0xCC && *(p + 5) == 0xCC) {
        return true;
    }
    return false;
}

int main()
{
    unsigned char* pCur = (unsigned char*)ShellCode;
    unsigned int x = 0;
    while (1) {
        ++x;
        printf("%02X ", *pCur);
        if (x % 20 == 0) {
            printf("\n");
        }
        if (check(pCur)) {
            break;
        }
        ++pCur;
    }
    printf("\n\nShellCode Size: %d\n", x);
    getchar();
    return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
