// Charles Baxter
// PhotoMagic.cpp for PS1b
// 9/22/2021

#include <iostream>
#include <fstream>
#include <string>

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "FibLFSR.h"

int photoMagic(char* argv[]);
int passwordMagic(char* argv[]);

void transform(sf::Image& im, FibLFSR* f);
void transform(std::string& str, FibLFSR* f);

int main(int argc, char* argv[]) {
	if (argc != 3)
	{
		std::cerr << "You have provided " << argc - 1 << " arguments, you must provide 2!\n";
		return EXIT_FAILURE;
	}

	if (std::string(argv[1]).find(".png") != std::string::npos
		&& std::string(argv[2]).find(".png") != std::string::npos)
		return photoMagic(argv);

	else if (std::string(argv[1]).find(".txt") != std::string::npos
		&& std::string(argv[2]).find(".txt") != std::string::npos)
		return passwordMagic(argv);

	else
		std::cerr << "No files with those names could be found\n";

	return 0;
}

int photoMagic(char* argv[]) {
	sf::Image source;
	if (!source.loadFromFile(argv[1]))
		return EXIT_FAILURE;

	// creates two windoes that are the size of the image
	sf::Vector2u size = source.getSize();
	sf::RenderWindow sourceWin(sf::VideoMode(size.x, size.y), "Source(Original)");
	sf::RenderWindow destWin(sf::VideoMode(size.x, size.y), "Destination(Encrypted)");

	sf::Texture sourceTexture;
	sourceTexture.loadFromImage(source);
	sf::Sprite sourceSprite(sourceTexture);
	sf::Image dest(source);

	FibLFSR f1(argv[2]);

	transform(dest, &f1);

	sf::Texture destTexture;
	destTexture.loadFromImage(dest);
	sf::Sprite destSprite(destTexture);

	while (sourceWin.isOpen() && destWin.isOpen()) {
		sf::Event event;
		while (sourceWin.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				sourceWin.close();
		}
		while (destWin.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				destWin.close();
		}

		sourceWin.clear();
		sourceWin.draw(sourceSprite);
		sourceWin.display();
		destWin.clear();
		destWin.draw(destSprite);
		destWin.display();
	}

	if (!dest.saveToFile(argv[2]))
		return EXIT_FAILURE;

	return 0;
}

// read a password from a file, then paste the ecrypted version in the output file
int passwordMagic(char* argv[]) {
	std::ifstream inFile;
	inFile.open(argv[1]);
	if (!inFile.is_open()) {
		std::cout << argv[1] << " could not be opened" << std::endl;
		return 1;
	}

	std::ofstream oFile(argv[2]);
	std::string outputStr;
	std::getline(inFile, outputStr);
	std::cout << "input : '" << outputStr << "'" << std::endl;
	inFile.close();
	FibLFSR f1(argv[3]);
	transform(outputStr, &f1);
	oFile << outputStr;
	std::cout << "output: '" << outputStr << "'" << std::endl;
	return 0;
}

void transform(sf::Image& im, FibLFSR* f) {
	sf::Vector2u size = im.getSize();

	sf::Color pixel;
	// for each pixel in the destination image
	for (uint32_t i = 0; i < size.x; i++) {
		for (uint32_t j = 0; j < size.y; j++) {
			pixel = im.getPixel(i, j);
			// each color channel is 8 bits, so 8 bits will be needed
			// xor the output of the generate function and the original value,
			// then store in the original value
			// this allows for the same function to be used to decrypt and encrypt
			pixel.r ^= f->generate(8);
			pixel.g ^= f->generate(8);
			pixel.b ^= f->generate(8);
			im.setPixel(i, j, pixel);
		}
	}
}

void transform(std::string& str, FibLFSR* f) {
	// a character is 1byte/8bits so an 8bit number is needed for encryption
	// xor each character with the output of generate(8), store the outcome
	// in the original spot for the character
	for (size_t i = 0; i < str.size(); i++) {
		str[i] ^= f->generate(8);
	}
}