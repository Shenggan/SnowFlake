#ifndef _LIGHT_LOG_H
#define _LIGHT_LOG_H

#include <iostream>
#include <ostream>
#include <fstream>
#include <sstream>
#include <string>
#include <memory>
#include <mutex>
#include <cstdlib>

namespace llog {

enum LOG_LEVEL {
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARNING,
    LOG_LEVEL_ERROR
};

#define COLOR_ERRO 31 //31
#define COLOR_INFO 37 //37
#define COLOR_WARN 33 //33
#define COLOR_FILE 35 //35
#define COLOR_LINE 32 //32
#define COLOR_FUNC 36 //36
#define DELIMITER " "

class Message {
private:
    std::string head;
    std::string head_color;
    std::string msg;
    std::string colorStr(int color, int bold, const std::string & delimiter, const std::string & t, bool if_color = true) {
        std::stringstream ss;
        if (if_color)
            ss << "\033[" << bold << ";" << color << "m";
        ss << t;
        if (if_color)
            ss << "\033[0m";
        ss << delimiter;
		return ss.str();
    }
    
    std::string logHeader(int level, std::string file, std::string function, int linenumber, bool if_color = true) {
        std::stringstream ss;
        std::stringstream st;
        int color = 39;
        if (level==2) { color = COLOR_ERRO; st << "[ERRO]"; }
		if (level==0) { color = COLOR_INFO; st << "[INFO]"; }
		if (level==1) { color = COLOR_WARN; st << "[WARN]"; }
        ss << colorStr(color,1,DELIMITER,st.str(), if_color);

        time_t rawtime;
		struct tm * timeinfo;
		char timeBuffer[30];
		time(&rawtime);
		timeinfo = localtime(&rawtime);
		strftime(timeBuffer, 30, "%Y/%m/%d-%H:%M:%S", timeinfo); 
		ss << colorStr(color, 0, DELIMITER, timeBuffer, if_color);

        int pPos = 0;
		for (int i = file.length()-1; i > 0; i--)
			if (file.substr(i,1)==std::string("/"))
			{ pPos = i+1; break; }
		file = file.substr(pPos,file.length()-pPos);
		ss << colorStr(COLOR_FILE, 0, ":", file, if_color); //35
		ss << colorStr(COLOR_LINE, 1, ":", std::to_string(linenumber), if_color); //32
		ss << colorStr(COLOR_FUNC, 0, DELIMITER, function, if_color); //36

        return ss.str();
    }
public:
    Message(const int level = 0, std::string file = nullptr, std::string function = nullptr, const int linenumber = 0) {
        head_color = logHeader(level, file, function, linenumber);
        head = logHeader(level, file, function, linenumber, false);
    }
    template <typename T> Message& operator<<(const T & t) {
        std::stringstream ss;
        ss << t;
        msg += ss.str();
        return *this;
    }
    std::string get_string(bool if_color = true) const {
        if (if_color)
            return head_color + msg;
        return head + msg;
    }
};

// Singleton Class
class Logger {
private:
    static std::unique_ptr<Logger>          m_instance_;
    LOG_LEVEL                               m_level_;
    static std::mutex                       mtx_;
    bool                                    console_;
    bool                                    file_;
    std::ofstream                           log_file;
    Logger() {
        if (getenv("LLOG_LV") == NULL)
            m_level_ = LOG_LEVEL_INFO;
        else{
            std::string ev_level = getenv("LLOG_LV");
            if (ev_level == "WARN")
                m_level_ = LOG_LEVEL_WARNING;
            else if (ev_level == "ERRO")
                m_level_ = LOG_LEVEL_ERROR;
            else
                m_level_ = LOG_LEVEL_INFO;
        }
        console_ = true;
        file_ = false;
    };
public:
    static Logger* get_instance() {
        if (m_instance_ == nullptr) {
            mtx_.lock();
            if (m_instance_ == nullptr) {
                m_instance_.reset(new Logger);
            }
            mtx_.unlock();
        }
        return m_instance_.get();
    }

    void init_log_file(std::string log_file_name = "output.log") {
        if (file_ == true)
            log_file.close();
        log_file.open(log_file_name);
        file_ = true;
    }
    
    void set_level(int level) {
        m_level_ = LOG_LEVEL(level);
    }

    int get_level() const {
        return m_level_;
    }

    void set_console(bool b) {
        console_ = b;
    }
    
    int get_console() const {
        return console_;
    }

    void operator+=(const Message& m)
    {
        mtx_.lock();
        if (console_)
            std::cout << m.get_string() + '\n';
        if (file_)
            log_file << m.get_string(false) + '\n';
        mtx_.unlock();
    }

};

std::unique_ptr<Logger> Logger::m_instance_ = nullptr;
std::mutex Logger::mtx_;

# define LLOG(level) if (level >= (*llog::Logger::get_instance()).get_level()) (*llog::Logger::get_instance()) += llog::Message(level, __FILE__, __FUNCTION__, __LINE__)

# define INFO 0
# define WARN 1
# define ERRO 2

# define LLOG_INFO LLOG(INFO)
# define LLOG_WARN LLOG(WARN)
# define LLOG_ERRO LLOG(ERRO)

# define LLOG_LV(level) (*llog::Logger::get_instance()).set_level(level)

# define LLOG_LV_INFO LLOG_LV(INFO)
# define LLOG_LV_WARN LLOG_LV(WARN)
# define LLOG_LV_ERRO LLOG_LV(ERRO)

# define LLOG_FILE(fn) (*llog::Logger::get_instance()).init_log_file(fn);

# define LLOG_CONSOLE_OFF() (*llog::Logger::get_instance()).set_console(false)
# define LLOG_CONSOLE_ON() (*llog::Logger::get_instance()).set_console(true)

} //namespace llog

#endif // _LIGHT_LOG_H