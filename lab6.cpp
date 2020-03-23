#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>
#include <deque>

//=======================INFO FOR A POINT===========================
struct Point{

   int name;
   int x;
   int y; 
};

//========================INFO FOR AN EDGE==============================================

struct edge{
   
   Point start;
   Point end;
   double length;  
   
};
//=========================CLASS FOR THE TREE======================================

class graph{

   private:
      std::vector<Point> nodes;     // Vector to hold all points in the graph
      std::deque<edge> lines;      // Holds all edges
      //std::vector<edge> MST;        // Holds the edges/points of the MST
      std::vector< std::vector<edge> > forest; 
      
   public:
      graph(std::vector<double>);   // fills node vector
      ~graph();                     // prints destruction message
      double distance(int, int);    // determines distance between 2 points
      void buildEdges();            // creates all possible edges
      void sortEdges();             // sorts the edges by distance, smallest distance first
      void printNodes();            // prints all the nodes
      void printLines();            // prints all the possible edges
      bool checkBranch(edge);         // checks if the MST has a loop
      void buildTree();             // builds the MST
      void printMST();              // prints the Nodes of the MST
      bool checkSpan();             // checks if the MST spans all points
      int isIncluded(Point);        // checks if a point is in a tree
      void merge(int, int);         // combines two trees
      void printtoFile(); 
      
};


graph::graph(std::vector<double> bin){
   
   int j = 1;  // fill in node vector, and forest
   for(int i = 0; i < bin[0]; i++){
      nodes.push_back(Point());
      nodes[i].name = bin[j];
      nodes[i].x = bin[j+1];
      nodes[i].y = bin[j+2];
      j+=3;  
   } 
  
   std::cout << "Created " << nodes.size() << " Element Vector of Nodes" << std::endl;
   std::cout << std::endl; 
   
}

graph::~graph(){
   
   std::cout << "Destroying Vectors" << std::endl;  
    
}

double graph::distance(int i, int j){
   
   return sqrt(pow((nodes[i].x - nodes[j].x), 2) + pow((nodes[i].y - nodes[j].y), 2)); 

}

void graph::buildEdges(){
   
   // undirected graph
   int k = 0; 
   for(int i = 0; i < nodes.size()-2; i++){
      for(int j = (i+1); j < nodes.size(); j++){
        lines.push_back(edge());
        lines[k].start.name = nodes[i].name; 
        lines[k].start.x = nodes[i].x;
        lines[k].start.y = nodes[i].y;
        lines[k].end.name = nodes[j].name;
        lines[k].end.x = nodes[j].x;
        lines[k].end.y = nodes[j].y;
        lines[k].length = distance(i, j);
        k++;  
      }
   } 
}

void graph::sortEdges(){ // simple bubble sort
   
   edge temp; 
   bool swap = true;
   while(swap){
      swap = false; 
      for(int i = 0; i < (lines.size()-1); i++){
         if(lines[i].length > lines[i+1].length){
            temp = lines[i+1]; 
            lines[i+1] = lines[i];
            lines[i] = temp; 
            swap = true; 
         }  
      }
   }
}

void graph::printNodes(){
   
   for(int i = 0; i < nodes.size(); i++){
      
      std::cout << "Node " << i << ": " << std::endl;
      std::cout << "Name: " << nodes[i].name << ", X: " << nodes[i].x << ", Y: " << nodes[i].y  << std::endl;   
      std::cout << std::endl;  
   }

}

void graph::printLines(){

   for(int i = 0; i < lines.size(); i++){
      
      std::cout << "line " << i << ": " << std::endl; 
      std::cout << "Start Name: " << lines[i].start.name << ", End Name: " << lines[i].end.name << ", Distance: " << lines[i].length << std::endl;   
      std:: cout << std::endl; 
   
   }
}

