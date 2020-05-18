#include "linux_parser.h"
#include "format.h"
#include <dirent.h>
#include <string>
#include <unistd.h>
#include <vector>

using std::ifstream;
using std::istringstream;
using std::string;
using std::to_string;
using std::vector;

string LinuxParser::OperatingSystem() {
  string line, key, value;
  ifstream filestream(OSPath);
  if (filestream.is_open())
    while (getline(filestream, line)) {
      replace(line.begin(), line.end(), ' ', '_');
      replace(line.begin(), line.end(), '=', ' ');
      replace(line.begin(), line.end(), '"', ' ');
      istringstream linestream(line);
      while (linestream >> key >> value)
        if (key == "PRETTY_NAME") {
          replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
    }
  return value;
}

string LinuxParser::Kernel() {
  string os, kernel, line;
  ifstream stream(ProcDirectory + VersionFilename);
  if (stream.is_open()) {
    getline(stream, line);
    istringstream linestream(line);
    linestream >> os >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR *directory = opendir(ProcDirectory.c_str());
  struct dirent *file;
  while ((file = readdir(directory)) != nullptr) {
    if (file->d_type == DT_DIR) {
      string filename(file->d_name);
      if (all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

float LinuxParser::MemoryUtilization() {
  string line, key, value, totalMem = "", freeMem = "";
  ifstream filestream(ProcDirectory + MeminfoFilename);
  if (filestream.is_open())
    while (getline(filestream, line)) {
      istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "MemTotal:")
          totalMem = value;
        if (key == "MemFree:")
          freeMem = value;
      }
      if ((freeMem != "") && (totalMem != ""))
        break;
    }
  return ((stof(totalMem) - stof(freeMem)) / stof(totalMem));
}

long LinuxParser::UpTime() {
  string line, value;
  std::ifstream filestream(ProcDirectory + UptimeFilename);
  if (filestream.is_open()) {
    getline(filestream, line);
    istringstream linestream(line);
    linestream >> value;
  }
  return stol(value);
}

long LinuxParser::Jiffies() {
  string line, cpu, sUser, sNice, sSystem, sIdle, sIOwait, sIRQ, sSoftIRQ,
      sSteal, sGuest, sGuestNice;
  ifstream filestream(ProcDirectory + StatFilename);
  if (filestream.is_open()) {
    getline(filestream, line);
    istringstream linestream(line);
    linestream >> cpu >> sUser >> sNice >> sSystem >> sIdle >> sIOwait >>
        sIRQ >> sSoftIRQ >> sSteal >> sGuest >> sGuestNice;
  }
  return stol(sUser) + stol(sNice) + stol(sSystem) + stol(sIRQ) +
         stol(sSoftIRQ) + stol(sSteal) + stol(sGuest) + stol(sGuestNice) +
         stol(sIdle) + stol(sIOwait);
}

long LinuxParser::ActiveJiffies(int pid) {
  string line, shlp, sUtime, sStime, sCUtime, sCStime;
  ifstream filestream(ProcDirectory + to_string(pid) + StatFilename);
  if (filestream.is_open()) {
    getline(filestream, line);
    istringstream linestream(line);
    for (int i = 1; i < 14; i++)
      linestream >> shlp;
    linestream >> sUtime >> sStime >> sCUtime >> sCStime;
  }
  return stol(sUtime) + stol(sStime) + stol(sCUtime) + stol(sCStime);
}

long LinuxParser::ActiveJiffies() {
  string line, cpu, sUser, sNice, sSystem, sIdle, sIOwait, sIRQ, sSoftIRQ,
      sSteal;
  ifstream filestream(ProcDirectory + StatFilename);
  if (filestream.is_open()) {
    getline(filestream, line);
    istringstream linestream(line);
    linestream >> cpu >> sUser >> sNice >> sSystem >> sIdle >> sIOwait >>
        sIRQ >> sSoftIRQ >> sSteal;
  }
  return stol(sUser) + stol(sNice) + stol(sSystem) + stol(sIRQ) +
         stol(sSoftIRQ) + stol(sSteal);
}

long LinuxParser::IdleJiffies() {
  string line, cpu, sUser, sNice, sSystem, sIdle, sIOwait, sIRQ, sSoftIRQ,
      sSteal, sGuest, sGuestNice;
  ifstream filestream(ProcDirectory + StatFilename);
  if (filestream.is_open()) {
    getline(filestream, line);
    istringstream linestream(line);
    linestream >> cpu >> sUser >> sNice >> sSystem >> sIdle >> sIOwait >>
        sIRQ >> sSoftIRQ >> sSteal >> sGuest >> sGuestNice;
  }
  return stol(sIdle) + stol(sIOwait);
}

vector<string> LinuxParser::CpuUtilization() {
  long lOldActiveJiffies = ActiveJiffies();
  long lOldIdleJiffies = IdleJiffies();
  sleep(10);
  long lActiveJiffies = ActiveJiffies();
  long lIdleJiffies = IdleJiffies();
  vector<string> vResult;
  vResult.push_back(Format::ElapsedTime(((lActiveJiffies - lOldActiveJiffies) -
                                         (lIdleJiffies - lOldIdleJiffies)) /
                                        (lActiveJiffies - lOldActiveJiffies)));
  return vResult;
}

int LinuxParser::TotalProcesses() {
  string line, key, value;
  ifstream filestream(ProcDirectory + StatFilename);
  int totalProcesses;
  if (filestream.is_open())
    while (getline(filestream, line)) {
      istringstream linestream(line);
      linestream >> key >> value;
      if (key == "processes") {
        totalProcesses = stoi(value);
        break;
      }
    }
  return totalProcesses;
}

int LinuxParser::RunningProcesses() {
  string line, key, value;
  ifstream filestream(ProcDirectory + StatFilename);
  int runningProcesses;
  if (filestream.is_open())
    while (getline(filestream, line)) {
      istringstream linestream(line);
      linestream >> key >> value;
      if (key == "procs_running") {
        runningProcesses = stoi(value);
        break;
      }
    }
  return runningProcesses;
}

string LinuxParser::Command(int pid) {
  string line;
  ifstream filestream(ProcDirectory + to_string(pid) + CmdlineFilename);
  if (filestream.is_open())
    getline(filestream, line);
  return line;
}

string LinuxParser::Ram(int pid) {
  string line, key, value, ram;
  ifstream filestream(ProcDirectory + to_string(pid) + StatusFilename);
  if (filestream.is_open())
    while (getline(filestream, line)) {
      istringstream linestream(line);
      linestream >> key >> value;
      if (key == "VmData:") {
        int tmp = (100 * stof(value) / 1024 + 0.5);
        ram = to_string(tmp / 100) + "." +
              ((tmp % 100 < 10) ? "0" + to_string(tmp % 100)
                                : to_string(tmp % 100));
        break;
      }
    }
  return ram;
}

string LinuxParser::Uid(int pid) {
  string line, key, value, uid;
  ifstream filestream(ProcDirectory + to_string(pid) + StatusFilename);
  if (filestream.is_open())
    while (getline(filestream, line)) {
      istringstream linestream(line);
      linestream >> key >> value;
      if (key == "Uid:") {
        uid = value;
        break;
      }
    }
  return uid;
}

string LinuxParser::User(int pid) {
  string line, search_token = "x:" + to_string(pid);
  ifstream filestream(PasswordPath);
  if (filestream.is_open())
    while (getline(filestream, line)) {
      auto pos = line.find(search_token);
      if (pos != string::npos)
        return line.substr(0, pos - 1);
    }
  return "root"; // default value
}

long LinuxParser::UpTime(int pid) {
  string line, hlp;
  ifstream filestream(ProcDirectory + to_string(pid) + StatFilename);
  if (filestream.is_open()) {
    getline(filestream, line);
    istringstream linestream(line);
    for (int i = 0; i < 14; i++)
      linestream >> hlp;
  }
  return stol(hlp);
}
