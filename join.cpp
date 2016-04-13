#include <iostream>
#include <stdio.h>
#include <string>
#include <math.h>
#include <vector>
#include <sstream>
#include <stdlib.h>
#include <fstream>
using namespace std;


namespace patch
{
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
}

//relation1 file, relation 2 file, record size, page size, number of available pages, maximum number
//of hashing rounds
//int results;
bool chart=true;
FILE* result;
ofstream log_file;
ofstream quiz_output;
//FILE* log_file;

//int hash(int value,int rounds,int size)
//{
  
/*  long long ans=1;
    for(int i=1;i<=rounds;i++)
    {
      ans=ans*value+value*(value + rounds);
      ans=ans%size;
    }
  */
  //  return (value*rounds+value)%size;
//long long value^rounds + value*(value + rounds)
 // return int(value*rounds*3.14157)%size;
//}

int hash(int value,int rounds,int max_buckets)
{

vector<int> bin;

int sig_bits=rounds+max_buckets;

for(int i=0;i<16;i++)
{
  bin.push_back(0);
}

int temp= value;

for(int i=0;i<16;i++)
{
  bin[15-i]=temp%2;
  temp=temp/2;
//  cout<<bin[i];
  if(temp==0)
    break;
}

int ans=0;
for(int i=0;i<sig_bits;i++)
{
  ans=ans+bin[15-i]*pow(2,i);
  //cout<<bin[i];
}
//cout<<"k+ i value"<<ans<<endl;
ans=ans%max_buckets;
//cout<<ans<<endl;
//cout<<endl<<endl;
return ans;
}

void hash_relation(string input_file,int record_size,int page_size,int max_avail_pages,int max_hash_rounds,int rounds,int relation)
{
  FILE* f1;
//max_avail_pages=5;
//page_size=4;
//record_size=2;

int records_per_page=floor(page_size/record_size);

//int rounds=1;

int max_buckets=max_avail_pages-1;
FILE* bucket[max_buckets];
FILE* t_pages;
//char buffer[50];

//int relation=2;
//char file_name[50];
vector<int> t_records,t_flush;
cout<<endl;
log_file<<endl;

cout<<"Performing hashing round : "<<rounds<<endl;
log_file<<"Performing hashing round : "<<rounds<<endl;
//quiz_output<<"Round : "<<rounds<<endl;

string temp,file_name;
temp=input_file+".txt";
cout<<"Reading: "<<temp<<endl<<endl;
log_file<<"Reading: "<<temp<<endl<<endl;
//quiz_output<<"Reading: "<<temp<<endl<<endl;

f1=fopen(temp.c_str(),"r");

cout<<"Creating following files:"<<endl;
log_file<<"Creating following files:"<<endl;

for(int i=0;i<max_buckets;i++)
       {
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        //file_name="rel"+string(relation)+"_round"+string(itoa(rounds))+"_bucket"+string(itoa(i))+".txt";
        //sprintf(file_name,"rel%d_round%d_bucket%d.txt",relation,rounds,i);
        file_name=input_file+"_bucket"+patch::to_string(i);
        temp=file_name+".txt";
        cout<<temp<<endl;
        log_file<<temp<<endl;
        bucket[i]=fopen(temp.c_str(),"w");
        
        t_records.push_back(0);
        t_flush.push_back(0);
     }
     cout<<endl;
     log_file<<endl;
//sprintf(file_name,"rel%d_round%d.txt",relation,rounds);
file_name=input_file+"_round"+patch::to_string(rounds);
temp=file_name+".txt";
//fwrite(result,"%s\n",temp);
//fputs(temp.c_str(),result);
//fprintf(result,"\n");

t_pages=fopen(temp.c_str(),"w");
//cout<<"hello";
  if(f1 == NULL) 
    {
      cout<<"Error opening file"<<endl;
      log_file<<"Error opening file"<<endl;
      return ;
    }
    else
    {
      int count =1;
      vector<int> data[max_buckets];

     while ( ! feof (f1) )
     {
       int d;
       fscanf(f1,"%d",&d);
       

       if(feof(f1))
        break;

      //  cout<<d<<endl;
      int t1=hash(d,rounds,max_buckets);
      //int t1=floor((d+ rounds*3.14157));
      //t1=t1%max_buckets;
        cout<<"Tuple "<<count<<" : value "<<d<<"  Mapped to bucket : "<< t1<<endl ;
        log_file<<"Tuple "<<count<<" : value "<<d<<"  Mapped to bucket : "<< t1<<endl ;
        
        //fprintf(bucket[t1], "%d\n", d);
        data[t1].push_back(d);
        t_records[t1]++;
        t_flush[t1]++;

        if(int((t_flush[t1])/records_per_page)==1)
        {
          cout<<"Page for bucket "<<t1<<" is full. Flushing to secondary memory"<<endl;
          log_file<<"Page for bucket "<<t1<<" is full. Flushing to secondary memory"<<endl;
  
          for(int i=0;i<data[t1].size();i++)
            {
              fprintf(bucket[t1], "%d\n", data[t1][i]);
            }  
          data[t1].clear();
          t_flush[t1]=0;
        }

        count++;      
      }

cout<<"Flushing all buckets to secondary memory"<<endl;
log_file<<"Flushing all buckets to secondary memory"<<endl;

for(int i=0;i<max_buckets;i++)
{
  for(int j=0;j<data[i].size();j++)
  {
       fprintf(bucket[i], "%d\n", data[i][j]);
  }
  data[i].clear();
}      

       rounds++;
     //  cout<<d<<endl;
     }
cout<<endl;
log_file<<endl;
for(int i=0;i<t_records.size();i++)
{
  cout<<"Bucket "<<i<<" : "<<"Pages required : "<< int(ceil(float(t_records[i])/records_per_page))<<endl;
  log_file<<"Bucket "<<i<<" : "<<"Pages required : "<< int(ceil(float(t_records[i])/records_per_page))<<endl;
  fprintf(t_pages,"%d\n",int(ceil(float(t_records[i])/records_per_page)));
}

for(int i=0;i<max_buckets;i++)
{
  fclose(bucket[i]);
}

fclose(t_pages);
fclose (f1);
cout<<endl;
}


