#include <iostream>

#include "analog.h"

using namespace std;

/*typedef Catalogue*/

int main(int argc, char const *argv[])
{
	/*gérer les erreurs propres au main*/

	/*générer catalogue*/

	/*en fonction des arguments générerGraphe ou pas
	puis affichage de la réponse*/
	ReferersMap resA={{"B",2},{"C",1}};
	ReferersMap resB={{"A",12},{"C",3},{"B",4}};
	Catalogue ressources={{"A",resA},{"B",resB}};
	ajouterRessource("A", "B", ressources);
	ajouterRessource("B", "B", ressources);
	genererGraphe("banane.dot", ressources);
	cout<<ressources["A"]["B"];
	return 0;
}

void genererGraphe(string dotFileName, Catalogue &ressources){
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
}	

void ajouterRessource(string resName, string refName, Catalogue &ressources){
	//check si la ressource est déja dans la table
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
