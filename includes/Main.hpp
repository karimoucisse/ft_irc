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
#include <sstream>

#include "Client.hpp"
#include "Channel.hpp"
#include "Server.hpp"

#define INVALID_SOCKET -1
#define MAX_EVENTS 10
#define MAX_CLIENTS 10
