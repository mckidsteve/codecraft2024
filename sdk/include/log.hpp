#pragma once

#include <iostream>
#include <fstream>

namespace std {
#ifdef _WIN32
    ofstream outfile; // 日志文件
#endif

    // 日志信息
    void log(string s) {
#ifdef _WIN32
        outfile << s << endl;
#endif
    }

    void LogInit() {
#ifdef _WIN32
        // 以写模式打开文件
        outfile.open("log.txt", ios::out | ios::trunc);
#endif
    }

    void LogClose() {
#ifdef _WIN32
        outfile.close();
#endif
    }
}