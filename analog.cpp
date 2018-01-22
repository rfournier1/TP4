#include <iostream>
#include <fstream>
#include "analog.h"

using namespace std;

string logName="anonyme.log";

int main(int argc, char const *argv[])
{
	/*gérer les erreurs propres au main*/

	/*générer catalogue*/
	bool option [2];
	option [0] = true;
	option [1] = true;
	genererCatalogue(option, 11);
	/*en fonction des arguments générerGraphe ou pas
	puis affichage de la réponse*/
	
	return 0;
}

/*filtrer d'autre status 
nettoyer ressources et url ? (slash en fin de chaîne)
*/
void genererCatalogue(bool option [], int heure)
{
	//ouverture d'un flux en lecture sur le fichier
	ifstream logStream(logName, ios::in);

	if(logStream)
	{
		//sortie du programme si le fichier est vide
		if(logStream.peek() == std::ifstream::traits_type::eof())
		{
			cerr << "-genererCatalogue- Fichier vide !" << endl;
			exit(0);
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
				pos = line.find(":", line.find("[")) + 1;
				hour = stoi(line.substr(pos, line.find(":", pos)-pos));//extraction de l'heure du log

				//si le log est concerné par l'intervalle de recherche
				if(hour == heure || hour == heure+1)
				{
					pos = line.find("GET")+1;

					//si le log concerne une méthode GET 200 OK
					if(pos != string::npos && line.substr(line.find("\"", line.find("/", pos)) + 2, 3) == "200")
					{
						string ressource = line.substr(line.find("/", pos), line.find(" ", line.find("/", pos)) - line.find("/", pos));//extraction de la ressource

						//option -e : filtrage des images
						if(option[0] == true)
						{
							if(ressource.find(".png") == string::npos && ressource.find(".jpg") == string::npos && ressource.find(".bmp") == string::npos 
								&& ressource.find(".png") == string::npos && ressource.find(".gif") == string::npos && ressource.find(".css") == string::npos
								&& ressource.find(".js") == string::npos && ressource.find(".ico") == string::npos)
							{
								//insertion dans la map------------------------------------------------------------------------------------
								cout << line << endl;
								cout << ressource << endl;
							}
						}else
						{
							//insertion dans la map------------------------------------------------------------------------------------
							cout << line << endl;
							cout << ressource << endl;
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
				pos = line.find("GET")+1;

				//si le log concerne une méthode GET 200 OK
				if(pos != string::npos && line.substr(line.find("\"", line.find("/", pos)) + 2, 3) == "200")
				{
					string ressource = line.substr(line.find("/", pos), line.find(" ", line.find("/", pos)) - line.find("/", pos));//extraction de la ressource

					//option -e : filtrage des images
					if(option[0] == true)
					{
						if(ressource.find(".png") == string::npos && ressource.find(".jpg") == string::npos && ressource.find(".bmp") == string::npos 
							&& ressource.find(".png") == string::npos && ressource.find(".gif") == string::npos && ressource.find(".css") == string::npos
							&& ressource.find(".js") == string::npos && ressource.find(".ico") == string::npos)
						{
							//insertion dans la map------------------------------------------------------------------------------------
							cout << ressource << endl;
						}
					}else
					{
						//insertion dans la map------------------------------------------------------------------------------------
						cout << ressource << endl;
					}
				}
				
			}
		}
	}else
	{
		cerr << "-genererCatalogue- Impossible d'ouvrir le fichier !" << endl;
	}

}
