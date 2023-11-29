#include <iostream>
#include <memory>
#include <string>
using namespace std;

class AbstractLogger
{
public:
    static int INFO;
    static int DEBUG;
    static int ERROR;

public:
    virtual ~AbstractLogger() = default;
    AbstractLogger(int m_level) : level(m_level) {}
    void setNextLogger(const shared_ptr<AbstractLogger> &m_nextLogger)
    {
        nextLogger = m_nextLogger;
    }

    void logMessage(int m_level, const string &message)
    {
        if (level <= m_level)
            write(message);

        if (nextLogger != nullptr)
            nextLogger->logMessage(m_level, message);
    }

protected:
    int level;
    shared_ptr<AbstractLogger> nextLogger;
    virtual void write(const string &message) const = 0;
};
int AbstractLogger::INFO = 1;
int AbstractLogger::DEBUG = 2;
int AbstractLogger::ERROR = 3;

class ConsoleLogger : public AbstractLogger
{
public:
    ConsoleLogger(int level) : AbstractLogger(level) {}

protected:
    void write(const string &message) const override
    {
        cout << "Standard Console::Logger: " + message << endl;
    }
};

class ErrorLogger : public AbstractLogger
{
public:
    ErrorLogger(int level) : AbstractLogger(level) {}

protected:
    void write(const string &message) const override
    {
        cout << "ErrorLogger  Console::Logger: " + message << endl;
    }
};

class FileLogger : public AbstractLogger
{
public:
    FileLogger(int level) : AbstractLogger(level) {}

protected:
    void write(const string &message) const override
    {
        cout << "FileLogger   Console::Logger: " + message << endl;
    }
};

shared_ptr<AbstractLogger> getChainOfLoggers()
{
    shared_ptr<AbstractLogger> errorLogger = make_shared<ErrorLogger>(AbstractLogger::ERROR);
    shared_ptr<AbstractLogger> fileLogger = make_shared<FileLogger>(AbstractLogger::DEBUG);
    shared_ptr<AbstractLogger> consoleLogger = make_shared<ConsoleLogger>(AbstractLogger::INFO);

    errorLogger->setNextLogger(fileLogger);
    fileLogger->setNextLogger(consoleLogger);

    return errorLogger;
}

int main()
{
    shared_ptr<AbstractLogger> loggerChain = getChainOfLoggers();
    loggerChain->logMessage(AbstractLogger::INFO, "This is an information.");
    loggerChain->logMessage(AbstractLogger::DEBUG, "This is a debug level information.");
    loggerChain->logMessage(AbstractLogger::ERROR, "This is an error information.");
    return 0;
}
