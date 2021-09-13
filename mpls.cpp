#include<bits/stdc++.h>
#include<iostream>
#include<ostream>
#include<iomanip>
#include<istream>
#include<string>
using namespace std;

vector<vector<vector<long double>>> forwarding_table(100, vector<vector<long double>>(100,vector<long double>(5)));
vector<vector<vector<int>>> sec_shortest_path(100,vector<vector<int>>(100,vector<int>(30))); //storing second shortest path
vector<vector<int>> sec_shortest_dist(100,vector<int> (100)); //strore second_shortest distance
vector<vector<vector<int>>> shortest_path(100,vector<vector<int>>(100,vector<int>(30))); //store shortest path
vector<vector<int>> dist(100,vector<int> (100)); //store shortest distance
long long int total_est_conn=0;
long long int label=2001;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//used to read the topology file
ifstream fin1;
vector<vector<long double>> read_top_file(long int &num_edges,long int &num_nodes)
{
	fin1>>num_nodes>>num_edges;
  	vector<vector<long double>> topology(num_edges,vector<long double>(4));
	cout<<endl;
	for (int i = 0; i < num_edges; i++)
	{
  		fin1 >> topology[i][0] >> topology[i][1] >> topology[i][2] >> topology[i][3];      		
	}
	for(int i = 0; i < num_edges; i++)
	{
		//cout<<topology[i][0]<<topology[i][1]<<topology[i][2]<<topology[i][3]<<topology[i][4]<<endl;
	}
	return topology;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//used to check whether user has chosen hop or dist 
void check_hop_dist(string s,long int num_edges,vector<vector<long double>> &topology)
{
	bool temp=1;
    	string s2="hop";
    	int k=0;
    	for(auto j:s2)
    	{
    		if(j!=s[k])
    		{
    			temp=0;
    			break;
    		}
    		k++;
    	}
    	//cout<<endl<<temp<<endl;
    	if(temp==1)
    	{
    		//cout<<"hop";
    		for (int i = 0; i < num_edges; i++) 
    		{
        		topology[i][2] = 1;//set distance as 1
      		}
    	}
}
void Bellman(vector<vector<long double>> topology,vector<vector<int>> &path ,int num_edges,int num_nodes)
{
	for (long int i = 0; i < num_nodes; i++)
	{
	   for (long int j = 0; j < num_nodes; j++) {
	        if (i == j) {
	        //updating distance matrix of nodes 
	          dist[i][j] = 0;
	          path[i][j] = 0;
	        } else {
	          path[i][j] = -1;
	          dist[i][j] = 100000;
	        }
	     }
	 }
	 for (long int i = 0; i < num_edges; i++) 
	 {//update dist and path matrix as given in topology file
      	dist[topology[i][0]][topology[i][1]] = topology[i][2];
      	dist[topology[i][1]][topology[i][0]] = topology[i][2];
      	path[topology[i][0]][topology[i][1]] = topology[i][0];
      	path[topology[i][1]][topology[i][0]] = topology[i][1];
      	//cout<<topology[i][2]<<" copied to dist "<<topology[i][0]<<topology[i][1]<<endl;
    }
    for (int k = 0; k < num_nodes; k++) {
      for (int i = 0; i < num_nodes; i++) {
        for (int j = 0; j < num_nodes; j++) {
          if (dist[i][k] + dist[k][j] < dist[i][j]) {//updating distance matrix on basis of shortest path
            dist[i][j] = dist[i][k] + dist[k][j];
            path[i][j] = path[k][j];//update path matrix also
            //cout<<dist[i][j]<<endl;
          }
        }
      }
    }
    //printing path
    for (int i = 0; i < num_nodes; i++) {
      for (int j = 0; j < num_nodes; j++) {
        if (i != j && path[i][j] != -1) {
          
          int s = i, t = j;
          int cont = 0;
          
          shortest_path[i][j][cont++] = j;
          while (path[s][t] != s) 
          {
            //cout<<path[s][t]<<"--";
            shortest_path[i][j][cont++] = path[s][t];//updatig shortest path 3D array
            t = path[s][t];
          }
          //cout<<i;
          shortest_path[i][j][cont++] = i;
        }
      }  
    }
}

//dijstras algorithm//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void dijkstra(vector<vector<int>> G, int n, int source_node, int sink_node) {
  int cost[500][500], distance[500], predecessor[500];
  int vis_array[500], count, mindistance, nextnode, i, j;
  for (i = 0; i < n; i++)
    {
    	for (j = 0; j < n; j++)
    	{
    		//cout<<G[i][j];
    	}
    	//cout<<endl;
    }
  //determining cost matrix from input graph
  for (i = 0; i < n; i++)
    for (j = 0; j < n; j++)
      if (G[i][j] == 0)
        cost[i][j] = 100000;//if source and destination are same
      else
        cost[i][j] = G[i][j];//otherwise cost is same as that of input graph
  
  
  for (i = 0; i < n; i++) {
    distance[i] = cost[source_node][i];//initializing distance array 
    predecessor[i] = source_node;//initializing predecessor aray so that we can trace path
    vis_array[i] = 0;//initializing visisted array
  }
  distance[source_node] = 0;//source node distance set to zero
  vis_array[source_node] = 1;//mark source as visisted
  count = 1;

  while (count < n - 1) {
    mindistance = 100000;
    for (i = 0; i < n; i++)
      if (distance[i] < mindistance && !vis_array[i]) {
        mindistance = distance[i];
        nextnode = i;//finding the next node to be visisted with minimum distance from  source node
      }
    vis_array[nextnode] = 1;//mark next node as visisted
    for (i = 0; i < n; i++)
      if (!vis_array[i])
        if (mindistance + cost[nextnode][i] < distance[i]) {
          distance[i] = mindistance + cost[nextnode][i];
          predecessor[i] = nextnode;//updating predecesssor of node
        }
    count++;
  }
  //cout<<"\nDistance from "<< source_node<< " To "<<sink_node<<"="<<distance[sink_node];
  //cout<<"\nPath="<<sink_node;
  sec_shortest_dist[source_node][sink_node] = distance[sink_node];//storing second shortest distance in matrix
  j = sink_node;
  int countter = 0;
  sec_shortest_path[source_node][sink_node][countter++] = j;//storing second shortest path
  do {
    j = predecessor[j];
    //cout<<"<-"<<j<<endl;
    sec_shortest_path[source_node][sink_node][countter++] = j;
  } while (j != source_node);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//calculating the second shortest path using dijkastra algo
void dijkastra_second_shortest_path(vector<vector<long double>> topology,vector<vector<int>> path,vector<vector<int>> dist1,int num_nodes,int num_edges)
{
    for (int i = 0; i < num_nodes; i++)
      for (int j = 0; j < num_nodes; j++) {
        if (i == j) {
          dist1[i][j] = 0;
        } 
        else 
        {
          dist1[i][j] = 100000;
        }
      }
    for (int i = 0; i < num_edges; i++) {
      dist1[topology[i][0]][ topology[i][1]] = topology[i][2];
      dist1[topology[i][1]][ topology[i][0]] = topology[i][2];
      //cout<<(int)topology[i][2]<<" copied to dist "<<(int)topology[i][0]<<" "<<(int)topology[i][1]<<endl;
    }
    for (int i = 0; i < num_edges; i++) {
    	//cout<<dist1[topology[i][0]][ topology[i][1]];
      	//cout<<dist1[topology[i][1]][ topology[i][0]];
    }

    //cout<<path.size()<<" "<<path[0].size()<<endl;
    for(int i=0;i<path.size();i++)
    {
    	for(int j=0;j<path[0].size();j++)
    	{
    		//cout<<path[i][j];
    	}
    	//cout<<endl;
    }
    for (int i = 0; i < num_nodes; i++) 
    {
      for (int j = 0; j < num_nodes; j++) 
      {
        int ded_path[100];//finding dedicated path between i and j
        long int s = i, t = j;
        stack < long int > stac;
        while (path[s][t] != s) 
        {
          //cout<<path[s][t]<<" ";
          stac.push(path[s][t]);
          t = path[s][t];
        }
        //cout<<"path between "<<i<<"and"<<j<<endl;
        //cout<<endl;
        
        int count = 0;
        ded_path[count] = i;
        count++;
        while (!stac.empty())
        {
          ded_path[count] = stac.top();
          count++;
          stac.pop();
        }
        ded_path[count++] = j;
        vector<long int> tempp(100);
        int ind = 0;

        for (int x = 0; x < count - 1; x++) {
          //cout<<dist1[ded_path[x]][ded_path[x + 1]]<<" ";	
          tempp[ind++] = dist1[(int)ded_path[x]][(int)ded_path[x + 1]];  
          dist1[ded_path[x]][ded_path[x + 1]] = 100000;//removing the first shortest path
        }
        dijkstra(dist1, num_nodes, i, j);//calling dijkstras to find second shoretst path
        ind = 0;
        for (int x = 0; x < count - 1; x++) {
          dist1[ded_path[x]][ded_path[x + 1]] = tempp[ind++];//again adding removed path 
        }
      }
    }
    //dijkstras over
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//creating routing table
void routing(char ** argv,int num_nodes)
{
	ofstream out1;
	out1.open(argv[6]);
	if(!out1)
	{
		cout<<"routing file is not able to open";
		return ;
	}
	for (int i = 0; i < num_nodes; i++) {
      out1 << "Routing Table of " << i << ":"<<endl;
      for (int j = 0; j < num_nodes; j++) {
        if (i != j) 
        {
          long int count1 = 0;
          out1 << shortest_path[i][j][0] << "   ";
          //storing shortest path from i to j in a file
          do 
          {  
            out1 << shortest_path[j][i][count1] << "-->";
            count1=count1+1;
          } while (shortest_path[j][i][count1] != j);
          
          //storing path delay and path cost in file
          out1 << shortest_path[j][i][count1] << "   ";
          out1 << dist[i][j] << "   " << count1;
          out1 << endl;

          long int count2 = 0;
          out1 << sec_shortest_path[i][j][0] << "   ";
          //storing second shortest path from i to j in a file
          do 
          {
            out1 << sec_shortest_path[j][i][count2] << "-->";
            count2=count2+1;
          } while (sec_shortest_path[j][i][count2] != j);
          //stroring second shortest path delay and path cost in file
          out1 << sec_shortest_path[j][i][count2] << "   ";
          out1 << sec_shortest_dist[i][j] << "   " << count2;
          out1 << endl << endl;
        }
      }
    }
	return ;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//reading connection file
vector<vector<long double>> read_conn(char **argv,long int &no_of_conn)
{
	
	ifstream fin2;
	fin2.open(argv[4]);
	if(!fin2)
	{
		cout<<"unable to open connection file "<<endl;
		exit(0);
	}
	fin2>>no_of_conn;
	vector<vector<long double>> conn(no_of_conn,vector<long double>(6));
	for(int i=0;i<no_of_conn;i++)
	{
		fin2>>conn[i][0]>>conn[i][1]>>conn[i][2]>>conn[i][3]>>conn[i][4];
		long double no1=1.0*conn[i][4];
		long double no2=conn[i][3]+0.25*(conn[i][4]-conn[i][2]);
		conn[i][5]=min(no1,no2);
	}
	return conn;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//storing the first and second shortest path int 1 d vector first_path and second_path
void store_shortest_path(long int source,long int dest,vector<int> &first_path,vector<int> &second_path)
{
	if(source!=dest)
	{
	  long int count1 = 0;
      do 
      {
      		first_path[count1] = shortest_path[dest][source][count1];
      } while (shortest_path[dest][source][count1++] != dest);//storing shortest path in 1D array
      
      for(int i=0;i<first_path.size();i++)
      {
      	//cout<<first_path[i]<<" ";
      }         	
 	  long int count2 = 0;
      do {
      second_path[count2] = sec_shortest_path[dest][source][count2];
      } while(sec_shortest_path[dest][source][count2++] != dest);//storing second shortest path to 1D array
      
      for(int i=0;i<second_path.size();i++)
      {
      	//cout<<second_path[i]<<" ";
      }           
	}
	else
	{
		cout<<"destinantion and source cant be same";
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//checking whether we can grant the connection or not
void can_grant(vector<vector<long double>> topology,vector<int> path,bool &valid_path,int dest,long double b_eq,int num_edges)
{
	long int count_top = 0;
    long int path_count = 0;
    do {
      if ((topology[count_top][0] == path[path_count] && topology[count_top][1] == path[path_count + 1])
       || (topology[count_top][1] == path[path_count] && topology[count_top][0] == path[path_count + 1]) 
       && count_top < num_edges) 
      {
          if (topology[count_top][3] < b_eq) 
          {
              valid_path = 0;//checking whether we can grant connection or not
          }
          path_count++;
          count_top = 0;
      } 
      else 
      {
        count_top++;
      }
    } while (path[path_count] != dest);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//creating fowarding table
vector<int> forward_table_size(100,0);
ofstream out;
void forward(vector<int> path, int source, int  dest, int flag,int conn_id ,char **argv)
{
	out << "     " << conn_id << "     "<< source << "     "<< dest << "    ";//<< source << "-->";
	int start_label=label;
	int counter = 1;
	forwarding_table[source][forward_table_size[source]][0] = source;//update forwarding table of source
    forwarding_table[source][forward_table_size[source]][1] = -1;
    forwarding_table[source][forward_table_size[source]][2] = -1;
    forwarding_table[source][forward_table_size[source]][3] = path[1];
    forwarding_table[source][forward_table_size[source]++][4] = label;
    do
    {
    	if(path[counter]!=dest)
    	{
    	  forwarding_table[path[counter]][forward_table_size[path[counter]]][0] = path[counter];//update forwarding table of intermediate node
    	  //cout<<path[counter];
    	  forwarding_table[path[counter]][forward_table_size[path[counter]]][1]= path[counter - 1];
	      forwarding_table[path[counter]][forward_table_size[path[counter]]][2] = label;
	      label=label+1;
	      forwarding_table[path[counter]][forward_table_size[path[counter]]][3] = path[counter + 1];
	      forwarding_table[path[counter]][forward_table_size[path[counter]]++][4] = label;
	      //out << path[counter] << "-->";
	      //cout<<path[counter]<<" "<<label<<"  "<<endl;
	      //cout<<forwarding_table[path[counter]][forward_table_size[path[counter]]][0];
	      //cout<<forwarding_table[path[counter]][forward_table_size[path[counter]]][1];
	    } 
	    else 
	    {
	      forwarding_table[path[counter]][forward_table_size[path[counter]]][0] = path[counter];//update forwarding table of destination node
	      forwarding_table[path[counter]][forward_table_size[path[counter]]][1] = path[counter - 1];
	      forwarding_table[path[counter]][forward_table_size[path[counter]]][2] = label;
	      label=label+1;
	      forwarding_table[path[counter]][forward_table_size[path[counter]]][3] = -1;
	      forwarding_table[path[counter]][forward_table_size[path[counter]]++][4] = -1;
	      //out<< path[counter] << "   ";
	    }
    }while (path[counter++] != dest);
  
  	int q=start_label;
  	while(q < label)
  	{
  		out<< q << ",";		//storing label list in forwarding table
  		q++;
  	}
  	if (flag == 0)			//check whether it follows shortest path or second shortest.
    	out<< "     "<<dist[source][dest];
  	else 
    	out << "     "<<sec_shortest_dist[source][dest];
  	out<< "     "<<endl;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//updating the forwarding table and updating link capacity
void update(vector<vector<long double>> &topology,vector<int> path,int source,int dest,int flag,int i,long double b_eq,int num_edges,char **argv)
{
	total_est_conn++;
	forward(path, source, dest, i, flag, argv);//updating forwarding table
	long int count_top = 0;
	long int path_count = 0;
	do 
	{
		if ((topology[count_top][0] == path[path_count] && topology[count_top][1] == path[path_count + 1])
		 || (topology[count_top][1] == path[path_count] && topology[count_top][0] == path[path_count + 1]) 
		 && count_top < num_edges) 
		{
		  topology[count_top][3] = topology[count_top][3] - b_eq;//update link capacity
		  //cout<<b_eq<<"  ";
		  //cout<<topology[count_top][3]<<"  ";
		  path_count++;
		  count_top = 0;
		} 
		else 
		{
		  count_top++;
		}
	} while (path[path_count] != dest);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
//updating forwarding table
ofstream out3;
void update_forwarding_table(int num_nodes)
{   	
    for(int i = 0; i < num_nodes; i++) 
    {
      out3 << endl <<"For router " << i << endl << endl;
      for(int j = 0; j < forward_table_size[i]; j++) 
      {
        for(int k = 1; k < 5; k++) 
        {
          out3 << forwarding_table[i][j][k] << "  ";

        }
        out3 << endl;
      }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//print_output
void print_output(char **argv)
{
	cout<<"Input--"<<endl;
    cout<<"Topology File = "<<argv[2]<<endl;
    cout<<"Approach(pessimistic/optimistic"<<"1-pessimistic and 0-optimistic) = "<<argv[14]<<endl;
    cout<<"Dist/Hop = "<<argv[12]<<endl;
    cout<<"Connection file = "<<argv[4]<<endl;
    cout<<endl;
    cout<<"Output -- "<<endl;
    cout<<"Routing Table File = "<<argv[6]<<endl;
    cout<<"Forwarding Table File = "<<argv[8]<<endl;
    cout<<"Path Table File = "<<argv[10]<<endl;
    cout<<"Total number of establised connection = "<<total_est_conn<<endl;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//main function
int main(int argc, char ** argv)
{
	//argument check
	//./routing −top topologyfile −conn connectionsfile −rt routingtablefile −ft forwardingtablefile 
	//− path pathsfile − flag hop|dist − p 0|1    
	if (argc < 15)
    {
    	cout << "Too few arguments...\n";
  	}
  	else if (argc == 15) 
  	{
  		fin1.open(argv[2]);
  		if(!fin1)
  		{
  			cout << "Unable to open Toplogy file";
      		return -1;
  		}
  		//file opened
  		//////////////////////store the content of topology file///////////////////////
  		vector<vector<long double>> topology;
  		long int num_edges,num_nodes;
  		topology=read_top_file(num_edges,num_nodes);
  		fin1.close();

  		/////////////////////checking whether hop or dist is used///////////////////////
    	string s=argv[12];
    	check_hop_dist(s,num_edges,topology);
 
	    /////////////// first shortest path using Bellman ford /////////////////////////
	    vector<vector<int>> path(500,vector<int>(500));
	    Bellman(topology,path,num_edges,num_nodes);
	    for(int i=0;i<500;i++)
	    {
	    	for(int j=0;j<500;j++)
	    	{
	    		//cout<<path[i][j];
	    	}
	    }
	    /////////////// second shortest using Dijstras implememtaion ///////////////////
     	vector<vector<int>> dist1(num_nodes,vector<int>(500));
    	dijkastra_second_shortest_path(topology,path,dist1,num_nodes,num_edges);
    	for(int i=0;i<num_nodes;i++)
	    {
	    	for(int j=0;j<500;j++)
	    	{
	    		//cout<<dist1[i][j];
	    	}
	    }
    	/////////////////////////////routing table//////////////////////////////////////
    	routing(argv,num_nodes);

    	///////////////////////////reading connection file//////////////////////////////
    	vector<vector<long double>> conn;
    	long int num_conn;
    	conn=read_conn(argv,num_conn);
    	//cout<<endl<<conn.size()<<" "<<conn[0].size();

    	//////////////////////////////opening path file/////////////////////////////////
    	out.open(argv[10]);
    	if(!out)
		{
			cout<<"Unable to open path file..";
			exit(0);
		}

    	/////////setting up the connextion using optimistic and pessimistic approach/////
    	for(int i=0;i<num_conn;i++)
    	{
    		long double band_eq=conn[i][5];
    		long double band_min=conn[i][2];
    		long double band_av=conn[i][3];
    		long double band_max=conn[i][4];
    		long int source=conn[i][0];
    		long int dest=conn[i][1];
    		//cout<<endl<<source<<" "<<dest<<endl;
    		
    		//storing value of p i.e which approach is used(optimistic/pessimistic)
    		char *ps=(argv[14]);
    		int p=*ps-48;

    		//cout<<p;
    		vector<int> first_path(31);
    		vector<int> second_path(31);

    		//storing the shortest path and second shortest path in 1 D array first_path and second_path respectively
    		store_shortest_path(source,dest,first_path,second_path);
    		
    		//pessimistic approach
    		bool valid_path=true;
	    	if(p==1)
	    	{
	    		//used to check whether we can grant connection or not and store it in valid_path variable
	    		can_grant(topology,first_path,valid_path,dest,band_max,num_edges);
	    		if(valid_path)
	    		{
	    			update(topology,first_path,source,dest,i,0,band_max,num_edges,argv);//update the forwarding table and link capacity
	    		}
	    		else
	    		{
	    			valid_path=true;
	    			//check the grant permission for second shortest path
	    			can_grant(topology,second_path,valid_path,dest,band_max,num_edges);
	    			if(valid_path)
	    			{
	    				update(topology,second_path,source,dest,i,0,band_max,num_edges,argv);//update the forwarding table and link capacity
	    			}
	    		}	
	    	}//pessimistic approach ends
	    	//optimistic approach
    		else if(p==0)
	    	{
	    		bool valid_path=true;
	    		//used to check whether we can grant connection or not and store it in valid_path variable
	    		can_grant(topology,first_path,valid_path,dest,band_eq,num_edges);
	    		if(valid_path)
	    		{
	    			update(topology,first_path,source,dest,i,0,band_eq,num_edges,argv);//update the forwarding table and link capacity
	    		}
	    		else
	    		{
	    			valid_path=true;
	    			//check the grant permission for second shortest path
	    			can_grant(topology,second_path,valid_path,dest,band_eq,num_edges);
	    			if(valid_path)
	    			{
	    				update(topology,second_path,source,dest,i,0,band_eq,num_edges,argv);//update the forwarding table and link capacity
	    			}
	    		}	
	    	}
    		//optimistic approach ends
    	}

    	/////////////////////////////////////print the required output////////////////////////////////////////////////
    	print_output(argv);

    	//////////////////////////////////////updating the forwarding table////////////////////////////////////////////
    	out3.open(argv[8]);
	    if (!out3) 
	    {
	      cout << "Failed to open forwarding table file..";
	      exit(0);
	    }
	    //updating forwarding table
    	update_forwarding_table(num_nodes);
  	}
  	return 0;
}
