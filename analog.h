/*************************************************************************
                           analog  -  description
                             -------------------
    d�but                : ${date}
    copyright            : (C) ${year} par ${user}
*************************************************************************/

//---------- Interface de la classe analog (fichier ${analog.h}) ------
#if ! defined ( ANALOG_H )
#define Analog_H

//--------------------------------------------------- Interfaces utilis�es
#include <unordered_map>
#include <string>
#include <fstream>

//using namespace std;
//------------------------------------------------------------- Constantes 

//------------------------------------------------------------------ Types 
typedef std::unordered_map<std::string, int> ReferersMap;
typedef std::unordered_map<std::string,  ReferersMap> Catalogue;

//---------------------------------------------------------------Fonctions

	void printUsages();

	void genererCatalogue(bool option [], int heure = -1);

	void genererGraphe(std::string dotFileName);

  	void ajouterRessource(std::string res, std::string ref);
//---------------------------------------------------------------Attributs

#endif // ANALOG_H