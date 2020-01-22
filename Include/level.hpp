#include "blockattrs.hpp"
#include <string>
#include <map>
#include <vector>
namespace GD {
	typedef struct LevelOptions {
	std::string description="";
	int stars=0;
	bool unlisted=false;
	int levelpassword=0;
	int songid=0;
	int audiotrack=1;
	bool twoPlayer=false;
	int coins=0;
	} LevelOptions;
	class Block {
	private:
		std::map<std::string,std::string> attributes;
	public:
		Block();
		Block(int blockID,int xPosition,int yPosition);
		Block(std::string blockID,std::string xPosition,std::string yPosition);
		std::string& operator [] (std::string attribute);
		operator std::string();
		Block& operator= (const std::string& str);
		Block& operator= (const std::map<std::string,std::string>& attrs);
		Block& operator += (const std::map<std::string,std::string>& attrs);

	};
	class Header {
	private:
		std::map<std::string,std::string> attributes;
	public:
		Header();
		std::string& operator [] (std::string attribute);
		operator std::string();
		Header& operator = (const std::map<std::string,std::string>& attrs);
		Header& operator= (const std::string& str);
		Header& operator += (const std::map<std::string,std::string>& attrs);
	};
	class Level {
	public:
		Header header;
		std::vector<Block> blocks;
		std::string levelName;
		void addBlock(Block b);
		Level(std::string levelName="GDLevel");
		int uploadLevel(std::string username,std::string password,LevelOptions options);
		int uploadLevel(std::string username,std::string password);
		operator std::string();
		Level& operator= (const std::string& str);
		static Level& downloadLevel(int levelID);
		static Level& downloadLevel(std::string levelID);
	};
}