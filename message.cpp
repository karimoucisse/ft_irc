
void irc_cmd_join(SkllEvent *event, void *user_data)
{
    IRCServer *server = (IRCServer *)user_data;
    IRCClient *client = (IRCClient *)event->client->userdata;
    if (!client)
        return;
    if (!client->is_registered())
        return (client->send(irc_err_notregistered(client->nickname)));
    std::string chan_name = event->message->to<std::string>();
    std::string key;
    if (event->message->has_more())
        key = event->message->to<std::string>();
    if (!chan_name.empty() && chan_name[0] == '#')
        chan_name = chan_name.substr(1);
    if (client->is_in_channel(chan_name))
        return;
    IRCChannel *channel = server->create_channel(chan_name);
    if (!channel->can_join(client->get_fd(), key))
    {
        if (channel->is_full())
        {
            client->send(irc_err_channelisfull(client->nickname, chan_name));
        }
        else if (channel->invite_only)
        {
            client->send(irc_err_inviteonlychan(client->nickname, chan_name));
        }
        else
            client->send(irc_err_badchannelkey(client->nickname, chan_name));
        return;
    }
    bool is_first = (channel->get_client_count() == 0);
    if (is_first)
        channel->set_operator(client->get_fd(), true);
    channel->skll_channel->add_client(client->get_fd(), client->skll_client);
    client->join_channel(chan_name);
    std::ostringstream join_msg;
    join_msg << ":" << client->nickname << "!" << client->username << "@" << client->hostname
             << " JOIN :#" << chan_name << "\r\n";
    channel->broadcast(join_msg.str());
    if (!channel->topic.empty())
        client->send(irc_rpl_topic(client->nickname, chan_name, channel->topic));
    client->send(IRCErrors::joined_channel(chan_name));
    if (chan_name == "help")
        client->send(IRCErrors::help_commands());
    std::ostringstream log_msg;
    log_msg << "#" << chan_name << " [" << channel->get_client_count() << " users]";
    if (is_first)
        log_msg << " (creator @)";
    irc_log("JOIN", client, log_msg.str());
}
