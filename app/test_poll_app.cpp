#include "test_poll_app.h"
#include <cassert>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <memory>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
PollStruct::PollStruct(int poll_nums, int timeout)
    : poll_nums(poll_nums)
    , timeout(timeout)
    , poll_fds(poll_nums)

{
    if (poll_nums <= 0) {
        assert(poll_nums > 0 && "poll_nums must be greater than 0");
    }
    poll_fds.resize(poll_nums);
}

short int PollStruct::get_revents(int which)
{
    return this->poll_fds[which].revents;
}

short int PollStruct::get_events(int which)
{
    return this->poll_fds[which].events;
}

void PollStruct::set_poll_info(int set_num, int fd, int event)
{
    if (set_num < 0 || set_num >= poll_nums) {
        std::cerr << "poll_nums must be greater than 0" << std::endl;
    }
    if (fd < 0) {
        std::cerr << "poll_nums must be greater than 0" << std::endl;
    }

    poll_fds[set_num].fd = fd;
    poll_fds[set_num].events = event;
}

int PollStruct::poll_func()
{
    for (auto it = poll_fds.begin(); it != poll_fds.end(); ++it) {
        if (it->fd < 0) {
            std::cerr << "poll_nums must be greater than 0" << std::endl;
        }
    }
    for (auto& polls : poll_fds) {
        polls.revents = 0;
    }
    int ret = poll(poll_fds.data(), poll_nums, timeout);
    if (ret == -1) {
        std::cerr << "poll_nums must be greater than 0" << std::endl;
    }
    return ret;
}

int PollStruct::set_to_signal_func(int fd)
{
    fcntl(fd, F_SETOWN, getpid());
    int flags = fcntl(fd, F_GETFL);
    fcntl(fd, F_SETFL, flags | FASYNC);
    return 0;
}
