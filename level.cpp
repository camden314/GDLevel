#include "Include/level.hpp"
#include "Include/GDCrypto.hpp"
#include "Include/httplib.hpp"
#include <iostream>
#include <sstream>
#include <vector>
#include <iterator>
#include <assert.h>

using namespace GD;
httplib::Client cli("boomlings.com", 80);
std::unordered_map<std::string,std::string> splitToMap(const std::string &s, char delim) {
    std::istringstream iss(s);
    std::string item, item2;
    std::unordered_map<std::string,std::string> out;
    while (std::getline(iss, item, delim) && std::getline(iss, item2, delim)) {
        out.insert(std::make_pair(item,item2));
    }
    return out;
}
std::vector<std::string> split( std::string const& original, char separator )
{
    std::vector<std::string> results;
    std::string::const_iterator start = original.begin();
    std::string::const_iterator end = original.end();
    std::string::const_iterator next = std::find( start, end, separator );
    while ( next != end ) {
        results.push_back( std::string( start, next ) );
        start = next + 1;
        next = std::find( start, end, separator );
    }
    results.push_back( std::string( start, next ) );
    return results;
}
std::string httpRequest(std::string url,std::string params) {
	auto res = cli.Post(url.c_str(), params.c_str(), "application/x-www-form-urlencoded");
	if (res && res->status == 200) {
	        return res->body;
	} else {
		throw std::runtime_error("POST Request Failed");
	}
}
std::string grabAccountID(std::string username) {
	std::string query = "gameVersion=21&binaryVersion=35&gdw=0&str="+username+"&total=0&page=0&secret=Wmfd2893gb7";
	std::string response = httpRequest("/database/getGJUsers20.php",query);
	try {
		return splitToMap(response,':')["16"];
	} catch(const std::exception& e) {
		throw std::runtime_error("Invalid Username");
	}
}
std::string buildUploadQueryString(GD::LevelOptions options,std::string username,std::string password,GD::Level level,std::string levelName) {
	GDCrypto::RobTopEncoder gjp_encoder(GDCrypto::Keys::GJP_KEY);
	GDCrypto::LevelEncoder level_encoder;
	GDCrypto::CheckGenerator seed_encoder(GDCrypto::Keys::LEVEL_KEY,GDCrypto::Salts::LEVEL_SALT);

	std::string encodedLevelString = (level_encoder << std::string(level)).digestAsString();
	std::string levelSeed2 = (seed_encoder << GDCrypto::Utility::levelSeed(encodedLevelString)).digestAsString();
	std::string gjp = (gjp_encoder << password).digestAsString();

	std::string out = "gameVersion=21&binaryVersion=35&levelVersion=1&levelLength=0&gdw=0&levelID=0&auto=0&original=0&wt=13&wt2=0&ldm=0&seed=bruhmoment&extraString=0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0&secret=Wmfd2893gb7&levelInfo=H4sIAAAAAAAAEzOwNrQ20DMwsrY21DMztbYGAJp7I8gQAAAA&accountID="+grabAccountID(username);
	out += "&gjp="+gjp;
	out += "&userName="+username;
	out += "&levelName="+levelName;
	out += "&levelDesc="+options.description;
	out += "&audioTrack="+std::to_string(options.audiotrack);
	out += "&password="+std::to_string(options.levelpassword);
	out += "&twoPlayer="+std::to_string(options.twoPlayer);
	out += "&songID="+std::to_string(options.songid);
	out += "&objects="+std::to_string(level.blocks.size());
	out += "&coins="+std::to_string(options.coins);
	out += "&requestedStars="+std::to_string(options.stars);
	out += "&unlisted="+std::to_string(options.unlisted);
	out += "&seed2="+levelSeed2;
	out += "&levelString="+encodedLevelString;
	return out;

}
std::string& Block::operator [] (std::string attribute) {
	return this->attributes[attribute];
}
Block& Block::operator= (const std::string& str) {
	attributes = splitToMap(str,',');
	return *this;
}
Block& Block::operator= (const std::unordered_map<std::string,std::string>& attrs) {
	this->attributes = attrs;
	return *this;
}
Block& Block::operator += (const std::unordered_map<std::string,std::string>& attrs) {
	for(auto& [key, val] : attrs) {
		this->attributes[key] = val;
	}

	return *this;
}
Block::Block() {
	this->attributes[BlockAttrs::BLOCK_ID] = "0";
	this->attributes[BlockAttrs::X_POSITION] = "0";
	this->attributes[BlockAttrs::Y_POSITION] = "0";
}
Block::Block(int blockID,int xPosition,int yPosition) {
	this->attributes[BlockAttrs::BLOCK_ID] = std::to_string(blockID);
	this->attributes[BlockAttrs::X_POSITION] = std::to_string(xPosition);
	this->attributes[BlockAttrs::Y_POSITION] = std::to_string(yPosition);
}
Block::Block(std::string blockID,std::string xPosition,std::string yPosition) {
	this->attributes[BlockAttrs::BLOCK_ID] = blockID;
	this->attributes[BlockAttrs::X_POSITION] = xPosition;
	this->attributes[BlockAttrs::Y_POSITION] = yPosition;
}