void perform_hashing(string input_file1,string input_file2,int record_size1,int record_size2,int page_size,int max_avail_pages,int max_hash_rounds,int rounds)
{
  //int max_avail_pages=5;
//int rounds=1;
string temp;

if(rounds > max_hash_rounds)
{
  quiz_output<<"Unable to perform join in "<<max_hash_rounds<<" hashing rounds"<<endl<<endl;
  cout<<"Unable to perform join in "<<max_hash_rounds<<" hashing rounds"<<endl<<endl;
  log_file<<"Unable to perform join in "<<max_hash_rounds<<" hashing rounds"<<endl<<endl;
  chart=false;
  return;
}

int max_buckets=max_avail_pages-1;
//temp = input_file1+".txt";  
 hash_relation(input_file1,record_size1,page_size,max_avail_pages,max_hash_rounds,rounds,1);
 hash_relation(input_file2,record_size2,page_size,max_avail_pages,max_hash_rounds,rounds,2);
FILE *f1,*f2;
string file_name,file_name1;
//char file_name[50],file_name1[50];
 // sprintf(file_name,"rel1_round%d.txt",rounds);
file_name1=input_file1+"_round"+patch::to_string(rounds);
temp=file_name1+".txt";
//fputs(temp.c_str(),log_file);
//fprintf(log_file,"\n");
//cout<<"opening file "<<temp;
f1=fopen(temp.c_str(),"r");
//  sprintf(file_name,"rel2_round%d.txt",rounds);
file_name1=input_file2+"_round"+patch::to_string(rounds);
temp=file_name1+".txt";
//cout<<"opening file "<<temp;
f2=fopen(temp.c_str(),"r");

  if(f1 == NULL) 
    {
      cout<<"Error opening file"<<endl;
      log_file<<"Error opening file"<<endl;
      return ;
    }

      if(f2 == NULL) 
    {
      cout<<"Error opening file"<<endl;
      log_file<<"Error opening file"<<endl;
      return ;
    }
  
 FILE *out1,*out2;
    int b=0;

    cout<<"For "+input_file1+" and "+input_file2<<endl<<endl;
    log_file<<"For "+input_file1+" and "+input_file2<<endl<<endl;
    quiz_output<<"round : "<<rounds<<endl<<endl;
    quiz_output<<"Processed "<<input_file1<<endl;
    quiz_output<<"Processed "<<input_file2<<endl;

      while ( ! feof (f1) )
     {
       int d1,d2;
       fscanf(f1,"%d",&d1);
       fscanf(f2,"%d",&d2);
      // cout<<d1<<"  "<<d2<<endl;  
       if(feof(f1))
        break;

        cout<<"For bucket "<<b<<" :"  <<endl;
     log_file<<"For bucket "<<b<<" :"  <<endl;    
       // cout<<d1+d2<<endl;
        if(d1+d2>max_buckets)
        {
          cout<<"Size of relation "<<input_file1<<" : "<<d1<<endl;
          cout<<"Size of relation "<<input_file2<<" : "<<d2<<endl;
          cout<<"Total pages required : "<<d1+d2<<endl;
          cout<<"Cannot perform in memory join for bucket "<<b<<endl;
          cout<<"Total pages available: " <<max_avail_pages<<endl;
          log_file<<"Size of relation "<<input_file1<<" : "<<d1<<endl;
          log_file<<"Size of relation "<<input_file2<<" : "<<d2<<endl;
          log_file<<"Total pages required : "<<d1+d2<<endl;
          log_file<<"Cannot perform in memory join for bucket "<<b<<endl;
          log_file<<"Total pages available: " <<max_avail_pages<<endl;
          

            file_name=input_file1+"_bucket"+patch::to_string(b);
          temp=file_name+".txt";
          out1=fopen(temp.c_str(),"r");
          //sprintf(file_name,"rel2_round%d_bucket%d.txt",rounds,b);
          file_name=input_file2+"_bucket"+patch::to_string(b);
          temp=file_name+".txt";
          out2=fopen(temp.c_str(),"r");  
          
            if(out1 == NULL) 
            {
             cout<<"Error opening file"<<endl;
              return ;
            }

           if(out2 == NULL) 
           {
            cout<<"Error opening file"<<endl;
            return ;
          }
        vector<int> vec1;
        vector<int> vec2;
  
     while ( ! feof (out1) )
     {
       int d1,d2;
       fscanf(out1,"%d",&d1);
      
       if(feof(out1))
        break;
      vec1.push_back(d1);
    }

    while ( ! feof (out2) )
     {
       int d1,d2;
       fscanf(out2,"%d",&d2);

       if(feof(out2))
        break;

      vec2.push_back(d2);
    }
//sprintf(file_name,"result%d",results);
//FILE* r1=fopen(file_name,"w");
//results++;
//Zcout<<endl;

bool flag=false; 

quiz_output<<"bucket "<<b<<" :"<<endl;
quiz_output<<input_file1<<" Keys :";
  
  for(int i=0;i<vec1.size();i++)
    {
      quiz_output<<vec1[i]<<" ";
    }
quiz_output<<" Pages : "<<d1;
quiz_output<<endl;

quiz_output<<input_file2<<" Keys :";
    for(int i=0;i<vec2.size();i++)
    {
      quiz_output<<vec2[i]<<" ";
    }
quiz_output<<"Pages : "<<d2<<endl;
quiz_output<<"In memory Join :No"<<endl<<endl;


        //  quiz_output<<"In memory join: No"<<endl;
          //sprintf(file_name,"rel1_round%d_bucket%d.txt",rounds,b);
          file_name=input_file1+"_bucket"+patch::to_string(b);
          //temp=file_name+".txt";
         // hash_relation(temp.c_str(),record_size1,page_size,max_avail_pages,max_hash_rounds,rounds+1,1);
          //sprintf(file_name1,"rel2_round%d_bucket%d.txt",rounds,b);
          file_name1=input_file2+"_bucket"+patch::to_string(b);
          //temp=file_name+".txt";
          //hash_relation(temp.c_str(),record_size2,page_size,max_avail_pages,max_hash_rounds,rounds+1,2);
          perform_hashing(file_name,file_name1,record_size1,record_size2,page_size,max_avail_pages,max_hash_rounds,rounds+1); 
         // cout<<endl;         
        }
        else
        {
          cout<<"Size of relation "<<input_file1<<" : "<<d1<<endl;
          cout<<"Size of relation "<<input_file2<<" : "<<d2<<endl;
          cout<<"Total pages required "<<d1+d2<<endl;
          cout<<"Total pages available: " <<max_avail_pages<<endl;
          cout<<"Performing in memory join"<<endl;         
          log_file<<"Size of relation "<<input_file1<<" : "<<d1<<endl;
          log_file<<"Size of relation "<<input_file2<<" : "<<d2<<endl;
          log_file<<"Total pages required "<<d1+d2<<endl;
          log_file<<"Total pages available: " <<max_avail_pages<<endl;
          log_file<<"Performing in memory join"<<endl;         

          //sprintf(file_name,"rel1_round%d_bucket%d.txt",rounds,b);
          //file_name="rel1_round"+to_string(rounds)+
          file_name=input_file1+"_bucket"+patch::to_string(b);
          temp=file_name+".txt";
          out1=fopen(temp.c_str(),"r");
          //sprintf(file_name,"rel2_round%d_bucket%d.txt",rounds,b);
          file_name=input_file2+"_bucket"+patch::to_string(b);
          temp=file_name+".txt";
          out2=fopen(temp.c_str(),"r");  
          
            if(out1 == NULL) 
            {
             cout<<"Error opening file"<<endl;
              return ;
            }

           if(out2 == NULL) 
           {
            cout<<"Error opening file"<<endl;
            return ;
          }
        vector<int> vec1;
        vector<int> vec2;
  
     while ( ! feof (out1) )
     {
       int d1,d2;
       fscanf(out1,"%d",&d1);
      
       if(feof(out1))
        break;
      vec1.push_back(d1);
    }

    while ( ! feof (out2) )
     {
       int d1,d2;
       fscanf(out2,"%d",&d2);

       if(feof(out2))
        break;

      vec2.push_back(d2);
    }
//sprintf(file_name,"result%d",results);
//FILE* r1=fopen(file_name,"w");
//results++;
//Zcout<<endl;

bool flag=false; 

quiz_output<<"bucket "<<b<<" :"<<endl;
quiz_output<<input_file1<<" Keys :";
  
  for(int i=0;i<vec1.size();i++)
    {
      quiz_output<<vec1[i]<<" ";
    }
quiz_output<<" Pages : "<<d1;
quiz_output<<endl;

quiz_output<<input_file2<<" Keys :";
    for(int i=0;i<vec2.size();i++)
    {
      quiz_output<<vec2[i]<<" ";
    }
quiz_output<<"Pages : "<<d2<<endl;
quiz_output<<"In memory Join Yes"<<endl<<endl;


    for(int i=0;i<vec1.size();i++)
    {
      for(int j=0;j<vec2.size();j++) 
      {
            if(vec1[i]==vec2[j])
            {
              fprintf(result,"%d\n",vec1[i]);
              if(!flag)
               { 
                cout<<"Matching tuples are : ";
                log_file<<"Matching tuples are : ";   
                flag=true;
                }
                cout<<vec1[i]<<" ";
                log_file<<vec1[i]<<" ";
            }

      }
    }
   
    if(!flag)
    {
      cout<<"No matching tuple. No further processing required";
     log_file<<"No matching tuple. No further processing required";

    }
 cout<<endl;
 log_file<<endl;
//fclose(r1);
fclose(out1);
fclose(out2);
}

cout<<endl;
log_file<<endl;  
        b++;
      }


fclose(f1);
fclose(f2);
}


