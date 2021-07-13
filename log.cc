#include "log.h"

std::string Logger::print_logLevel(logLevel l)
{
  switch (l)
	{
	  case logLevel::INFO:
	    return "INFO";
	    break;

	  case logLevel::DEBUG:
	    return"DEBUG";
		  break;

	  case logLevel::WARNING:
	    return "WARNING";
		  break;

	  case logLevel::ERROR:
		  return "ERROR";
		  break;

	  default:
		  return "Inexisting log level. Error";
	}
}

std::string Logger::getTimestamp()
{
  auto now = std::chrono::system_clock::now();
	auto in_time_t = std::chrono::system_clock::to_time_t(now);

	std::stringstream s;
	s << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");

	return s.str();
}

std::string Logger::getThreadId()
{
    auto id = std::this_thread::get_id();
	std::stringstream s;
	s << id;
	return s.str();
}


#define INFO logLevel::INFO
#define DEBUG logLevel::DEBUG
#define WARNING logLevel::WARNING
#define ERROR logLevel::ERROR



void f1() {
    auto &consoleLogger = getLogger(); // Log to console
    // Console output: 23.02.2020 18:06:06; DEBUG; (2561): Running a thread
    consoleLogger(DEBUG) << "Running a thread";

    auto &fileLogger = getLogger("log.txt"); // Log to file with name log.txt
    // File output: 23.02.2020 18:06:06; INFO; (2561): Running a thread
    fileLogger << "Running a thread"; // Default log level INFO
}

void f2()
{

    auto &logger = getLogger("log.txt", "f2"); // Log to file with name log.txt. Each message has f2 prefix
    // File output: 23.02.2020 18:06:06; DEBUG; f2(3444): Running a thread
    logger(DEBUG) << "Running a thread";

    double timeSpent = 10.0;
    // File output: 23.02.2020 18:06:06; WARNING; f2(3444): Time spent in the thread: 10.0 seconds
    logger(WARNING) << "Time spent in the thread: " << timeSpent << " seconds";
}


void f3()
{

auto &logger = getLogger("log2.txt", "f3");  // Log to file with name log2.txt. Each message has f3 prefix
    // File output: 23.02.2020 18:06:06; INFO; f3(3444): Running a thread
    logger << "Running a thread"; // Default log level INFO

    int i = 123;
    // File output: 23.02.2020 18:06:06; ERROR; f3(3444): My int is 123;
    logger(ERROR) << "My int is " << i;
}

int main()
{

//f1(); f2(); f3(); f4();

	std::thread t1(f1);
	std::thread t2(f2);
	std::thread t3(f3);

	t1.join();
	t2.join();
	t3.join();

}