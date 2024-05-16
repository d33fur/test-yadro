#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <sstream>
#include <cctype>
#include <ranges>
#include <numeric>
#include <queue>
#include <iomanip>

class timeType 
{
  public:
    timeType() : 
      hours_(0), 
      minutes_(0),
      str_("00:00")
    {
    }

    timeType(const std::string& str) :
      hours_(std::stoi(str.substr(0, 2))),
      minutes_(std::stoi(str.substr(3, 5))),
      str_(str) 
    {
    }


    bool operator<(const timeType& r) const 
    {
      return hours_ * 60 + minutes_ < r.hours_ * 60 + r.minutes_;
    }

    timeType operator-(const timeType& r) const
    {
      unsigned int total = (hours_ * 60 + minutes_) - (r.hours_ * 60 + r.minutes_);
      unsigned int newHours = total / 60;
      unsigned int newMinutes = total % 60;
      std::stringstream newStr;
      newStr << std::setw(2) << std::setfill('0') << newHours << ":"
             << std::setw(2) << std::setfill('0') << newMinutes;
      return timeType(newStr.str());
    }

    timeType operator+(const timeType& r) const
    {
      unsigned int total = (hours_ * 60 + minutes_) + (r.hours_ * 60 + r.minutes_);
      unsigned int newHours = total / 60;
      unsigned int newMinutes = total % 60;
      std::stringstream newStr;
      newStr << std::setw(2) << std::setfill('0') << newHours << ":"
             << std::setw(2) << std::setfill('0') << newMinutes;
      return timeType(newStr.str());
    }


    unsigned int hours_;
    unsigned int minutes_;
    std::string str_;
};


class tableType 
{
  public:
    unsigned int money_ = 0;
    timeType time_;
    timeType sumOfTime_;
    bool isFree_ = 1;
};


class eventType 
{
  public:
    eventType(const std::vector<std::string>& v) :
      time_(v[0]),
      id_(std::stoi(v[1])),
      clientName_(v[2]) 
    {
      str_ = std::accumulate(v.begin(), v.end(), std::string{},
        [](const std::string& a, const std::string& b) 
      {
          return a.empty() ? b : a + " " + b;
      });

      if(v.size() == 4)
      {
        table_ = std::stoi(v[3]);
      }
    }


    timeType time_;
    unsigned int id_;
    unsigned int table_;
    std::string clientName_;
    std::string str_;
};


class cyberclub 
{
  public:
    cyberclub(const std::vector<std::string>& v) :
      worktime_({v[1].substr(0, 5), v[1].substr(6, 11)}),
      hourPrice_(std::stoi(v[2])) 
    {
      if(!isNumber(v[0]))
      {
        std::cout << v[0] << std::endl;
        throw std::runtime_error("err");
      }
      else if(std::stoi(v[0]) <= 0)
      {
        std::cout << v[0] << std::endl;
        throw std::runtime_error("err");
      }
      else
      {
        numOfTables_ = std::stoi(v[0]);
      }
    }

    bool isNumber(const std::string &s) 
    {
      return !s.empty() && std::all_of(s.begin(), s.end(), ::isdigit);
    }

    bool isNameValid(const std::string& line) 
    {
      for(const auto& i : line)
      {
        if(i != '_' 
          && i != '-'
          && !std::isdigit(i)
          && !(std::isalpha(i) && std::islower(i)))
        {
          return 0; 
        }
      }
      return 1;
    }
    
    bool loadLogLine(const std::string& line) 
    {
      std::string s;
      std::stringstream ss(line);
      std::vector<std::string> v;
      while(getline(ss, s, ' ')) 
      {
        v.push_back(s);
      }

      // size check
      if(   v.size() != 3 
        && (v.size() != 4 || v[1] != "2")) 
      {
        err(line);
        return 1;
      }

      // time check
      if(  v[0].length() != 5      
        || v[0][2] != ':'           
        || !std::isdigit(v[0][0]) 
        || !std::isdigit(v[0][1]) 
        || !std::isdigit(v[0][3]) 
        || !std::isdigit(v[0][4])
        || std::stoi(v[0].substr(0, 2)) > 23
        || std::stoi(v[0].substr(3, 5)) > 59) 
      {
        err(line);
        return 1;
      }

      // event check
      if(  v[1] != "1" 
        && v[1] != "2" 
        && v[1] != "3" 
        && v[1] != "4"
        || (v[1] == "2" && (std::stoi(v[3]) > numOfTables_ || std::stoi(v[3]) < 1)))
      {
        err(line);
        return 1;
      }


      // client check
      if(!isNameValid(v[2]))
      {
        err(line);
        return 1;
      }

      eventType e(v);

      // subsequence check
      if( !events_.empty())
      {
        if(e.time_ < events_.back().time_)
        {
          err(line);
          return 1;
        }

      }

      events_.push_back(e);

      return 0;
    }

    void err(const std::string& line) 
    {
      std::cout << line << std::endl;
    }

    void checkEvents() {
      for(const auto& e : events_)
      {
        std::cout << e.str_ << std::endl;
        (this->*eventsFunc_[e.id_ - 1])(e);
      }
    }

    void event1(const eventType& e) {
      if(e.time_ < worktime_.first || worktime_.second < e.time_)
      {
        std::cout << e.time_.str_ << " 13 NotOpenYet" << std::endl;
      }
      else if(clients_.find(e.clientName_) != clients_.end())
      {
        std::cout << e.time_.str_ << " 13 YouShallNotPass" << std::endl;
      }
      else 
      {
        clients_[e.clientName_] = 0;
      }
    }

