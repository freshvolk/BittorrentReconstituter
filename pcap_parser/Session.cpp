/**
 * Implementation of the Session data type.
 *
 * Original Authors: Aaron A. Lovato and Thomas Coppi
 */

#include <vector>
#include <string>
#include <map>
#include "Session.hpp"

Session::Session() {}

Session::Session(std::string host_ip, u_short port, std::string tracker_ip, std::string info_hash) {

    //Set completed flag
    completed = false;
    
    //Set the host ip and info hash
    host = std::string(host_ip);
    host_port = port;
    this->info_hash = std::string(info_hash);

    //Add the tracker to the list of trackers
    trackers.push_back(tracker_ip);
}

void Session::setCompleted(bool comp) {
    completed = comp;
}
std::string Session::getHost() {
    return std::string(host);
}

std::string Session::getHash() {
    return std::string(info_hash);
}

void Session::addTracker(std::string tracker_ip) {
    //Check to make sure this tracker isn't already in the list
    if(not hasTracker(tracker_ip)) {
        //Add tracker to list
        trackers.push_back(tracker_ip);
    }
}

bool Session::hasTracker(std::string tracker_ip) {
    //Find out if the given IP address is a known tracker
    std::vector<std::string>::iterator it;

    for ( it=trackers.begin(); it < trackers.end(); it++ ) {
        if(*it == tracker_ip) {
            //This tracker is already in the list
            return true;
        }
    }
    return false;
}

void Session::addPeer(std::string peer_ip, u_short peer_port) {
    //Make sure this peer isn't already in the list
    if(not hasPeer(peer_ip, peer_port)) {
        //Add the peer
        Peer new_peer;
        new_peer.ip = peer_ip;
        new_peer.port = peer_port;
        new_peer.active = false;
        peers[peer_ip] = new_peer;
    }
}

/**
 * Returns whether the given peer and port combination is part of this session
 */
bool Session::hasPeer(std::string peer_ip, u_short peer_port) {
    //Find peer by IP
    std::map<std::string, Peer>::iterator it = peers.find(peer_ip);
    if(it != peers.end()) {
        //Check peer port
        if((*it).second.port == peer_port) {
            return true;
        }
    }
    return false;
}

/**
 * Returns the peer with this port and ip combination
 */
Peer *Session::getPeer(std::string peer_ip, u_short peer_port) {
    //Find peer by IP
    std::map<std::string, Peer>::iterator it = peers.find(peer_ip);
    if(it != peers.end()) {
        //Check peer port
        if((*it).second.port == peer_port) {
            return &((*it).second);
        }
    }
    return NULL;
}

void Session::activatePeer(std::string peer_ip) {
    //Find peer in list
    std::map<std::string, Peer>::iterator it = peers.find(peer_ip);
    if(it != peers.end()) {
        //Activate the peer
        (*it).second.active = true;
    }
}

Piece * Session::getLastPiece() {
    if(pieces.size() != 0) {
        return pieces.back();
    }
    return NULL;
}

void Session::addPiece(Piece *newPiece) {
    pieces.push_back(newPiece);
}

u_short Session::getHostPort() {
    return host_port;
}

// vim: tabstop=4:expandtab
