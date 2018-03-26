//
// Created by mlacaud on 04/03/18.
//

#ifndef SRDSSERVER_TRACKERDB_H
#define SRDSSERVER_TRACKERDB_H
#include <mutex>
#include <map>
#include <list>
#include <string>

class TrackerDB {
public:
    typedef std::mutex Mutex;
    TrackerDB();
    ~TrackerDB();

    void createKey(std::string videoId);

    void addAddrByVideoId(std::string videoId);

    void getPeersForId(std::string videoId);

    void deletePeerForId(std::string videoId, std::string peer);

private:
    std::map<std::string,std::list<std::string>> database;

};


#endif //SRDSSERVER_TRACKERDB_H
