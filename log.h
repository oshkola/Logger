#ifndef LOG_H
#define LOG_H

#include <atomic>
#include <thread>
#include <string>
#include <memory>
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <iomanip>
#include <mutex>

enum class logLevel
{
  INFO,
  DEBUG,
  WARNING,
  ERROR
};

class Logger
{
  Logger() = default;
  logLevel ll;
  std::string _filePath{};
  std::string _prefix{};
  std::atomic<bool> _writeToFile{false};
  std::atomic<bool> _addPrefix{false};

public:
  Logger(const Logger&) = delete;
  Logger& operator=(const Logger&) = delete;
	
static Logger& getInstance()
{
  static Logger _logger;
  return _logger;
}

std::string print_logLevel(logLevel l);

void writeToConsole()
{
  _writeToFile = false;
}
void resetPrefix()
{
  _addPrefix = false;
}

std::string getTimestamp();
std::string getThreadId();
  
void setFilePath(std::string& filePath)
{
  _filePath = filePath;
  _writeToFile = true;
} 
void setPrefix(std::string& prefix)
{
  _prefix = prefix;
  _addPrefix = true;
}

template <typename T>
void log(T message)
{
  std::mutex _mtx;
  _mtx.lock();

  std::string _message;
  _message.append(getTimestamp() + "; " + print_logLevel(ll) 
		                           + "; ");
  if (_addPrefix.load())
    _message.append(_prefix);
		
  _message.append("(" + getThreadId() + "): ");

  if (_writeToFile.load())
  {
    std::ofstream logFile(_filePath, std::ios_base::app | std::ios_base::out);
    logFile << _message << message << "\n";
    logFile.close();
  }
  else
  {
    std::cout << _message << message;
    std::cout << std::flush;
  }
	_mtx.unlock();
}

Logger& operator()(logLevel l)
{
  ll = l;
  return *this;
}

template <typename T>
Logger& operator << ( T message)
{
  this->log(message); 
  return *this;
}
};

Logger& getLogger()
{
  auto& logger = Logger::getInstance();
  logger.writeToConsole();
  return logger;
}

Logger& getLogger(std::string filePath)
{
  auto& logger = Logger::getInstance();
  logger.setFilePath(filePath);
  logger.resetPrefix();
  return logger;
}

Logger& getLogger(std::string filePath, std::string prefix)
{
  auto &logger = Logger::getInstance();
  logger.setFilePath(filePath);
  logger.setPrefix(prefix);
  return logger;
}

#endif
