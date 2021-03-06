
/*************************************************************************
                           $analog  -  description
                             -------------------
    début                : ${date}
    copyright            : (C) ${year} par ${user}
*************************************************************************/

//---------- Réalisation de la classe analog (fichier analog.cpp) --

//---------------------------------------------------------------- INCLUDE

//-------------------------------------------------------- Include système
#include <iostream>
#include <map>
#include <string.h>

//------------------------------------------------------ Include personnel
#include "analog.h"

//----------------------------------------------------- Variables portée fichier

using namespace std;

string logFileName;
Catalogue ressources;

//----------------------------------------------------- Fonctions

int main(int argc, char const *argv[])
{
	//gestion de l'appel au programme et du passage des arguments en ligne de commande
	bool option [3];
	option [0] = false;
	option [1] = false;
	option [2] = false;
	int heure = -1;
	string dotOutputFile;
	
	if(argc > 1 && argc < 8)
	{
		for (int i = 1; i < argc; ++i)
		{
			//option : -e
			if(strcmp(argv[i], "-e") == 0)
			{
				option [0] = true;
			//option : -t heure
			}else if(strcmp(argv[i], "-t") == 0)
			{
				if(i+1 != argc)
				{
					try
					{
						heure = stoi(argv[i+1]);
						if(heure >= 24)
						{
							cerr << "-main- Heure incorrecte !" << endl;
							printUsages();
						}
					}catch (const std::invalid_argument& ia) {
	  					cerr << "-main- Invalid argument: " << ia.what() << '\n';
	  					printUsages();
  					}
				}else
				{
					cerr << "-main- Heure manquante" << endl;
					printUsages();
				}
				option [1] = true;
				i++;
			//option : -g
			}else if(strcmp(argv[i], "-g") == 0)
			{
				if(i+1 != argc)
				{
					dotOutputFile = argv[i+1];
				}else
				{
					cerr << "-main- Nom du fichier .dot non renseigné" << endl;
					printUsages();
				}
				option[2] = true;
				i++;
			//nom du fichier cible 
			}else
			{
				logFileName = argv[i];
			}
		}
	}else
	{
		cerr << "-main- Trop ou pas d'arguments" << endl;
		printUsages();
	}

	if(logFileName == "")
	{
		cerr << "-main- Nom de fichier manquant" << endl;
		printUsages();
	}

	genererCatalogue(option, heure);

	//option : -g
	if(option[2])
	{
		genererGraphe(dotOutputFile);
	}
	
	//tri des ressources par ordre de consultation
	int popularite;
	//map triée par défault par int croissant
	std::multimap<int, string> top10;
	//parcours du catalogue
	for (Catalogue::iterator resIt = ressources.begin(); resIt != ressources.end(); ++resIt)
	{
		popularite = 0;
		//parcours des referers associés à la ressource
		for (ReferersMap::iterator refIt = resIt->second.begin(); refIt != resIt->second.end(); ++refIt)
		{
			//somme des nombres de consultations depuis chaque referer
			popularite += refIt->second;
		}
		top10.insert(pair<int, string>(popularite, resIt->first));
	}

	//affichage des 10 ressources les plus consultées ! :)
	int compteur = 0;
	for (multimap<int, string>::reverse_iterator top = top10.rbegin(); top != top10.rend() && compteur < 10; ++top, ++compteur)
	{
		cout << top->first << "(hits)" << "---------" << top->second << endl;
	}
	return 0;
}

void printUsages()
{
	string answer;
	while(answer!="o" && answer!="n"){
		cout << endl;
		cout << "Souhaitez-vous consulter le manuel d'utilisation ? (o/n)"<<endl;
		cin >> answer;
	}
	if(answer == "o"){
		//ouverture d'un flux en lecture sur le fichier
		ifstream logStream("../man", ios::in);
		if(logStream)
		{
			string line;
			while(getline(logStream, line))
			{
				cout << line << endl;
			}
		}
	}
	exit(1);
}

