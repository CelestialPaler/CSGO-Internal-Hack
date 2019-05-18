// CTHackFramework 														      
//	A framework for general game hacking								      
// Copyright © 2019 Celestial Tech All rights reserved.
//
// The MIT License (MIT)
// Copyright (c) 2019 Celestial Tech
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this softwareand associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
// 
// The above copyright noticeand this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "Process.h"

std::mutex mutexProcess;
static Process* instancePointer;

Process* Process::GetInstance(void)
{
	mutexProcess.lock();
	if (instancePointer == nullptr)
		instancePointer = new Process();
	mutexProcess.unlock();
	return instancePointer;
}

bool Process::Attach(const std::wstring& _szProcessName)
{
	GetDebugPrivileges();
	SetTargetProcess(_szProcessName);
	GetProcessID();
	if (this->dwProcessID <= 0)
		return false;
	OpenProcessHandle();
	if (this->hProcessHandle == INVALID_HANDLE_VALUE)
		return false;
	GetModuleBaseAddress();
	return true;
}

bool Process::Attach(const DWORD& _dwProcessID)
{
	GetDebugPrivileges();

	this->dwProcessID = _dwProcessID;
	OpenProcessHandle();
	if (this->hProcessHandle == INVALID_HANDLE_VALUE)
		return false;
	GetModuleBaseAddress();
	return true;
}

void Process::Dettach(void)
{
	CloseProcessHandle();
}

std::vector<DWORD> Process::AoBScan(const std::string& _signitureStr, const DWORD& _dwAddrBegin, const DWORD& _dwAddrEnd, const DWORD& _dwModeFlag)
{
	std::vector<Memory::SignitureByte> signiture = Memory::StringToSigniture(_signitureStr);
	Memory::SignitureToLog(signiture);
	std::vector<DWORD> result = Memory::AoBScan(this->hProcessHandle, signiture, _dwAddrBegin, _dwAddrEnd, _dwModeFlag);
	Memory::AoBResultToLog(result);
	return result;
}

DWORD Process::ReadDWORD(const DWORD& _dwAddr)
{
	return Memory::ReadData<DWORD>(this->hProcessHandle, _dwAddr);
}

WORD Process::ReadWORD(const DWORD& _dwAddr)
{
	return Memory::ReadData<WORD>(this->hProcessHandle, _dwAddr);
}

BYTE Process::ReadByte(const DWORD& _dwAddr)
{
	return Memory::ReadData<BYTE>(this->hProcessHandle, _dwAddr);
}

INT16 Process::ReadInt16(const DWORD& _dwAddr)
{
	return Memory::ReadData<INT16>(this->hProcessHandle, _dwAddr);
}

INT32 Process::ReadInt32(const DWORD& _dwAddr)
{
	return Memory::ReadData<INT32>(this->hProcessHandle, _dwAddr);
}

INT64 Process::ReadInt64(const DWORD& _dwAddr)
{
	return Memory::ReadData<INT64>(this->hProcessHandle, _dwAddr);
}

UINT16 Process::ReadUint16(const DWORD& _dwAddr)
{
	return Memory::ReadData<UINT16>(this->hProcessHandle, _dwAddr);
}

UINT32 Process::ReadUint32(const DWORD& _dwAddr)
{
	return Memory::ReadData<UINT32>(this->hProcessHandle, _dwAddr);
}

UINT64 Process::ReadUint64(const DWORD& _dwAddr)
{
	return Memory::ReadData<UINT64>(this->hProcessHandle, _dwAddr);
}

FLOAT Process::ReadFloat(const DWORD& _dwAddr)
{
	return Memory::ReadData<FLOAT>(this->hProcessHandle, _dwAddr);
}

//DOUBLE Process::ReadDouble(const DWORD& _dwAddr)
//{
//	return Memory::ReadData<DOUBLE>(this->hProcessHandle, _dwAddr);
//}

BOOL Process::ReadBool(const DWORD& _dwAddr)
{
	return Memory::ReadData<BOOL>(this->hProcessHandle, _dwAddr);
}

bool Process::WriteByte(const DWORD& _dwAddr, const BYTE& _newData)
{
	return Memory::WriteData<BYTE>(this->hProcessHandle, _dwAddr, _newData);
}

