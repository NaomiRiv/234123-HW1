//
// Created by nomi on 5/6/20.
//

#include <vector>
#include <time.h>

#include "JobsList.h"

/**
 * Aux functions
 */
/**
* Constructors and destructors
*/
JobsList::JobsList() {
    jobsList = {};
}

JobsList::JobEntry::JobEntry(Command *_cmd, bool _isStopped, int _jobId, pid_t _pid) : cmd(_cmd), isStopped(_isStopped), jobId(_jobId), jobPid(_pid) {
    startTime = time(0);
}

JobsList::~JobsList() {

}

/**
* getters and setters
*/
int JobsList::JobEntry::getJobId() {
    return jobId;
}

//void JobsList::JobEntry::setJobId(int _jobId) {
//    jobId = _jobId;
//}
/**
 * other class methods
 */
void JobsList::addJob(Command* cmd, pid_t pid, bool isStopped) {
    int jobId = getNextJobID(); // first jobId should be 1 !!
    JobEntry* entry = new JobEntry(cmd, isStopped, jobId, pid);
    jobsList.push_front(entry);
}

int JobsList::getNextJobID() {
    int maxId = -1;
    for(const auto& entry : jobsList) {
        maxId = entry->getJobId() > maxId ? entry->getJobId() : maxId;
    }
    return maxId + 1;
}

pid_t JobsList::getJobPid(int jobId) {
    // find pid with given jobId
    return 0;
}

bool JobsList::isEmpty() {
    return jobsList.empty();
}

bool JobsList::isIn(int jobId) {
    return false;
}