/*filtrer d'autre status 
nettoyer ressources et url ? (slash en fin de chaîne)
*/
void genererCatalogue(bool option [], int heure)
{
	//ouverture d'un flux en lecture sur le fichier
	ifstream logStream(logFileName, ios::in);

	if(logStream)
	{
		//sortie du programme si le fichier est vide
		if(logStream.peek() == std::ifstream::traits_type::eof())
		{
			cerr << "-genererCatalogue- Fichier vide !" << endl;
			exit(2);
		}

		//initialiser la map--------------------------------------------------------------------------
		string line;
		size_t pos;

		//option -t : filtrage selon l'heure des logs [heure ; heure + 1]
		if(option[1] == true && heure != -1)
		{
			int hour;

			//parcour du fichier
			while(getline(logStream, line))
			{
				hour = -1;

				pos = line.find(":", line.find("["));

				if(pos++ != string::npos)
					hour = stoi(line.substr(pos, line.find(":", pos)-pos));//extraction de l'heure du log

				//si le log est concerné par l'intervalle de recherche
				if(hour == heure || hour == ((heure+1) % 24))
				{
					pos = line.find("GET");

					//si la ligne n'est pas vide et le log concerne une méthode GET 200 OK
					if(pos++ != string::npos && line.substr(line.find("\"", line.find("/", pos)) + 2, 3) == "200")
					{
						string ressource = line.substr(line.find("/", pos), line.find(" ", line.find("/", pos)) - line.find("/", pos));//extraction de la ressource
						pos = line.find("\"", pos) + 1;
						string referer = line.substr(line.find("\"", pos) + 1, line.find("\"", line.find("\"", pos) + 1) - line.find("\"", pos) - 1);//extraction du referer
						//option -e : filtrage des images
						if(option[0] == true)
						{
							if(ressource.find(".png") == string::npos && ressource.find(".jpg") == string::npos && ressource.find(".bmp") == string::npos 
								&& ressource.find(".png") == string::npos && ressource.find(".gif") == string::npos && ressource.find(".css") == string::npos
								&& ressource.find(".js") == string::npos && ressource.find(".ico") == string::npos)
								
							{
								//insertion dans la map------------------------------------------------------------------------------------
								ajouterRessource(ressource,referer);
							}
						}else
						{
							//insertion dans la map------------------------------------------------------------------------------------
							ajouterRessource(ressource,referer);
						}
					}
				}
			}
		}
		else
		{
			//parcour du fichier
			while(getline(logStream, line))
			{
				pos = line.find("GET");

				//si la ligne n'est pas vide et le log concerne une méthode GET 200 OK
				if(pos++ != string::npos && line.substr(line.find("\"", line.find("/", pos)) + 2, 3) == "200")
				{
					string ressource = line.substr(line.find("/", pos), line.find(" ", line.find("/", pos)) - line.find("/", pos));//extraction de la ressource
					pos = line.find("\"", pos) + 1;
					string referer = line.substr(line.find("\"", pos) + 1, line.find("\"", line.find("\"", pos) + 1) - line.find("\"", pos) - 1);//extraction du referer

					//option -e : filtrage des images
					if(option[0] == true)
					{
						if(ressource.find(".png") == string::npos && ressource.find(".jpg") == string::npos && ressource.find(".bmp") == string::npos 
							&& ressource.find(".png") == string::npos && ressource.find(".gif") == string::npos && ressource.find(".css") == string::npos
							&& ressource.find(".js") == string::npos && ressource.find(".ico") == string::npos)
						{
							//insertion dans la map------------------------------------------------------------------------------------
							ajouterRessource(ressource,referer);
						}
					}else
					{
						//insertion dans la map------------------------------------------------------------------------------------
						ajouterRessource(ressource,referer);
					}
				}
				
			}
		}
	}else
	{

		cerr << "-genererCatalogue- Impossible d'ouvrir le fichier !" << endl;
		printUsages();
	}
}

void genererGraphe(string dotFileName){
	ifstream testPresence (dotFileName);
	//vérifier s'il existe déjà un .dot du même nom
	if(testPresence){
		string answer="";
		while(answer!="o" && answer!="n"){
		cout<<dotFileName<<" existe déjà, voulez vous le remplaçer ? (o/n)"<<endl;
		cin>>answer;
		}
		if(answer=="n"){
			cout<<"Le fichier file n'a pas été généré, fichier précédent conservé"<<endl;
			exit(3);
		}
	}
	testPresence.close();
	ofstream dotStream;
	dotStream.open(dotFileName, ios::out);
	//initializing dotstream
	dotStream<<"digraph g {\r\n"<<endl;
	//adding nodes to the graph
	for(auto resIt = ressources.begin();resIt!=ressources.end();++resIt){
		for(auto refIt=resIt->second.begin();refIt!=resIt->second.end();++refIt){
			dotStream<<refIt->first<<" -> "<<resIt->first<<" "<<"[label="<<refIt->second<<"];\r\n"<<endl;
		}
	}
	dotStream<<"}"<<endl;
	dotStream.close();
}	

void ajouterRessource(string resName, string refName){
	//check si la ressource est déja dans la table
	if(refName == "-"){
		refName="x";
	}
	resName = "\""+resName+"\"";
	refName = "\""+refName+"\"";

	Catalogue::iterator res= ressources.find(resName);
	if(res!=ressources.end()){
		ReferersMap::iterator ref= res->second.find(refName);
		if(ref!= res->second.end()){
			ref->second = (ref->second) + 1;
		}else{
			res->second.insert({refName,1});
		}
	}else{
		ressources.insert({resName, {{refName, 1}}});
	}
}
