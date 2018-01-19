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
	ReferersMap resC={{"A",3},{"B",1}};
	ReferersMap resB={{"A",12},{"C",3},{"B",4}};
	ressources={{"A",resA},{"C",resC},{"B",resB}};

	genererGraphe("banane.dot");

	return 0;
}

void genererGraphe(string dotFileName){
	ifstream testPresence (dotFileName);
	if(testPresence){
		string answer="";
		while(answer!="o" && answer!="n"){
		cout<<dotFileName<<" existe déjà, voulez vous le remplaçer ? (o/n)"<<endl;
		cin>>answer;
		cout<<answer<<endl;
		}
		if(answer=="n"){
			cout<<"Le fichier file n'a pas été généré, fichier précédent conservé"<<endl;
			exit;
		}
	}
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