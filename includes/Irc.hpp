#pragma once

#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <thread>
#include <vector>
#include <sys/types.h>
#include <fcntl.h>
#include <poll.h>
#include <csignal>
#include <sstream>

#define RED "\e[1;31m"
#define GREEN "\e[1;32m"
#define RESET "\e[0m"
#define MAX_EVENTS 64

#include "Client.hpp"
#include "Channel.hpp"
#include "Server.hpp"
#include "Cmds.hpp"
#include "Utils.hpp"

