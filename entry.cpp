#include "entry.h"

BOOL WINAPI DllMain(HINSTANCE baseAddress, DWORD reason, BOOL isStatic) {
    switch (reason) {
        case DLL_PROCESS_ATTACH:
            attach();
        break;
        case DLL_PROCESS_DETACH:
            detach();
        break;
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        default:
        break;
    }
    return true;
}

void attach() {
    EROnlineSummons::Logging::Init();

    #ifndef NDEBUG
    vehHandle = AddVectoredExceptionHandler(0, exception_handler);
    #endif

    if (MH_Initialize() != MH_OK) {
        EROnlineSummons::Logging::WriteLine("MinHook initialization failed");
    }

    auto summonBuddyManager = new EROnlineSummons::SummonBuddyManager();
    auto steamInterfaceProvider = new EROnlineSummons::SteamInterfaceProvider();
    steamInterfaceProvider->CreateInterfacesWhenReady();

    auto summonNetworking = new EROnlineSummons::SummonNetworking(steamInterfaceProvider->GetNetworkingMessages());

    auto summonBuddyStateFactory = new EROnlineSummons::SummonBuddyStateFactory(summonBuddyManager, summonNetworking);
    auto summonBuddyStateMachine = new EROnlineSummons::SummonBuddyStateMachine(
        summonBuddyStateFactory->CreateNoSummonState()
    );

    onlineSummons = new EROnlineSummons::OnlineSummons(
        summonBuddyStateMachine,
        summonBuddyStateFactory,
        summonNetworking
    );
    onlineSummons->Start();
}

void detach() {
    onlineSummons->Stop();

    #ifndef NDEBUG
    EROnlineSummons::Logging::WriteLine("Online Summons stopped");
    #endif

    EROnlineSummons::Logging::Deinit();

    #ifndef NDEBUG
    RemoveVectoredExceptionHandler(vehHandle);
    #endif
}

LONG WINAPI exception_handler(struct _EXCEPTION_POINTERS* exception) {
    PEXCEPTION_RECORD record  = exception->ExceptionRecord;
    PCONTEXT context = exception->ContextRecord;
    char exception_text[EXCEPTION_STRING_SIZE];
    bool shouldBlock = false;

    auto moduleBase = EROnlineSummons::GetBaseAddress();
    switch (record->ExceptionCode) {
        case EXCEPTION_ACCESS_VIOLATION:
            snprintf(
                    exception_text,
                    EXCEPTION_STRING_SIZE,
                    "Access violation!\n\n"
                    "Module Base: 0x%p\n"
                    "Instruction at 0x%p\n"
                    "attempted to %s\n"
                    "memory at 0x%p\n\n"
                    "Register dump:\n"
                    "RAX = 0x%p\n" "RBX = 0x%p\n" "RCX = 0x%p\n" "RDX = 0x%p\n"
                    "RSI = 0x%p\n" "RDI = 0x%p\n" "RBP = 0x%p\n" "RSP = 0x%p\n"
                    "R8  = 0x%p\n" "R9  = 0x%p\n" "R10 = 0x%p\n" "R11 = 0x%p\n"
                    "R12 = 0x%p\n" "R13 = 0x%p\n" "R14 = 0x%p\n" "R15 = 0x%p\n",
                    moduleBase,
                    record->ExceptionAddress,
                    record->ExceptionInformation[0]?"WRITE":"READ",
                    record->ExceptionInformation[1],
                    context->Rax, context->Rbx, context->Rcx, context->Rdx,
                    context->Rsi, context->Rdi, context->Rbp, context->Rsp,
                    context->R8,  context->R9,  context->R10, context->R11,
                    context->R12, context->R13, context->R14, context->R15
            );
            shouldBlock = true;
            break;
        default:
            snprintf(
                    exception_text,
                    EXCEPTION_STRING_SIZE,
                    "Caught unhandled exception\n"
                    "Module Base: 0x%p\n"
                    "Code: %x\n"
                    "Address: %p",
                    moduleBase,
                    record->ExceptionCode,
                    record->ExceptionAddress
            );
            break;
    }

    EROnlineSummons::Logging::WriteLine(exception_text);

    #ifndef NDEBUG
        if (shouldBlock) {
            MessageBoxA(NULL, "Access Violation", "Confirm after completing the debug", MB_ICONWARNING | MB_OK | MB_DEFBUTTON2);
        }
    #endif

    return EXCEPTION_CONTINUE_SEARCH;
}
