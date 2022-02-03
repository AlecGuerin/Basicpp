#include "basicpp/applog.h"

AppLog::AppLog(std:: string logPath)
{
    m_writeWait = 0;
    m_endThread = 0;

    m_logLevel = L_DEBUG;
    m_queueLength = 0;

    if(logPath != ""){
        m_logPath = logPath;
        create_file(m_logPath);
    }
    else{
        create_file(DEFAULT_LOG_PATH);
    }

    try {
        m_logStream.open(m_logPath, std::fstream::out | std::fstream::app);

    }  catch (std::exception e) {
        cout << e.what() << endl;
    }

    // Create a thread with the
     m_writerThread = std::thread([this]() { write_remaning_th(); });
}

AppLog::~AppLog(){

    // Send command to finish the thread function
    m_endThread = 1;
    if(m_writerThread.joinable()){
        m_writerThread.join();
    }


    try{
        if(m_logStream.is_open()) m_logStream.close();
    }catch(...){
        cout << "error" << endl;
    };


}

/**
 * @brief AppLog::AddValue
 * @param value
 */
void AppLog::add_value(std::string value)
{
    try {
        m_queueLock.lock();


        if(m_queueLength >= QUEUE_SIZE){
            // TODO: manage error
            throw std::runtime_error("Writing queue is full !");
        }
        m_queue.push_back(value);
        m_queueLength++;

    }  catch (std::exception e) {

    }

    m_queueLock.unlock();

    m_fileCheckth.lock();
    m_writeWait = 1;
    m_fileCheckth.unlock();
}

/**
 * @brief AppLog::write_queue_value
 */
void AppLog::write_queue_value()
{
    try {
        if(m_fileLock.try_lock()){
            if(!m_logStream.is_open()){
                m_logStream.open(m_logPath, std::fstream::out | std::fstream::app);
            }
            try{
                m_queueLock.lock();

                while(m_queueLength){
                    m_queueLength--;
                    m_logStream << m_queue[m_queueLength] << std::endl;
                    m_queue.erase(m_queue.begin()+m_queueLength);
                }

            }catch(std::exception ex){

            }
            m_fileLock.unlock();
            m_queueLock.unlock();

            m_fileCheckth.lock();
            m_writeWait = 0;
            m_fileCheckth.unlock();

        }
        else{

            m_fileCheckth.lock();
            m_writeWait = 1;
            m_fileCheckth.unlock();
        }

    }  catch (std::exception e) {

    }
}

/**
 * @brief log Log the provided message with the provided level.
 * @param level
 * @param msg
 */
void AppLog::log(std::int32_t level, std::string msg, std::string tag)
{
    // Skip log if it's less critical than expected
    if(m_logLevel < level )
        return;

    std::uint8_t date[20];
    std::string str = "";

    // get current time
    std::time_t time  = std::time(0);
    std::tm* now = std::localtime(&time);
    try {
        // Add date
        sprintf((char*)date, "%d-%02d-%02d %02d:%02d:%02d",
                now->tm_year+1900,
                now->tm_mon + 1,
                now->tm_mday,
                now->tm_hour + 1,
                now->tm_min + 1,
                now->tm_sec + 1
                );

        str.append((char*)date);

        // Add debug info
        str.append("["+ get_debug_letter(level) + "] ");
        // Add tag
        if(tag != ""){
          str.append("["+ tag + "] ");
        }
        str.append( msg);

        // Log on standard output
        add_value(str.c_str());

    } catch (std::exception e) {

    }
}

/**
 * @brief get_debug_letter
 * @param debug_level
 * @return
 */
const string AppLog::get_debug_letter(int debug_level)
{
    std::string res = "X";

    switch (debug_level) {
        case L_CRITICAL:
            res ="C";
        break;

        case L_ERROR:
            res = "E";
        break;

        case L_WARNING:
            res = "W";
            break;

        case L_INFO:
            res = "I";
            break;

           case L_VERBOSE:
            res = "V";
            break;

        case L_DEBUG:
            res = "D";
            break;
    }

    return res;
}

/**
 * @brief Create provided file if needed.
 * @param path File path (ex ..\..\file.txt). If set to default, use the field 'm_path'.
 */
void AppLog::create_file(std::string path){

    FILE* file;
    // Set curent path as default
    if("" == path) path = m_logPath;

    try{

        // Open or create file if needed
        file = fopen(path.c_str(), "a+");
        // Check error
        if(NULL == file){

            std::string err = "fopen() failed in file ";
            err.append(__FILE__).append(" at line ").append(to_string(__LINE__-4));
            throw std::runtime_error(err.c_str());
        }
        // Close file
        fclose(file);
    }
    catch(std::exception e){
        std::string w = e.what();
    }
}

/**
 * @brief AppLog::write_remaning_th
 */
void AppLog::write_remaning_th()
{
    while(!m_endThread){
        if(m_writeWait){

            write_queue_value();
        }
    }
}


