#ifndef INIFILE_H
#define INIFILE_H

#include<iostream>
#include <fstream>
#include <exception>
#include <experimental/filesystem>

#include "basicpp.h"

using namespace std;


/**
 * @brief The IniFile class is used to manage ini file documents.
 */
class IniFile
{

    private:
        
        struct m_Pivate;                // shadow pointer to the methods implementations

        std::uint8_t m_file_exists;     // Does the INI file exist ?

        std::int32_t m_debug_level;     // Log level to set

        std::string m_path;             // Path to the ini file

    public:
#pragma region Const
        /**
         * @brief CLASS_NAME Inifile class name.
         */
        const std::string CLASS_NAME = "IniFile";

#pragma endregion

#pragma region Getter/Setter

        /**
         * @brief Get the INI file path.
         */
        std::string GetIniFilePath() { return m_path; };
        /**
         * @brief Set the INI file path.
         * @param path Path to set.
         */
        void SetIniFilePath(string path) { m_path = path; };

        /**
         * @brief Get the File Exists checked state.
         *
         * @return 1 File has been created and found onece.
         * @return 0 File wasn't found or created.
         */
        std::uint8_t GetFileExists(){return m_file_exists;}

        /**
         * @brief GetDebugLevel Get the current debuging level.
         * @return Current debuging level.
         */
        std::int32_t GetDebugLevel(){return m_debug_level;}
        /**
         * @brief SetDebugLevel Set the debuging level.
         * @param level Level to set.
         */
        void SetDebugLevel(std::int32_t level){m_debug_level = level;}

#pragma endregion

#pragma region Constructor

        /**
         *
         * @brief Constructor for 'IniFile'.
         * @param path File path.
         */
        IniFile(string path = "");

        /**
         * @brief IniFile destructor.
         */
        ~IniFile();

#pragma endregion

#pragma region Method

        void get_sections(std::vector<std::string> * res, std::vector<std::int32_t> * linePos = NULL);
        void get_keys(std::string section, std::vector<std::string> * res, std::vector<std::int32_t> * linePos = NULL);

        void create_file(std::string path = "");
        void add_section(std::string section_name);

        void add_value(std::string section, std::string key, std::string value);

        void add_key(std::string section, std::string key, std::string value) { add_value(section, key, value); }
        void add_key(std::string section, std::string key) { add_value(section, key, ""); }

        void remove_key(std::string section, std::string key);
        void remove_value(std::string section, std::string key);

        void remove_section(std::string section);

        std::uint8_t get_value(std::string section, std::string key, std::string *value);

#pragma endregion
};

#endif // INIFILE_H
