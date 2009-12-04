#include "Torrent.hpp"
#include "bencode.h"
#include <ifstream>

Torrent::Torrent(std::string file) : filename(file) {}

Torrent::init() {
    std::ifstream in_file(filename);
    std::string data;
    char current;

    //Read all data from file;
    while (input_file.good()) {
        current = input_file.get();

        if (input_file.good())
            data.push_back(current);
    }

    // Find info dictionary
    size_t offset = data.find("4:info") + 6;
    std::string info_dict = data.substr(offset);

    //Get end of info dictionary
    size_t announce_off = data.find("8:announce");
    if (announce_off == std::string::npos) {
        continue; // Invalid torrent file
    }
    if (announce_off > offset) {
        this->announce_url = info_dict.substr(0, announce_off);
    }

    //Find the first if any field after the info dictionary
//     if(info_dict.find("8:url-list") != std::string::npos) {
//         info_dict = info_dict.substr(0, info_dict.find("8:url-list"));
//     }
    if(info_dict.find("13:creation date") != std::string::npos) {
        info_dict = info_dict.substr(0, info_dict.find("13:creation date"));
    }
    if(info_dict.find("7:comment") != std::string::npos) {
        info_dict = info_dict.substr(0, info_dict.find("7:comment"));
    }
    if(info_dict.find("10:created by") != std::string::npos) {
        info_dict = info_dict.substr(0, info_dict.find("10:created by"));
    }
    if(info_dict.find("8:encoding") != std::string::npos) {
        info_dict = info_dict.substr(0, info_dict.find("8:encoding"));
    }

    // etc, etc
    //Get the info hash
    const unsigned char* raw_info_dict = (const unsigned char*) info_dict.data();
    unsigned char raw_info_hash[20];
    SHA1(raw_info_dict, info_dict.size(), raw_info_hash);
    std::string info_hash = std::string((const char*) raw_info_hash, 20);

    //Get length of pieces
    size_t pieces_off = info_dict.find("6:pieces") + 8;
    size_t endoff = info_dict.find(":", pieces_off);
    size_t pieces_len = (int) atoi(info_dict.substr(pieces_off, endoff-pieces_off).c_str());

    //Get the pieces string
    std::string pieces = info_dict.substr(endoff+1, pieces_len);

    //Get the piece hashes themselves
    size_t num_pieces = pieces_len / 20;
    for(size_t i = 0; i < num_pieces; i++) {
        //Add to map
        hashes[info_hash].push_back(pieces.substr(i*20, 20));
    }

    // Get announce_url
    
    in_file.close();
}