bool Process::WriteInt16(const DWORD& _dwAddr, const INT16& _newData)
{
	return Memory::WriteData<INT16>(this->hProcessHandle, _dwAddr, _newData);
}

bool Process::WriteInt32(const DWORD& _dwAddr, const INT32& _newData)
{
	return Memory::WriteData<INT32>(this->hProcessHandle, _dwAddr, _newData);
}

bool Process::WriteInt64(const DWORD& _dwAddr, const INT64& _newData)
{
	return Memory::WriteData<INT64>(this->hProcessHandle, _dwAddr, _newData);
}

bool Process::WriteUint16(const DWORD& _dwAddr, const UINT16& _newData)
{
	return Memory::WriteData<UINT16>(this->hProcessHandle, _dwAddr, _newData);
}

bool Process::WriteUint32(const DWORD& _dwAddr, const UINT32& _newData)
{
	return Memory::WriteData<UINT32>(this->hProcessHandle, _dwAddr, _newData);
}

bool Process::WriteUint64(const DWORD& _dwAddr, const UINT64& _newData)
{
	return Memory::WriteData<UINT64>(this->hProcessHandle, _dwAddr, _newData);
}

bool Process::WriteFloat(const DWORD& _dwAddr, const FLOAT& _newData)
{
	return Memory::WriteData<FLOAT>(this->hProcessHandle, _dwAddr, _newData);
}

//bool Process::WriteDouble(const DWORD& _dwAddr, const DOUBLE& _newData)
//{
//	return Memory::WriteData<DOUBLE>(this->hProcessHandle, _dwAddr, _newData);
//}

bool Process::WriteBool(const DWORD& _dwAddr, const BOOL& _newData)
{
	return Memory::WriteData<BOOL>(this->hProcessHandle, _dwAddr, _newData);
}

BYTE* Process::ReadBytes(const DWORD& _dwAddr, const SIZE_T& _size)
{
	return Memory::ReadBytes(this->hProcessHandle, _dwAddr, _size);
}

void Process::SetTargetProcess(const std::wstring& _szProcessName)
{
	// 取得Logger
	Util::Logger* logger = Util::Logger::GetInstance();

	// 日志
	Util::Logger::LogDisc msg;
	msg.emPriority = Util::Logger::Priority::INFO;
	msg.szFrom = __func__;
	msg.szMsg = "Set target process name";
	msg.szTarget = Util::StringManipulation::WstringToString(this->szProcessName);
	logger->Write(msg);

	// 确保后缀正确
	const std::wstring suffix = L".exe";
	if (_szProcessName.find(suffix) == std::wstring::npos)
		this->szProcessName = _szProcessName + suffix;
	else
		this->szProcessName = _szProcessName;
}

void Process::GetProcessID(void)
{
	// 取得Logger
	Util::Logger* logger = Util::Logger::GetInstance();

	// 日志
	Util::Logger::LogDisc msg;
	msg.emPriority = Util::Logger::Priority::INFO;
	msg.szFrom = __func__;
	msg.szMsg = "Trying to get target process ID...";
	msg.szTarget = Util::StringManipulation::WstringToString(this->szProcessName);
	logger->Write(msg);

	// 重置错误信息
	SetLastError(0);

	// 获取系统进程快照
	PROCESSENTRY32 processEntry = { sizeof(PROCESSENTRY32) };
	HANDLE hProcessSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (hProcessSnapshot == INVALID_HANDLE_VALUE)
	{
		Util::Logger::LogDisc msg;
		msg.emPriority = Util::Logger::Priority::FATAL;
		msg.szFrom = __func__;
		msg.szMsg = "Invalid Handle value!";
		msg.szTarget = Util::StringManipulation::WstringToString(this->szProcessName);
		logger->Write(msg);
		return;
	}

	// 遍历快照直到找到目标进程
	if (Process32First(hProcessSnapshot, &processEntry))
	{
		do
		{
			if (!wcscmp(processEntry.szExeFile, this->szProcessName.c_str()))
			{
				CloseHandle(hProcessSnapshot);
				// 日志
				Util::Logger::LogDisc msg;
				msg.emPriority = Util::Logger::Priority::INFO;
				msg.szFrom = __func__;
				msg.szMsg = "Succeed!";
				msg.szTarget = Util::StringManipulation::WstringToString(this->szProcessName);
				msg.szResult = std::to_string(processEntry.th32ProcessID);
				logger->Write(msg);
				this->dwProcessID = processEntry.th32ProcessID;
				return;
			}
		} while (Process32Next(hProcessSnapshot, &processEntry));
	}
	// 处理错误信息
	int err = GetLastError();
	if (err != 0)
	{
		// 日志
		Util::Logger::LogDisc msg;
		msg.emPriority = Util::Logger::Priority::FATAL;
		msg.szFrom = __func__;
		msg.szMsg = "Get target process ID error!";
		msg.szTarget = Util::StringManipulation::WstringToString(this->szProcessName);
		msg.szCode = std::to_string(err);
		logger->Write(msg);
	}
	this->dwProcessID = NULL;
	return;
}

