#include "CondFormats/RunInfo/interface/RunSummary.h"
RunSummary::RunSummary(){}

RunSummary * RunSummary::Fake_RunSummary(){
  RunSummary * sum = new RunSummary();
  sum->m_run=-1;
  sum->m_hltkey="null";
  sum->m_start_time_str="null";
  sum->m_stop_time_str="null";
  sum->m_name="null";
  return sum; 
}



void RunSummary::printAllValues() const{
    std::cout<<"run number: " <<m_run << std::endl;
    std::cout<<"run name: " <<m_name << std::endl;
    std::cout<<"run start time as timestamp: "<<m_start_time_ll<<std::endl;
    std::cout<<"run start time as date: "<<m_start_time_str<<std::endl;
    std::cout<<"run stop time as timestamp: "<<m_stop_time_ll<< std::endl;
    std::cout<<"run stop time as date: "<<m_stop_time_str<<std::endl;
    std::cout<<"lumisection in the run: "<<m_lumisections<<std::endl;
    std::cout<<"run hltkey: "<<m_hltkey<<std::endl;
    std::cout<<"run number of events according hlt: "<<m_nevents<<std::endl;
    std::cout<<"hlt rate: "<<m_rate<<std::endl;
    std::cout<<"ids of subdetectors in run: "<<std::endl;
for (int i : m_subdt_in){
  std::cout<<"---> "<<i<<std::endl;
  }
  }

std::vector<std::string> RunSummary::getSubdtIn() const{
  std::vector<std::string> v; 
  for (int i : m_subdt_in){
    if (i==0) {
      v.push_back("PIXEL");
    }
    if (i==1) {
      v.push_back("TRACKER");
    }
    if (i==2) {
      v.push_back("ECAL");
    }
    if (i==3) {
      v.push_back("HCAL");
    }
    
    if (i==4) {
      v.push_back("DT");
    }  
    if (i==5) {
      v.push_back("CSC");
    }   
    if (i==6) {
      v.push_back("RPC");
    }  
  }
  return v;
}
