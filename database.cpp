// itemset_specific.cpp
// Made by Benjamin Negrevergne
// Started on  Fri Sep  3 14:09:56 2010
#include <sstream>
#include <fstream>
#include <set>
#include "database.hpp"
#include "utils.hpp"


using namespace std; 

void read_transaction_table(TransactionTable *tt, const char *filename){

  ifstream ifs (filename , ifstream::in);
  int nb_items = 0; 
  int nb_trans = 0; 
  while (ifs.good()){
    string line; 
    stringstream ss; 
    Transaction t;
    int item; 
    ss<<skipws; 
    getline(ifs, line); 
    ss<<line; 
    ss>>item;
    while(ss.good()){
      t.push_back(item); 
      ++nb_items; 
      ss>>item;
    }
    if(t.size() != 0){
      tt->push_back(t); 
      ++nb_trans; 
    }
  }

  cout<<"Data loaded, "<<nb_items<<" items within "<<nb_trans<<" transactions."<<endl;
  ifs.close();
}

void print_transaction(const Transaction &t){
  for(int i = 0; i < t.size(); i++){
    cout<<t[i]<<" "; 
  }
}

void print_transaction_table(const TransactionTable &tt){
  for(int i = 0; i < tt.size(); i++){
    cout<<i<<" : "; print_transaction(tt[i]); 
    cout<<endl; 
  }
}


void transpose(const TransactionTable &tt, TransactionTable *ot){
  assert(!ot->size()); 
  for(int i = 0; i < tt.size(); i++)
    for(int j = 0; j < tt[i].size(); j++){
      int v = tt[i][j]; 
      if(v >= ot->size())
	ot->resize(v+1);
      (*ot)[v].push_back(i); 
    }
}

void database_build_reduced(TransactionTable *new_tt, const TransactionTable &tt,
			    const Transaction &occurence){
  //TODO remove pattenr from db
  new_tt->reserve(occurence.size()); 
  for(Transaction::const_iterator occ_it = occurence.begin(); 
      occ_it != occurence.end(); ++occ_it){
    new_tt->push_back(tt[*occ_it]);
  }
}


void database_occuring_elements(set_t *elements, 
			       const TransactionTable &tt, const Transaction &occurences){
  
  std::set<element_t> uniq_set; 
  for(Transaction::const_iterator occ_it = occurences.begin(); 
      occ_it != occurences.end(); ++occ_it){
    Transaction::const_iterator trans_end = tt[*occ_it].end(); 
    for(Transaction::const_iterator trans_it = tt[*occ_it].begin(); 
	trans_it != trans_end; ++trans_it)
      uniq_set.insert(*trans_it);
  }
  
  elements->reserve(uniq_set.size()); 
  for(std::set<element_t>::const_iterator set_it = uniq_set.begin(); set_it != uniq_set.end(); ++set_it){
    elements->push_back(*set_it);
  }
}

#if 0 
int main(int argc, char **argv){
  TransactionTable tt; 
  read_transaction_table(&tt, "test.dat"); 
  print_transaction_table(tt); 
}
#endif 

int get_set_presence_1d(const Transaction &t, const set_t &set){
  return is_included_1d(t, set); 
}

int is_included_1d(const Transaction &t, const set_t &set){
  assert(is_sorted(t)); 
  if(set.size()==0)
    return 1; 
  if(t.size() == 0)
    return 0; 
    
  //  assert(is_sorted(t));
  // if(!is_sorted(set))
  //   set_print(set); 
  assert(is_sorted(set)); 
  Transaction::const_iterator it = t.begin(); 
  set_t::const_iterator set_it = set.begin(); 

  do{
    if(*set_it == *it)
      if(++set_it == set.end())
	return 1;
    ++it; 
  }while(it != t.end());
 
  return 0; 
}

int count_inclusion_1d();

int count_inclusion_2d(const TransactionTable &tt, const set_t &set){
  int count  = 0; 
  for(TransactionTable::const_iterator it = tt.begin(); it != tt.end(); ++it){
    if(is_included_1d(*it, set))
      count++; 
  }
  return count; 
}

void get_occurences_2d(const TransactionTable &tt, const set_t &set, Occurence *oc){
  int i = 0;
  for(TransactionTable::const_iterator it = tt.begin(); it != tt.end(); ++it, ++i){
    if(get_set_presence_1d(*it, set))
      oc->push_back(i); 
  }
}


set_t canonical_form(set_t set){
  return set; 
}

element_t canonical_transform_element(const set_t &set, element_t &element){
  return element; 
}

set_t canonical_form(set_t set, element_t *element){
  return set; 
}