void Process::OpenProcessHandle(void)
{
	// 取得Logger
	Util::Logger* logger = Util::Logger::GetInstance();

	HANDLE hTemp = OpenProcess(PROCESS_ALL_ACCESS, FALSE, this->dwProcessID);
	if (hTemp == INVALID_HANDLE_VALUE)
	{
		// 日志
		Util::Logger::LogDisc msg;
		msg.emPriority = Util::Logger::Priority::FATAL;
		msg.szFrom = __func__;
		msg.szMsg = "Get target process handle error!";
		msg.szTarget = Util::StringManipulation::WstringToString(this->szProcessName);
		logger->Write(msg);
		return;
	}
	this->hProcessHandle = hTemp;
	return;
}

void Process::CloseProcessHandle(void)
{
	// 取得Logger
	Util::Logger* logger = Util::Logger::GetInstance();

	if (this->hProcessHandle == INVALID_HANDLE_VALUE)
	{
		// 日志
		Util::Logger::LogDisc msg;
		msg.emPriority = Util::Logger::Priority::FATAL;
		msg.szFrom = __func__;
		msg.szMsg = "Close target process handle error!";
		msg.szTarget = Util::StringManipulation::WstringToString(this->szProcessName);
		logger->Write(msg);
		return;
	}
	CloseHandle(this->hProcessHandle);
	return;
}

void Process::GetModuleBaseAddress(void)
{
	// 取得Logger
	Util::Logger* logger = Util::Logger::GetInstance();

	// 重置错误信息
	SetLastError(0);

	// 日志
	Util::Logger::LogDisc msg;
	msg.emPriority = Util::Logger::Priority::INFO;
	msg.szFrom = __func__;
	msg.szMsg = "Trying to get the base address of all the modules used by target process...";
	msg.szTarget = Util::StringManipulation::WstringToString(this->szProcessName);
	logger->Write(msg);

	// 获取系统模块快照
	MODULEENTRY32 moduleEntry = { sizeof(MODULEENTRY32) };
	HANDLE hThreadSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, this->dwProcessID);
	if (hThreadSnapShot == INVALID_HANDLE_VALUE)
	{
		// 日志
		Util::Logger::LogDisc msg;
		msg.emPriority = Util::Logger::Priority::FATAL;
		msg.szFrom = __func__;
		msg.szMsg = "Get target process modules error!";
		msg.szTarget = Util::StringManipulation::WstringToString(this->szProcessName);
		logger->Write(msg);
		return;
	}

	// 遍历快照直到找到目标模块
	if (Module32First(hThreadSnapShot, &moduleEntry))
	{
		do
		{
			// 日志
			Util::Logger::LogDisc msg;
			msg.emPriority = Util::Logger::Priority::DEBUG;
			msg.szFrom = __func__;
			msg.szMsg = "Get target process modules succeed!";
			msg.szTarget = Util::StringManipulation::WstringToString(this->szProcessName);
			std::stringstream ss;
			ss << Util::StringManipulation::WstringToString(moduleEntry.szModule) << " 0x" << std::uppercase << std::hex << (QWORD)moduleEntry.modBaseAddr;
			msg.szResult = ss.str();
			logger->Write(msg);
			this->moduleBaseAddr.insert(std::pair<const std::wstring, const QWORD>(moduleEntry.szModule, (QWORD)moduleEntry.modBaseAddr));
		} while (Module32Next(hThreadSnapShot, &moduleEntry));
		CloseHandle(hThreadSnapShot);
		return;
	}

	CloseHandle(hThreadSnapShot);
	// 处理错误信息
	int err = GetLastError();
	if (err != 0)
	{
		// 日志
		Util::Logger::LogDisc msg;
		msg.emPriority = Util::Logger::Priority::FATAL;
		msg.szFrom = __func__;
		msg.szMsg = "Get target process modules error!";
		msg.szTarget = Util::StringManipulation::WstringToString(this->szProcessName);
		msg.szCode = std::to_string(err);
		logger->Write(msg);
	}
	return;
}

