#pragma once
#ifndef FIGUREMODEL_H
#define FIGUREMODEL_H

struct FigureModel {
	const int id;
	const int cantitie;
	const double probability;
	int model[4][5][5];

};

extern FigureModel arrow;
extern FigureModel recipe;
extern FigureModel cube;
extern FigureModel scalRigth;
extern FigureModel scalLeft;
extern FigureModel line;
extern FigureModel LRigth;
extern FigureModel LLeft;
extern FigureModel spiral;
extern FigureModel hiperLine;
extern FigureModel soloCube;


extern FigureModel figureModels[];

extern int cantFig;

#endif