void join(string input_file1,string input_file2,int record_size1,int record_size2,int page_size,int max_avail_pages,int max_hash_rounds)
{
  FILE *f1,*f2;
  string temp=input_file1+".txt";
  f1=fopen(temp.c_str(),"r");
  temp=input_file2+".txt";
  f2=fopen(temp.c_str(),"r");
  
  int records_per_page2=floor(page_size/record_size1);
  int records_per_page1=floor(page_size/record_size2);

   vector<int> vec1,vec2;

   if(f1 == NULL) 
    {
      cout<<"Error opening file"<<endl;
      log_file<<"Error opening file"<<endl;
      return ;
    }

      if(f2 == NULL) 
    {
      cout<<"Error opening file"<<endl;
      log_file<<"Error opening file"<<endl;
      return ;
    }

    int pages1=0,pages2=0;

 while ( ! feof (f1) )
     {
       int d;
       fscanf(f1,"%d",&d);
       

       if(feof(f1))
        break;
      vec1.push_back(d);
      pages1++;
    }

while ( ! feof (f2) )
     {
       int d;
       fscanf(f2,"%d",&d);
       

       if(feof(f2))
        break;
      vec2.push_back(d);
      pages2++;
    }
    
pages1=ceil(float(pages1)/records_per_page1);
pages2=ceil(float(pages2)/records_per_page2);
bool flag=false;

if(pages1+pages2<max_avail_pages)
{        

          cout<<"Size of relation "<<input_file1<<" : "<<pages1<<endl;
          cout<<"Size of relation "<<input_file2<<" : "<<pages2<<endl; 
          cout<<"Total pages required "<<pages1+pages2<<endl;
          cout<<"Total pages available: " <<max_avail_pages<<endl;
          cout<<"Performing in memory join"<<endl;
          log_file<<"Size of relation "<<input_file1<<" : "<<pages1<<endl;
          log_file<<"Size of relation "<<input_file2<<" : "<<pages2<<endl;         
          log_file<<"Total pages required "<<pages1+pages2<<endl;
          log_file<<"Total pages available: " <<max_avail_pages<<endl;
          log_file<<"Performing in memory join"<<endl;         

          quiz_output<<"round "<<0<<endl<<endl;
          quiz_output<<"Processed "<<input_file1<<endl;
          quiz_output<<"Processed "<<input_file2<<endl;

      quiz_output<<input_file1<<" keys ";
   for(int i=0;i<vec1.size();i++)
    {
      quiz_output<<vec1[i]<<" ";
}
quiz_output<<"Pages "<<pages1<<endl;

  quiz_output<<input_file1<<" keys ";
   for(int i=0;i<vec2.size();i++)
    {
      quiz_output<<vec2[i]<<" ";
}
quiz_output<<"Pages "<<pages2<<endl;

quiz_output<<"In memory join : yes"<<endl<<endl;

   for(int i=0;i<vec1.size();i++)
    {
      for(int j=0;j<vec2.size();j++) 
      {
            if(vec1[i]==vec2[j])
            {
              fprintf(result,"%d\n",vec1[i]);
              if(!flag)
               { 
                cout<<"Matching tuples are : ";
                log_file<<"Matching tuples are : ";   
                flag=true;
                }
                cout<<vec1[i]<<" ";
                log_file<<vec1[i]<<" ";
            }

      }
    }
}
else
{
  perform_hashing(input_file1,input_file2,record_size1,record_size2,page_size,max_avail_pages,max_hash_rounds,1);
}

}

