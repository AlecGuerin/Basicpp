#include "basicpp/inifile.h"
#include "basicpp/applog.h"

//using namespace std;

#pragma region Private Method
    /**
     * @brief Structure used to keep private methods out of the header.
     */
    struct IniFile::m_Pivate {
        


        /**
         * @brief Create a message from provided data like : '[CLASS_NAME] [Method_name] msg'
         *
         * @param self Init file object.
         * @param method_name Method from where the exception is triggered (ex: OpenFile). If set to default= [Method_name] = [UNKNOWN].
         * @param msg Message to write (ex: Invalide argumented detected). If set to default: msg = DEAULT_ERROR_MSG.
         * @return string Generated message.
         */
        static const std::string get_exception_msg(IniFile &self, std::string method_name ="", std::string msg ="") {

            std::string res =  "[" + self.CLASS_NAME + "]";

            if (method_name != "") {
                res += "[" + method_name + "] ";
            }
            else {
                res += "[UNKNOWN]";
            }

            if (msg != "") {
                res += msg;
            }
            else {
                res += DEAULT_ERROR_MSG;
            }

            return res;
        }



#ifdef APP_LOG_DONE
        /**
         * @brief Write to standard output from provided data like : '[CLASS_NAME] [Method_name] msg'
         *
         * @param self Init file object.
         * @param method_name Method from where the exception is triggered (ex: OpenFile). If set to default= [Method_name] = [UNKNOWN].
         * @param msg Message to write (ex: Invalide argumented detected). If set to default: msg = DEAULT_ERROR_MSG.
         */
        static void log_debug(IniFile &self, string method_name ="", string msg =""){

            if(L_DEBUG > self.m_debug_level)
                return;
            AppLog::log(L_DEBUG, get_exception_msg(self, method_name, msg));
            

        }

        static void log_verbose(IniFile &self, string method_name ="", string msg =""){

            if(L_VERBOSE > self.m_debug_level)
                return;

            AppLog::log(L_VERBOSE, get_exception_msg(self, method_name, msg));

        }

        static void log_info(IniFile &self, string method_name ="", string msg =""){

            if(L_INFO > self.m_debug_level)
                return;

            AppLog::log(L_INFO, get_exception_msg(self, method_name, msg));

        }

        static void log_warning(IniFile &self, string method_name ="", string msg =""){

            if(L_WARNING > self.m_debug_level)
                return;

            AppLog::log(L_WARNING, get_exception_msg(self, method_name, msg));

        }

        static void log_error(IniFile &self, string method_name ="", string msg =""){

            if(L_ERROR > self.m_debug_level)
                return;

            AppLog::log(L_ERROR, get_exception_msg(self, method_name, msg));

        }

        static void log_critical(IniFile &self, string method_name ="", string msg =""){

            if(L_CRITICAL > self.m_debug_level)
                return;

            AppLog::log(L_CRITICAL, get_exception_msg(self, method_name, msg));

        }


#else
        static void dummy_log(IniFile &self, std::int32_t level, std::string msg, std::string tag)
        {
            if(self.GetDebugLevel() < level)
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
                str.append("[");
                str.append(std::to_string(level));
                str.append("] ");
                // Add tag
                if(tag != ""){
                str.append("["+ tag + "] ");
                }
                str.append( msg);

                // Log on standard output
                std::cout << str << std::endl;

            } catch (std::exception e) {

            }
        }
        static void log_debug(IniFile &self, std::string method_name ="", std::string msg =""){dummy_log(self, L_DEBUG, get_exception_msg(self, method_name, msg), "TAG");};
        static void log_verbose(IniFile &self, std::string method_name ="", std::string msg =""){dummy_log(self, L_VERBOSE, get_exception_msg(self, method_name, msg), "TAG");};
        static void log_info(IniFile &self, std::string method_name ="", std::string msg =""){dummy_log(self, L_INFO, get_exception_msg(self, method_name, msg), "TAG");};
        static void log_warning(IniFile &self, std::string method_name ="", std::string msg =""){dummy_log(self, L_WARNING, get_exception_msg(self, method_name, msg), "TAG");};
        static void log_error(IniFile &self, std::string method_name ="", std::string msg =""){dummy_log(self, L_ERROR, get_exception_msg(self, method_name, msg), "TAG");};
        static void log_critical(IniFile &self, std::string method_name ="", std::string msg =""){dummy_log(self, L_CRITICAL, get_exception_msg(self, method_name, msg), "TAG");};
#endif

        /**
         * @brief Split a string to a vector with the provided characters as delimiter.
         * @param data String to split.
         * @param delimiter Characters used as delimiters.
         * @return const vector<string> Result as a string vector.
         */
        static const std::vector<std::string> string_split(const std::string& data, const std::uint8_t & delimiter)
        {
            std::string buff{""};
            std::vector<std::string> v;

            for(auto n:data)
            {
                if(n != delimiter){
                    buff+=n;
                }
                else if(n == delimiter && buff != "")
                {
                    v.push_back(buff); buff = "";
                }
            }
            if(buff != "") v.push_back(buff);

            return v;
        }
    };

