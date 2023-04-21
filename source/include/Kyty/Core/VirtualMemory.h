#ifndef INCLUDE_KYTY_CORE_VIRTUALMEMORY_H_
#define INCLUDE_KYTY_CORE_VIRTUALMEMORY_H_

#include "Kyty/Core/Common.h"
#include "Kyty/Core/String.h"

namespace Kyty::Core {

struct SystemInfo
{
	String ProcessorName;
};

SystemInfo GetSystemInfo();

namespace VirtualMemory {

class ExceptionHandlerPrivate;

class ExceptionHandler
{
public:
	enum class ExceptionType
	{
		Unknown,
		AccessViolation
	};

	enum class AccessViolationType
	{
		Unknown,
		Read,
		Write,
		Execute
	};

	struct ExceptionInfo
	{
		ExceptionType       type                   = ExceptionType::Unknown;
		AccessViolationType access_violation_type  = AccessViolationType::Unknown;
		uint64_t            access_violation_vaddr = 0;
		uint64_t            exception_address      = 0;
		uint64_t            rbp                    = 0;
		uint32_t            exception_win_code     = 0;
	};

	using handler_func_t = void (*)(const ExceptionInfo*);

	ExceptionHandler();
	virtual ~ExceptionHandler();

	KYTY_CLASS_NO_COPY(ExceptionHandler);

	static uint64_t GetSize();

	bool Install(uint64_t base_address, uint64_t handler_addr, uint64_t image_size, handler_func_t func);
	bool Uninstall();

	static bool InstallVectored(handler_func_t func);

private:
	ExceptionHandlerPrivate* m_p = nullptr;
};

enum class Mode : uint32_t
{
	NoAccess         = 0,
	Read             = 1,
	Write            = 2,
	ReadWrite        = Read | Write,
	Execute          = 4,
	ExecuteRead      = Execute | Read,
	ExecuteWrite     = Execute | Write,
	ExecuteReadWrite = Execute | Read | Write,
};

inline bool IsExecute(Mode mode)
{
	return (mode == Mode::Execute || mode == Mode::ExecuteRead || mode == Mode::ExecuteWrite || mode == Mode::ExecuteReadWrite);
}

void Init();

uint64_t Alloc(uint64_t address, uint64_t size, Mode mode);
uint64_t AllocAligned(uint64_t address, uint64_t size, Mode mode, uint64_t alignment);
bool     AllocFixed(uint64_t address, uint64_t size, Mode mode);
bool     Free(uint64_t address);
bool     Protect(uint64_t address, uint64_t size, Mode mode, Mode* old_mode = nullptr);
bool     FlushInstructionCache(uint64_t address, uint64_t size);
bool     PatchReplace(uint64_t vaddr, uint64_t value);

} // namespace VirtualMemory

} // namespace Kyty::Core

#endif /* INCLUDE_KYTY_CORE_VIRTUALMEMORY_H_ */
