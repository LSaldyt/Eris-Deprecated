#include "Window.hpp"
#include "ShipSpecs.hpp"
#include "Includes.hpp"
#include "Player.hpp"
#include "ProjectileSpecs.hpp"
#include "Chunk.hpp"
#include "Explosion.hpp"
#include "Wave.hpp"

void Window::loadPlanetTexture(std::string textureLocation)
{
	planetTextures.push_back(loadTexture(textureLocation));
	planetTextures.back()->setSmooth(true);
}

void Window::loadAsteroidTexture(std::string textureLocation)
{
	asteroidTextures.push_back(loadTexture(textureLocation));
}

void Window::loadFragmentTexture(std::string textureLocation)
{
	fragTextures.push_back(loadTexture(textureLocation));
}

void Window::loadStationTexture(std::string textureLocation)
{
	stationTextures.push_back(loadTexture(textureLocation));
	stationTextures.back()->setSmooth(true);
}

void Window::loadTextureSquare(std::string textureLocation, sf::IntRect rect, std::vector<sf::Texture*>* storeLocation)
{
	storeLocation->push_back(new sf::Texture);
	//Allocate and load a new texture into memory

	if (!storeLocation->back()->loadFromFile(textureLocation, rect))
	{
		std::cout << "Error Loading Texture" << std::endl;
	}
	storeLocation->back()->setSmooth(smoothTextures);
}

void Window::loadMultiTexture(std::string location, std::vector<sf::Texture*>* storeLocation, int size = 64, int rows = 1, int columns = 1)
{
	for (int i = 0; i < columns; i++)
	{
		for (int j = 0; j < rows; j++)
		{
			loadTextureSquare(location, sf::IntRect(j*size, i*size, size, size), storeLocation);
		}
	}
}

sf::Texture* Window::loadTexture(std::string textureLocation)
{
	//This is to keep track of our textures, and ensure they are freed when we delete the gameboard
	textureID++;

	textures.insert(std::pair<std::string, sf::Texture*>(std::to_string(textureID), new sf::Texture));
	//Allocate and load a new texture into memory

	if (!textures[std::to_string(textureID)]->loadFromFile(textureLocation))
	{
		std::cout << "Error Loading Texture" << std::endl;
	}

	textures[std::to_string(textureID)]->setSmooth(smoothTextures);

	return textures[std::to_string(textureID)];
}

void Window::loadBackground(std::string location, sf::Vector2f scale = sf::Vector2f(.5f,.5f))
{
	background.setTexture(*loadTexture(location));
	background.setScale(scale);
}

void Window::loadStart(std::string location, sf::Vector2f scale = sf::Vector2f(.5f, .5f))
{
	startScreen.setTexture(*loadTexture(location));
	startScreen.setScale(scale);
}

void Window::loadFuel(std::string location, sf::Vector2f size = sf::Vector2f(50, 100))
{
	fuel.setTexture(loadTexture(location));
	fuel.setSize(size);
	fuel.setPosition(0, 30);
	player.syncFuel(&fuel);
}

void Window::loadHealth(std::string location, sf::Vector2f size = sf::Vector2f(50, 100))
{
	health.setTexture(loadTexture(location));
	health.setSize(size);
	health.setPosition(fuel.getPosition().x + fuel.getSize().x, 30);
	player.syncHealth(&health);
}

void Window::loadGUI(std::string location, sf::Vector2f scale = sf::Vector2f(.5f, .5f))
{
	guiBox = *loadTexture(location);
}

void Window::loadAmmo(std::string location, sf::Vector2f size = sf::Vector2f(50, 100))
{
	ammo.setTexture(loadTexture(location));
	ammo.setSize(size);
	ammo.setPosition(0, 330);
	player.syncAmmo(&ammo);
}

void Window::loadInfo(std::string location, sf::Vector2f size = sf::Vector2f(50, 100))
{
	info.setTexture(loadTexture(location));
	info.setSize(size);
	info.setPosition(window.getSize().x - info.getGlobalBounds().width, 0);
}

void Window::loadHide(std::string location, sf::Vector2f scale = sf::Vector2f(1, 1))
{
	hideButton.setTexture(*loadTexture(location));
	hideButton.setScale(scale);
	hideButton.setPosition(window.getSize().x - hideButton.getGlobalBounds().width, info.getSize().y);
}

void Window::loadShow(std::string location, sf::Vector2f scale = sf::Vector2f(1, 1))
{
	showButton.setTexture(*loadTexture(location));
	showButton.setScale(scale);
	showButton.setPosition(window.getSize().x - hideButton.getGlobalBounds().width, 0);
}

void Window::loadInv(std::string location, sf::Vector2f size)
{
	inventory.setTexture(loadTexture(location));
	inventory.setSize(size);
	inventory.setPosition(window.getSize().x / 2 - inventory.getLocalBounds().width / 2, 0);
}


