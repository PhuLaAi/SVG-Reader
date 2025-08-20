#ifndef FactoryFigure_H
#define FactoryFigure_H

class factoryfigure {
private:
	unordered_map<string, int> figureId;

public:
	factoryfigure();
	figure* getFigure(string name);
	unordered_map<string, int> getFigureId();
};

#endif

