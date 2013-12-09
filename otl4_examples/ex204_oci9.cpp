#include <iostream>
using namespace std;
#include <stdio.h>

#define OTL_ORA9I // Compile OTL 4.0/OCI9i

// Uncomment the line below in case of OCI8.1.7 that 
// used against Oracle 9i.
//#define OTL_ORA8I

#define OTL_ORA_TIMESTAMP
#include <otlv4.h> // include the OTL 4.0 header file

otl_connect db; // connect object

void insert(void)
// insert rows into table
{ 
 otl_stream o(3, // stream buffer size in logical rows.
              "insert into test_tab "
              "values(:f1<int>,:f2<timestamp>)", 
                 // INSERT statement
              db // connect object
             );

 otl_datetime tm;

 for(int i=1;i<=10;++i){
  tm.year=1998;
  tm.month=10;
  tm.day=19;
  tm.hour=23;
  tm.minute=12;
  tm.second=12;
  tm.frac_precision=6; // microseconds
  tm.fraction=123456;
  o<<i<<tm;
 }
}

void select(void)
{ 
 otl_stream i(5, // stream buffer size in logical rows
              "select * from test_tab",
                 // SELECT statement
              db // connect object
             ); 
 
 int f1;
 otl_datetime f2;

 // Second's precision needs to be specified BEFORE the stucture can be
 // used either for writnig or reading timestamp values, which have second's 
 // fractional part.
 f2.frac_precision=6; // microseconds

 while(!i.eof()){ // while not end-of-data
  i>>f1>>f2;
  cout<<"f1="<<f1;
  cout<<", f2="<<f2.month<<"/"<<f2.day<<"/"
      <<f2.year<<" "<<f2.hour<<":"<<f2.minute<<":"
      <<f2.second<<"."
      <<f2.fraction;
  cout<<endl;
 }
 
}

int main()
{
 otl_connect::otl_initialize(); // initialize OCI environment
 try{

  db.rlogon("scott/tiger"); // connect to Oracle

  otl_cursor::direct_exec
   (
    db,
    "drop table test_tab",
    otl_exception::disabled // disable OTL exceptions
   ); // drop table

  otl_cursor::direct_exec
   (
    db,
    "create table test_tab(f1 int, f2 timestamp)"
    );  // create table

  insert(); // insert records into table
  select(); // select records from table

 }

 catch(otl_exception& p){ // intercept OTL exceptions
  cerr<<p.msg<<endl; // print out error message
  cerr<<p.stm_text<<endl; // print out SQL that caused the error
  cerr<<p.sqlstate<<endl; // print out SQLSTATE message
  cerr<<p.var_info<<endl; // print out the variable that caused the error
 }

 db.logoff(); // disconnect from Oracle

 return 0;

}
