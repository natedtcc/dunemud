// logger.c - basic logging object - Bebop 2021

string DEFAULT_LOG_PATH = "/players/bebop/log";

// This will hold path-to-logfile
string logFile = DEFAULT_LOG_PATH;


void setLogFile (string str);
string getLogFile ();
int writeLog (string str);
int clearLog();

// Set path-to-logfile
void setLogFile(string str){
  logFile = str;
}

// Get the path-to-logfile
string getLogFile(){
  return logFile;
}

// Write a string to the logfile
int writeLog(string str){
  if(write_file(logFile, str + "\n")) return 1;
  else return 0;
}

// Clear the logfile
int clearLog(){
  if(rm(logFile)) return 1;
  else return notify_fail(
    "Unable to clear log. Check the log file path (getLogFile()).\n");
}