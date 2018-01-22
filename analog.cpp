#include <iostream>
#include "analog.h"

using namespace std;

string logName="anonyme.log";
Catalogue ressources;

int main(int argc, char const *argv[])
{
	/*gérer les erreurs propres au main*/

	/*générer catalogue*/
	bool option [2];
	option [0] = true;
	option [1] = true;
	genererCatalogue(option);
	/*en fonction des arguments générerGraphe ou pas
	puis affichage de la réponse*/
	/*ReferersMap resA={{"B",2},{"C",1}};
	ReferersMap resB={{"A",12},{"C",3},{"B",4}};
	Catalogue ressources={{"A",resA},{"B",resB}};
	ajouterRessource("A", "B", ressources);
	ajouterRessource("B", "B", ressources);*/
	genererGraphe("banane.dot");
	cout<<ressources["A"]["B"];
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
					pos = line.find("GET") + 1;

					//si le log concerne une méthode GET 200 OK
					if(pos != string::npos && line.substr(line.find("\"", line.find("/", pos)) + 2, 3) == "200")
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
								/*cout << line << endl;
								cout << ressource << endl;
								cout << referer << endl;*/
								ajouterRessource(ressource,referer);
							}
						}else
						{
							//insertion dans la map------------------------------------------------------------------------------------
							/*cout << line << endl;
							cout << ressource << endl;
							cout << referer << endl;*/
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
				pos = line.find("GET") + 1;

				//si le log concerne une méthode GET 200 OK
				if(pos != string::npos && line.substr(line.find("\"", line.find("/", pos)) + 2, 3) == "200")
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
							/*cout << ressource << endl;
							cout << referer << endl;*/
							ajouterRessource(ressource,referer);
						}
					}else
					{
						//insertion dans la map------------------------------------------------------------------------------------
						/*cout << ressource << endl;
						cout << referer << endl;*/
						ajouterRessource(ressource,referer);
					}
				}
				
			}
		}
	}else
	{
		cerr << "-genererCatalogue- Impossible d'ouvrir le fichier !" << endl;
	}
}

void genererGraphe(string dotFileName){
	ifstream testPresence (dotFileName);
	if(testPresence){
		string answer="";
		while(answer!="o" && answer!="n"){
		cout<<dotFileName<<" existe déjà, voulez vous le remplaçer ? (o/n)"<<endl;
		cin>>answer;
		}
		if(answer=="n"){
			cout<<"Le fichier file n'a pas été généré, fichier précédent conservé"<<endl;
			exit;
		}
	}
	testPresence.close();
	ofstream dotStream;
	dotStream.open(dotFileName, ios::out);
	//initializing dotstream
	cout<<"starting creation"<<endl;
	dotStream<<"digraph g {\r\n"<<endl;
	//adding nodes to the graph
	for(auto resIt = ressources.begin();resIt!=ressources.end();++resIt){
		for(auto refIt=resIt->second.begin();refIt!=resIt->second.end();++refIt){
			dotStream<<refIt->first<<" -> "<<resIt->first<<" "<<"[label="<<refIt->second<<"];\r\n"<<endl;
		}
	}
	dotStream<<"}"<<endl;
	dotStream.close();
	cout<<"creation finished"<<endl;
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
		//ReferersMap test = {{refName, 1}};
		ressources.insert({resName, {{refName, 1}}});
	}
}
