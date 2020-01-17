//#include "../../util/header.h"
//#include "../../util/log.h"
//#include "../../main/client.h"
//#include "../../main/server.h"

class Task
{
public:
    std::string title;

    Task(std::string name){
        title = name;
    }

    std::string& refTitle() {
    	return title;
    }

};
