//
// Created by nomi on 5/6/20.
//
#include <unistd.h>
#include <string.h>
#include <vector>
#include <iostream>

#include "SmallShell.h"
#include "Utilities.h"
#include "Commands.h"
#include "BuiltInCommand.h"
#include "ExternalCommand.h"
#include "RedirectionCommand.h"
#include "PipeCommand.h"

/**
 * Aux functions
 */
void _setCmdType(char *cmdLine, char *cmd, CMD_TYPE *eCmdType) {
    string sCmdLine = string(cmdLine);
    bool isRedirectionAppend = (sCmdLine.find(">>") != std::string::npos);
    bool isRedirection = !isRedirectionAppend && (sCmdLine.find(">") != std::string::npos);
    bool isPipeErr = (sCmdLine.find("|&") != std::string::npos);
    bool isPipe = !isPipeErr && (sCmdLine.find("|") != std::string::npos);

    if (isRedirectionAppend) {
        *eCmdType = eRedAppend;
    } else if (isRedirection) {
        *eCmdType = eRed;
    } else if (isPipeErr) {
        *eCmdType = ePipeErr;
    } else if (isPipe) {
        *eCmdType = ePipe;
    } else if (strcmp(cmd, "chprompt") == 0) {
        *eCmdType = eChprompt;
    } else if (strcmp(cmd, "showpid") == 0) {
        *eCmdType = eShowpid;
    } else if (strcmp(cmd, "pwd") == 0) {
        *eCmdType = ePwd;
    } else if (strcmp(cmd, "cd") == 0) {
        *eCmdType = eCd;
    } else if ((strcmp(cmd, "kill") == 0)){
        *eCmdType = eKill;
    } else if ((strcmp(cmd, "fg") == 0)){
        *eCmdType = eFg;
    } else if ((strcmp(cmd, "bg") == 0)){
        *eCmdType = eBg;
    } else if ((strcmp(cmd, "quit") == 0)){
        *eCmdType = eQuit;
    } else if ((strcmp(cmd, "jobs") == 0)){
        *eCmdType = eJobs;
    } else {
        *eCmdType = eExternal;
    }
}

/**
 * Constructors and destructors
 */
SmallShell::SmallShell() {
}

SmallShell::~SmallShell() {
// TODO: add your implementation
}

/**
 * getters and setters
 */
void SmallShell::setPrompt(const char *_prompt) {
    prompt = _prompt;
}

string SmallShell::getPrompt() {
    return prompt;
}

void SmallShell::setPrevDir(const char* _dir) {
    prevDir = _dir;
}

string SmallShell::getPrevDir() {
    return prevDir;
}

/**
 * other class methods
 */

/**
* Creates and returns a pointer to Command class which matches the given command line (cmd_line)
*/
Command *SmallShell::CreateCommand(const char* cmd_line) {
    //std::cout << "cmd createCommand: " << cmd_line << '\n';
    char *argv[COMMAND_MAX_ARGS] = {};
    int argc = _parseCommandLine(cmd_line, argv);

    if (argc == 0) {
        _freeFields(argv, argc);
        return nullptr;
    }
    char cmdLine[COMMAND_ARGS_MAX_LENGTH];
    strcpy(cmdLine, cmd_line);
    bool isBackgroundCmd;
    CMD_TYPE eCmdType;

    RedInfo& redInfo = RedInfo::getInstance();

    // 1. check if &
    isBackgroundCmd = (string(cmd_line).back() == '&');
    // 2. remove last &
    _removeBackgroundSign(cmdLine);
    // now we have the cmd line with out the last & and we have a boolean value indicating if this is a background cmd
    // 3. set cmd type
    _setCmdType(cmdLine, argv[0], &eCmdType);
    // 4. free the cmdLine
    _freeFields(argv, argc);
    // 5. create te command

    switch(eCmdType) {
        case eChprompt:
            return new ChangePromptCommand(cmdLine);
        case eShowpid:
            return new ShowPidCommand(cmdLine);
        case ePwd:
            return new PwdCommand(cmdLine);
        case eCd:
            return new CdCommand(cmdLine);
        case eKill:
            return new KillCommand(cmdLine);
        case eFg:
            return new ForegroundCommand(cmdLine);
        case eBg:
            return new BackgroundCommand(cmdLine);
        case eQuit:
            return new QuitCommand(cmdLine);
        case eJobs:
            return new JobsCommand(cmdLine);
        case eRed:
            return new RedirectionCommand(cmd_line, isBackgroundCmd, false);
        case eRedAppend:
            return new RedirectionCommand(cmd_line, isBackgroundCmd, true);
        case ePipe:
            return new PipeCommand(cmd_line, isBackgroundCmd, false);
        case ePipeErr:
            return new PipeCommand(cmd_line, isBackgroundCmd, true);
        case eExternal:
            return new ExternalCommand(cmd_line, (redInfo.isRedirection) ? redInfo.isBackground : isBackgroundCmd); // if redirection get isBackground from redInfo instance class
        default:
            return nullptr;
    }
}

void SmallShell::executeCommand(const char *cmd_line) {
    // TODO: Add your implementation here

    // for example:
    Command* cmd = CreateCommand(cmd_line);
    if (cmd) {
        cmd->execute();
    }
    delete(cmd);
    // Please note that you must fork smash process for some commands (e.g., external commands....)
};
