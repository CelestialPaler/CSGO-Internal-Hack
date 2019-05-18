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

#include "Logger.h"

std::mutex mutexLogger;
static Util::Logger* instancePointer;

Util::Logger* Util::Logger::GetInstance(void)
{
	mutexLogger.lock();
	if (instancePointer == nullptr)
		instancePointer = new Logger();
	mutexLogger.unlock();
	return instancePointer;
}

void Util::Logger::Start(Priority _priorityThreshold, const std::string& _logFile)
{
	instancePointer->activated = true;
	instancePointer->priorityThreshold = _priorityThreshold;
	if (_logFile != "")
		instancePointer->logFileStream.open(_logFile.c_str());
}

void Util::Logger::Stop(void)
{
	instancePointer->activated = false;
	if (instancePointer->logFileStream.is_open())
		instancePointer->logFileStream.close();
}

void Util::Logger::Write(const LogDisc& _logDisc)
{
	mutexLogger.lock();
	if (instancePointer->activated && _logDisc.emPriority >= instancePointer->priorityThreshold)
	{
		std::ostream& stream = instancePointer->logFileStream.is_open() ? instancePointer->logFileStream : std::cout;
		stream << instancePointer->GetCurrentSystemTime() << "   ";
		switch (_logDisc.emPriority)
		{
		case Util::Logger::DEBUG:
			stream << white;
			break;
		case Util::Logger::INFO:
			stream << green;
			break;
		case Util::Logger::WARNING:
			stream << yellow;
			break;
		case Util::Logger::ERR:
			stream << red;
			break;
		case Util::Logger::FATAL:
			stream << purple;
			break;
		default:
			break;
		}
		stream << instancePointer->priorityNames[_logDisc.emPriority];
		if (_logDisc.szFrom != "")
			stream << " | From: " << _logDisc.szFrom;
		if (_logDisc.szMsg != "")
			stream << " | Info: " << _logDisc.szMsg;
		if (_logDisc.szTarget != "")
			stream << " | Target: " << _logDisc.szTarget;
		if (_logDisc.szCode != "")
			stream << " | Code: " << _logDisc.szCode;
		if (_logDisc.szResult != "")
			stream << " | Result: " << _logDisc.szResult;
		stream << white;
		stream << std::endl;
	}
	mutexLogger.unlock();
}

const std::string Util::Logger::GetCurrentSystemTime() const
{
	__time64_t sysTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	struct tm pTime;
	localtime_s(&pTime, &sysTime);
	char timeInString[60] = { 0 };
	sprintf_s(timeInString, "%d-%02d-%02d %02d:%02d:%02d",
		(int)pTime.tm_year + 1900,
		(int)pTime.tm_mon + 1,
		(int)pTime.tm_mday,
		(int)pTime.tm_hour,
		(int)pTime.tm_min,
		(int)pTime.tm_sec);
	return std::string(timeInString);
}

