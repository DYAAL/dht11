#ifndef TEST_POLL_APP_H
#define TEST_POLL_APP_H

#include <poll.h>

#include <vector>

class PollStruct {
public:
    PollStruct() = default;
    PollStruct(int poll_nums, int timeout);
    ~PollStruct() = default;
    short int get_revents(int which);
    short int get_events(int which);
    void set_poll_info(int set_num, int fd, int event);
    int poll_func();
    int set_to_signal_func(int fd);
    int set_driver_poll(int which_fd);

protected:
    int poll_nums;
    int timeout;
    std::vector<struct pollfd> poll_fds;
};

#endif // TEST_POLL_APP_H