bool graph::checkBranch(edge checkLine){  // if added piece has both nodes in the loop already, it cant be added
   
   
   //std::cout << "===Start CheckLoop===" << std::endl;
   int startIndex; 
   int endIndex;
  
   if(forest.empty()){
      forest.push_back(std::vector<edge>());
      forest.back().push_back(checkLine);
      return false; 
   }
   
   startIndex = isIncluded(checkLine.start);
   endIndex = isIncluded(checkLine.end); 
   
   if((startIndex == -1) && (endIndex == -1)){ // branch is unique
      forest.push_back(std::vector<edge>());
      forest.back().push_back(checkLine);
      return false;
        
   }else if(startIndex == endIndex){ // branch creates a loop
      //std::cout << "Didnt add, it would create a loop" << std::endl;
      return true;
      
   }else{ // branch fits into one or more trees
      if(startIndex == -1){ // the branch continues a tree
         forest[endIndex].push_back(checkLine);
         
      }else if(endIndex == -1){ //the branch continues a tree
         forest[startIndex].push_back(checkLine);
         
      }else{ // branch connects two trees
          if(startIndex < endIndex){
             forest[startIndex].push_back(checkLine);
             merge(startIndex, endIndex);
          }else{
             forest[endIndex].push_back(checkLine);
             merge(endIndex, startIndex);
          }
      }
   }
}

void graph::buildTree(){
   
   int i = 0;
   //std::cout << "===Start BuildTree===" << std::endl; 
   while((!checkSpan()) && (!lines.empty())){
      //std::cout << "BT: Checking Edge " << i << std::endl; 
      checkBranch(lines.front());
      lines.pop_front(); 
      i++; 
   } 
   //std::cout << "===end BuildTree===" << std::endl;
}   

void graph::printMST(){ // rewrite
   
   for(int i = 0; i < forest[0].size(); i++){
      std::cout << "Start Edge: " << forest[0][i].start.name << ", End Edge: " << forest[0][i].end.name << std::endl;
   }
}

bool graph::checkSpan(){ // check first start and then all endings  REWRITE
   
   int count = 0;
   int i = 0; 
   int j = 0; 
   
   if(forest.empty()){ // there can be no loops if the tree has 1 or 2 branches
      //std::cout << "There are 0 edges, it is too small to span" << std::endl; 
      return false; 
   } 
 
   while(i < nodes.size()){ 
      if((nodes[i].name == forest[0][j].start.name) || (nodes[i].name == forest[0][j].end.name)){
         std::cout << nodes[i].name << std::endl;
         count++;  
         i++;
         j = 0;  
      }else{
         j++; 
      }
      
      if(j > forest[0].size()){
         j = 0; 
         i++; 
      }
   }
   
   if(count == nodes.size()){
      //std::cout << "It spans" << std::endl;
      //std::cout << "===End CheckSpan===" << std::endl;
      return true;
   }else{
     // std::cout << "It does not span" << std::endl;
     // std::cout << "===end CheckSpan===" << std::endl;
      return false; 
   }     
}

int graph::isIncluded(Point checkpoint){
   
   
   for(int i = 0; i < forest.size(); i++){
      for(int j = 0; j < forest[i].size(); j++){
         
         if(checkpoint.name == forest[i][j].start.name){
            return i;  
         }
      
         if(checkpoint.name == forest[i][j].end.name){
            return i;
         }
      }
   } 
   return -1; 
}

void graph::merge(int grow, int remove){
   
   forest[grow].insert(forest[grow].end(), forest[remove].begin(), forest[remove].end());
   forest[remove].erase(forest[remove].begin(), forest[remove].end()); 

}

void graph::printtoFile(){
  
 std::ofstream output("output.txt"); 
 output << "The total number of edges: " << forest[0].size() << std::endl;
 for(int i = 0; i < forest[0].size(); i++){
      output << forest[0][i].start.name << " " << forest[0][i].end.name << std::endl;
   }
  output.close(); 
}

//========================READING============================
//pass in the object by reference to fill the parts with the pieces from the file 

void read(std::vector<double> &bin){
   
   double temp;    
   std::string line;  // space to take in one line
   std::ifstream input("input.txt"); // input file
    
   while(!input.eof()){                        // runs through all lines
      getline(input, line);                    // gets a line
      if(line != ""){                          // remove blank lines
         if(line.at(0) != '/'){                // remove comments
            std::stringstream digits(line);    // makes an input from the string
            while(digits.good()){              // runs through all tokens
               digits >> temp;                 // Get the token
               bin.push_back(temp);            // put in the value      
            }
         }
      }
   }
   input.close();  // close the file
}

//====================================MAIN===================================================


using namespace std;

int main(){

   vector<double> file; 
   
   read(file);
   
   graph tree(file);
   
   //tree.printNodes();
   tree.buildEdges(); 
   tree.sortEdges(); 
   tree.printLines();
   tree.buildTree();
   //tree.printMST();
   tree.printtoFile(); 
      
   return 0; 
}

