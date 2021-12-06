#include "recipe.hpp"

using namespace std;

#ifndef __FILTER__
#define __FILTER__

class Filter {
	public:
	enum Type {tag, vegetarian, time, rate};
	Filter(int userId);
	virtual vector<Recipe*> applyFilter(const vector<Recipe*>& recipes) = 0;
	Filter::Type getType();
	int getId();
	protected:
	Filter::Type type;
	int userId;
};

class FilterTag : public Filter {
	public:
	FilterTag(string newTag, int userId);
	vector<Recipe*> applyFilter(const vector<Recipe*>& recipes);
	private:
	string tag;
};

class FilterVegterian : public Filter {
	public:
	FilterVegterian(int userId);
	vector<Recipe*> applyFilter(const vector<Recipe*>& recipes);
};

class FilterTime : public Filter {
	public:
	FilterTime(int newMinTime, int newMaxTime, int userId);
	vector<Recipe*> applyFilter(const vector<Recipe*>& recipes);
	private:
	int minTime;
	int maxTime;
};

class FilterRate : public Filter {
	public:
	FilterRate(int newMinRate, int newMaxRate, int userId);
	vector<Recipe*> applyFilter(const vector<Recipe*>& recipes);
	private:
	int minRate;
	int maxRate;
};

#endif