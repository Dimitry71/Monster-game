#include <string>
#include <iostream>
using namespace std;

int getRandomNumber(int min, int max)
{
	static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0);
	return static_cast<int>(rand() * fraction * (max - min + 1) + min);
}
class Creature
{
protected:
	string m_name;
	char m_symbol;
	int m_health;
	int m_damage;
	int m_gold;
public:

	Creature(std::string name, char symbol, int health, int damage, int gold) :
		m_name(name), m_symbol(symbol), m_health(health), m_damage(damage), m_gold(gold)
	{
	}
	const std::string& getName() { return m_name; }
	char getSymbol() { return m_symbol; }
	int getHealth() { return m_health; }
	int getDamage() { return m_damage; }
	int getGold() { return m_gold; }

	void reduceHealth(int takedamage)
	{
		m_health -= takedamage;
	}
	bool isDead()
	{
		if (m_health <= 0) return true;
		else return false;
	}

	void addGold(int takegold)
	{
		m_gold += takegold;
	}
};


class Player : public Creature
{
	int m_level = 1;
public:
	Player(string name)
		: Creature(name, '@', 10, 1, 0)
	{
	}
	int getLevel() { return m_level; }
	void LevelUp() 
	{ 
		m_level++;
		m_damage++;
	}
	bool hasWon() { return m_level == 20; }
};


class Monster : public Creature
{
public:
	enum Type
	{
		DRAGON,
		ORC,
		SLIME,
		MAX_TYPES
	};
	struct MonsterData
	{
		const char* name;
		char symbol;
		int health;
		int damage;
		int gold;
	};
	static MonsterData monsterData[MAX_TYPES];

	Monster(Type type)
		: Creature(monsterData[type].name, monsterData[type].symbol, monsterData[type].health, monsterData[type].damage, monsterData[type].gold)
	{
	}

	static Monster getRandomMonster()
	{
		int num = getRandomNumber(0, MAX_TYPES - 1);
		return Monster(static_cast<Type>(num));
	}
};

Monster::MonsterData Monster::monsterData[Monster::MAX_TYPES]
{
	{ "dragon", 'D', 20, 4, 100 },
	{ "orc", 'o', 4, 2, 25 },
	{ "slime", 's', 1, 1, 10 }
};

void AttackPlayer(Player &p , Monster &m)
{
	if (p.isDead()) return;
	cout << "You hit the " << m.getName() << " for " << p.getDamage() << " damage.\n";
	m.reduceHealth(p.getDamage());
	if (m.isDead())
	{
		cout << "You killed " << m.getName() << endl;
		p.LevelUp();
		std::cout << "You are now level " << p.getLevel() << ".\n";
		std::cout << "You found " << m.getGold() << " gold.\n";
		p.addGold(m.getGold());
	}
}

void AttackMonster(Monster &m, Player& p)
{
	if (m.isDead()) return;
	cout << m.getName() << "Hit you for " << m.getDamage() << " damage.\n";
	p.reduceHealth(m.getDamage());
}

void FightMonster(Player& p)
{
	Monster m = Monster::getRandomMonster();
	std::cout << "You have encountered a " << m.getName() << " (" << m.getSymbol() << ").\n";
	while (!m.isDead() && !p.isDead())
	{
		std::cout << "(R)un or (F)ight: ";
		char input;
		std::cin >> input;
		if (input == 'R' || input == 'r')
		{
			if (getRandomNumber(1, 2) == 1)
			{
				cout << "You successfully fled.\n ";
				return;
			}
			else
			{
				std::cout << "You failed to flee.\n";
				AttackMonster(m, p);
				continue;
			}
		}
		if (input == 'F' || input == 'f')
		{
			AttackPlayer(p, m);
			AttackMonster(m, p);
		}
	}
}

int main()
{
	srand(static_cast<unsigned int>(time(0))); // устанавливаем значение системных часов в качестве стартового числа
	rand();
	std::cout << "Enter your name: ";
	std::string playerName;
	std::cin >> playerName;

	Player p(playerName);
	std::cout << "Welcome, " << p.getName() << '\n';
	while (!p.isDead() && !p.hasWon())
		FightMonster(p);

	if (p.isDead())
	{
		cout << "You died at level " << p.getLevel() << " and with " << p.getGold() << " gold.\n";
		cout << "Too bad you can't take it with you!\n";
	}
	else 
		cout << "You won the game with " << p.getGold() << " gold!\n";
	return 0;
}