int main()
{
//results=0;
result=fopen("result.txt","w");
log_file.open("log_file.txt");
quiz_output.open("quiz_output.txt");
int record_size1;
int record_size2;
int page_size;
int max_avail_pages;
int max_hash_rounds;
string file1;
string file2;

cout<<"Enter relation 1 name"<<endl;
cin>>file1;

cout<<"Enter relation 2 name"<<endl;
cin>>file2;

cout<<"Enter record_size1"<<endl;
cin>>record_size1;

cout<<"Enter record_size2"<<endl;
cin>>record_size2;

cout<<"Enter page size"<<endl;
cin>>page_size;

cout<<"Enter maximum available pages"<<endl;
cin>>max_avail_pages;

cout<<"Enter maximum hashing rounds"<<endl;
cin>>max_hash_rounds;


//perform_hashing(file1,file2,record_size1,record_size2,page_size,max_avail_pages,max_hash_rounds,1);
join(file1,file2,record_size1,record_size2,page_size,max_avail_pages,max_hash_rounds);
//hash_relation("relation1",180,400,5,3,1,1);
if(!chart)
{
  cout<<endl;
  cout<<"Unable to perform join in "<<max_hash_rounds<<" hashing rounds"<<endl<<endl;
  log_file<<"Unable to perform join in "<<max_hash_rounds<<" hashing rounds"<<endl<<endl;
}
fclose(result);
log_file.close();
quiz_output.close();

} 