void Window::loadShipSpecs(std::string filename)
{
	std::ifstream indata;

	indata.open(filename);

	std::string name;
	float shootDelayModifier;
	float engineForce;
	float mass;
	float maxVelocity;
	float maxAngularVelocity;
	float structuralIntegrity;
	float repair;
	std::string texture;
	std::vector<std::string> missleTypes;
	int cost;

	std::string                line;
	std::getline(indata, line);
	while (getline(indata, line)) //For every line of csv, gather the data for each ship type and push it to a map
	{
		missleTypes.clear();

		std::stringstream          lineStream(line);

		//This would be better with some operator overloading, but for now....?

		std::string                cell;
		std::getline(lineStream, name, ',');

		std::getline(lineStream, cell, ',');
		shootDelayModifier = std::stof(cell);

		std::getline(lineStream, cell, ',');
		engineForce = std::stof(cell);

		std::getline(lineStream, cell, ',');
		mass = std::stof(cell);

		std::getline(lineStream, cell, ',');
		maxVelocity = std::stof(cell);

		std::getline(lineStream, cell, ',');
		maxAngularVelocity = std::stof(cell);

		std::getline(lineStream, cell, ',');
		structuralIntegrity = std::stof(cell);

		std::getline(lineStream, cell, ',');
		repair = std::stof(cell);

		std::getline(lineStream, texture, ',');

		while (std::getline(lineStream, cell, ','))
		{
			if (cell != "none")
			{
				missleTypes.push_back(cell);
			}
			else
			{
				break; //Discard the none value
			}
		}

		while (std::getline(lineStream, cell, ','))
		{
			if (cell != "none")
			{
				cost = std::stoi(cell);
			}
			else if (cell == "")
			{
				break;
			}
		}


		spec_Keys.push_back(name);
		specs.insert(
			std::pair<std::string, ShipSpecs*>
			(
				name,
				new ShipSpecs
		{
			name,
			shootDelayModifier,
			engineForce,
			mass,
			maxVelocity,
			maxAngularVelocity,
			structuralIntegrity,
			repair,
			texture,
			missleTypes, 
			cost
		}
				)
			);

		std::unordered_map<std::string, sf::Texture*>::const_iterator got = textures.find(texture);
		if (got == textures.end())
		{
			textures.insert(std::pair<std::string, sf::Texture*>(texture, new sf::Texture()));
			if (!textures[texture]->loadFromFile(texture))
			{
				std::cout << "Error Loading Texture" << std::endl;
			}
		}
		else
		{
			std::cout << "Already Loaded: " << texture << std::endl;
		}

		std::cout << name << ": " << std::endl
			<< "Shoot Delay: " << shootDelayModifier << std::endl
			<< "Engine Force: " << engineForce << std::endl
			<< "Mass: " << mass << std::endl
			<< "Maximum Velocity: " << maxVelocity << std::endl
			<< "Maximum Angular Velocity: " << maxAngularVelocity << std::endl
			<< "Structure: " << structuralIntegrity << std::endl
			<< "Repair Rate: " << repair << std::endl
			<< "Cost" << cost << std::endl;

	}
	std::cout << "\n\n Type Loading Complete.. \n\n" << std::endl;
}

void Window::loadProjectileSpecs(std::string filename)
{
	std::ifstream indata;

	indata.open(filename);

	std::string name;
	int lifetime;
	float hitRadius;
	float damage;
	float maxVelocity;
	float acceleration;
	std::string texture;
	int baseRate;

	std::string                line;
	std::getline(indata, line);
	while (getline(indata, line))
	{
		std::stringstream          lineStream(line);

		//This would be better with some operator overloading, but for now....?

		std::string                cell;
		std::getline(lineStream, name, ',');

		std::getline(lineStream, cell, ',');
		lifetime = std::stoi(cell);

		std::getline(lineStream, cell, ',');
		hitRadius = std::stof(cell);

		std::getline(lineStream, cell, ',');
		damage = std::stof(cell);

		std::getline(lineStream, cell, ',');
		maxVelocity = std::stof(cell);

		std::getline(lineStream, cell, ',');
		acceleration = std::stof(cell);

		std::getline(lineStream, texture, ',');

		std::getline(lineStream, cell, ',');
		baseRate = std::stoi(cell);

		p_specs.insert(
			std::pair<std::string, ProjectileSpecs*>
			(
				name,
				new ProjectileSpecs
		{
			lifetime,
			hitRadius,
				damage,
				maxVelocity,
				acceleration,
				texture,
				baseRate
		}
				)
			);

		std::unordered_map<std::string, sf::Texture*>::const_iterator got = textures.find(texture);
		if (got == textures.end())
		{
			textures.insert(std::pair<std::string, sf::Texture*>(texture, new sf::Texture()));
			if (!textures[texture]->loadFromFile(texture))
			{
				std::cout << "Error Loading Texture" << std::endl;
			}
		}
		else
		{
			std::cout << "Already Loaded: " << texture << std::endl;
		}
		std::cout << "Loaded " << name << std::endl;

	}
	std::cout << "\n\n Type Loading Complete.. \n\n" << std::endl;
}

void Window::loadWaves(std::string filename)
{
	std::ifstream indata;

	indata.open(filename);

	int dispersion;
	std::string type;

	std::string                line;
	std::getline(indata, line); //Skip first line
	while (getline(indata, line))
	{
		Wave currentwave;
		std::vector<std::string> units;

		std::stringstream          lineStream(line);
		std::string                cell;

		std::getline(lineStream, cell, ',');
		dispersion = std::stoi(cell);

		while (std::getline(lineStream, type, ','))
		{
			if (type != "")
			{
				units.push_back(type);
			}
			else
			{
				break;

			}
		}

		waves.push_back(Wave{ dispersion, units });
	}
	std::cout << "\n\n Type Loading Complete.. \n\n" << std::endl;
}


void Window::loadPlayer(sf::Vector2f position, sf::Vector2f scale, std::string name = "Frigate")
{
	player.loadType(*specs[name], this);
	std::unordered_map<std::string, sf::Texture*>::const_iterator got = textures.find(specs[name]->texture);
	if (got == textures.end())
	{
		std::cout << "ALERT: " << specs[name]->texture << "Not found... " << std::endl;
	}
	else
	{
		player.setTexture(*textures[specs[name]->texture]);
	}
	player.setPosition(position);
	player.scale(scale);
	player.setOrigin(player.getLocalBounds().width / 2, player.getLocalBounds().height / 2);
}
