#pragma once
#include <string>
#include <vector>
#include <mutex>

class LogBuffer {
public:
    void Add(const std::string& msg) {
        std::lock_guard<std::mutex> lock(mutex);
        logs.push_back(msg);
    }

    const std::vector<std::string>& GetLogs() const {
        return logs;
    }

private:
    std::vector<std::string> logs;
    mutable std::mutex mutex;
};