Block::operator std::string() {
	std::string out;
	for(auto& [key, val] : this->attributes) {
		out += key+std::string(",")+val+std::string(",");
	}
	out.pop_back();
	return out;
}

Block::operator std::unordered_map<std::string,std::string>() {
	return this->attributes;
}

std::string& Header::operator [] (std::string attribute) {
	return this->attributes[attribute];
}
Header& Header::operator= (const std::string& str) {
	attributes = splitToMap(str,',');
	return *this;
}
Header& Header::operator= (const std::unordered_map<std::string,std::string>& attrs) {
	this->attributes = attrs;
	return *this;
}
Header& Header::operator += (const std::unordered_map<std::string,std::string>& attrs) {
	for(auto& [key, val] : attrs) {
		this->attributes[key] = val;
	}
	return *this;
}
Header::Header() {
	this->attributes = GD::DefaultHeader;
}
Header::operator std::string() {
	std::string out;
	for(auto& [key, val] : this->attributes) {
		out += key+std::string(",")+val+std::string(",");
	}
	out.pop_back();
	return out;
}


Level::Level(std::string levelName) {
	this->levelName = levelName;
}

void Level::addBlock(Block* b) {
	this->blocks.push_back(b);
}

Level& Level::operator= (const std::string& str) {
	std::vector<std::string> stuff = split(str,';');
	this->header = stuff[0];
	stuff.erase(stuff.begin());
	this->blocks.reserve(stuff.size());
	for(std::string objstr : stuff) {
		if(objstr.size()) {
			Block* blk = new Block();
			*blk = objstr;
			this->blocks.emplace_back(blk);
		}
	}
	return *this;
}

Level::operator std::string() {
	std::string strbuild = this->header;
	strbuild+=";";
	for(Block* blk : this->blocks) {
		strbuild += *blk;
		strbuild += ";";
	}
	return strbuild;

}

int Level::uploadLevel(std::string username,std::string password,LevelOptions options) {
	std::string out = buildUploadQueryString(options,username,password,*this,this->levelName);
	int levelid = std::stoi(httpRequest("/database/uploadGJLevel21.php",out));
	return levelid;
}

int Level::uploadLevel(std::string username,std::string password) {
	LevelOptions options;
	return uploadLevel(username,password,options);
}

Level& Level::downloadLevel(std::string levelID) {
	std::string query = "gameVersion=21&binaryVersion=35&gdw=0&levelID="+levelID+"&inc=0&extras=0&secret=Wmfd2893gb7";
	std::string reqOutput = httpRequest("/database/downloadGJLevel22.php",query);

	std::unordered_map<std::string,std::string> list = splitToMap(reqOutput,':');

	if(reqOutput.size()<5)
		throw std::runtime_error("Invalid Level ID");

	GDCrypto::LevelDecoder level_decoder;
	Level* l = new Level("GDLevel");
	l->levelName = list["2"];
	*l = (level_decoder << list["4"]).digestAsString();

	return *l;
}
Level& Level::downloadLevel(int levelID) {
	return Level::downloadLevel(std::to_string(levelID));
}

LevelOptions Misc::optionsFromLevel(std::string lvlid) {
	std::string query = "gameVersion=21&binaryVersion=35&gdw=0&levelID="+lvlid+"&inc=0&extras=0&secret=Wmfd2893gb7";
	std::string reqOutput = httpRequest("/database/downloadGJLevel22.php",query);

	if(reqOutput.size()<5)
		throw std::runtime_error("Invalid Level ID");

	std::unordered_map<std::string,std::string> resp = splitToMap(reqOutput,':');

	LevelOptions opts;
	opts.description = resp["3"];
	opts.stars = std::stoi(resp["39"]);
	opts.levelpassword = std::stoi(resp["27"]);
	opts.songid = std::stoi(resp["35"]);
	opts.audiotrack = std::stoi(resp["12"]);

	return opts;
}
LevelOptions Misc::optionsFromLevel(int lvlid) {
	return optionsFromLevel(std::to_string(lvlid));
}