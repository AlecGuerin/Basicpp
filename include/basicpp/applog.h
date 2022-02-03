#ifndef APPLOG_H
#define APPLOG_H

#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <mutex>
#include <vector>
#include <exception>

#include "basicpp.h"

/**
 * @brief The AppLog class
 */
class AppLog
{
#pragma region Const
public:
    const std::int32_t QUEUE_SIZE = 100;              ///< Queue size for the remaining data queue.
    const string DEFAULT_LOG_PATH = "log.txt";  ///< Default log path to use.

#pragma endregion

private:

    volatile std::uint8_t m_writeWait;
    volatile std::uint8_t m_endThread;

    std::int32_t m_queueLength;

    std::int32_t m_logLevel;
    std::string m_logPath;
    std::vector<string> m_queue;

    std::fstream m_logStream;

    std::mutex m_queueLock;
    std::mutex m_fileLock;
    std::mutex m_fileCheckth;

    std::thread m_writerThread;


#pragma region Getter/Setter
public:
    // Log path
    /**
     * @brief GetLogPath Get the current path to the log file.
     * @return Current log file path.
     */
    std::string GetLogPath(){return m_logPath;}
    /**
     * @brief SetLogPath
     * @param logPath
     */
    void SetLogPath(std::string logPath){m_logPath = logPath;}

    // Log level
    /**
     * @brief GetLogLevel Get the current log level.
     * @return Current log level.
     */
    std::int32_t GetLogLevel(){return m_logLevel;}
    /**
     * @brief SetLogLevel Set the current log level.
     * @param logLevel Log level to set.
     */
    void SetLogLevel(std::int32_t logLevel){m_logLevel = logLevel;}

#pragma endregion

#pragma region Constructor/Descructor

    AppLog(std::string path = "");
    ~AppLog();

#pragma endregion

#pragma region Method
public:

    void log(std::int32_t level, std::string msg, std::string tag = "");

private:
    const std::string get_debug_letter(std::int32_t debug_level);

    void add_value(std::string value);
    void write_queue_value();
    void create_file(std::string path);

    void write_remaning_th();

#pragma endregion

};

#endif // APPLOG_H
