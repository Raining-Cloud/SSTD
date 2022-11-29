#include "Thread.h"
#include "Platform/IncludePlatform.h"

namespace SSTD
{

#ifdef PLATFORM_WIN64
  Thread::Thread(const Function<void()>& func, bool launch) noexcept
  {
    struct InternalThread
    {
    public:
      InternalThread(const Function<void()>& func)
        :m_Function(func)
      {}
      static DWORD threadProc(LPVOID param)
      {
        InternalThread* data = reinterpret_cast<InternalThread*>(param);
        data->m_Function.Invoke();
        return 0;
      }
      Function<void()> m_Function;
    };

    m_Handle = CreateThread(NULL, 0, InternalThread::threadProc, new InternalThread(func), CREATE_SUSPENDED, reinterpret_cast<LPDWORD>(&m_Id));

    //TODO: add Thread name

    if (launch)
      Start();
  }
  Thread::~Thread()
  {
    Join();
  }
  void Thread::Start()
  {
    ResumeThread(m_Handle);
  }
  void Thread::Join()
  {
    WaitForSingleObject(m_Handle, INFINITE);
  }
  void Thread::Detach()
  {
    CloseHandle(m_Handle);
  }
  void* Thread::GetNative()
  {
    return m_Handle;
  }
#elif  PLATFORM_WIN32
#elif  PLATFORM_IPHONE_SIM
#elif  PLATFORM_IPHONE
#elif  PLATFORM_MAC
#elif  PLATFORM_LINUX
#endif
}