BOOL Process::SetPrivilege(HANDLE _hToken, LPCTSTR _szPrivilege, BOOL _enablePrivilege)
{
	// 取得Logger
	Util::Logger* logger = Util::Logger::GetInstance();

	TOKEN_PRIVILEGES newTokenPrivileges;
	LUID luid;

	// 重置错误信息
	SetLastError(0);

	// 查询LUID权限值
	if (!LookupPrivilegeValue(NULL, _szPrivilege, &luid))
	{
		// 处理错误信息
		int err = GetLastError();
		if (err != 0)
		{
			// 日志
			Util::Logger::LogDisc msg;
			msg.emPriority = Util::Logger::Priority::WARNING;
			msg.szFrom = __func__;
			msg.szMsg = "Get target process privilege value (LUID) error!";
			msg.szCode = std::to_string(err);
			logger->Write(msg);
		}
		return FALSE;
	}

	// 创建新的token
	newTokenPrivileges.PrivilegeCount = 1;
	newTokenPrivileges.Privileges[0].Luid = luid;
	newTokenPrivileges.Privileges[0].Attributes = _enablePrivilege ? SE_PRIVILEGE_ENABLED : 0;

	// 使用新的token调整权限
	if (!AdjustTokenPrivileges(_hToken, FALSE, &newTokenPrivileges, sizeof(TOKEN_PRIVILEGES), (PTOKEN_PRIVILEGES)NULL, (PDWORD)NULL))
	{
		// 处理错误信息
		int err = GetLastError();
		if (err != 0)
		{
			// 日志
			Util::Logger::LogDisc msg;
			msg.emPriority = Util::Logger::Priority::WARNING;
			msg.szFrom = __func__;
			msg.szMsg = "Set target process token privilege error!";
			msg.szCode = std::to_string(err);
			logger->Write(msg);
		}
		return FALSE;
	}

	// 处理错误信息
	int err = GetLastError();
	if (err == ERROR_NOT_ALL_ASSIGNED) {
		// 日志
		Util::Logger::LogDisc msg;
		msg.emPriority = Util::Logger::Priority::WARNING;
		msg.szFrom = __func__;
		msg.szMsg = "New token do not have debug privileges!";
		msg.szCode = std::to_string(err);
		logger->Write(msg);
		return FALSE;
	}

	return TRUE;
}

BOOL Process::GetDebugPrivileges(void)
{
	// 取得Logger
	Util::Logger* logger = Util::Logger::GetInstance();

	// 日志
	Util::Logger::LogDisc msg;
	msg.emPriority = Util::Logger::Priority::INFO;
	msg.szFrom = __func__;
	msg.szMsg = "Trying to get debug privileges...";
	logger->Write(msg);

	// 重置错误信息
	SetLastError(0);

	// 获取当前进程的权限token
	HANDLE hToken = NULL;
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken))
	{
		// 处理错误信息
		int err = GetLastError();
		if (err != 0)
		{
			Util::Logger::LogDisc msg;
			msg.emPriority = Util::Logger::Priority::WARNING;
			msg.szFrom = __func__;
			msg.szMsg = "Get current process privilege token error!";
			msg.szCode = std::to_string(err);
			logger->Write(msg);
		}
		return FALSE;
	}
	else
	{
		Util::Logger::LogDisc msg;
		msg.emPriority = Util::Logger::Priority::INFO;
		msg.szFrom = __func__;
		msg.szMsg = "Get current process privilege token succeed!";
		logger->Write(msg);
	}

	// 赋予当前进程debug权限
	if (!SetPrivilege(hToken, SE_DEBUG_NAME, TRUE))
	{
		// 处理错误信息
		int err = GetLastError();
		if (err != 0)
		{
			Util::Logger::LogDisc msg;
			msg.emPriority = Util::Logger::Priority::WARNING;
			msg.szFrom = __func__;
			msg.szMsg = "Get debug privilege error!";
			logger->Write(msg);
		}
		return FALSE;
	}

	return TRUE;
}