#pragma endregion

#pragma region Constructor

    /**
     * @brief Construct a new Ini File:: Ini File object.
     * @param path Ini file path.
     */
    IniFile::IniFile(std::string path) {

        m_path = path;
        m_file_exists = false;

        m_debug_level = L_INFO;        

        if(m_path != ""){
            create_file(m_path);
            m_file_exists = std::experimental::filesystem::exists(m_path);
        }

        // TODO: replace with a log
        m_Pivate::log_debug(*this , "IniFile", "constructor ended");
    }

    /**
     * @brief Destroy the Ini File:: Ini File object
     */
    IniFile::~IniFile() {
        m_Pivate::log_debug(*this , "~IniFile", "destructor ended");
    }

#pragma endregion

#pragma region Method

    /**
     * @brief Create provided file if needed.
     * @param path File path (ex ..\..\file.txt). If set to default, use the field 'm_path'.
     */
    void IniFile::create_file(std::string path){

        std::FILE* file;
        // Set curent path as default
        if("" == path) path = m_path;

        try{

            // Open or create file if needed
            file = fopen(path.c_str(), "a+");
            // Check error
            if(NULL == file){

                std::string err = "fopen() failed in file ";
                err.append(__FILE__).append(" at line ").append(to_string(__LINE__-4)); // TODO: check if multiplateform proof
                throw std::runtime_error(err.c_str());
            }
            // Close file
            fclose(file);
            m_file_exists = true;
        }
        catch(std::exception e){
            std::string w = e.what();
            //TODO: log message
            m_Pivate::log_warning(*this , "create_file", "An exception occured: " + w);
        }
    }

    /**
     * @brief Get the section as string vector.
     *
     * @param res Results are set in the provided vector.
     * @exception This method can throw exception.
     */
    /*void IniFile::get_sections(std::vector<string> * res, std::vector<int> * linePos) {

        std::regex rg(R"(\[(.+)\])");       // Cretae regex for sections
        std::ifstream inputStream(m_path, std::ifstream::in);// get the input stream
        std::stringstream buffer;           // Create a buffer        

        // Check if file still exists
        m_file_exists = std::experimental::filesystem::exists(m_path);

        if (m_file_exists) {
            m_Pivate::log_debug(*this, "get_sections", "Starts");

            try {
                buffer << inputStream.rdbuf();  // Read buffer
                const std::string test = buffer.str(); // Get the stream text
                std::sregex_iterator next(test.begin(), test.end(), rg);    // Get matches
                std::sregex_iterator end;       // Get last match

                // Add all matches to result array
                while (next != end) {
                    std::smatch match = *next;
                    res->push_back( match.str());
                    next++;
                }

            } catch(std::exception e){

                throw std::exception(m_Pivate::get_exception_msg(*this, "get_sections", e.what()).c_str());
            }
        }
        else
        {
            throw std::invalid_argument(m_Pivate::get_exception_msg(*this, "get_sections", "Path doesn't exist."));
        }
    }
*/
    void IniFile::get_sections(std::vector<std::string> *res, std::vector<std::int32_t> *linePos){

        std::int32_t line = 0;
        std::size_t charIndex = -1;
        std::string data;
        std::ifstream inputStream(m_path, std::ifstream::in);

        m_Pivate::log_debug(*this , "get_keys", "Starts");
        res->clear(); // reset result

        while(getline(inputStream, data)){
            // Check for the section begining
            charIndex = data.find('[');
            if(std::string::npos != charIndex){
                res->push_back(data.substr(charIndex, data.find(']')+1));
                // Add line if needed
                if(linePos != NULL){
                    linePos->push_back(line);
                }
            }
            // Update line index
            line++;
        }
    }

    /**
     * @brief Get the keys of the provided section.
     *
     * @param section Section to search on.
     * @param res Found results.
     * @exception This method can throw exception.
     */
    void IniFile::get_keys(std::string section, std::vector<std::string> * res, std::vector<std::int32_t> * linePos) {

        std::uint8_t sectionFound = 0;
        std::uint32_t lineIndex = -1;
        std::string data;
        std::ifstream inputStream(m_path, std::ifstream::in);

        m_Pivate::log_debug(*this , "get_keys", "Starts");
        res->clear(); // reset result

        //Update section if needed
        // Add crochet if needed to don't mistake a key or a value as a section
        if( '[' != section[0]){
            section = '[' + section;
        }
        if( ']' != section.back()){
            section = section + ']';
        }

        while(getline(inputStream, data)){
            lineIndex++;
            // Check for the begining
            if(std::string::npos != data.find(section)){
                sectionFound = 1;
                continue;
            }
            // Check for end of section
            else if(sectionFound && std::string::npos != data.find('[')){
                break;
            }
            // Add key to result if the section is found
            else if(sectionFound){
                res->push_back(m_Pivate::string_split(data, '=')[0]);
                // Add line corresponding to key
                if(linePos != NULL){
                    linePos->push_back(lineIndex);
                }
            }
        }
    }

    /**
     * @brief Check and create the provided section if it doesn't exist.
     *
     * @param section_name Section name.
     */
    void IniFile::add_section(std::string section_name) {

        m_Pivate::log_debug(*this , "create_section", "Starts");

        std::vector<std::string> curentSections;
        std::fstream iniFile(m_path , std::fstream::out | std::fstream::app);

        // Get current sections
        get_sections(&curentSections);
        //  Check if the section already exists
        for(std::uint32_t i = 0; i < curentSections.size();i++){
            // Return if dection already exists
           if(std::string::npos != curentSections[i].find(section_name)){
               m_Pivate::log_verbose(*this , "create_section", "Section already exists");
               return;
           }
        }
        m_Pivate::log_verbose(*this , "create_section", "Section doesn't exist");

        // Add crochet if needed to don't mistake a key or a value as a section
        if( '[' != section_name[0]){
            section_name = '[' + section_name;
        }
        if( ']' != section_name.back()){
            section_name = section_name + ']';
        }
        if(iniFile.is_open())
        {
            iniFile.write(section_name.c_str(), section_name.size());
            iniFile << std::endl;
            iniFile.close();
            m_Pivate::log_verbose(*this , "create_section", "Section added");
        }
        else
        {
            m_Pivate::log_warning(*this, "create_section", "Error while writting section to file");
        }

    }

    /**
     * @brief Add a value to the provided section and key. If nether of them exist, they are created.
     *
     * @param section Section to use.
     * @param key Key to use.
     * @param value Value to set.
     * @exception exception This method can throw exceptions.
     */
    void IniFile::add_value(std::string section, std::string key, std::string value) {
        m_Pivate::log_debug(*this , "add_value", "Starts");

        std::uint8_t valueAdded = 0;
        std::uint8_t sectionFound = 0;
        std::uint8_t keyFound = 0;
        std::int32_t lineIndex = -1;
        std::string data;
        std::vector<std::string> fileData;
        std::fstream iniFile(m_path , std::fstream::in);  // Open or create file if needed

        // Add crochet if needed to don't mistake a key or a value as a section
        if( '[' != section[0]){
            section = '[' + section;
        }
        if( ']' != section.back()){
            section = section + ']';
        }

        // Read file and add key if a place has been found for it
        while(getline(iniFile, data)){

            // Update the line index
            lineIndex ++;
            // Add line to temporary data
            fileData.push_back(data);

            // Juste add values if the key has already been udpated
            if(valueAdded){
                continue;
            }
            // Check for the begining of section
            else if(!sectionFound && std::string::npos != data.find(section)){
               sectionFound = 1;
               continue;
            }
            // Check for end of section
            else if(sectionFound && !keyFound && std::string::npos != data.find('[')){
                // Add a copy of last element
                fileData.push_back(fileData[lineIndex]);
                // Replace previous element with key value
                fileData[lineIndex] = key + '=' + value;
                // update counter to match the added element
                lineIndex++;
                keyFound = 1;
                valueAdded = 1;
            }
            // Check for key
            else if(sectionFound && !keyFound &&
                    std::string::npos != m_Pivate::string_split(data, '=')[0].find(key)){
                fileData[lineIndex] = key + '=' + value;
                keyFound = 1;
                valueAdded = 1;
            }
        }
        // Close file
        iniFile.close();

        // If value hasn't been added, add it to the end
        if(!valueAdded){
            // Add section (manage the case it in last section so next section hast been found)
            if(!sectionFound){

                fileData.push_back(section);
            }
            // Add key
            fileData.push_back(key + '=' + value);
        }

        // Write new file
        std::ofstream outFile(m_path);
        for (const auto &data : fileData){
            outFile << data << std::endl;
        }
        outFile.close();
    }

    /**
     * @brief
     *
     * @param section
     * @param key
     */
    void IniFile::remove_key(std::string section, std::string key) {
        m_Pivate::log_debug(*this , "remove_key", "Starts");

        std::uint8_t keyRemoved = 0;
        std::uint8_t sectionFound = 0;
        std::string data;
        std::vector<std::string> fileData;
        std::fstream iniFile(m_path , std::fstream::in);

        // Add crochet if needed to don't mistake a key or a value as a section
        if( '[' != section[0]){
            section = '[' + section;
        }
        if( ']' != section.back()){
            section = section + ']';
        }

        // Read file and remove key if it has been found
        while(getline(iniFile, data)){

            // Continue flow if key has already been found
            if(keyRemoved){
            // Do nothing
            }
            // Check for provided section
            else if(!sectionFound && std::string::npos != data.find(section)){
               sectionFound = 1;
            }
            // Check for end of section
            else if (sectionFound && std::string::npos != data.find('[')) {
               // Key doesn't exist in provided section
               keyRemoved = 1;
            }
            // Check for key
            else if (sectionFound && std::string::npos != m_Pivate::string_split(data, '=')[0].find(key)) {
               // tell the key is removed
               keyRemoved = 1;
               // Don't add the data
               continue;
            }
            // Add line to temporary data
            fileData.push_back(data);

        }
        // Close file
        iniFile.close();

        // Write new file
        std::ofstream outFile(m_path);
        for (const auto &data : fileData){
            outFile << data << std::endl;
        }
        outFile.close();

    }

    /**
     * @brief
     *
     * @param section
     * @param key
     */
    void IniFile::remove_value(std::string section, std::string key) {
        m_Pivate::log_debug(*this , "remove_value", "Starts");

        std::uint8_t valueRemoved = 0;
        std::uint8_t sectionFound = 0;
        std::int32_t lineIndex = 0;
        std::string data;
        std::vector<std::string> fileData;
        std::fstream iniFile(m_path , std::fstream::in);

        // Add crochet if needed to don't mistake a key or a value as a section
        if( '[' != section[0]){
            section = '[' + section;
        }
        if( ']' != section.back()){
            section = section + ']';
        }

        // Read file and add key if a place has been found for it
        while(getline(iniFile, data)){

            // Add line to temporary data
            fileData.push_back(data);

            if(valueRemoved){
                continue;
            }
            // Check for the begining of section
            else if(!sectionFound && std::string::npos != data.find(section)){
               sectionFound = 1;
            }
            // Check for end of section
            else if(sectionFound && std::string::npos != data.find('[')){
                valueRemoved = 1;
            }
            // Check for key
            else if(sectionFound &&
                    std::string::npos != m_Pivate::string_split(data, '=')[0].find(key)){
                fileData[lineIndex] = key + '=';
                valueRemoved = 1;
            }
            lineIndex++;
        }
        // Close file
        iniFile.close();

        // Write new file
        std::ofstream outFile(m_path);
        for (const auto &data : fileData){
            outFile << data << std::endl;
        }
        outFile.close();
    }

    /**
     * @brief IniFile::remove_section Remove all data of the provided section.
     * @param section Section to delete.
     */
    void IniFile::remove_section(std::string section)
    {
        m_Pivate::log_debug(*this , "remove_section", "Starts");

        std::uint8_t sectionFound = 0;
        std::uint8_t sectionErased = 0;
        std::string data;
        std::vector<string> fileData;
        std::fstream iniFile(m_path , std::fstream::in);

        // Add crochet if needed to don't mistake a key or a value as a section
        if( '[' != section[0]){
            section = '[' + section;
        }
        if( ']' != section.back()){
            section = section + ']';
        }

        // Read file and remove section data
        while(getline(iniFile, data)){

            // Check for the begining of section
            if(!sectionErased && !sectionFound && std::string::npos != data.find(section)){
               sectionFound = 1;
               continue;
            }
            // Check for end of section
            else if(sectionFound && std::string::npos != data.find('[')){
                sectionFound = 0;
                sectionErased = 1;
            }

            if(!sectionFound){
                // Add line to temporary data
                fileData.push_back(data);
            }

        }
        // Close file
        iniFile.close();

        // Write new file
        std::ofstream outFile(m_path);
        for (const auto &data : fileData){
            outFile << data << std::endl;
        }
        outFile.close();
    }

    /**
     * @brief IniFile::get_value
     * @param section
     * @param key
     * @return
     */
    std::uint8_t IniFile::get_value(std::string section, std::string key, std::string *value)
    {
        std::uint8_t res = 0;
        std::uint8_t sectionFound = 0;
        std::string data;
        std::fstream iniFile(m_path , fstream::in);

        *value = "";

        // Read file
        while(getline(iniFile, data)){

            // Check for the begining of section
            if(!sectionFound && std::string::npos != data.find(section)){
               sectionFound = 1;
            }
            // Check for end of section
            else if(sectionFound && std::string::npos != data.find('[')){
                break;
            }
            // Check for key
            else if(sectionFound &&
                    std::string::npos != m_Pivate::string_split(data, '=')[0].find(key)){
                vector<string> temp = m_Pivate::string_split(data, '=');

                if(temp.size()> 1){
                    //temp[1].copy((char*)value, temp[1].length());
                    *value = temp[1];
                }

                res = 1;
                break;
            }
        }
        // Close file
        iniFile.close();
        return res;
    }

#pragma endregion