    void event2(const eventType& e) {
      if(clients_.find(e.clientName_) == clients_.end())
      {
        std::cout << e.time_.str_ << " 13 ClientUnknown" << std::endl;
      }
      else if(tables_[e.table_].isFree_)
      {
        tables_[e.table_].isFree_ = 0;
        tables_[e.table_].time_ = e.time_;
        tables_[clients_[e.clientName_]].isFree_ = 1;
        clients_[e.clientName_] = e.table_;
      }
      else
      {
        std::cout << e.time_.str_ << " 13 PlaceIsBusy" << std::endl;
      }
    }

    void event3(const eventType& e) {
      bool isFreeTableExists = 0;
      unsigned int sum = 0;
      for(const auto& i : tables_)
      {
        if(i.first != 0) sum++;
      }

      tables_[0].isFree_ = 0;

      if(sum < numOfTables_)
      {
        isFreeTableExists = 1;
      }
      else {
        for(const auto& i : tables_ | std::views::values)
        {
          if(i.isFree_) {
            isFreeTableExists = 1;
            tables_[0].isFree_ = 1;
            break;
          }
        }
      }

      if(isFreeTableExists)
      {
        clients_[e.clientName_] = 0;
        std::cout << e.time_.str_ << " 13 ICanWaitNoLonger!" << std::endl;
      }
      else if(clientsQ_.size() > numOfTables_)
      {
        auto it = clients_.find(e.clientName_);
        clients_.erase(it); // сомнительно
        std::cout << e.time_.str_ << " 11 " << e.clientName_ << std::endl;
      }
      else
      { 
        clients_[e.clientName_] = 0;
        clientsQ_.push(e.clientName_);
      }
      
    }

    void event4(const eventType& e) {
      if(clients_.find(e.clientName_) == clients_.end())
      {
        std::cout << e.time_.str_ << " 13 ClientUnknown" << std::endl;
      }
      else
      {
        tables_[clients_[e.clientName_]].isFree_ = 1;
        timeType t = e.time_ - tables_[clients_[e.clientName_]].time_;
        tables_[clients_[e.clientName_]].sumOfTime_ = tables_[clients_[e.clientName_]].sumOfTime_ + t;
        tables_[clients_[e.clientName_]].money_ += (t.hours_ + 1) * hourPrice_;
        unsigned int num = clients_[e.clientName_];

        auto it = clients_.find(e.clientName_);

        clients_.erase(it);
        // clients[e.clientName] = 0;

        if(!clientsQ_.empty()) // мб учесть, что кто то может уйти прямо из очереди
        {
          tables_[num].isFree_ = 0;
          tables_[num].time_ = e.time_;
          clients_[clientsQ_.front()] = num;
          std::cout << e.time_.str_ << " 12 " << clientsQ_.front() << ' ' << num << std::endl;

          clientsQ_.pop();

        }
      }
    }

    void dayEnd()
    {
      std::vector<std::string> v;
      while(!clientsQ_.empty())
      {
        v.push_back(clientsQ_.front());
        clientsQ_.pop();
      }

      for(const auto& c : clients_)
      {
        v.push_back(c.first);
      }

      std::sort(v.begin(), v.end());

      for(const auto& c : v)
      { 
        timeType t = worktime_.second - tables_[clients_[c]].time_;
        tables_[clients_[c]].sumOfTime_ = tables_[clients_[c]].sumOfTime_ + t;
        tables_[clients_[c]].money_ += (t.hours_ + 1) * hourPrice_;
        std::cout << worktime_.second.str_ << " 11 " << c << std::endl;
      }
    }

    void daySummary()
    {
      for(const auto& [i, j] : tables_)
      {
        if(i != 0)
          std::cout << i << ' ' << j.money_ << ' ' << j.sumOfTime_.str_ << std::endl;
      }
    }


    unsigned int numOfTables_;
    unsigned int hourPrice_;
    std::pair<timeType, timeType> worktime_;
    std::map<unsigned int, tableType> tables_;
    std::vector<eventType> events_;
    std::map<std::string, unsigned int> clients_;
    std::queue<std::string> clientsQ_;

    using eventFuncType = void (cyberclub::*)(const eventType& e);
    std::vector<eventFuncType> eventsFunc_ = {
        &cyberclub::event1,
        &cyberclub::event2,
        &cyberclub::event3,
        &cyberclub::event4
    };

};


int main(int argc, char* argv[]) 
{
  try
  {
    if(argc != 2) 
    {
      std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
      return 1;
    }

    std::ifstream file(argv[1]);

    if(!file.is_open()) 
    {
      std::cerr << "Error opening file: " << argv[1] << std::endl;
      return 1;
    }

    std::vector<std::string> v(3);
    for(auto& i : v) 
    {
      std::getline(file, i);
    }

    cyberclub club(v);

    std::string line;
    while(std::getline(file, line)) 
    {
      if(club.loadLogLine(line)) return 0;
    }

    std::cout << club.worktime_.first.str_ << std::endl;

    club.checkEvents();

    club.dayEnd();

    std::cout << club.worktime_.second.str_ << std::endl;

    club.daySummary();

    file.close();
  }
  catch(const std::exception& e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
  return 0;
}
